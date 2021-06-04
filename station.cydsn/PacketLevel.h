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
#ifndef PACKET_LEVEL_H
#define PACKET_LEVEL_H
    #include <cytypes.h>
extern volatile uint32 Packet_prepared;
extern volatile uint32 SectionsToSend;
int AddTheSectionIntoPacket( void* buf, uint16 buf_size);
void  AddUint32IntoPacket( uint32 value);
int ResumeThePacket(void);

#endif// PACKET_LEVEL_H
/* [] END OF FILE */
