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
#include <string.h>

// возвращает 0 если совпало
int8 ComparePattern(void* ToCheck, major_t Pattern){
    return memcmp(ToCheck + Pattern.BeginOff, Pattern.Pattern + Pattern.BeginOff, Pattern.lenght);
}
/* [] END OF FILE */
