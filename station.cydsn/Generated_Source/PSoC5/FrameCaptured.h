/*******************************************************************************
* File Name: FrameCaptured.h
* Version 1.71
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_FrameCaptured_H)
#define CY_ISR_FrameCaptured_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void FrameCaptured_Start(void);
void FrameCaptured_StartEx(cyisraddress address);
void FrameCaptured_Stop(void);

CY_ISR_PROTO(FrameCaptured_Interrupt);

void FrameCaptured_SetVector(cyisraddress address);
cyisraddress FrameCaptured_GetVector(void);

void FrameCaptured_SetPriority(uint8 priority);
uint8 FrameCaptured_GetPriority(void);

void FrameCaptured_Enable(void);
uint8 FrameCaptured_GetState(void);
void FrameCaptured_Disable(void);

void FrameCaptured_SetPending(void);
void FrameCaptured_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the FrameCaptured ISR. */
#define FrameCaptured_INTC_VECTOR            ((reg32 *) FrameCaptured__INTC_VECT)

/* Address of the FrameCaptured ISR priority. */
#define FrameCaptured_INTC_PRIOR             ((reg8 *) FrameCaptured__INTC_PRIOR_REG)

/* Priority of the FrameCaptured interrupt. */
#define FrameCaptured_INTC_PRIOR_NUMBER      FrameCaptured__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable FrameCaptured interrupt. */
#define FrameCaptured_INTC_SET_EN            ((reg32 *) FrameCaptured__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the FrameCaptured interrupt. */
#define FrameCaptured_INTC_CLR_EN            ((reg32 *) FrameCaptured__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the FrameCaptured interrupt state to pending. */
#define FrameCaptured_INTC_SET_PD            ((reg32 *) FrameCaptured__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the FrameCaptured interrupt. */
#define FrameCaptured_INTC_CLR_PD            ((reg32 *) FrameCaptured__INTC_CLR_PD_REG)


#endif /* CY_ISR_FrameCaptured_H */


/* [] END OF FILE */
