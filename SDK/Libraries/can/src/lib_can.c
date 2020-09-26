
/* LICENSE */
/* Note: Not Compatible with HAL Driver older than V1.10.0 */

#include "app_config.h"
#if CONFIG_CAN_LIB_ENABLED
#include "drv_irq.h"
#include "fifo.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "lib_can.h"
#include "can.h"

#define MAX_CAN_FRAMES_COUNT    56
#define MAX_CAN_DATA_COUNT      56
#define SOFTWARE_TX_FIFO        10
#define SOFTWARE_RX_FIFO        10
#define VALIDATION_MULT         4

typedef struct
{
	CAN_RxHeaderTypeDef can_rx;
    uint8_t Data[8];
    uint32_t rec_time;
}lib_can_rx_message_t;

typedef struct
{
    lib_can_rx_message_t rx0;
    lib_can_rx_message_t rx1;
}lib_can_rx_fifo_message_t;

typedef struct
{
	CAN_TxHeaderTypeDef can_tx;
    uint8_t Data[8];
}lib_can_tx_message_t;

static lib_can_frame_t         *s_lib_can_tx_frame_list[MAX_CAN_FRAMES_COUNT];
static lib_can_frame_t         *s_lib_can_rx_frame_list[MAX_CAN_FRAMES_COUNT];
static uint8_t                 s_lib_can_tx_frame_count;
static uint8_t                 s_lib_can_rx_frame_count;

static lib_can_data_t          *s_lib_can_tx_data_list[MAX_CAN_DATA_COUNT];
static lib_can_data_t          *s_lib_can_rx_data_list[MAX_CAN_DATA_COUNT];
static uint8_t                 s_lib_can_tx_data_count;
static uint8_t                 s_lib_can_rx_data_count;

static lib_can_data_t          *s_lib_can_tx_evt_data_list[MAX_CAN_DATA_COUNT];
static lib_can_data_t          *s_lib_can_rx_evt_data_list[MAX_CAN_DATA_COUNT];
static uint8_t                 s_lib_can_tx_evt_data_count;
static uint8_t                 s_lib_can_rx_evt_data_count;

static bool                    s_registering_frame;
static bool                    s_registering_data;

static Fifo_Handle_t           s_rx_fifo;
static Fifo_Handle_t           s_tx_fifo;

static lib_can_rx_fifo_message_t rx_message;

lib_can_frame_t * register_frame(uint16_t __std_id, uint16_t __period, uint8_t __dlc, bool __transmit_frame);
lib_can_data_t * register_data(lib_can_frame_t *frame,
							   uint8_t __byte_offset,
							   uint16_t __multiplier,
							   uint16_t __divider,
							   int16_t __offset,
							   lib_can_data_e __transmitted_type,
							   lib_can_data_e __data_type,
							   bool __event_enable,
							   void (*__event_handler_ptr)(lib_can_frame_t * frame),
							   void (*__frame_2_real_data_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data),
							   void (*__real_data_2_frame_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data));
void lib_can_default_frame_2_real_data(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data);
void lib_can_default_real_data_2_frame(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data);
void lib_can_filter_set(CAN_HandleTypeDef * hcan);

lib_can_frame_t * lib_can_register_frame_tx(uint16_t __std_id, uint16_t __period, uint8_t __dlc)
{
	return register_frame(__std_id, __period, __dlc, true);
}

lib_can_frame_t * lib_can_register_frame_rx(uint16_t __std_id, uint16_t __period, uint8_t __dlc)
{
	return register_frame(__std_id, __period, __dlc, false);
}

lib_can_frame_t * register_frame(uint16_t __std_id, uint16_t __period, uint8_t __dlc, bool __transmit_frame)
{
    void                 *mem;
    bool                 may_register;
    lib_can_frame_t      *can_frame_ptr;

    CRITICAL_REGION_ENTER();
    if(!s_registering_frame)
    {
        s_registering_frame = true;
        may_register = true;
    }
    else
    {
        may_register = false;
    }
    CRITICAL_REGION_EXIT();

    if(!may_register)
    {
        return (void *) 0;
    }

    // Check if frame was already registered
    if (__transmit_frame)
    {
        for(uint8_t frame_ctr = 0; frame_ctr < s_lib_can_tx_frame_count; frame_ctr++)
        {
        	if (s_lib_can_tx_frame_list[frame_ctr]->std_id == __std_id)
        	{
        	    CRITICAL_REGION_ENTER();
        	    s_registering_frame = false;
        	    CRITICAL_REGION_EXIT();
        		return s_lib_can_tx_frame_list[frame_ctr];
        	}
        }
    }
    else
    {
        for(uint8_t frame_ctr = 0; frame_ctr < s_lib_can_rx_frame_count; frame_ctr++)
        {
        	if (s_lib_can_rx_frame_list[frame_ctr]->std_id == __std_id)
        	{
        	    CRITICAL_REGION_ENTER();
        	    s_registering_frame = false;
        	    CRITICAL_REGION_EXIT();
        		return s_lib_can_rx_frame_list[frame_ctr];
        	}
        }
    }

    mem = malloc(sizeof(lib_can_frame_t) + __dlc);
    can_frame_ptr = (lib_can_frame_t *)mem;

    can_frame_ptr->dlc              = __dlc;
    can_frame_ptr->period           = __period;
    can_frame_ptr->std_id           = __std_id;
    can_frame_ptr->transmit_frame   = __transmit_frame;
    can_frame_ptr->frame_up_to_date = false;
    can_frame_ptr->update_time      = 0;
    can_frame_ptr->p_fdata           = (uint8_t *)(can_frame_ptr + 1);

    if (__transmit_frame)
    {
        can_frame_ptr->local_id = s_lib_can_tx_frame_count;
        s_lib_can_tx_frame_count++;
        s_lib_can_tx_frame_list[s_lib_can_tx_frame_count - 1] = can_frame_ptr;
    }
    else
    {
        can_frame_ptr->local_id = s_lib_can_rx_frame_count;
        s_lib_can_rx_frame_count++;
        s_lib_can_rx_frame_list[s_lib_can_rx_frame_count - 1] = can_frame_ptr;
     //   lib_can_filter_set(&hcan);
    }

    CRITICAL_REGION_ENTER();
    s_registering_frame = false;
    CRITICAL_REGION_EXIT();

    return can_frame_ptr;
}


retcode_t lib_can_unregister_frame(lib_can_frame_t *frame)
{
    bool            may_unregister;

    CRITICAL_REGION_ENTER();
    if(!s_registering_frame)
    {
        s_registering_frame = true;
        may_unregister = true;
    }
    else
    {
        may_unregister = false;
    }
    CRITICAL_REGION_EXIT();

    if (!may_unregister)
    {
        return retcode_busy;
    }

    if (frame->transmit_frame)
    {
        for(uint8_t frame_ctr = frame->local_id; frame_ctr < (s_lib_can_tx_frame_count - 1); frame_ctr++)
        {
            s_lib_can_tx_frame_list[frame_ctr] = s_lib_can_tx_frame_list[frame_ctr + 1];
        }
        s_lib_can_tx_frame_list[--s_lib_can_tx_frame_count] = (void *)0;
    }
    else
    {
        for(uint8_t frame_ctr = frame->local_id; frame_ctr < (s_lib_can_rx_frame_count - 1); frame_ctr++)
        {
            s_lib_can_rx_frame_list[frame_ctr] = s_lib_can_rx_frame_list[frame_ctr + 1];
        }
        s_lib_can_rx_frame_list[--s_lib_can_rx_frame_count] = (void *)0;
      //  lib_can_filter_set(&hcan);
    }
    free((void *)frame);

    CRITICAL_REGION_ENTER();
    s_registering_frame = false;
    CRITICAL_REGION_EXIT();

    return retcode_ok;
}

lib_can_data_t * lib_can_register_data(lib_can_frame_t *frame,
                                       uint8_t __byte_offset,
                                       uint16_t __multiplier,
                                       uint16_t __divider,
                                       int16_t __offset,
									   lib_can_data_e __transmitted_type,
                                       lib_can_data_e __data_type)
{
	return register_data(frame, __byte_offset, __multiplier, __divider, __offset, __transmitted_type, __data_type, true, (void *)0, (void *)0, (void *)0);//TODO
}

lib_can_data_t * lib_can_register_data_it(lib_can_frame_t *frame,
                                       	  uint8_t __byte_offset,
                                          uint16_t __multiplier,
                                          uint16_t __divider,
                                          int16_t __offset,
										  lib_can_data_e __transmitted_type,
                                          lib_can_data_e __data_type,
                                          bool __event_enable,
                                          void (*__event_handler_ptr)(lib_can_frame_t * frame))
{
	return register_data(frame, __byte_offset, __multiplier, __divider, __offset, __transmitted_type, __data_type, true, __event_handler_ptr, (void *)0, (void *)0);
}

lib_can_data_t * lib_can_register_data_custom(lib_can_frame_t *frame,
                                       	   	  uint8_t __byte_offset,
											  uint16_t __multiplier,
											  uint16_t __divider,
											  int16_t __offset,
											  lib_can_data_e __transmitted_type,
											  lib_can_data_e __data_type,
											  void (*__frame_2_real_data_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data),
											  void (*__real_data_2_frame_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data))
{
	return register_data(frame, __byte_offset, __multiplier, __divider, __offset, __transmitted_type, __data_type, false, (void *)0, __frame_2_real_data_ptr, __real_data_2_frame_ptr);
}

lib_can_data_t * lib_can_register_data_custom_it(lib_can_frame_t *frame,
                                       	   	   	 uint8_t __byte_offset,
												 uint16_t __multiplier,
												 uint16_t __divider,
												 int16_t __offset,
												 lib_can_data_e __transmitted_type,
												 lib_can_data_e __data_type,
												 bool __event_enable,
												 void (*__event_handler_ptr)(lib_can_frame_t * frame),
												 void (*__frame_2_real_data_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data),
												 void (*__real_data_2_frame_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data))
{
	return register_data(frame, __byte_offset, __multiplier, __divider, __offset, __transmitted_type, __data_type, true, __event_handler_ptr, __frame_2_real_data_ptr, __real_data_2_frame_ptr);
}

lib_can_data_t * register_data(lib_can_frame_t *frame,
							   uint8_t __byte_offset,
							   uint16_t __multiplier,
							   uint16_t __divider,
							   int16_t __offset,
							   lib_can_data_e __transmitted_type,
							   lib_can_data_e __data_type,
							   bool __event_enable,
							   void (*__event_handler_ptr)(lib_can_frame_t * frame),
							   void (*__frame_2_real_data_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data),
							   void (*__real_data_2_frame_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data))
{
    void    * mem;
    bool    may_register;
    uint8_t size_of_real_data;
    lib_can_data_t * can_data_ptr;

    CRITICAL_REGION_ENTER();
    if(!s_registering_data)
    {
        s_registering_data = true;
        may_register = true;
    }
    else
    {
        may_register = false;
    }
    CRITICAL_REGION_EXIT();

    if(!may_register)
    {
        return (void *) 0;
    }

    // Check if data was already registered
    if (frame->transmit_frame)
    {
        for(uint8_t data_ctr = 0; data_ctr < s_lib_can_tx_data_count; data_ctr++)
        {
        	if ((s_lib_can_tx_data_list[data_ctr]->p_can_frame == frame) && (s_lib_can_tx_data_list[data_ctr]->offset == __byte_offset))
        	{
        	    CRITICAL_REGION_ENTER();
        	    s_registering_data = false;
        	    CRITICAL_REGION_EXIT();
        		return s_lib_can_tx_data_list[data_ctr];
        	}
        }
    }
    else
    {
        for(uint8_t data_ctr = 0; data_ctr < s_lib_can_rx_data_count; data_ctr++)
        {
        	if ((s_lib_can_rx_data_list[data_ctr]->p_can_frame == frame) && (s_lib_can_rx_data_list[data_ctr]->offset == __byte_offset))
        	{
        	    CRITICAL_REGION_ENTER();
        	    s_registering_data = false;
        	    CRITICAL_REGION_EXIT();
        		return s_lib_can_rx_data_list[data_ctr];
        	}
        }
    }

    switch(__data_type)
    {
    case _boolean:
        size_of_real_data = sizeof(bool);
        break;
    case _uint8_t:
        size_of_real_data = sizeof(uint8_t);
        break;
    case _uint16_t:
		size_of_real_data = sizeof(uint16_t);
		break;
    case _float:
        size_of_real_data = sizeof(float);
        break;
    default:
        size_of_real_data = 1;
    }

    mem = malloc(sizeof(lib_can_data_t) + size_of_real_data);
    can_data_ptr = (lib_can_data_t *)mem;

    can_data_ptr->multiplier          = __multiplier;
    can_data_ptr->divider             = __divider;
    can_data_ptr->offset              = __offset;
    can_data_ptr->p_data_up_to_date   = &frame->frame_up_to_date;
    can_data_ptr->p_data              = &frame->p_fdata[__byte_offset];
    can_data_ptr->data_type           = __data_type;
    can_data_ptr->p_real_data         = (uint8_t *)(can_data_ptr + 1);
    can_data_ptr->enable_evt_handler  = __event_enable;
    can_data_ptr->evt_handler         = __event_handler_ptr;
    can_data_ptr->transmitted_type    = __transmitted_type;
	if (__frame_2_real_data_ptr == (void *)0)
	{
		can_data_ptr->p_frame_2_real_data = &lib_can_default_frame_2_real_data;
	}
	else
	{
		can_data_ptr->p_frame_2_real_data = __frame_2_real_data_ptr;
	}
    if (__real_data_2_frame_ptr == (void *)0)
	{
		can_data_ptr->p_real_data_2_frame = &lib_can_default_real_data_2_frame;
	}
    else
    {
    	can_data_ptr->p_real_data_2_frame = __real_data_2_frame_ptr;
    }
    can_data_ptr->p_can_frame         = frame;

    if (frame->transmit_frame)
    {
        if (__event_enable)
        {
            s_lib_can_tx_evt_data_count++;
            s_lib_can_tx_evt_data_list[s_lib_can_tx_evt_data_count - 1] = can_data_ptr;
        }
        else
        {
            s_lib_can_tx_data_count++;
            s_lib_can_tx_data_list[s_lib_can_tx_data_count - 1] = can_data_ptr;
        }
    }
    else
    {
        if (__event_enable)
        {
            s_lib_can_rx_evt_data_count++;
            s_lib_can_rx_evt_data_list[s_lib_can_rx_evt_data_count - 1] = can_data_ptr;
        }
        else
        {
            s_lib_can_rx_data_count++;
            s_lib_can_rx_data_list[s_lib_can_rx_data_count - 1] = can_data_ptr;
        }
    }

    CRITICAL_REGION_ENTER();
    s_registering_data = false;
    CRITICAL_REGION_EXIT();

    return can_data_ptr;
}


retcode_t lib_can_unregister_data(lib_can_data_t *data)
{
    bool may_unregister;

    CRITICAL_REGION_ENTER();
    if(!s_registering_data)
    {
        s_registering_data = true;
        may_unregister = true;
    }
    else
    {
        may_unregister = false;
    }
    CRITICAL_REGION_EXIT();

    if(!may_unregister)
    {
        return retcode_busy;
    }

    if (data->p_can_frame->transmit_frame)
    {
        if (data->enable_evt_handler)
        {
            for(uint8_t data_ctr = data->local_id; data_ctr < (s_lib_can_tx_evt_data_count - 1); data_ctr++)
            {
                s_lib_can_tx_evt_data_list[data_ctr] = s_lib_can_tx_evt_data_list[data_ctr + 1];
            }
            s_lib_can_tx_evt_data_list[--s_lib_can_tx_evt_data_count] = (void *)0;
        }
        else
        {
            for(uint8_t data_ctr = data->local_id; data_ctr < (s_lib_can_tx_data_count - 1); data_ctr++)
            {
                s_lib_can_tx_data_list[data_ctr] = s_lib_can_tx_data_list[data_ctr + 1];
            }
            s_lib_can_tx_data_list[--s_lib_can_tx_data_count] = (void *)0;
        }
    }
    else
    {
        if (data->enable_evt_handler)
        {
            for(uint8_t data_ctr = data->local_id; data_ctr < (s_lib_can_rx_evt_data_count - 1); data_ctr++)
            {
                s_lib_can_rx_evt_data_list[data_ctr] = s_lib_can_rx_evt_data_list[data_ctr + 1];
            }
            s_lib_can_rx_evt_data_list[--s_lib_can_rx_evt_data_count] = (void *)0;
        }
        else
        {
            for(uint8_t data_ctr = data->local_id; data_ctr < (s_lib_can_rx_data_count - 1); data_ctr++)
            {
                s_lib_can_rx_data_list[data_ctr] = s_lib_can_rx_data_list[data_ctr + 1];
            }
            s_lib_can_rx_data_list[--s_lib_can_rx_data_count] = (void *)0;
        }
    }
    free((void *)data);

    CRITICAL_REGION_ENTER();
    s_registering_data = false;
    CRITICAL_REGION_EXIT();

    return retcode_ok;
}


void lib_can_handler(void)
{
    lib_can_rx_message_t     rx_message;
    lib_can_tx_message_t     tx_message;
    lib_can_frame_t          *frame;
    uint32_t 		 		 tx_mailbox = 0;

    // receive messages
    while (FIFO_OK == Fifo_PullElement(&s_rx_fifo, (void *)&rx_message))
    {
        uint16_t id;

        id = rx_message.can_rx.StdId;
        for (uint8_t frame_ctr = 0; frame_ctr < s_lib_can_rx_frame_count; frame_ctr++)
        {
            if (s_lib_can_rx_frame_list[frame_ctr]->std_id == id)
            {
                s_lib_can_rx_frame_list[frame_ctr]->update_time = rx_message.rec_time;
                memcpy((uint8_t *)s_lib_can_rx_frame_list[frame_ctr]->p_fdata, rx_message.Data, s_lib_can_rx_frame_list[frame_ctr]->dlc);
                break;
            }
        }
    }

    for (uint8_t frame_ctr = 0; frame_ctr < s_lib_can_tx_frame_count; frame_ctr++)
    {
        frame = s_lib_can_tx_frame_list[frame_ctr];
        if (frame->period == 0)
        {
            continue;
        }
        else if (frame->update_time + frame->period < HAL_GetTick())
        {
            tx_message.can_tx.DLC   = frame->dlc;
            tx_message.can_tx.StdId = frame->std_id;
            tx_message.can_tx.RTR   = CAN_RTR_DATA;
            tx_message.can_tx.IDE   = CAN_ID_STD;
            tx_message.can_tx.TransmitGlobalTime = DISABLE;
            memcpy(tx_message.Data, (uint8_t *)frame->p_fdata, frame->dlc);
            frame->update_time = HAL_GetTick();
            Fifo_PushElement(&s_tx_fifo, (void *)&tx_message);
        }
    }

    // send if no tx is ongoing
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) > 0)
	{
    	if (FIFO_OK == Fifo_PullElement(&s_tx_fifo, (void *)&tx_message))
		{
    		HAL_CAN_AddTxMessage(&hcan, &(tx_message.can_tx), tx_message.Data, &tx_mailbox);
		}
        else
        {
            break;
        }
	}

    volatile uint32_t time;
    // update rx validation
    for (uint8_t frame_ctr = 0; frame_ctr < s_lib_can_rx_frame_count; frame_ctr++)
    {
    	time = HAL_GetTick();
        if(s_lib_can_rx_frame_list[frame_ctr]->update_time + VALIDATION_MULT*s_lib_can_rx_frame_list[frame_ctr]->period > HAL_GetTick())
        {
            s_lib_can_rx_frame_list[frame_ctr]->frame_up_to_date = true;
        }
        else
        {
        	time;
            s_lib_can_rx_frame_list[frame_ctr]->frame_up_to_date = false;
        }
    }
}

void lib_can_empty_evt_handler(lib_can_frame_t * frame);

void lib_can_evt_trigger(lib_can_data_t * data);


void lib_can_default_frame_2_real_data(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data)
{
    float 	tmp_data_float;
    static int32_t	tmp_data;

    switch (tr_typ)
	{
	case _uint8_t:
		tmp_data = *(uint8_t *)data;
		break;

	case _int8_t:
		tmp_data = *(int8_t *)data;
		break;

	case _uint16_t:
		tmp_data = *(uint16_t *)data;
		break;

	case _int16_t:
		tmp_data = *(int16_t *)data;
		break;

	default:
		break;
	}

    switch (data_typ)
    {
    case _boolean:
        if (tmp_data > 0)
        {
            *(bool *)real_data = true;
        }
        else
        {
            *(bool *)real_data = false;
        }
        break;

    case _uint8_t:
    case _int8_t:
        memcpy(real_data, &tmp_data, 1);
        break;

    case _uint16_t:
    case _int16_t:
		memcpy(real_data, &tmp_data, 2);
		break;

    case _float:
        tmp_data_float = ((float)(tmp_data) * (float)(mult)
									 	 	/ (float)(div))
											+ (float)(offs);
        memcpy(real_data, &tmp_data_float, sizeof(float));
        break;
    }
}


void lib_can_default_real_data_2_frame(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data)
{
    float 	tmp_data_float;
    static int32_t	tmp_data;

    switch (data_typ)
    {
    case _boolean:
        if (*(bool *)real_data)
        {
        	tmp_data = 0xFF;
        }
        else
        {
        	tmp_data = 0x00;
        }
        break;

    case _uint8_t:
    case _int8_t:
        memcpy(&tmp_data, real_data, 1);
        break;
    case _uint16_t:
    case _int16_t:
		memcpy(&tmp_data, real_data, 2);
		break;

    case _float:
    	tmp_data_float = ((*(float *)real_data) - (float)(offs))
                                    			/ (float)(mult)
												* (float)(div);
    	tmp_data = tmp_data_float;
        break;
    }

    switch (tr_typ)
	{
	case _uint8_t:
		memcpy(data, &tmp_data, 1);
		break;

	case _int8_t:
		memcpy(data, &tmp_data, 1);
		break;

	case _uint16_t:
	case _int16_t:
		memcpy(data, &tmp_data, 2);
		break;

	default:
		break;
	}
}


retcode_t lib_can_data_set(lib_can_data_t *data_hdlr, void *data)
{
    lib_can_tx_message_t tx_message;
    uint32_t tx_mailbox = 0;

    data_hdlr->p_real_data_2_frame(data_hdlr->multiplier,
                                   data_hdlr->divider,
                                   data_hdlr->offset,
								   data_hdlr->transmitted_type,
                                   data_hdlr->data_type,
                                   (uint8_t *)data_hdlr->p_data,
                                   data);
    if (data_hdlr->enable_evt_handler)
    {
        tx_message.can_tx.DLC   = data_hdlr->p_can_frame->dlc;
        tx_message.can_tx.StdId = data_hdlr->p_can_frame->std_id;
        tx_message.can_tx.RTR   = CAN_RTR_DATA;
        tx_message.can_tx.IDE   = CAN_ID_STD;
        memcpy(tx_message.Data, (uint8_t *)data_hdlr->p_can_frame->p_fdata, data_hdlr->p_can_frame->dlc);
        data_hdlr->p_can_frame->update_time = HAL_GetTick();

		if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0)
		{
			if (FIFO_OK == Fifo_PushElementToFront(&s_tx_fifo, &tx_message))
			{
				return retcode_ok;
			}
			else
			{
				return retcode_error;
			}
		}
		return HAL_CAN_AddTxMessage(&hcan, &tx_message.can_tx, tx_message.Data, &tx_mailbox);
    }
    return retcode_ok;
}


bool lib_can_data_get(lib_can_data_t *data_hdlr, void *data)
{
    data_hdlr->p_frame_2_real_data(data_hdlr->multiplier,
                                   data_hdlr->divider,
                                   data_hdlr->offset,
								   data_hdlr->transmitted_type,
                                   data_hdlr->data_type,
                                   (uint8_t *)data_hdlr->p_data,
                                   data);
    return *data_hdlr->p_data_up_to_date;
}

void lib_can_filter_set(CAN_HandleTypeDef * hcan){
	CAN_FilterTypeDef filterConfig;
    uint32_t id_and = 0xFFFF;
    uint32_t id_or 	= 0;

#if defined(STM32F042x6)
    filterConfig.BankNumber =           0;  // Must be kept 0, only 1 bank in uc
    filterConfig.FilterActivation =     ENABLE;
    filterConfig.FilterScale =          CAN_FILTERSCALE_16BIT;
    filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filterConfig.FilterNumber =         0;
	filterConfig.FilterIdLow =          0;
	filterConfig.FilterIdHigh =         0;
	filterConfig.FilterMaskIdLow =      0;
	filterConfig.FilterMaskIdHigh =     0;

    if (s_lib_can_rx_frame_count <= 4)
    {
    	filterConfig.FilterMode =           CAN_FILTERMODE_IDLIST;

    	if (s_lib_can_rx_frame_count >= 1)
    	{
    		filterConfig.FilterIdLow =          (s_lib_can_rx_frame_list[0]->std_id)<<5;
    	}
    	if (s_lib_can_rx_frame_count >= 2)
    	{
    		filterConfig.FilterIdHigh =         (s_lib_can_rx_frame_list[1]->std_id)<<5;
    	}
    	if (s_lib_can_rx_frame_count >= 3)
    	{
    		filterConfig.FilterMaskIdLow =      (s_lib_can_rx_frame_list[2]->std_id)<<5;
    	}
    	if (s_lib_can_rx_frame_count == 4)
    	{
    		filterConfig.FilterMaskIdHigh =     (s_lib_can_rx_frame_list[3]->std_id)<<5;
    	}
    }
    else
    {
    	filterConfig.FilterMode =           CAN_FILTERMODE_IDMASK;
        for (uint8_t id_counter = 0; id_counter < s_lib_can_rx_frame_count; id_counter++)
        {
        	id_and &= s_lib_can_rx_frame_list[id_counter]->std_id;
        	id_or  |= s_lib_can_rx_frame_list[id_counter]->std_id;
        }

        filterConfig.FilterIdHigh =         id_and << 5;
		filterConfig.FilterMaskIdLow =      0xFFF8;
		filterConfig.FilterMaskIdHigh =     (0x7FF & (id_and | ~(id_or))) << 5;
    }
#elif defined(STM32F303xC)
    filterConfig.SlaveStartFilterBank =           0;  // Must be kept 0, only 1 bank in uc
    filterConfig.FilterActivation =     ENABLE;
    filterConfig.FilterScale =          CAN_FILTERSCALE_16BIT;
    filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filterConfig.FilterBank =         	0;
	filterConfig.FilterIdLow =          0;
	filterConfig.FilterIdHigh =         0;
	filterConfig.FilterMaskIdLow =      0;
	filterConfig.FilterMaskIdHigh =     0;
	filterConfig.FilterMode =           CAN_FILTERMODE_IDMASK;
/*
	if (s_lib_can_rx_frame_count <= 56)
	{
		filterConfig.FilterMode =           CAN_FILTERMODE_IDLIST;
		for (uint8_t frame_number = 0; frame_number < s_lib_can_rx_frame_count; frame_number += 4)
		{
			filterConfig.FilterBank =         frame_number % 4;
			if (frame_number + 1 <= s_lib_can_rx_frame_count)
			{
				filterConfig.FilterIdLow =          (s_lib_can_rx_frame_list[frame_number]->std_id)<<5;
			}
			if (frame_number + 2 <= s_lib_can_rx_frame_count)
			{
				filterConfig.FilterIdHigh =         (s_lib_can_rx_frame_list[frame_number + 1]->std_id)<<5;
			}
			if (frame_number + 3 <= s_lib_can_rx_frame_count)
			{
				filterConfig.FilterMaskIdLow =      (s_lib_can_rx_frame_list[frame_number + 2]->std_id)<<5;
			}
			if (frame_number + 4 <= s_lib_can_rx_frame_count)
			{
				filterConfig.FilterMaskIdHigh =     (s_lib_can_rx_frame_list[frame_number + 3]->std_id)<<5;
			}
		}
	}
	*/
    UNUSED(id_or);
    UNUSED(id_and);
#else
    UNUSED(id_or);
    UNUSED(id_and);
    filterConfig.FilterActivation =     DISABLE;
#endif

    HAL_CAN_ConfigFilter(hcan, &filterConfig);
}


void (*lib_can_init(void))(void)
{
	uint32_t active_interrupts = 0;

    s_rx_fifo = Fifo_Init(sizeof(lib_can_rx_message_t), SOFTWARE_RX_FIFO);
    s_tx_fifo = Fifo_Init(sizeof(lib_can_tx_message_t), SOFTWARE_TX_FIFO);

   lib_can_filter_set(&hcan);

    HAL_CAN_Start(&hcan);

    active_interrupts |= CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR | CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL;
    HAL_CAN_ActivateNotification(&hcan, active_interrupts);

    return &lib_can_handler;
}

bool lib_can_find_and_handle_event(volatile lib_can_rx_message_t * rx_msg)
{
    uint16_t id;

    id = rx_msg->can_rx.StdId;
    for (uint8_t data_ctr = 0; data_ctr < s_lib_can_rx_evt_data_count; data_ctr++)
    {
        if (s_lib_can_rx_evt_data_list[data_ctr]->p_can_frame->std_id == id)
        {
            memcpy((uint8_t *)s_lib_can_rx_evt_data_list[data_ctr]->p_data, (uint8_t *)rx_msg->Data, s_lib_can_rx_evt_data_list[data_ctr]->p_can_frame->dlc);
            s_lib_can_rx_evt_data_list[data_ctr]->evt_handler(s_lib_can_rx_evt_data_list[data_ctr]->p_can_frame);
            return true;
        }
    }
    return false;
}

void lib_can_handle_rx_irq(CAN_HandleTypeDef* hcan, uint32_t RxFifo, lib_can_rx_message_t * lib_rx_mes)
{
	while (HAL_CAN_GetRxFifoFillLevel(hcan, RxFifo) >= 1)
	{
		HAL_CAN_GetRxMessage(hcan, RxFifo, &lib_rx_mes->can_rx, lib_rx_mes->Data);
		// Check if received real data
		if ((lib_rx_mes->can_rx.IDE == CAN_ID_STD) && (lib_rx_mes->can_rx.RTR == CAN_RTR_DATA))
		{
			if (!lib_can_find_and_handle_event(lib_rx_mes))
			{
				lib_rx_mes->rec_time = HAL_GetTick();
		        Fifo_PushElement(&s_rx_fifo, (void *)lib_rx_mes);
			}
		}
	}

}

void lib_can_handle_tx_irq(CAN_HandleTypeDef* hcan)
{
    lib_can_tx_message_t tx_message;
    uint32_t tx_mailbox = 0;

    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0)
    {
        if (FIFO_OK == Fifo_PullElement(&s_tx_fifo, (void *)&tx_message))
        {
        	HAL_CAN_AddTxMessage(hcan, &tx_message.can_tx, tx_message.Data, &tx_mailbox);
        }
        else
        {
            break;
        }
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_rx_irq(hcan, CAN_RX_FIFO0, &rx_message.rx0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_rx_irq(hcan, CAN_RX_FIFO1, &rx_message.rx1);
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_rx_irq(hcan, CAN_RX_FIFO0, &rx_message.rx0);
}

void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_rx_irq(hcan, CAN_RX_FIFO1, &rx_message.rx1);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_tx_irq(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_tx_irq(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan)
{
	lib_can_handle_tx_irq(hcan);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    // tx failure
//    if ((hcan->ErrorCode & HAL_CAN_ERROR_TX_ALST0)	||
//		(hcan->ErrorCode & HAL_CAN_ERROR_TX_TERR0)	||
//		(hcan->ErrorCode & HAL_CAN_ERROR_TX_ALST1)	||
//		(hcan->ErrorCode & HAL_CAN_ERROR_TX_TERR1)	||
//		(hcan->ErrorCode & HAL_CAN_ERROR_TX_ALST2)	||
//		(hcan->ErrorCode & HAL_CAN_ERROR_TX_TERR2)	||
//    	(hcan->ErrorCode & HAL_CAN_ERROR_ACK)		||
//		(hcan->ErrorCode & HAL_CAN_ERROR_EWG))
}

#endif /* CONFIG_CAN_LIB_ENABLED */
