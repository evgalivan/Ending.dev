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
#ifndef GLOBAL_H
    #define GLOBAL_H
#endif

#ifndef uint32 
    #include <project.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#undef ABS
#define ABS(A) ((A)<0?(-(A)):(A))

#define LENGTH_OF(Array) (sizeof(Array)/sizeof(Array[0]))
#define RING_SIZE 64
#define IN_RING( B)  if (++(B)>=RING_SIZE)(B) -= RING_SIZE
#define LENGTH_OF(Array) (sizeof(Array)/sizeof(Array[0]))

typedef enum {WAITINGOFDOLLAR, WAITINGOFSTAR, WAITINGOFCHSUM, WAITINGOFCHSUM2} State;
typedef enum {INFRAME, OUTFRAME} ss_State;


typedef struct{
    uint8 buf[RING_SIZE];
    uint8 begin;
    uint8 end;
}RingBuff_t;

typedef struct{    
    RingBuff_t primary_buf;
    uint8 sentence[256];
    uint16 length;
    State state;
    uint16 count_char;
    uint16 checksum;
    uint8 sentence_ready;
} uart_context;

extern uint32 incr_compare;
extern long long utc_time;
extern long long pps_time;
extern uint32 capture_flag;
extern uint32 UpdateFrequencyFlag;
extern volatile long  NewFrequency;
extern long   LowFrequency;
extern long   HighFrequency;
extern ss_State ssState;
extern uint8 alert_flag;
extern volatile uint8 timer;

uint8 ByteToInt(uint8);
uint8 IsNotEmpty(RingBuff_t*);
uint8 InsertByte(RingBuff_t* , uint8);
uint8 ExtractByte(RingBuff_t*);
uint8 agg_sent(uart_context*);

/*comp_cap_tc.c*/

void PrepareNewFrequency(void);


/* [] END OF FILE */
