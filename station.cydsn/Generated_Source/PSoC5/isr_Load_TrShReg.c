/*******************************************************************************
* File Name: isr_Load_TrShReg.c  
* Version 1.70
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
#include <isr_Load_TrShReg.h>
#include "cyapicallbacks.h"

#if !defined(isr_Load_TrShReg__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_Load_TrShReg_intc` */
#include <TransmitShiftReg.h>
#include <StartTransmit.h>
#include <sender.h>    
#include <BitCounterEnc.h>
#include <isr_TransmitWordShift.h>
#include <line_buf.h>
//#include <FirstLoad.h>
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
* Function Name: isr_Load_TrShReg_Start
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
void isr_Load_TrShReg_Start(void)
{
    /* For all we know the interrupt is active. */
    isr_Load_TrShReg_Disable();

    /* Set the ISR to point to the isr_Load_TrShReg Interrupt. */
    isr_Load_TrShReg_SetVector(&isr_Load_TrShReg_Interrupt);

    /* Set the priority. */
    isr_Load_TrShReg_SetPriority((uint8)isr_Load_TrShReg_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_Load_TrShReg_Enable();
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_StartEx
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
void isr_Load_TrShReg_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    isr_Load_TrShReg_Disable();

    /* Set the ISR to point to the isr_Load_TrShReg Interrupt. */
    isr_Load_TrShReg_SetVector(address);

    /* Set the priority. */
    isr_Load_TrShReg_SetPriority((uint8)isr_Load_TrShReg_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_Load_TrShReg_Enable();
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_Stop
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
void isr_Load_TrShReg_Stop(void)
{
    /* Disable this interrupt. */
    isr_Load_TrShReg_Disable();

    /* Set the ISR to point to the passive one. */
    isr_Load_TrShReg_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for isr_Load_TrShReg.
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
CY_ISR(isr_Load_TrShReg_Interrupt)
{
    #ifdef isr_Load_TrShReg_INTERRUPT_INTERRUPT_CALLBACK
        isr_Load_TrShReg_Interrupt_InterruptCallback();
    #endif /* isr_Load_TrShReg_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START isr_Load_TrShReg_Interrupt` */
/*
    //   if (StartTransmit_Read() != 0)
        Load();

    *(char*)(&curStat)=TransmitShiftReg_SR_STATUS;
//    if( TransmitShiftReg_RET_FIFO_EMPTY == TransmitShiftReg_GetFIFOStatus(TransmitShiftReg_IN_FIFO)){
    if(curStat.F0_empty != 0){
        StartTransmit_Write(0);
        BitCounterEnc_ReadStatusRegister( );
        isr_TransmitWordShift_ClearPending( );
        isr_TransmitWordShift_Enable( );
    }
    */
    
    register uint8 tmp = StartTransmit_Read(); // ?????????? ???????????????????? ??????????????????
    StartTransmit_Write(tmp|2); //???? pin 00 ?????????????? ?????????? ?? ????????????????????
    if ((tmp & 0x1) != 0){ //???????? ?????????????? ???????????????? ????????????, ???? ???? ??????????????????
        while(count_to_send){
            if((TransmitShiftReg_SR_STATUS & 0x10) ==0 )break; //full
            TransmitShiftReg_WriteData(*(current_read++));
            count_to_send--;
        }
    }
    if((TransmitShiftReg_SR_STATUS&8) != 0){ //empty
        tmp = 0;//?????????? ???????????????????????? ???????????????? ??????????
        BitCounterEnc_ReadStatusRegister();
        isr_TransmitWordShift_ClearPending();
        isr_TransmitWordShift_Enable();
        //TransmitShiftReg_WriteData(0xffffffff);//???????????? ???????????????????? ?????????? ?? ??????????????, ?????????? ?????????????????? ???????? ??????????????.
        isr_Load_TrShReg_Disable();
    }
    else     BitCounterEnc_ReadStatusRegister();//?????????????? ???????????????????? ?????????????? ????????????????????

    
    /*if ((StartTransmit_Read() & 0x1) == 0){
        //status = 0;
        flag_read_done = 1;
        isr_TransmitWordShift_Disable();
    }*/
    StartTransmit_Write(tmp);
    

    TransmitShiftReg_GetIntStatus();//???????????????????? ???????????????????? ?????????????? ????????????????????
    isr_Load_TrShReg_ClearPending();//?????????????? ???????????????????? ????????????????????
    
    /* `#END` */
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling isr_Load_TrShReg_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use isr_Load_TrShReg_StartEx instead.
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
void isr_Load_TrShReg_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)isr_Load_TrShReg__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_GetVector
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
cyisraddress isr_Load_TrShReg_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)isr_Load_TrShReg__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling isr_Load_TrShReg_Start or isr_Load_TrShReg_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after isr_Load_TrShReg_Start or isr_Load_TrShReg_StartEx has been called. 
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
void isr_Load_TrShReg_SetPriority(uint8 priority)
{
    *isr_Load_TrShReg_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_GetPriority
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
uint8 isr_Load_TrShReg_GetPriority(void)
{
    uint8 priority;


    priority = *isr_Load_TrShReg_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_Enable
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
void isr_Load_TrShReg_Enable(void)
{
    /* Enable the general interrupt. */
    *isr_Load_TrShReg_INTC_SET_EN = isr_Load_TrShReg__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_GetState
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
uint8 isr_Load_TrShReg_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*isr_Load_TrShReg_INTC_SET_EN & (uint32)isr_Load_TrShReg__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_Disable
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
void isr_Load_TrShReg_Disable(void)
{
    /* Disable the general interrupt. */
    *isr_Load_TrShReg_INTC_CLR_EN = isr_Load_TrShReg__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_SetPending
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
void isr_Load_TrShReg_SetPending(void)
{
    *isr_Load_TrShReg_INTC_SET_PD = isr_Load_TrShReg__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_Load_TrShReg_ClearPending
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
void isr_Load_TrShReg_ClearPending(void)
{
    *isr_Load_TrShReg_INTC_CLR_PD = isr_Load_TrShReg__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
