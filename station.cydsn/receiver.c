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
#include "receiver.h"
#include <RecieveShiftReg.h>
#include <BitCounterDec.h>
#include <StartTransmit.h>
#include <FrameAllow.h>
#include <line_buf.h>


volatile unsigned int first = 0;
volatile unsigned int CountToRecieve, rcstatus=0;


inline void Store(uint32 tmp){
    
    if(CountToRecieve)
    {
        CountToRecieve--;
        *(current_write++) = tmp;
    }
}



/*
Func PrepareToStore 
*/

RcResult PrepareToStore(void){
    
    
//    while (RecieveShiftReg_OUT_FIFO_EMPTY != (RecieveShiftReg_SR_STATUS&RecieveShiftReg_OUT_FIFO_MASK)){
    while ((RecieveShiftReg_SR_STATUS&0x40)/*|(!(tmp&0x20))*/){
        RecieveShiftReg_ReadData();
    };
    
    CountToRecieve = PACKET_LENGTH;
    current_send = current_write = in_buf.buf;
    BitCounterDec_WriteCounter(31);
    BitCounterDec_ReadStatusRegister();
    first = 1;
    flag_write_done = 0;
    return RCSUCCSSY;
}


/* [] END OF FILE */
