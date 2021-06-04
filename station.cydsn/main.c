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


#include <global.h>
#include "sender.h"
#include "receiver.h"
#include <BitCounterDec.h>
#include <line_buf.h>
#include <FirstFrame.h>
#include <ControlFrame.h>
#include "hbi.h"
#include <KSZ8852.h>
#include <eth_ip_udp.h>
#include <PacketLevel.h>
#include <RestartAcc.h>
uint32 incr_compare = 512; // зависит от той частоты, которую мы хотим получить

uint32 massage[1] = { 0xFAAAAAAF };

uint32 tmp;

uint8 timer_config = 0;
uint8 state_adc_config = 0;
uint8 reset_flag = 0;
uint8 test_reg, test_reg2;
ss_State ssState = OUTFRAME;
uint32 divide_stay;


#define LENGTH_OF(Array) (sizeof(Array)/sizeof(Array[0]))

volatile long   NewFrequency;
long   LowFrequency;
long   HighFrequency;
uint32 error_counter = 0;

control_ind_adc_frame ind_control_adc;
control_gen_adc_frame gen_control;


int main(void)

{   
    KSZ8852Reset();   
    CyGlobalIntDisable; /* Enable global interrupts. */
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    // Init Encoder devices
    TransmitShiftReg_Start( );
    BitCounterEnc_Start( );
    
	// Init Decoder devices
	RecieveShiftReg_Start() ;
	//Waiter_Start() ;
    BitCounterDec_Start() ;
    VDAC8_1_Start();
    Comp_1_Start();
    Comp_2_Start();
    Opamp_1_Start();
    
    // Init Clock devices

    EndFrame_Start();
    isr_Load_TrShReg_Start();
    //isr_Load_TrShReg_Disable();
	WordShifted_Start();
    isr_TransmitWordShift_Start( );
    isr_TransmitWordShift_Disable( );
    
    /*Init delay compensation*/
    Counter_2_Start();
    Counter_2_WritePeriod( ValueOfNewPeriod);
    Counter_3_Start();
    Counter_3_WritePeriod( ValueOfNewPeriod);
    Counter_4_Start();
    
    UpdateAllowed_Start();
    FrameCaptured_Start();
    
    /*Call func PrepareToStore until initialisation*/
    PrepareToStore();
    KSZ8852BusInit();
    KSZ8852_Init();
    while(frame_pin_Read()==0);
    Counter_4_ReadStatusRegister(); /*clear all events*/
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    FrameAllow_Write(0);
    flag_read_done = 0;
    PrepareANDSend(); // зарядить ОДНО слово!
    //Send(); //передать его безусловно
    while (flag_read_done == 0);
    FrameAllow_Write(1);
    PrepareDone = 1;
    uint16 packet_size;
    #define NUMBER_OF_SECTIONS (11)
    packet_size = RefreshItem(NUMBER_OF_SECTIONS*sizeof(in_buf));
    EthHeaderMustBeUpdated =1;
//only if You known about it
    while(1) 
    {
       
        /*  ======================================================
         *   flag_write_done set 1 in EndFrame interrupte
         *   flag_read_done set 1 in isr_Load_TrShReg interrupte
         *  ======================================================
        */
        if(Packet_prepared == 0){
            if ( EthHeaderMustBeUpdated ){
                ConfigureMAC( mac_addr);
                packet_size = RefreshItem(NUMBER_OF_SECTIONS*sizeof(in_buf));
            }
            StartNewUDPPacket((void*)&item, packet_size);
            SectionsToSend = NUMBER_OF_SECTIONS;
            Packet_prepared = 1;
        }
        
    }
}
/* [] END OF FILE */