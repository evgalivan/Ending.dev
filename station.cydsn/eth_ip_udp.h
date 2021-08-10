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

#ifndef ETH_IP_UDP_H
#define ETH_IP_UDP_H
#include "cytypes.h"    
    
#include "ControlFrame.h"  
    
#define ETH_IP_UDP_HEAD_SIZE 42
extern uint8 item[];

#define ARP_HW_TYPE_ETH        htons(0x0001)
#define ARP_PROTO_TYPE_IP    htons(0x0800)

#define ARP_TYPE_REQUEST    htons(1)
#define ARP_TYPE_RESPONSE    htons(2)
   
 // Ethernet-фрейм
typedef struct eth_frame {
    uint8_t to_addr[6]; // адрес получателя
    uint8_t from_addr[6]; // адрес отправителя
    uint16_t type; // протокол
    uint8_t data[];
}CY_PACKED_ATTR eth_frame_t;


// Коды протоколов
#define IP_PROTOCOL_ICMP    1
#define IP_PROTOCOL_TCP        6
#define IP_PROTOCOL_UDP        17

// IP-пакет
typedef struct ip_packet {
    uint8_t ver_head_len; // версия и длина заголовка =0x45
    uint8_t tos; //тип сервиса
    uint16_t total_len; //длина всего пакета
    uint16_t fragment_id; //идентификатор фрагмента
    uint16_t flags_framgent_offset; //смещение фрагмента
    uint8_t ttl; //TTL
    uint8_t protocol; //код протокола
    uint16_t cksum; //контрольная сумма заголовка
    uint32_t from_addr; //IP-адрес отправителя
    uint32_t to_addr; //IP-адрес получателя
    uint8_t data[];
}CY_PACKED_ATTR ip_packet_t;

// UDP-пакет
 typedef struct udp_packet {
    uint16_t from_port;
    uint16_t to_port;
    uint16_t len;
    uint16_t cksum;
    uint8_t data[]; 
}CY_PACKED_ATTR udp_packet_t; 

// ARP-пакет
typedef struct arp_message {
    uint16_t hw_type; // протокол канального уровня (Ethernet)
    uint16_t proto_type; // протокол сетевого уровня (IP)
    uint8_t hw_addr_len; // длина MAC-адреса =6
    uint8_t proto_addr_len; // длина IP-адреса =4
    uint16_t type; // тип сообщения (запрос/ответ)
    uint8_t mac_addr_from[6]; // MAC-адрес отправителя
    uint32_t ip_addr_from; // IP-адрес отправителя
    uint8_t mac_addr_to[6]; // MAC-адрес получателя, нули если неизвестен
    uint32_t ip_addr_to; // IP-адрес получателя
} arp_message_t;

uint16 arp_prepare(void);

uint16 RefreshItem( uint16_t  size);

int AddTheSectionIntoPacket( void* buf, uint16 buf_size);

int ResumeThePacket(void);

int NotReady(void);

int StartNewUDPPacket(void *header, uint16 size);
int sendPacket(void *header, uint16 size);
extern uint8_t mac_addr[6];
extern uint8_t IPv4multicast[6];
extern uint8_t remote_mac_addr[6];
extern int EthHeaderMustBeUpdated;

extern uint8 arp_frame[64];

void SetPort(uint32 port);
 
void SetUnicast(AnyFrame_u *frame);
 
void SetMulticast(AnyFrame_u *frame);

void SetMac(AnyFrame_u *frame);


#endif

/* [] END OF FILE */
