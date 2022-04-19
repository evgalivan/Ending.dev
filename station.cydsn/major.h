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
#ifndef MAJOR
    #define MAJOR
    #include <cytypes.h>
    #include <stdlib.h>
typedef struct major_s{
    size_t Flag;
    size_t Counter;
    size_t BeginOff;
    size_t lenght;
    void* Pattern;
}major_t;    
   
int8 ComparePattern(void* ToCheck, major_t Pattern);
    #endif
/* [] END OF FILE */
