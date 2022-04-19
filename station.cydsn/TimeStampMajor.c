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
#include "major.h"
#include "ControlFrame.h"

static TimeStamp_t ts_pattern;
static major_t ts_compare = {
    .Flag = 0,
    .Counter = 0,
    .BeginOff = offsetof(TimeStamp_t, CRC),
    .Pattern = (void*)&ts_pattern,
    .lenght = 1
};
/* [] END OF FILE */
