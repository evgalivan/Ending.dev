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
#ifndef CONTROL_FRAME_H
#define CONTROL_FRAME_H

#include <cytypes.h>

#define FRAME_TAG_MASK (0xF0000000)
#define FRAME_TAG_CONTROL (0x90000000)
#define SUBFRAME_TAG_MASK (0x07000000)
#define SUBFRAME_TAG_IND_CONTROL (0x0100000)
#define SUBFRAME_TAG_GEN_CONTROL (0x0200000)
#define TAG_TIME_STAMP (0xD)
#define TAG_RENUM (0xC)
    //kill control_ind_adc_frame and control_gen_adc_frame maybe???
typedef struct {
    uint32 : 8;
    uint32 mux_gidro: 8;
    uint32 gain_gidro: 8;
    uint32 subframe: 3;
    uint32 fake: 1;
    uint32 frame: 3;
    uint32 label: 1;
    //first word
    uint32 gain_geophone3: 8;
    uint32 gain_geophone2: 8;
    uint32 gain_geophone1: 8;
    uint32 crc:8;
    //second word
    uint32 mux_geophone3: 8;
    uint32 mux_geophone2: 8;
    uint32 mux_geophone1: 8;
    uint32 aux_data_low: 8;
    //third word
    uint32 : 32;
    //last word
}control_ind_adc_frame;

typedef struct {
    uint32 TEST_FREQ: 8;
    uint32 TEST_AMP: 8;
    uint32 INT_TEST: 8;
    uint32 subframe: 3;
    uint32 fake: 1;
    uint32 frame: 3;
    uint32 label: 1;
    //first word
    uint32 : 8;
    uint32 CMRR_SIGNAL: 8;
    uint32 VIBRATOR_SIGNAL: 8;
    uint32 crc:8;
    //second word
    uint32 : 32;
    //third word
    uint32 : 32;
    //last word
}control_gen_adc_frame;
// 

typedef struct {
    uint32 MacMiLo:8;
    uint32 MacMiHi:8;
    uint32 MacHiLo:8;
    uint32 MacHiHi:8;
    uint32 port:16;
    uint32 MacLoLo:8;
    uint32 MacLoHi:8;
    uint32 UniLoLo:8;
    uint32 UniLoHi:8;
    uint32 UniHiLo:8;
    uint32 UniHiHi:8;
    uint32 MulLoLo:8;
    uint32 MulLoHi:8;
    uint32 MulHiLo:8;
    uint32 MulHiHi:8;
}renumFrame_t;
#define MAX_USEC (1024000L)
typedef struct {
    uint32 disc_period: 24; // discretisation period
    uint32 tag: 8; 
    uint32 time_corr: 24; // time correction
    uint8 CRC; //
    uint32 sec: 32; // 
    uint32 usec: 24;
    uint32 : 8;
}TimeStamp_t;

typedef struct{
    uint32 /*EF_1*/: 28;
    uint32 tag: 4;
    uint32 EF_2;
    uint32 EF_3;
    uint32 EF_4;
}elementaryFrame_t;

typedef union{
    elementaryFrame_t eF;
    control_gen_adc_frame GenADC;
    control_ind_adc_frame IndADC;
    renumFrame_t rF;
    TimeStamp_t TimeStamp; 
}AnyFrame_u;


extern control_ind_adc_frame ind_control_adc;
extern control_gen_adc_frame gen_control;

void CopyFirstFrame(void);

#endif

/* [] END OF FILE */
