/*
 * m_can.h
 *
 *  Created on: Aug 2, 2017
 *      Author: Wojciech
 */

#ifndef LIB_CAN_H_
#define LIB_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "can.h"
#include "drv_common.h"

typedef enum{
    _boolean = 0,
    _uint8_t,
	_int8_t,
	_uint16_t,
	_int16_t,
    _float,
}lib_can_data_e;

typedef struct{
    uint16_t    		period;             /* Period in ms */
    uint16_t    		std_id;             /* Standard identifier */
    uint8_t     		dlc;                /* Data length count */
    volatile uint32_t   update_time;        /* Time of frame send or receive event */
    volatile bool       frame_up_to_date;   /* Rx only */
    volatile uint8_t    *p_fdata;           /* Pointer to data */
    bool        		transmit_frame;     /* Rx mode or Tx mode */
    uint8_t     		local_id;           /* local id */
}lib_can_frame_t;

typedef struct{
    lib_can_frame_t 	*p_can_frame;                               /* Pointer to frame */
    uint16_t        	multiplier;                                 /* Multiplier */
    uint16_t        	divider;                                    /* Divider */
    int16_t         	offset;                                     /* Offset */
    volatile bool       *p_data_up_to_date;                        	/* Is data actual */
    volatile uint8_t    *p_data;                                   	/* Pointer to data */
    lib_can_data_e		transmitted_type;							/* Type and size of transmitted data */
    lib_can_data_e  	data_type;                                  /* Type of data passed in/out of library*/
    volatile void       *p_real_data;                               /* Pointer to user data variable */
    bool            	enable_evt_handler;                         /* In rx enable looking for frame, in tx enable trigger when data_set */
    void            	(*evt_handler)(lib_can_frame_t * frame);    /* Handler when received frame */
    void            	(*p_frame_2_real_data)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data);
    void            	(*p_real_data_2_frame)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data);
    uint8_t         	local_id;                                   /* local id */
}lib_can_data_t;

void (*lib_can_init(void))(void);

lib_can_frame_t * lib_can_register_frame_tx(uint16_t __std_id, uint16_t __period, uint8_t __dlc);
lib_can_frame_t * lib_can_register_frame_rx(uint16_t __std_id, uint16_t __period, uint8_t __dlc);
retcode_t lib_can_unregister_frame(lib_can_frame_t *frame);
lib_can_data_t * lib_can_register_data(lib_can_frame_t *frame,
                                       uint8_t __byte_offset,
                                       uint16_t __multiplier,
                                       uint16_t __divider,
                                       int16_t __offset,
									   lib_can_data_e __transmitted_type,
                                       lib_can_data_e __data_type);
lib_can_data_t * lib_can_register_data_it(lib_can_frame_t *frame,
                                       	  uint8_t __byte_offset,
										  uint16_t __multiplier,
										  uint16_t __divider,
										  int16_t __offset,
										  lib_can_data_e __transmitted_type,
										  lib_can_data_e __data_type,
										  bool __event_enable,
										  void (*__event_handler_ptr)(lib_can_frame_t * frame));

lib_can_data_t * lib_can_register_data_custom(lib_can_frame_t *frame,
                                       	   	  uint8_t __byte_offset,
											  uint16_t __multiplier,
											  uint16_t __divider,
											  int16_t __offset,
											  lib_can_data_e __transmitted_type,
											  lib_can_data_e __data_type,
											  void (*__frame_2_real_data_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data),
											  void (*__real_data_2_frame_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data));
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
												 void (*__real_data_2_frame_ptr)(uint16_t mult, uint16_t div, int16_t offs, lib_can_data_e tr_typ, lib_can_data_e data_typ, uint8_t *data, void *real_data));
retcode_t lib_can_unregister_data(lib_can_data_t *data);

retcode_t lib_can_data_set(lib_can_data_t *data_hdlr, void *data);
bool lib_can_data_get(lib_can_data_t *data_hdlr, void *data);

void lib_can_empty_evt_handler(lib_can_frame_t * frame);

#ifdef __cplusplus
}
#endif
#endif /* LIB_CAN_H_ */
