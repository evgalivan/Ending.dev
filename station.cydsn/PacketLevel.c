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
#include "string.h"
#include "eth_ip_udp.h"
#include "hbi.h"
#include "line_buf.h"

volatile uint32 Packet_prepared =0;
volatile uint32 SectionsToSend =0;
static uint16 PackId; 
typedef struct{
    uint16 PacketId:6;
    uint16  :2;
    uint16 TXDPN1:1;
    uint16 TXDPN2:1;
    uint16  :5;
    uint16 TXIC:1;
} CommandA;

typedef struct{
    uint16 TransmitByteCount:11;
    uint16  :5;
} CommandB;

static uint16 packet_bytes_to_send;
static uint16 packet_id;
static uint16 transaction_bytes_to_send;
static CommandA firsrt;
static CommandB second;

int AddTheSectionIntoPacket( void* buf, uint16 buf_size){
    uint8* ptmp = (uint8*)buf;
    if(transaction_bytes_to_send < buf_size) buf_size = transaction_bytes_to_send;
    transaction_bytes_to_send -= buf_size;
    while(buf_size--){
        FIFO_WRITE(ptmp++);
    }
    return 0;
}

void  AddUint32IntoPacket( uint32 value){
    uint8* ptmp = (uint8*)value;
    uint32 buf_size = sizeof(value);
    if(transaction_bytes_to_send < buf_size) buf_size = transaction_bytes_to_send;
    transaction_bytes_to_send -= buf_size;
    while(buf_size--){
        FIFO_WRITE(ptmp++);
    }
}

int ResumeThePacket(void){
    
    uint8* ptmp = (uint8*)&firsrt;
    while(transaction_bytes_to_send--){
        FIFO_WRITE( ptmp);
    }
    StopQMU_Fifo( );
    SetBitsInInternalRegister( TXQCR, 1); //send manually   
    Packet_prepared =0;
    return 0;
}

int NotReady(void){
    return ReadInternalRegister(TXQCR)& 1;
}

void preamble( uint16 size){
    
    if(NotReady( )) 
        ResetQMU();//not sended
    
    packet_bytes_to_send = size;
    uint16 NeedSpace = (packet_bytes_to_send + 4 );
    if(NeedSpace > (GetFreeSpace())) 
        ResetQMU();
//    now enaf
    
    firsrt.PacketId = packet_id++;
    firsrt.TXDPN1 = 1;
    firsrt.TXDPN2 = 1;
    firsrt.TXIC = 1;
    
    second.TransmitByteCount = packet_bytes_to_send;

    transaction_bytes_to_send=((packet_bytes_to_send+3)>>2) * 4;
    
    StartQMU_Fifo( );
    
    uint8* ptmp = (uint8*)&firsrt;
    FIFO_WRITE(ptmp++);
    FIFO_WRITE(ptmp); 
    ptmp = (uint8*)&second;
    FIFO_WRITE(ptmp++);
    FIFO_WRITE(ptmp); 
}

int StartNewUDPPacket(void *header, uint16 size){
    preamble(size);
    int result = AddTheSectionIntoPacket( header, ETH_IP_UDP_HEAD_SIZE);
    Packet_prepared = 1;
    SectionsToSend = transaction_bytes_to_send/sizeof(in_buf);
    return result;
}

int sendPacket(void *header, uint16 size){
    preamble(size);
    AddTheSectionIntoPacket( header, size);
    return ResumeThePacket();
}



/* [] END OF FILE */
