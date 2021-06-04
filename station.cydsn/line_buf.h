/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "ControlFrame.h"

#ifndef uint32 
    #include "cytypes.h"
#endif

#define FRAME_SIZE (4)
#define FRAME_NUM (8)

#define PACKET_LENGTH (FRAME_NUM*FRAME_SIZE)

typedef union{
    uint32 buf[PACKET_LENGTH];
    AnyFrame_u Frames[FRAME_NUM];
}ExchangeBuf_u;

extern ExchangeBuf_u in_buf;
extern ExchangeBuf_u out_buf;
extern uint32 line_buf_fake[];
extern volatile uint32 *current_write;
extern volatile uint32 *current_send;
extern volatile uint32 *current_read;
extern volatile uint8 flag_read_done, flag_write_done;

/* [] END OF FILE */
