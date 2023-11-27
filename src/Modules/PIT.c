/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       PIT.c$
* @file             PIT.c
*
* $Date:            Jul-29-2014$
* @date             Jul-29-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      PIT driver source file
* @brief            PIT driver source file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage PIT driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains PIT driver for MPC5744P allowing to 
*	initialize and configure PIT module.
*
*  The key features of this package are the following:
*  - Initialize PIT for the desired time period
*  - Launch and stop PIT for specified channel
*  For more information about the functions and configuration items see these documents: 
*
*******************************************************************************
*
* @attention 
*            
*******************************************************************************/
/*==================================================================================================
*   Project              : PowerSBC
*   Platform             : MPC5744P
*   Dependencies         : MPC5744P - Basic SW drivers.
*   All Rights Reserved.
==================================================================================================*/

/*==================================================================================================
Revision History:
                             Modification     Function
Author (core ID)              Date D/M/Y       Name		  Description of Changes
B35993		 				  29/07/2014 	   ALL		  Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

#include "MPC5744P_drv.h"
//#include "typedefs.h"
#include "PIT.h"

/***************************************************************************//*!
*   @brief The function PIT_Init initializes the Periodic Interrupt Timer.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function turns on clock for the PIT module. 
*	@remarks 	In debug mode is the PIT module frozen by default.
*	@par Code sample
*			PIT_Init();
*			- Command activates clock for the PIT module.
********************************************************************************/
void PIT_Init(void)
{
	PIT_0.MCR.B.MDIS=0; 	// Module clock on
	PIT_0.MCR.B.FRZ=1;  	// Freeze timer in debug	
}

/***************************************************************************//*!
*   @brief The function PIT_Setup computes and sets up a period for the 
*			specified channel.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function computes register settings from clock frequency and expected time base.
*	@param[in] Channel 
*					Number of the PIT's channel (from 0 till 3).
*	@param[in] Clock_Freq
*					System clock frequency in Mhz
*	@param[in] ExpectedTimeBase
*					Desired time period for the specified channel in µsec
*	@remarks 	Maximal setable time is given by the maximal value of the 32-bit LDVAL register divided 
*				by system the clock frequency ((2^32)/fsys). PIT module should be initialized before 
*				(see PIT_Init function).
*	@par Code sample
*			PIT_Setup(0, 16, 10000);
*			- Command sets up channel no. 0 of the timer with expected time 
*			base 10 ms assuming clock frequency 16 MHz.
********************************************************************************/
void PIT_Setup(int8_t Channel, uint32_t Clock_Freq, uint32_t ExpectedTimeBase)
{
	PIT_0.TIMER[Channel].LDVAL.R=(int32_t)Clock_Freq*ExpectedTimeBase-1;
}

/***************************************************************************//*!
*   @brief The function PIT_EnableInt enables an interrupt for a specified channel.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function enables an interrupt for a specified channel 
*					(sets TIE bit) and clears 
*					interrupt flag (TIF) in the same time.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_EnableInt(1);
*			- Command enables interruption for the first channel of the PIT.
********************************************************************************/
void PIT_EnableInt(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TIE = 1; 
    PIT_0.TIMER[Channel].TFLG.R = 1;
}
 
/***************************************************************************//*!
*   @brief The function PIT_EnableChannel enables a channel of the PIT.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function enables specified channel by setting TEN bit.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_EnableInt(2);
*			- Command enables second channel of the PIT.
********************************************************************************/
void PIT_EnableChannel(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TEN=1;
}

/***************************************************************************//*!
*   @brief The function PIT_Disable disables a channel of the PIT 
*			(including interrupts).
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function disables the whole  PIT channel. The associated 
*					interrupt flag is cleared and the interrupt deisabled in the 
*					same time. 
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_Disable(3);
*			- Command disables channel no. 3 of the PIT.
********************************************************************************/
void PIT_Disable(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TIE=0;
    PIT_0.TIMER[Channel].TCTRL.B.TEN=0;
    PIT_0.TIMER[Channel].TFLG.R=1;
}

/***************************************************************************//*!
*   @brief The function PIT_GetFlag returns an interrupt flag.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function returns the interrupt flag (TIF) of the 
*					specified channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@return Time Interrupt Flag.
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			while(PIT_GetFlag(3)==0);
*			- Command waits until the PIT channel no. 3 will overflow (Simple 
*			waiting without any interruption).
********************************************************************************/
int8_t PIT_GetFlag(int8_t Channel)
{
   return (int8_t)PIT_0.TIMER[Channel].TFLG.B.TIF;
}

/***************************************************************************//*!
*   @brief The function PIT_ClearFlag clears an interrupt flag.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function clears Time Interrupt Flag (TIF) for the specified channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_ClearFlag(0);
*			- Command clears TIF flag of the channel no. 0.
********************************************************************************/
void PIT_ClearFlag(int8_t Channel)
{
    PIT_0.TIMER[Channel].TFLG.R=1;
}

/***************************************************************************//*!
*   @brief The function PIT_DisableChannel disables a PIT channel.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function clears Timer Enable Bit (TEN) for the specified PIT channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_DisableChannel(1);
*			- Command disables first PIT channel.
********************************************************************************/
void PIT_DisableChannel(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TEN=0;
}

/***************************************************************************//*!
*   @brief The function PIT_IsChannelEnabled checks if a specified PIT channel 
*			is enabled.
*	@par Include 
*					PIT.h
* 	@par Description 
*					This function returns content of the Timer Enable Bit (TEN) for the specified PIT channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@return Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			res = PIT_IsChannelEnabled(0);
*			- Command returns state of PIT channel number 0.
********************************************************************************/
uint32_t PIT_IsChannelEnabled(int8_t Channel)
{
	return PIT_0.TIMER[Channel].TCTRL.B.TEN;			//returns 1 if PIT channel is enabled or 0
}

/***************************************************************************//*!
*   @brief The function PIT_wait_micsec 

********************************************************************************/
void PIT_wait_micsec(uint32_t duration)
{
    PIT_DisableChannel(2);
    PIT_ClearFlag(2);
    PIT_Setup(2,PIT_CLK/1000000,duration);
    PIT_EnableChannel(2);
    do;
    while (PIT_GetFlag(2)==0);
}
    




