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
#include    "project.h"
#include    "hbi.h"



#define StableRead2(pdata)  *(pdata) = DATA_Read_Status
#define StableData(pdata)    DATA_Write_Control = pdata

void SetByte( uint8* pdata){
    Direction_Control = D_WRITE;
    StableData(*pdata);
    Transaction_Control = T_BEGIN;
    while(Transaction_Control);
}

void GetByte( uint8* pdata){

    Direction_Control = D_READ;
    StableRead2(pdata);
    Transaction_Control = T_BEGIN;
    while(Transaction_Control);
    StableRead2(pdata);
}



#define SetCMD2( A)        ADDR_Control =(A)

void AddrWrite(uint16 addr){
    uint8* tmp =(uint8*)&addr;
    SetCMD2(1);
    SetByte(tmp++);
    SetByte(tmp);
    SetCMD2(0);
}
    
    
void RegWrite(uint16 addr, void* pdata){
    Direction_Control = D_WRITE;  
    if(addr == EXCEPTION){
        AddrWrite( addr+1);
        SetByte(pdata+1);
        AddrWrite( addr);
        SetByte(pdata);
    }else{
        AddrWrite( addr);
        SetByte(pdata);
        AddrWrite( addr+1);
        SetByte(pdata+1);
    }
}

void RegRead(uint16 addr, void* pdata){
    Direction_Control = D_READ;  
    AddrWrite( addr++);
    GetByte(pdata++);
    AddrWrite( addr);
    GetByte(pdata);
}

uint16  ReadInternalRegister( uint16   RegAddr){
    uint16 tmp;
    RegRead(RegAddr, (void*)&tmp);
    return tmp;
}
void  WriteInternalRegister( uint16   RegAddr,uint16 data){
    RegWrite(RegAddr, (void*)&data);
}

void SetBitsInInternalRegister( uint16   RegAddr, uint16 bitmask){
    uint16 tmp;
    RegRead( RegAddr, &tmp);
    tmp |= bitmask;
    RegWrite( RegAddr, &tmp);
}

void ClrBitsInInternalRegister( uint16   RegAddr, uint16 bitmask){
    uint16 tmp;
    RegRead( RegAddr, &tmp);
    tmp &= ~bitmask;
    RegWrite( RegAddr, &tmp);
}

void KSZ8852Reset(void){
    RST_LOW_Write(0);
}

void KSZ8852Set(void){
    RST_LOW_Write(1);
}

uint32 res_ind;
#define  ATTEMPTS  (1000000)
void KSZ8852BusInit(void){
    
    while(1){
        long i=0;
        for(i=0;i<100000;i++){
        }
        KSZ8852Set();
       
        for(;i<ATTEMPTS;i++){
            res_ind=(0xfffe)&ReadInternalRegister(0);
            if (res_ind == 0x8432)break;
        }
        if(i>=ATTEMPTS)
            KSZ8852Reset();
        else break;
   
    }
  
}


/* [] END OF FILE */
