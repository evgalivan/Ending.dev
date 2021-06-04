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
#include <line_buf.h>

ExchangeBuf_u in_buf;
ExchangeBuf_u out_buf;
volatile uint32* current_write = in_buf.buf;
volatile uint32* current_read = out_buf.buf;
volatile uint32 *current_send = in_buf.buf;
volatile uint8 flag_read_done = 0, flag_write_done = 0;

uint32 line_buf_fake[PACKET_LENGTH] =      {0xf05aa8fc, 0x5aa8fc, 0x5aa8fc, 0x5aa8fc,   //datasample 1
                                            0, 0, 0, 0,                                 //empty 2
                                            0, 0, 0, 0,                                 //empty 3
                                            0, 0, 0, 0,                                 //empty 4
                                            0, 0, 0, 0,                                 //empty 5
                                            0, 0, 0, 0,                                 //empty 6
                                            0, 0, 0, 0,                                 //empty 7
                                            0xD0000400, 0, 0, 0};                       //timestamp 8
/* [] END OF FILE */