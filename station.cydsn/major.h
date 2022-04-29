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

    #define MAJOR_COUNTER (3)
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

void* CheckTS(void* ToCheck);
void* CheckRenumber(void* ToCheck);


    #endif
/* [] END OF FILE */
