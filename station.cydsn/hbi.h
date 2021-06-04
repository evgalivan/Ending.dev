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
#ifndef HOST_BUS_INTERFACE
#define HOST_BUS_INTERFACE
#include "cytypes.h"  
    
#define EXCEPTION 0x182
#define INDEX_2 0x08   
#define INDEX_3 0x10   
#define DATA_1  0x04   
#define DATA_2  0x0c   
#define DATA_3  0x14   
#define FIF0    0x18  
#define HBI_CFG    0x1c  
    
typedef enum  {D_READ,D_WRITE} Direction_t;
typedef enum  {T_END,T_BEGIN} Transaction_t;
    
void    KSZ8852Reset(void);  

uint16  ReadInternalRegister( uint16   RegAddr);
//void ReadInternalRegisterTo( uint16   RegAddr, void* pdata);
void    WriteInternalRegister( uint16   RegAddr, uint16 value);
//void    WriteInternalRegisterFrom( uint16   RegAddr, void *pvalue);
void SetBitsInInternalRegister( uint16   RegAddr, uint16 bitmask);
void ClrBitsInInternalRegister( uint16   RegAddr, uint16 bitmask);

void KNZ9352IndexTest(void);

void SetByte( uint8* pdata);
#define FIFO_WRITE(A) SetByte(A)


void KSZ8852BusInit(void);
    
#endif    
/* [] END OF FILE */
