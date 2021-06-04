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
#ifndef RESTART_ACC_H
#define RESTART_ACC_H

#include <cytypes.h>

#define HALF_OF_MS (36000L) /*half of system ticks number per ms*/

typedef struct {
    uint8 Cmp: 1; // 
    uint8 Zero: 1; 
    uint8 Overflow: 1; // i.e. TC, True
    uint8 Underflow: 1; //
    uint8 Capture: 1; // True
    uint8 FIFO_Full: 1; // False
    uint8 FIFO_Not_Empty: 1; //False
}StatusReg_t;

typedef union {
    StatusReg_t field;
    uint8 reg;
}status_u;

extern uint32 pre_capt_flag;
extern uint32 LastCapturedPeriod;
extern uint32 DelayCompensation;
extern uint32 ValueOfNewPeriod;


#endif
/* [] END OF FILE */
