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
#ifndef KSZ8852_H
#define KSZ8852_H
#include "cytypes.h"    
    
    ///register deginitions
    #define GRR 0x126// general reset
    #define CIDER 0x0// CHIP ID AND ENABLE REGISTER
    #define SGCR1 0x002//SWITCH GLOBAL CONTROL REGISTER 1
    #define SGCR2 0x004//SWITCH GLOBAL CONTROL REGISTER 2
    #define P1CR2 0x06E//
    #define P1CR4 0x07E
    #define P2CR2 0x086
    #define P2CR4 0x096
    #define TXFDPR 0x184
    #define TXCR   0x170
    #define RXFDPR 0x186
    #define RXFCTR 0x19C
    #define RXCR1 0x174
    #define RXCR2 0x176
    #define RXQCR 0x182
    #define ISR 0x192
    #define IER 0x190
    #define MACQL 0x110    
    #define MACQM 0x112    
    #define MACQH 0x114
    #define MACSL 0x010
    #define MACSM 0x012
    #define MACSH 0x014
    #define TXMIR 0x178
    #define TXQCR 0x180

    
    void KSZ8852_Init(void);   
    void ConfigureMAC(uint8_t mac[6]);
    void ResetQMU(void);
    uint16 GetFreeSpace(void);
    
    void StopQMU_Fifo(void);
    void StartQMU_Fifo(void);

    
#endif

/* [] END OF FILE */
