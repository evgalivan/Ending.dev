/*******************************************************************************
* File Name: UpdateAllowed.h
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
#if !defined(CY_ISR_UpdateAllowed_H)
#define CY_ISR_UpdateAllowed_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void UpdateAllowed_Start(void);
void UpdateAllowed_StartEx(cyisraddress address);
void UpdateAllowed_Stop(void);

CY_ISR_PROTO(UpdateAllowed_Interrupt);

void UpdateAllowed_SetVector(cyisraddress address);
cyisraddress UpdateAllowed_GetVector(void);

void UpdateAllowed_SetPriority(uint8 priority);
uint8 UpdateAllowed_GetPriority(void);

void UpdateAllowed_Enable(void);
uint8 UpdateAllowed_GetState(void);
void UpdateAllowed_Disable(void);

void UpdateAllowed_SetPending(void);
void UpdateAllowed_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the UpdateAllowed ISR. */
#define UpdateAllowed_INTC_VECTOR            ((reg32 *) UpdateAllowed__INTC_VECT)

/* Address of the UpdateAllowed ISR priority. */
#define UpdateAllowed_INTC_PRIOR             ((reg8 *) UpdateAllowed__INTC_PRIOR_REG)

/* Priority of the UpdateAllowed interrupt. */
#define UpdateAllowed_INTC_PRIOR_NUMBER      UpdateAllowed__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable UpdateAllowed interrupt. */
#define UpdateAllowed_INTC_SET_EN            ((reg32 *) UpdateAllowed__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the UpdateAllowed interrupt. */
#define UpdateAllowed_INTC_CLR_EN            ((reg32 *) UpdateAllowed__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the UpdateAllowed interrupt state to pending. */
#define UpdateAllowed_INTC_SET_PD            ((reg32 *) UpdateAllowed__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the UpdateAllowed interrupt. */
#define UpdateAllowed_INTC_CLR_PD            ((reg32 *) UpdateAllowed__INTC_CLR_PD_REG)


#endif /* CY_ISR_UpdateAllowed_H */


/* [] END OF FILE */
