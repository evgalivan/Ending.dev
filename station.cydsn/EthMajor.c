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

static renumFrame_t pattern;
static renumFrame_t keep={0,0,0,0x80,
    5555,0,0,
    0,0,0,0,
    0,0,0,1
};

major_t Eth_compare = {
    .Flag = 0,
    .Counter = 0,
    .BeginOff = 0,
    .Pattern = (void*)&pattern,
    .lenght = 16
};

void* CheckRenumber(void* ToCheck){
    int8 result = ComparePattern( ToCheck, Eth_compare);
    if(result == 0){//equal
        if(Eth_compare.Counter < MAJOR_COUNTER){
            Eth_compare.Counter++;
            if(Eth_compare.Counter >= MAJOR_COUNTER){
                keep = *(renumFrame_t*)ToCheck;
                SetMac((AnyFrame_u*)&keep);
                SetPort(((AnyFrame_u*)&keep)->rF.port);
                SetUnicast((AnyFrame_u*)&keep);
                SetMulticast((AnyFrame_u*)&keep);
                
                EthHeaderMustBeUpdated =1;
            }
        }
    }else{
        Eth_compare.Counter =0;
        pattern = *(renumFrame_t*)ToCheck;
    }
    return (void*)&keep;
}
/* [] END OF FILE */
