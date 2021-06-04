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
#include "KSZ8852.h"
#include "hbi.h"
#include "eth_ip_udp.h"


void ConfigureMAC(uint8_t mac[6]){
    uint16 macl = (((uint16)mac[4])<<8) + mac[5];
    WriteInternalRegister(MACQL,macl);//Write QMU MAC address (low)
    WriteInternalRegister(MACSL,macl);//Write Switch MAC address(low) for sending PAUSE frame.
    uint16 macm = (((uint16)mac[2])<<8) + mac[3];
    WriteInternalRegister(MACQM,macm);//Write QMU MAC address (Medium)
    WriteInternalRegister(MACSM,macm);//Write Switch MAC address (Medium) for sending PAUSE frame.
    uint16 mach = (((uint16)mac[0])<<8) + mac[1];
    WriteInternalRegister(MACQH,mach);//Write QMU MAC address (Hi)
    WriteInternalRegister(MACSH,mach);//Write Switch MAC address (Hi) for sending PAUSE frame.
}


uint16 GetFreeSpace(void){
    uint16 result = (0x1fff)&ReadInternalRegister(TXMIR);
    if(result == 0) result = (0x1fff)&ReadInternalRegister(TXMIR);
    return result;
}
//Read value from TXMIR to check if QMU TXQ has enough amount of memory for the Ethernet packet data
//plus 4-byte frame header. Compare the read value with (txPacketLength+4), if less than (txPacketLength+4),}

void ResetQMU(void){
    SetBitsInInternalRegister(GRR,1<<1);     //reset the QMU.
    ClrBitsInInternalRegister(GRR,1<<1);     //set the QMU.
        
    SetBitsInInternalRegister(TXFDPR,(1<<14));//Enable QMU Transmit Frame Data Pointer Auto Increment.
    WriteInternalRegister(TXCR ,0x00EE);//Enable QMU Transmit flow control / Transmit padding / Transmit CRC,
    //and IP/TCP/UDP checksum generation.
    SetBitsInInternalRegister(RXFDPR,(1<<14));//Enable QMU Receive Frame Data Pointer Auto Increment.
    WriteInternalRegister(RXFCTR,0x0001);//Configure Receive Frame Threshold for one frame.1
    WriteInternalRegister(RXCR1,0x7CE0);//Enable QMU Receive flow control / Receive all broadcast frames 
    //Receive unicast frames, and IP/TCP/UDP checksum verification etc.
    WriteInternalRegister(RXCR2,0x0115);//Enable QMU Receive UDP Lite frame checksum verification/generation,
    //IPv4/IPv6 UDP fragment frame pass, drop the received frame if SA is same as device MAC address, 
    //and QMU Flow Control Pause Timer.
    WriteInternalRegister(RXQCR,0x0230);//Enable QMU Receive IP Header Two-Byte Offset 
    //Receive Frame Count Threshold/RXQ Auto-Dequeue frame.);
    WriteInternalRegister(ISR,0xFFFF);//Clear the interrupts status.
    WriteInternalRegister(IER,0xE000);//Enable Link Change\Transmit\Receive interrupt 
    //if your host processor can handle the interrupt, otherwise do not need to do this step
    SetBitsInInternalRegister(TXCR ,1);//Enable QMU Transmit.
    SetBitsInInternalRegister(RXCR1,1);//Enable QMU Receive.
}

void StartQMU_Fifo(void){
    SetBitsInInternalRegister(RXQCR,1<<3);//Start QMU DMA transfer operation to write frame data
    //from host CPU to the TxQ.
}
void StopQMU_Fifo(void){
    ClrBitsInInternalRegister(RXQCR,1<<3);//Start QMU DMA transfer operation to write frame data
    //from host CPU to the TxQ.
}

void KSZ8852_Init(void){
    
    SetBitsInInternalRegister(CIDER,1);     //Start the chip.
    SetBitsInInternalRegister(SGCR1,(1<<8));//Enable more aggressive back off algorithm in half-duplex mode 
    //to enhance performance.
    SetBitsInInternalRegister(SGCR2,(1<<3));//Enable Switch don’t drop packets when 16 or more collisions 
    //occur in half-duplex mode.
    ClrBitsInInternalRegister(P1CR2,(1<<12));//Disable ‘Force Flow Control’. The flow control is enabled based 
    //on auto-negotiation result.
    SetBitsInInternalRegister(P1CR4,(1<<5));//Force Port 1 in half duplex if auto-nego fails when link partner
    //doesn’t support auto-nego (like HUB device).
    SetBitsInInternalRegister(P1CR4,(1<<13));//Restart Port 1 auto-Negotiation
    ClrBitsInInternalRegister(P2CR2,(1<<12));//Disable ‘Force Flow Control’. The flow control is enabled based 
    //on auto-negotiation result.
    SetBitsInInternalRegister(P2CR4,(1<<5));//Force Port 2 in half duplex if auto-nego fails when link partner
    //doesn’t support auto-nego (like HUB device).
    SetBitsInInternalRegister(P2CR4,(1<<13));//Restart Port 2 auto-Negotiation
    
    ConfigureMAC( mac_addr);
    
    ResetQMU( );
    
}

/* [] END OF FILE */
