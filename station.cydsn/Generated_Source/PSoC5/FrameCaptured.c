/*******************************************************************************
* File Name: FrameCaptured.c  
* Version 1.71
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <FrameCaptured.h>
#include "cyapicallbacks.h"

#if !defined(FrameCaptured__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START FrameCaptured_intc` */
    
    
#include <Counter_4.h>
#include <Counter_3.h>
#include <RestartAcc.h>
#include <line_buf.h>
#include <PacketLevel.h>
#include <eth_ip_udp.h>
#include <sender.h>
#include <receiver.h>

    
    
    
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: FrameCaptured_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_Start(void)
{
    /* For all we know the interrupt is active. */
    FrameCaptured_Disable();

    /* Set the ISR to point to the FrameCaptured Interrupt. */
    FrameCaptured_SetVector(&FrameCaptured_Interrupt);

    /* Set the priority. */
    FrameCaptured_SetPriority((uint8)FrameCaptured_INTC_PRIOR_NUMBER);

    /* Enable it. */
    FrameCaptured_Enable();
}


/*******************************************************************************
* Function Name: FrameCaptured_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    FrameCaptured_Disable();

    /* Set the ISR to point to the FrameCaptured Interrupt. */
    FrameCaptured_SetVector(address);

    /* Set the priority. */
    FrameCaptured_SetPriority((uint8)FrameCaptured_INTC_PRIOR_NUMBER);

    /* Enable it. */
    FrameCaptured_Enable();
}


/*******************************************************************************
* Function Name: FrameCaptured_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_Stop(void)
{
    /* Disable this interrupt. */
    FrameCaptured_Disable();

    /* Set the ISR to point to the passive one. */
    FrameCaptured_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: FrameCaptured_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for FrameCaptured.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(FrameCaptured_Interrupt)
{
    #ifdef FrameCaptured_INTERRUPT_INTERRUPT_CALLBACK
        FrameCaptured_Interrupt_InterruptCallback();
    #endif /* FrameCaptured_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START FrameCaptured_Interrupt` */
    status_u CauseOfInterrupt;
    FrameCaptured_ClearPending();
    CauseOfInterrupt.reg= Counter_4_ReadStatusRegister();
    if(CauseOfInterrupt.field.Overflow){ 
        pre_capt_flag = 0;
    }else  if(CauseOfInterrupt.field.Capture){
        
        while(CauseOfInterrupt.field.FIFO_Not_Empty){
            LastCapturedPeriod = Counter_4_ReadCapture();
            CauseOfInterrupt.reg= Counter_4_ReadStatusRegister();
        }
        
        if( pre_capt_flag){           //calculate the adequate value
            uint32 PartOfDelayCompensation = DelayCompensation>>1;
            ValueOfNewPeriod = LastCapturedPeriod - PartOfDelayCompensation;
            Counter_3_WritePeriod( ValueOfNewPeriod);
            ValueOfNewPeriod = LastCapturedPeriod - (DelayCompensation- PartOfDelayCompensation);
        }
        else pre_capt_flag = 1; //only the next captured value will be adequate
        //here we will wait until incoming datagram finished
        while(flag_write_done == 0){
//            if(Packet_prepared){
//                if(current_send < current_write){
//                    AddUint32IntoPacket(*current_send);//
//                    current_send++;
//                }
//            }
        }
        if(Packet_prepared){
//            while(current_send < current_write){
//                AddUint32IntoPacket(*current_send);
//                current_send++;
//            }
            if(SectionsToSend){
                AddTheSectionIntoPacket((void*)&in_buf.buf[0],sizeof(in_buf));
                SectionsToSend--;
                if(SectionsToSend == 0){
                    ResumeThePacket();
                }
            }
        }
        {
            //implying regeneration frame rule depending on type - here
            AnyFrame_u *last_frame =  &in_buf.Frames[FRAME_NUM-1];
            
            switch(last_frame->eF.tag){
            case TAG_RENUM://copy to mine & inc
                SetMac(last_frame);
                last_frame->rF.MacLoLo++;// MAC inc
                SetPort(last_frame->rF.port++);// port inc
                SetUnicast(last_frame);
                last_frame->rF.UniLoLo++; // Unicast inc
                SetMulticast(last_frame);
                EthHeaderMustBeUpdated = 1;// new field values must be placed into corresponding fields of Ethernet packet header     
            break;
            case TAG_TIME_STAMP:
                last_frame->TimeStamp.usec += last_frame->TimeStamp.disc_period;
                if(last_frame->TimeStamp.usec >= MAX_USEC){
                    last_frame->TimeStamp.usec -= MAX_USEC;
                    last_frame->TimeStamp.sec++;
                }
            break;
            default:
            break;
            }
            
            for(int i =0; i < FRAME_NUM; i++){
                out_buf.Frames[i] = *last_frame;
            }
            if(PrepareDone){
                PrepareToSend();
                Send();
            }
            PrepareToStore();
        }
    }
    
    /* `#END` */
}


/*******************************************************************************
* Function Name: FrameCaptured_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling FrameCaptured_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use FrameCaptured_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)FrameCaptured__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: FrameCaptured_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress FrameCaptured_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)FrameCaptured__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: FrameCaptured_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling FrameCaptured_Start or FrameCaptured_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after FrameCaptured_Start or FrameCaptured_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_SetPriority(uint8 priority)
{
    *FrameCaptured_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: FrameCaptured_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 FrameCaptured_GetPriority(void)
{
    uint8 priority;


    priority = *FrameCaptured_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: FrameCaptured_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_Enable(void)
{
    /* Enable the general interrupt. */
    *FrameCaptured_INTC_SET_EN = FrameCaptured__INTC_MASK;
}


/*******************************************************************************
* Function Name: FrameCaptured_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 FrameCaptured_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*FrameCaptured_INTC_SET_EN & (uint32)FrameCaptured__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: FrameCaptured_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_Disable(void)
{
    /* Disable the general interrupt. */
    *FrameCaptured_INTC_CLR_EN = FrameCaptured__INTC_MASK;
}


/*******************************************************************************
* Function Name: FrameCaptured_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void FrameCaptured_SetPending(void)
{
    *FrameCaptured_INTC_SET_PD = FrameCaptured__INTC_MASK;
}


/*******************************************************************************
* Function Name: FrameCaptured_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void FrameCaptured_ClearPending(void)
{
    *FrameCaptured_INTC_CLR_PD = FrameCaptured__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
