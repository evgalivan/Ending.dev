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
#include "sender.h"
#include <TransmitShiftReg.h>
#include <BitCounterEnc.h>
#include <StartTransmit.h>
#include <FrameAllow.h>
#include <line_buf.h>
#include <isr_TransmitWordShift.h>
#include <isr_Load_TrShReg.h>

volatile unsigned int PrepareDone = 0;
statReg curStat;
volatile unsigned int count_to_send=0;
volatile unsigned int status=0, tmp_tr;
int stsh;

void Load(void){
    while(count_to_send){
        if((TransmitShiftReg_SR_STATUS & 0x10) ==0 )break;//full
        TransmitShiftReg_WriteData(*(current_read++));
        count_to_send--;
    }
}

void HardLoad(void){
        TransmitShiftReg_WriteData(*(current_read++));
        count_to_send--;
        TransmitShiftReg_WriteData(*(current_read++));
        count_to_send--;
        TransmitShiftReg_WriteData(*(current_read++));
        count_to_send--;
        TransmitShiftReg_WriteData(*(current_read++));
        count_to_send--;
}


TrResult  PrepareToSend(void){
    if (status)
        return TRBUSY;
    count_to_send  = PACKET_LENGTH;
    current_read = out_buf.buf;
    *current_read |= 0x80000000;
    HardLoad();
    return TRSUCCSSY;
}

void  PrepareANDSend(void){
    uint32 tmp = 0xffffffff;
    count_to_send  = 1;
    current_read = &tmp;
    Load();
    Send();
}

void   Send(){
    CyGlobalIntDisable;
    StartTransmit_Write(0);
    //status=1;    
    BitCounterEnc_WriteCounter(BitCounterEnc_ReadCompare());    
    //BitCounterEnc_ReadStatusRegister();
    //isr_Load_TrShReg_ClearPending();
    StartTransmit_Write(1);
    isr_Load_TrShReg_Enable();
    CyGlobalIntEnable;
}


void   ClearStatus(void){   //должна быть вызвана из прерывания bitcouner, когда сброшен StartTransmit
    status=0;        
}
/* [] END OF FILE */
   


