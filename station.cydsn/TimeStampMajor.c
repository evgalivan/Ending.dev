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
#include "major.h"
#include "ControlFrame.h"
#include <RestartAcc.h>
#include "eth_ip_udp.h"

static TimeStamp_t ts_pattern;
static TimeStamp_t ts_keep={1024,0x80,
    0,0,
    0,
    1
};

major_t ts_compare = {
    .Flag = 0,
    .Counter = 0,
    .BeginOff = 0,
    .Pattern = (void*)&ts_pattern,
    .lenght = 8
};

static uint32 RegenerationPeriod =1000;//1ms

void* CheckTS(void* ToCheck){
    int8 result = ComparePattern( ToCheck, ts_compare);
    if(result == 0){//equal
        if(ts_compare.Counter < MAJOR_COUNTER){
            ts_compare.Counter++;
            if(ts_compare.Counter >= MAJOR_COUNTER){
                ts_keep = *(TimeStamp_t*)ToCheck;
                SequenceNumber = ts_keep.CRC;
                ts_keep.CRC++;

                switch(ts_keep.disc_period){//
                case 512:
                    RegenerationPeriod =500;
                    break;
                case 256:
                    RegenerationPeriod =250;
                    break;
                default:
                case 1024:
                    RegenerationPeriod =1000;
                    break;
                }

                DelayCompensation = ts_keep.time_corr;


                EthHeaderMustBeUpdated =1;

            }
        }else{
            ts_keep = *(TimeStamp_t*)ToCheck;
        }
    }else{
        ts_compare.Counter =0;
        ts_pattern = *(TimeStamp_t*)ToCheck;
    }

    ts_keep.usec += RegenerationPeriod;

    if(ts_keep.usec >= MAX_USEC){
        ts_keep.usec -= MAX_USEC;
        ts_keep.sec++;
    }

    ts_keep.usec |= 1;//stop bit
    ts_keep.CRC = SequenceNumber;//renumbering

    return (void*)&ts_keep;
}
/* [] END OF FILE */
