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

 extra thanks
http://we.easyelectronics.ru/electro-and-pc/podklyuchenie-mikrokontrollera-k-lokalnoy-seti-udp-server.html
*/

#include "eth_ip_udp.h"  
#include "string.h"  

// Перекодирование word'а
#define htons(a)            ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
#define ntohs(a)            htons(a)

// Перекодирование dword'а
#define htonl(a)            ( (((a)>>24)&0xff) | (((a)>>8)&0xff00) |\
                                (((a)<<8)&0xff0000) | (((a)<<24)&0xff000000) )
#define ntohl(a)            htonl(a)

#define ETH_TYPE_ARP        htons(0x0806)
#define ETH_TYPE_IP            htons(0x0800)

// Макрос для IP-адреса
#define inet_addr(a,b,c,d)    ( ((uint32_t)a) | ((uint32_t)b << 8) |\
                                ((uint32_t)c << 16) | ((uint32_t)d << 24) )

uint8 item[1514];

// MAC-адрес
uint8_t mac_addr[6] = {0x00,0x13,0x37,0x01,0x23,0x45};
uint8_t IPv4multicast[6] = {0x01,0x00,0x5e,0x00,0x00,0x00};
uint8_t remote_mac_addr[6] = {0x80,0xE8,0x2C,0xC2,0x70,0xF9};

// IP-адрес
uint32_t ip_addr = inet_addr(192,168,0,5);
uint32_t ip_addr_to = inet_addr(230,1,1,0);
uint32_t remote_ip_addr = inet_addr(192,168,0,2);
int EthHeaderMustBeUpdated = 1;

// port
uint16_t Port = 11111;

void SetPort(uint32 port){
   Port = port;
}
void SetUnicast(uint32 cast){
   ip_addr = cast;
}
void SetMulticast(uint32 cast){
   ip_addr_to = cast;
}

void SetMac(uint32* cast){
    uint8* tmp = ( uint8*)cast;
    mac_addr[2] = *(tmp++);
    mac_addr[3] = *(tmp++);
    mac_addr[4] = *(tmp++);
    mac_addr[5] = *(tmp++);
    tmp+=2;
    mac_addr[0] = *(tmp++);
    mac_addr[1] = *(tmp++);
}

uint16_t ip_cksum(uint32_t sum, uint8_t *buf, uint16_t len)
{
    // Рассчитываем сумму word'ов блока (big endian)
    // (блок выравнивается на word нулём)
    while(len >= 2)
    {
        sum += ((uint16_t)*buf << 8) | *(buf+1);
        buf += 2;
        len -= 2;
    }

    if(len)
        sum += (uint16_t)*buf << 8;

    // Складываем старший и младший word суммы
    // пока не получим число, влезающее в word
    while(sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    // Снова конвертируем в big endian и берём дополнение
    return ~htons((uint16_t)sum);
}

void GenerateIPv4multicast(uint32_t ip_addr_to){
    void* ptmp = (void*)&ip_addr_to + 1;
    memcpy( &IPv4multicast[3], ptmp, 3);
    IPv4multicast[3] &= 0x7f;    
}

uint16 eth_reply(uint8_t* buf, uint16_t len)
{
    eth_frame_t *frame = (void*)buf;
    GenerateIPv4multicast(ip_addr_to);
    memcpy(frame->to_addr, IPv4multicast, 6);
//    memcpy(frame->to_addr, remote_mac_addr, 6);
    memcpy(frame->from_addr, mac_addr, 6);
    frame->type = ETH_TYPE_IP;
    return ( len + sizeof(eth_frame_t));
}

uint16 eth_arp_reply(uint8_t* buf, uint16_t len){
    eth_frame_t *frame = (void*)buf;
//    GenerateIPv4multicast(ip_addr_to);
//    memcpy(frame->to_addr, IPv4multicast, 6);
    memset(frame->to_addr, 0xFF, 6);
    memcpy(frame->from_addr, mac_addr, 6);
    frame->type = ETH_TYPE_ARP;
    return ( len + sizeof(eth_frame_t));
}


#define IP_PACKET_TTL (127)

uint16_t ip_reply(uint8_t* buf, uint16_t len)
{
    eth_frame_t *frame = (void*)buf;
    ip_packet_t *packet = (void*)(frame->data);

    // Заполняем заголовок
    packet->protocol = IP_PROTOCOL_UDP;
    packet->tos = 0;
    packet->ver_head_len = 0x45;
    packet->total_len = htons(len + sizeof(ip_packet_t));
    packet->fragment_id = 0;
    packet->flags_framgent_offset = 0;
    packet->ttl = IP_PACKET_TTL;
    packet->cksum = 0;
    packet->to_addr = ip_addr_to;
//    packet->to_addr = remote_ip_addr;
    packet->from_addr = ip_addr;
    packet->cksum = ip_cksum(0, (void*)packet, sizeof(ip_packet_t));

    // Заворачиваем в Ethernet-фрейм и отправляем
    return eth_reply(buf, len + sizeof(ip_packet_t));
}

uint16_t udp_reply(uint8_t* buf, uint16_t len)
{
    eth_frame_t *frame = (void*)buf;
    ip_packet_t *ip = (void*)(frame->data);
    udp_packet_t *udp = (void*)(ip->data);

    // Рассчитываем длину всего пакета
    len += sizeof(udp_packet_t);

    // Меняем местами порт отправителя и получателя
    
    udp->from_port = htons(Port);
    udp->to_port = udp->from_port;

    // Длина пакета
    udp->len = htons(len);

    // Рассчитываем контрольную сумму от псведозаголовка + данных
    // Псведозаголовок = длина пакета+протокол+IP адреса+нормальный udp-заголовок
    // длину пакета+протокол передаём как начальное значение для 
    // рассчёта контрольной суммы
    // ip адреса берём из заголовка IP-пакета (udp-пакет - 8)
    udp->cksum = 0;
//    udp->cksum = ip_cksum(len + IP_PROTOCOL_UDP, 
//        (uint8_t*)udp-8, len+8);
// не расчитываем
    return ip_reply(buf, len);
}
uint8 ERR_level;


uint16  RefreshItem( uint16_t  size){
    return  udp_reply( item,size);
}

uint8 arp_frame[64];
 
// Обработчик ARP-пакетов
uint16 arp_prepare()
{
    eth_frame_t* frame = (eth_frame_t*) arp_frame;
    arp_message_t *msg = (void*)(frame->data);
    
    msg->hw_addr_len = 6;
    
    msg->proto_addr_len = 4;
    

    // Отправляем ответ
    msg->type = ARP_TYPE_REQUEST;
    memset(msg->mac_addr_to, 0xFF, 6);
    memcpy(msg->mac_addr_from, mac_addr, 6);
    msg->ip_addr_to = remote_ip_addr;
    msg->ip_addr_from = ip_addr;

    return eth_arp_reply((uint8_t*)frame, sizeof(arp_message_t));
}
     

/* [] END OF FILE */
