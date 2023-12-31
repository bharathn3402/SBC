/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       ADC.c$
* @file             ADC.c
*
* $Date:            Jul-04-2014$
* @date             Jul-04-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      ADC driver source file
* @brief            ADC driver source file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage ADC driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains ADC driver for MPC5744P allowing to configure and 
*	control ADC module. Driver is not portable on another platform without 
*	additional changes. 

*  The key features of this package are the following:
*  - Initializate ADC module for Normal and Injected conversions
*  - Change setting of the ADC module
*  - Launch Normal and Injected conversions
*  - Treat results from ADC conversions
*
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
B35993		 				  04/07/2014 	   ALL		  Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/


#include <math.h>
#include "MPC5744P_drv.h"
#include "SIUL.h"
#include "ADC.h"

/****************************************************************************
* STATUS functions
****************************************************************************/

/***************************************************************************//*!
*   @brief The function ADC_IsNormalConvRunning indicates whether the Normal 
*			conversion of the ADCx is in process.
*	@par Include 
*					ADC.h
* 	@par Description
*					 This function returns a 1 if the normal conversion is 
*					 in process (if NSTART bit is set); otherwise it returns 0.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	
*				- "1" - if normal conversion is in process 
*				- "0" - if normal conversion has stopped
*	@remarks ADCx must be initialized before using of this function using of this function (see ADC_Init function for 
*			 details).
*	@par Code sample
*			ADC_IsNormalConvRunning(1);
*			- Command returns NSTART status bit of ADC1.
********************************************************************************/
uint8_t ADC_IsNormalConvRunning(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.NSTART;
}

/***************************************************************************//*!
*   @brief The function ADC_IsInjectedConvAborted indicates whether the Injected 
*			conversion of the ADCx was aborted.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function returns JABORT status bit. This status bit is used 
*					to signal that an injected conversion has been aborted. 
*					This bit is reset when a new injected conversion starts.
*
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	
*				- "1" - if injected conversion was aborted 
*				- "0" - if injected conversion was not aborted
*	@remarks ADCx must be initialized before using of this function (see ADC_Init function for 
*			 details).
*	@par Code sample
*			ADC_IsInjectedConvAborted(0);
*			- Command returns JABORT status bit of ADC0.
********************************************************************************/
uint8_t ADC_IsInjectedConvAborted(uint8_t nbADC){		//injected conversion aborted ?
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.JABORT;
}

/***************************************************************************//*!
*   @brief The function ADC_IsInjectedConvRunning indicates whether the Injected 
*			conversion of the ADCx is in process.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function returns 1 if injected conversion is in process 
*					(if JSTART bit is set), otherwise it returns 0.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	- "1" - if injected conversion is in process 
*				- "0" - if injected conversion has stopped
*	@remarks ADCx must be initialized before using of this function (see ADC_Init function for 
*			 details).
*	@par Code sample
*			ADC_IsInjectedConvRunning(1);
*			- Command returns JSTART status bit of ADC1.
********************************************************************************/
uint8_t ADC_IsInjectedConvRunning(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.JSTART;
}


/***************************************************************************//*!
*   @brief The function ADC_IsCTUconvRunning indicates an ongoing CTU conversion 
*			for ADC.
*	@par Include 
*					ADC.h
* 	@par Description 
*			 This function returns the CTUSTART status bit. It is used to 
*			signal a CTU conversion is ongoing, and is set when a CTU trigger pulse 
*			is received and the CTU conversion starts. This bit automatically resets 
*			when the conversion is completed and the CTU trigger mode is enabled. 
*			Otherwise, if the Control mode is enabled, it resets when the CTU is 
*			disabled (CTUEN set to 0). 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	
*				- "1" - if CTU conversion is ongoing
*				- "0" - if CTU conversion has stopped
*	@remarks ADCx must be initialized before using of this function (see ADC_Init function for 
*			 details).
*	@par Code sample
*			ADC_IsCTUconvRunning(0);
*			- Command returns CTUSTART status bit of ADC0.
********************************************************************************/
uint8_t ADC_IsCTUconvRunning(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.CTUSTART;
}

/***************************************************************************//*!
*   @brief The function ADC_GetCurrentChannelAddress returns a number of ADC 
*			channels that are currently in the conversion.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function returns address (number) of the channel, which 
*					is actually being converted.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	Number of the channel currently in conversion (from 0 till 15)
*	@remarks ADCx must be initialized before using of this function (see ADC_Init function for 
*			 details).
*	@par Code sample
*			ADC_GetCurrentChannelAddress(1);
*			- Command returns number of the channel currently being converted 
*			in ADC1.
********************************************************************************/
uint8_t ADC_GetCurrentChannelAddress(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.CHADDR;
}

/***************************************************************************//*!
*   @brief The function ADC_GetAutoClockOffState returns status of the Auto 
*			clock off state bit for ADC.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function returns status of the auto-clock-off feature (ACK0 status 
*					bit).
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	
*				- "1" - if auto-clock-off feature is disabled 
*				- "0" - if auto-clock-off feature is enabled
*	@remarks 	Auto-clock-off feature allows processor to switch off CLK 
*				for ADC automatically if no conversion is ongoing. This reduces 
*				power consumption without switching to the power down mode. 
*				ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_GetAutoClockOffState(0);
*			- Command returns ACK0 status bit of ADC0.
********************************************************************************/
uint8_t ADC_GetAutoClockOffState(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.ACKO;					}

/***************************************************************************//*!
*   @brief The function ADC_GetStatus returns the current status of ADC.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function returns status of the specified ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@return 	
*				- "000" - Idle			
*				- "001"	- Power-down		
*				- "010"	- Wait state		
*				- "011"	- X				
*				- "100"	- Sample			
*				- "101"	- X				
*				- "110"	- Conversion		
*				- "111"	- X				
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_GetStatus(1);
*			- Command returns status of ADC1.
********************************************************************************/
uint8_t ADC_GetStatus(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	return (uint8_t)p_ADC->MSR.B.ADCSTATUS;
}

/****************************************************************************
* Basic ADC functions
****************************************************************************/

/***************************************************************************//*!
*	@brief The function ADC_Init initializes registers of ADC.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function initializes registers of the ADCx and activates 
*					channels, which are masked in function parameters. 
*					Configuration of the registers can be changed through 
*					the config parameter.
*   @brief Initialization of ADC registers.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@param[in] chMaskInjected 	
*				Mask of channels for injected conversion.
*				Predefined values:
*				- CHSmask - Mask for ALL standard channels
*				- CH0Smask - Mask for standard channel no.0
*				- CH1Smask - Mask for standard channel no.1
*				- CH2Smask - Mask for standard channel no.2
*				- CH3Smask - Mask for standard channel no.3
*				- CH4Smask - Mask for standard channel no.4
*				- CH5Smask - Mask for standard channel no.5
*				- CH6Smask - Mask for standard channel no.6
*				- CH7Smask - Mask for standard channel no.7
*				- CH8Smask - Mask for standard channel no.8
*				- ch.9 - DO NOT USE - internally connected
*				- ch.10 - DO NOT USE - internally connected
*				- CH11Smask - Mask for standard channel no.11
*				- CH12Smask - Mask for standard channel no.12
*				- CH13Smask - Mask for standard channel no.13
*				- CH14Smask - Mask for standard channel no.14
*				- ch.15 - DO NOT USE - internally connected to TSENS_0
* 	@param[in] chMaskNormal 
*				Mask of channels for normal conversion. 
*				Predefined values are the same as for injected 
*				conversion.
*	@param[in] config 
*				Configuration setting for the Main Control Register (MCR).
*				- OWRITE_DIS - Prevents overwrite of unread converted data. 
*				New result is discarded.
*				- OWRITE_EN - Enables converted data to be overwritten by 
*				a new conversion.
*				- ALIGN_R - The conversion data is written right-aligned.
*				- ALIGN_L - Data is left-aligned (from 15 to 
*				(15 - resolution + 1)).
*				- ONE_SHOT - One Shot Mode - Configures the normal conversion 
*				of one chain.
*				- SCAN - Scan Mode - Configures continuous chain conversion 
*				mode. When the programmed chain conversion is finished it 
*				restarts immediately.
*				- PWDN_DIS - ADC is in normal mode.
*				- PWDN_EN - ADC has been requested to power down.
*				- ACKO_EN - Auto clock off is enabled.
*				- ACKO_DIS - Auto clock off is disabled.
*			 	- DEFAULT_ADC - Default configuration defined as follows:
*			 	
*	@remarks Configuration parameters (instead of DEFAULT_ADC) as well as 
*				channel masks should be combined arbitrarily using logical 
*				OR (|) operator, please do not use DEFAULT_ADC with other 
*				parameters.
*	@par Code sample1
*			ADC_Init(0, CH0Smask | CH5Smask, 0, DEFAULT_ADC);
*			- Command will initialize channels 0 and 5 of ADC0 in injected 
*			mode with DEFAULT configuration.
*	@par Code sample2
*			ADC_Init(1, CH0Smask, CH2Smask | CH4Smask, OWRITE_DIS | 
*						ONE_SHOT |  PWDN_DIS | ALIGN_R  | ACKO_DIS);
*			- This command initializes the channels of the ADC1 (channel 0 in injected 
*			mode, channels 2 and 4 in normal mode) with the specified parameters 
*			(overwrite disabled, one shot mode, power down disabled, result 
*			right aligned, auto clock off disabled).
*******************************************************************************/
void ADC_Init(uint8_t nbADC, uint64_t chMaskNormal, uint64_t chMaskInjected, uint32_t config){		//uint8_t nbADC, uint32_t sysCLK
		volatile struct ADC_tag *p_ADC;
		
		uint64_t chMask = 0;
		chMask = chMaskNormal | chMaskInjected;
	//pointer settings	
		switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
				
//ADCs with 12bit precision
//Standard inputs
//channels 9, 10, 15 internally connected
	switch(nbADC){
		case 0 :	//ADC0	
					if(chMask & CH0Smask)	SIUL_AnalogInput(SIUL_PB7, 0);		//AN0
					if(chMask & CH1Smask)	SIUL_AnalogInput(SIUL_PB8, 0);		//AN1
					if(chMask & CH2Smask)	SIUL_AnalogInput(SIUL_PC1, 0);		//AN2
					if(chMask & CH3Smask)	SIUL_AnalogInput(SIUL_PC2, 0);		//AN3
					if(chMask & CH4Smask)	SIUL_AnalogInput(SIUL_PE6, 0);		//AN4
					if(chMask & CH5Smask)	SIUL_AnalogInput(SIUL_PE2, 0);		//AN5
					if(chMask & CH6Smask)	SIUL_AnalogInput(SIUL_PE7, 0);		//AN6
					if(chMask & CH7Smask)	SIUL_AnalogInput(SIUL_PE4, 0);		//AN7
					if(chMask & CH8Smask)	SIUL_AnalogInput(SIUL_PE5, 0);		//AN8
					if(chMask & CH11Smask)	SIUL_AnalogInput(SIUL_PB9, 0);		//AN11	shared between ADC0 and ADC1
					if(chMask & CH12Smask)	SIUL_AnalogInput(SIUL_PB10, 0);		//AN12	shared between ADC0 and ADC1
					if(chMask & CH13Smask)	SIUL_AnalogInput(SIUL_PB11, 0);		//AN13	shared between ADC0 and ADC1
					if(chMask & CH14Smask)	SIUL_AnalogInput(SIUL_PB12, 0);		//AN14	shared between ADC0 and ADC1	
		break;
		case 1 :	//ADC1
					if(chMask & CH0Smask)	SIUL_AnalogInput(SIUL_PB13, 0);		//AN0
					if(chMask & CH1Smask)	SIUL_AnalogInput(SIUL_PB14, 0);		//AN1
					if(chMask & CH2Smask)	SIUL_AnalogInput(SIUL_PB15, 0);		//AN2
					if(chMask & CH3Smask)	SIUL_AnalogInput(SIUL_PC0, 0);		//AN3
					if(chMask & CH4Smask)	SIUL_AnalogInput(SIUL_PE11, 0);		//AN4
					if(chMask & CH5Smask)	SIUL_AnalogInput(SIUL_PE0, 0);		//AN5
					if(chMask & CH6Smask)	SIUL_AnalogInput(SIUL_PE12, 0);		//AN6
					if(chMask & CH7Smask)	SIUL_AnalogInput(SIUL_PE9, 0);		//AN7
					if(chMask & CH8Smask)	SIUL_AnalogInput(SIUL_PE10, 0);		//AN8
					if(chMask & CH11Smask)	SIUL_AnalogInput(SIUL_PB9, 0);		//AN11	shared between ADC0 and ADC1
					if(chMask & CH12Smask)	SIUL_AnalogInput(SIUL_PB10, 0);		//AN12	shared between ADC0 and ADC1
					if(chMask & CH13Smask)	SIUL_AnalogInput(SIUL_PB11, 0);		//AN13	shared between ADC0 and ADC1
					if(chMask & CH14Smask)	SIUL_AnalogInput(SIUL_PB12, 0);		//AN14	shared between ADC0 and ADC1	
		break;
		case 2 :	//ADC2
					if(chMask & CH0Smask)	SIUL_AnalogInput(SIUL_PJ5, 0);		//AN0	shared between ADC2 and ADC3
					if(chMask & CH1Smask)	SIUL_AnalogInput(SIUL_PJ6, 0);		//AN1	shared between ADC2 and ADC3
					if(chMask & CH2Smask)	SIUL_AnalogInput(SIUL_PJ7, 0);		//AN2	shared between ADC2 and ADC3
					if(chMask & CH4Smask)	SIUL_AnalogInput(SIUL_PE6, 0);		//AN4	shared between ADC0 and ADC2
		break;
		case 3 :	//ADC3
					if(chMask & CH0Smask)	SIUL_AnalogInput(SIUL_PJ5, 0);		//AN0	shared between ADC2 and ADC3
					if(chMask & CH1Smask)	SIUL_AnalogInput(SIUL_PJ6, 0);		//AN1   shared between ADC2 and ADC3
					if(chMask & CH2Smask)	SIUL_AnalogInput(SIUL_PJ7, 0);		//AN2   shared between ADC2 and ADC3
					if(chMask & CH3Smask)	SIUL_AnalogInput(SIUL_PE11, 0);		//AN3	shared between ADC1 and ADC3
					if(chMask & CH4Smask)	SIUL_AnalogInput(SIUL_PE0, 0);		//AN4   shared between ADC1 and ADC3
					if(chMask & CH5Smask)	SIUL_AnalogInput(SIUL_PE12, 0);		//AN5   shared between ADC1 and ADC3
					if(chMask & CH6Smask)	SIUL_AnalogInput(SIUL_PE9, 0);		//AN6	shared between ADC1 and ADC3
					if(chMask & CH7Smask)	SIUL_AnalogInput(SIUL_PE10, 0);		//AN7   shared between ADC1 and ADC3
		break;                                                                          
	
	
	}
	
		 	p_ADC->MCR.R = config;
		
			//Injected conversion
			p_ADC->JCMR0.R = (vuint32_t)chMaskInjected;		//sampling enabled for masked channels
						
			//Normal conversion
			p_ADC->NCMR0.R = (vuint32_t)chMaskNormal;		//sampling enabled for masked channels
		
			//Timing register
			p_ADC->CTR0.R = DEFAULT_CTR;		
}


/***************************************************************************//*!
*   @brief The function ADC_StartNormalConversion starts conversion of ADC 
*			in a Normal mode.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function starts normal conversion ofthe ADC specified by 
*					the nbADC. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@param[in] chMaskNormal
*				Mask with channels used for Normal conversion.
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_StartNormalConversion(0);
*			- Command starts normal conversion of ADC0.
********************************************************************************/
void ADC_StartNormalConversion(uint8_t nbADC, uint32_t chMaskNormal){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	p_ADC->NCMR0.R = chMaskNormal;		//sampling enabled for masked channels
	
	p_ADC->MCR.B.NSTART = 1;
}

/***************************************************************************//*!
*   @brief The function ADC_StopConversion stops all on-going conversions of 
*			ADC immediately.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function stops all conversions (injected as well as normal) 
*					of ADC specified by the nbADC number immediately.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_StopConversion(1);
*			- Command stops all conversions of ADC1.
********************************************************************************/
void ADC_StopConversion(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	p_ADC->MCR.B.NSTART = 0;
	p_ADC->MCR.B.JSTART = 0;	
}

/***************************************************************************//*!
*   @brief The function ADC_StartInjectedConversion starts conversion of ADC 
*			in the Injected mode.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function starts injected conversion of ADC specified by 
*					the nbADC. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@param[in] chMaskInjected 
*				Mask with channels used for Injected conversion.
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_StartInjectedConversion(0);
*			- Command starts injected conversion of ADC0.
********************************************************************************/
void ADC_StartInjectedConversion(uint8_t nbADC, uint64_t chMaskInjected){
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	p_ADC->JCMR0.R = (vuint32_t)chMaskInjected;		//sampling enabled for masked channels
	
	p_ADC->MCR.B.JSTART = 1;	
}

/***************************************************************************//*!
*   @brief The function ADC_SetOneShotMode sets a One Shot conversion mode for
*			the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function sets one shot mode for ADC specified by the nbADC. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_SetOneShotMode(1);
*			- Command sets one shot mode for ADC1.
********************************************************************************/
void ADC_SetOneShotMode(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
		p_ADC->MCR.B.MODE = 0;			//one shot mode
}

/***************************************************************************//*!
*   @brief The function ADC_SetScanMode sets a Cyclic conversion chain mode 
*			(Scan mode) for ADC.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function sets cyclic conversion chain (scan mode) for 
*					ADC specified by the nbADC. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_SetScanMode(1);
*			- Command sets one scan mode for ADC1.
********************************************************************************/
void ADC_SetScanMode(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
		
		p_ADC->MCR.B.MODE = 1;			//scan mode
}

/***************************************************************************//*!
*   @brief The function ADC_AutoClockOffEnable enables Auto clock off feature 
*			for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function enables AutoClockOff feature for ADC specified 
*					by the nbADC. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* @remarks The AutoClockOff feature allows the processor to switch off 
*			CLK for ADC automatically, if no conversion is ongoing. This reduces 
*			power consumption without switching to the power down mode. ADCx must be 
*			initialized beforehand (see ADC_Init function for details). 
*	@par Code sample
*			ADC_AutoClockOffEnable(0);
*			- Command enables the AutoClockOff feature for ADC0.
********************************************************************************/
void ADC_AutoClockOffEnable(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
		p_ADC->MCR.B.ACKO = 1;
}

/***************************************************************************//*!
*   @brief The function ADC_AutoClockOffDisable disables AutoClockOff feature 
*			for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function disables the AutoClockOff feature for ADC specified 
*					by the nbADC. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	The AutoClockOff feature allows processor to switch off CLK for 
*				ADC automatically if no conversion is ongoing. This reduces 
*				power consumption without switching to the power down mode. 
*				the ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_AutoClockOffDisable(1);
*			- Command disables AutoClockOff feature for ADC1.
********************************************************************************/
void ADC_AutoClockOffDisable(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
		p_ADC->MCR.B.ACKO = 0;
}

/***************************************************************************//*!
*   @brief The function ADC_GetChannelValue returns the value converted by the ADCx 
*			from the specified channel.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function waits until data of the specified channel is valid 
*					(conversion has been finished). Then it reads the 12-bit 
*					CDATA result and returns it. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@param[in] nbCH 
*				Number of the channel to be read.
*	@remarks 	The function returns only <i>right aligned</i> data from the CDATA register. 
*				If the ADC is configured to be left aligned, this function will 
*				return only the eight lower bits shifted by four to the left.
*	@par Code sample
*			ADC_GetChannelValue(1, 0);
*			- Command returns result of the conversion for ADC1, channel 0.
********************************************************************************/
uint16_t ADC_GetChannelValue(uint8_t nbADC, uint32_t nbCH){
	volatile struct ADC_tag *p_ADC;
	
	uint16_t result = 0;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	
	while (p_ADC->CDR[nbCH].B.VALID != 1){}; 		//Wait for last scan to complete
	result= (uint16_t)p_ADC->CDR[nbCH].B.CDATA; 				//Read conversion result data

	return result;	
}

/****************************************************************************
* Interrupt functions
****************************************************************************/

/***************************************************************************//*!
*   @brief The function ADC_SetInt sets interrupt masks for specified channels 
*			and flags for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function initializes interruption for specified ADC (nbADC) 
*					with masked flags (intFlagMask) for masked channels 
*					(intChannelMask).
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@param[in] intFlagMask 
*				Mask to enable different interrupt flags.
*				Predefined values:
*				- EOCTU_FLAG - End of CTU Conversion interrupt enable
*				- JEOC_FLAG - End of Injected Channel Conversion interrupt enable
*				- JECH_FLAG - End of Injected Chain Conversion interrupt enable
*				- EOC_FLAG - End of Channel Conversion interrupt enable
*				- ECH_FLAG - End of Chain Conversion interrupt enable
*	@param[in] intChannelMask 
*				Interrupt mask for channels. 
*				Predefined values:
*				- CHSmask - Mask for ALL standard channels
*				- CH0Smask - Mask for standard channel no.0
*				- CH1Smask - Mask for standard channel no.1
*				- CH2Smask - Mask for standard channel no.2
*				- CH3Smask - Mask for standard channel no.3
*				- CH4Smask - Mask for standard channel no.4
*				- CH5Smask - Mask for standard channel no.5
*				- CH6Smask - Mask for standard channel no.6
*				- CH7Smask - Mask for standard channel no.7
*				- CH8Smask - Mask for standard channel no.8
*				- ch.9 - DO NOT USE - internally connected
*				- ch.10 - DO NOT USE - internally connected
*				- CH11Smask - Mask for standard channel no.11
*				- CH12Smask - Mask for standard channel no.12
*				- CH13Smask - Mask for standard channel no.13
*				- CH14Smask - Mask for standard channel no.14
*				- ch.15 - DO NOT USE - internally connected to TSENS_0
*	@remarks 	Interrupt masks as well as channel interrupt masks should be 
*				combined arbitrarily using logical OR (|) operator. ADCx has 
*				to be initialized before (see ADC_Init function for details).
*	@par Code sample
*			ADC_SetInt(1, ECH_FLAG | EOC_FLAG, CH1Smask | CH2Smask | CH3Smask);
*			- Command sets ECH and EOC interrupts for channels 1, 2 and 3 in 
*			ADC1. 
********************************************************************************/
void ADC_SetInt(uint8_t nbADC, uint32_t intFlagMask, uint32_t intChannelMask){
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	

	p_ADC->IMR.R = intFlagMask;				//flag selection	
	p_ADC->CIMR0.R = 	intChannelMask;			//channel mask
	
}

/***************************************************************************//*!
*   @brief The function ADC_ClearEOCflag clears End of Channel conversion flag 
*			for specified channels of the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears the End of Channel Conversion (EOC) flags, which are 
*					specified by the intFlagClearMask.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
* 	@param[in] intFlagClearMask
*				The higher 16 bits (16 till 31) correspond 
*				to interrupt flags of 16 channels.
*				Predefined values:
*				- CHSmask - Mask for ALL standard channels
*				- CH0Smask - Mask for standard channel no.0
*				- CH1Smask - Mask for standard channel no.1
*				- CH2Smask - Mask for standard channel no.2
*				- CH3Smask - Mask for standard channel no.3
*				- CH4Smask - Mask for standard channel no.4
*				- CH5Smask - Mask for standard channel no.5
*				- CH6Smask - Mask for standard channel no.6
*				- CH7Smask - Mask for standard channel no.7
*				- CH8Smask - Mask for standard channel no.8
*				- ch.9 - DO NOT USE - internally connected
*				- ch.10 - DO NOT USE - internally connected
*				- CH11Smask - Mask for standard channel no.11
*				- CH12Smask - Mask for standard channel no.12
*				- CH13Smask - Mask for standard channel no.13
*				- CH14Smask - Mask for standard channel no.14
*				- ch.15 - DO NOT USE - internally connected to TSENS_0
*	@remarks 	As a mask we can use directly 16-bit binary value (logical 1 will 
*				clear the flag, logical 0 will not make any influence) or 
*				predefined masks like in the previous cases. Each bit corresponds 
*				to 1 channel (bit no.16 corresponds to channel no.15, bit 31 to 
*				channel 0).
*	@par Code sample
*			void ADC_ClearEOCflag(0, 0b101);
*			- Command clears EOC flags of channels no.2 and 0 in ADC0
********************************************************************************/
void ADC_ClearEOCflag(uint8_t nbADC,uint32_t intFlagClearMask){
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
		
	p_ADC->CEOCFR0.R = intFlagClearMask;	//clear EOC flag for each channel
	p_ADC->ISR.B.EOC = 1;							//clear EOC flag in global
}

/***************************************************************************//*!
*   @brief The function ADC_ClearAllEOCflags clears End of Channel conversion 
*			flag for all channels of the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears the End of Channel (EOC) conversion flags for all 
*					channels of the specified ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			void ADC_ClearAllEOCflags(1);
*			- Command clears EOC flags of all channels as well as the common EOC flag of ADC1.
********************************************************************************/
void ADC_ClearAllEOCflags(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	p_ADC->CEOCFR0.R = 0xFFFFFFFF;		//clear all end of conversion flags
	p_ADC->ISR.B.EOC = 1;					//clear EOC flag in global
}

/***************************************************************************//*!
*   @brief The function ADC_ClearEOCTUflag clears End of CTU conversion flag 
*			for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears the End of CTU conversion flag for the specified ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			void ADC_ClearEOCTUflag(0);
*			- Command clears EOCTU interrupt flag of ADC0.
********************************************************************************/
void ADC_ClearEOCTUflag(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	p_ADC->ISR.B.EOCTU = 1;		//clear CTU interrupt conversion flag
}

/***************************************************************************//*!
*   @brief The function ADC_ClearJEOCflag clears End of Injected Channel conversion 
*			flag for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears the End of Injected Channel (JEOC) conversion flag for 
*					the specified ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			void ADC_ClearJEOCflag(1);
*			- Command clears JEOC interrupt flag of ADC1.
********************************************************************************/
void ADC_ClearJEOCflag(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	p_ADC->ISR.B.JEOC = 1;		//clear End of Injected Channel Conversion interrupt flag
}

/***************************************************************************//*!
*   @brief The function ADC_ClearJECHflag clears End of Injected Chain conversion 
*			flag for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears the End of Injected Chain (JECH) conversion flag 
*					for the specified ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			void ADC_ClearJECHflag(0);
*			- Command clears JECH interrupt flag of ADC0.
********************************************************************************/
void ADC_ClearJECHflag(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	p_ADC->ISR.B.JECH = 1;		//clear End of Injected Chain Conversion interrupt flag
}

/***************************************************************************//*!
*   @brief The function ADC_ClearECHflag clears End of Chain conversion flag 
*			for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears the End of Chain (ECH) conversion interrupt flag 
*					for the specified ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			void ADC_ClearECHflag(1);
*			- Command clears ECH interrupt flag of ADC1.
********************************************************************************/
void ADC_ClearECHflag(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	p_ADC->ISR.B.ECH = 1;		//clear End of Chain Conversion interrupt flag
}


/****************************************************************************
* Threshold functions
****************************************************************************/

/***************************************************************************//*!
*   @brief The function ADC_SetThldRegister initializes of the Thresholdx registers 
*			for the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function initializes thresholdx (specified by nbThld) with 
*					low and high limit (valueL, valueH). For each channel we 
*					can explicitly enable or disable interrupts corresponding 
*					to the set range. These interrupts are maskable using the 
*					intMask parameter, where each channel has two bits to mask 
*					high and low (higher than valueH and lower than valueL) 
*					interruption independently.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@param[in] nbThld 
*				Number of threshold to be initialized (from 0 till 3).
*	@param[in] valueL 
*				12bit low value of the threshold (from 0 till 4095)
*	@param[in] valueH 
*				12bit high value of the threshold (from 0 till 4095).
*	@param[in] intMask 
*				Interruption mask where each ADC channel has 2 maskable 
*				bits corresponding to high and low 
*				(higher than valueH and lower than valueL) interruption.		
*	@remarks 	Interruption mask should be done with respect to the bit 
*				distribution (see img for details). ADCx must be initialized 
*				before (see ADC_Init function for details).
*	@par Code sample
*			ADC_SetThldRegister(0, 2, 100, 400, 0x00000003);
*			- Command sets threshold register no.2 in ADC0 with the threshold 
*			range between 100 and 400. Interruption will be called if conversion 
*			of channel no.0 will be out of range (lower or higher than the set 
*			values).
********************************************************************************/
void ADC_SetThldRegister(uint8_t nbADC, uint8_t nbThld, uint16_t valueL, uint16_t valueH, uint32_t intMask){
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	switch(nbThld){											//choose base DSPI address
			case 0 :
		             p_ADC->THRHLR0.B.THRH = valueH;
                             p_ADC->THRHLR0.B.THRL = valueL;
                             break;
			case 1 :
		             p_ADC->THRHLR1.B.THRH = valueH;
                             p_ADC->THRHLR1.B.THRL = valueL;
                             break;
			case 2 :
		             p_ADC->THRHLR2.B.THRH = valueH;
                             p_ADC->THRHLR2.B.THRL = valueL;
                             break;
			case 3 :
		             p_ADC->THRHLR3.B.THRH = valueH;
                             p_ADC->THRHLR3.B.THRL = valueL;
                             break;
			case 4 :
		             p_ADC->THRHLR4.B.THRH = valueH;
                             p_ADC->THRHLR4.B.THRL = valueL;
                             break;
			case 5 :
		             p_ADC->THRHLR5.B.THRH = valueH;
                             p_ADC->THRHLR5.B.THRL = valueL;
                             break;
			case 6 :
		             p_ADC->THRHLR6.B.THRH = valueH;
                             p_ADC->THRHLR6.B.THRL = valueL;
                             break;
			case 7 :
		             p_ADC->THRHLR7.B.THRH = valueH;
                             p_ADC->THRHLR7.B.THRL = valueL;
                             break;
			case 8 :
		             p_ADC->THRHLR8.B.THRH = valueH;
                             p_ADC->THRHLR8.B.THRL = valueL;
                             break;
			case 9 :
		             p_ADC->THRHLR9.B.THRH = valueH;
                             p_ADC->THRHLR9.B.THRL = valueL;
                             break;
			case 10 :
		             p_ADC->THRHLR10.B.THRH = valueH;
                             p_ADC->THRHLR10.B.THRL = valueL;
                             break;
			case 11 :
		             p_ADC->THRHLR11.B.THRH = valueH;
                             p_ADC->THRHLR11.B.THRL = valueL;
                             break;
			case 12 :
		             p_ADC->THRHLR12.B.THRH = valueH;
                             p_ADC->THRHLR12.B.THRL = valueL;
                             break;
			case 13 :
		             p_ADC->THRHLR13.B.THRH = valueH;
                             p_ADC->THRHLR13.B.THRL = valueL;
                             break;
			case 14 :
		             p_ADC->THRHLR14.B.THRH = valueH;
                             p_ADC->THRHLR14.B.THRL = valueL;
                             break;
			case 15 :
		             p_ADC->THRHLR15.B.THRH = valueH;
                             p_ADC->THRHLR15.B.THRL = valueL;
                             break;
        }
	
	p_ADC->WTIMR.R = intMask;
}

/***************************************************************************//*!
*   @brief The function assigns Thresholdx to specified channel of the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function assigns threshold (nbThld) to the specified 
*					channel (nbCH).
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@param[in] nbCH 
*				Number of channel (from 0 till 15).
*	@param[in] nbThld 
*				Number of threshold (from 0 till 3).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_SetThldForChannel(1, 4, 2);
*			- Command assigns threshold no.2 to channel no.4 of ADC1. 
********************************************************************************/
void ADC_SetThldForChannel(uint8_t nbADC, uint32_t nbCH, uint8_t nbThld){		
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	switch(nbCH){
		case 0  : p_ADC->CWSELR0.B.WSEL_CH0 = nbThld; 
							p_ADC->CWENR0.B.CWEN0 = 1; break;
		case 1  : p_ADC->CWSELR0.B.WSEL_CH1 = nbThld; 
							p_ADC->CWENR0.B.CWEN1 = 1; break;
		case 2  : p_ADC->CWSELR0.B.WSEL_CH2 = nbThld;  
							p_ADC->CWENR0.B.CWEN2 = 1; break;
		case 3  : p_ADC->CWSELR0.B.WSEL_CH3 = nbThld; 
							p_ADC->CWENR0.B.CWEN3 = 1; break;
		case 4  : p_ADC->CWSELR0.B.WSEL_CH4 = nbThld; 
							p_ADC->CWENR0.B.CWEN4 = 1; break;
		case 5  : p_ADC->CWSELR0.B.WSEL_CH5 = nbThld; 
							p_ADC->CWENR0.B.CWEN5 = 1; break;
		case 6  : p_ADC->CWSELR0.B.WSEL_CH6 = nbThld; 
							p_ADC->CWENR0.B.CWEN6 = 1; break;
		case 7  : p_ADC->CWSELR0.B.WSEL_CH7 = nbThld; 
							p_ADC->CWENR0.B.CWEN7 = 1; break;
		case 8  : p_ADC->CWSELR1.B.WSEL_CH8 = nbThld; 
							p_ADC->CWENR0.B.CWEN8 = 1; break;
		case 9  : p_ADC->CWSELR1.B.WSEL_CH9 = nbThld; 
							p_ADC->CWENR0.B.CWEN9 = 1; break;
		case 10 : p_ADC->CWSELR1.B.WSEL_CH10 = nbThld; 
							p_ADC->CWENR0.B.CWEN10 = 1; break;
		case 11 : p_ADC->CWSELR1.B.WSEL_CH11 = nbThld; 
							p_ADC->CWENR0.B.CWEN11 = 1; break;
		case 12 : p_ADC->CWSELR1.B.WSEL_CH12 = nbThld; 
							p_ADC->CWENR0.B.CWEN12 = 1; break;
		case 13 : p_ADC->CWSELR1.B.WSEL_CH13 = nbThld; 
							p_ADC->CWENR0.B.CWEN13 = 1; break;
		case 14 : p_ADC->CWSELR1.B.WSEL_CH14 = nbThld; 
							p_ADC->CWENR0.B.CWEN14 = 1; break;
		case 15 : p_ADC->CWSELR1.B.WSEL_CH15 = nbThld; 
							p_ADC->CWENR0.B.CWEN15 = 1; break;
	}	
}

/***************************************************************************//*!
*   @brief The function clears Threshold flags of the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function clears all threshold interrupt flags of specified 
*					ADC.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_ClearThldFlags(0);
*			- Command clears all threshold interrupt flags for ADC0.
********************************************************************************/
void ADC_ClearThldFlags(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	p_ADC->WTISR.R = 0xFFFFFFFF;
}


/****************************************************************************
* Presampling functions
****************************************************************************/

/***************************************************************************//*!
*   @brief The function ADC_SetVDDforPresampling sets a positive power supply for the presampling of the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function sets the positive power supply for the presampling 
*					period of the ADC conversion.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@param[in] channelMask 
*				Mask indicates which channels will use presampling 
*				with Vdd.
*				Predefined values:
*				- CHSmask - Mask for ALL standard channels
*				- CH0Smask - Mask for standard channel no.0
*				- CH1Smask - Mask for standard channel no.1
*				- CH2Smask - Mask for standard channel no.2
*				- CH3Smask - Mask for standard channel no.3
*				- CH4Smask - Mask for standard channel no.4
*				- CH5Smask - Mask for standard channel no.5
*				- CH6Smask - Mask for standard channel no.6
*				- CH7Smask - Mask for standard channel no.7
*				- CH8Smask - Mask for standard channel no.8
*				- ch.9 - DO NOT USE - internally connected
*				- ch.10 - DO NOT USE - internally connected
*				- CH11Smask - Mask for standard channel no.11
*				- CH12Smask - Mask for standard channel no.12
*				- CH13Smask - Mask for standard channel no.13
*				- CH14Smask - Mask for standard channel no.14
*				- ch.15 - DO NOT USE - internally connected to TSENS_0
*	@remarks 	Function in the same time enables presampling phase for the 
*				specified channels. Presampling values cannot be set separately 
*				for different channels so the presampling voltage should be 
*				either positive or negative (ground). Channel masks should be combined arbitrarily 
*				using logical OR (|) operator. ADCx must be initialized before using of this function 
*				(see ADC_Init function for details).
*	@par Code sample
*			ADC_SetVDDforPresampling(1, CH2Smask | CH3Smask);
*			- Command enables presampling for channels 2 and 3 of the ADC1 and 
*			sets Vdd as the presampling voltage at the same time.
********************************************************************************/
void ADC_SetVDDforPresampling(uint8_t nbADC, vuint32_t channelMask){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	p_ADC->PSCR.B.PREVAL0 = VDD;		//set Vdd/Vss voltage for presampling		
	p_ADC->PSR0.R = channelMask;								//activate presampling for masked channels
	p_ADC->PSCR.B.PRECONV = 0;							//presampling + sampling + conversion

}

/***************************************************************************//*!
*   @brief The function ADC_SetVSSforPresampling sets a negative power supply 
*			for presampling of the ADCx.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function sets the negative power supply for the presampling 
*					period of the ADC conversion.
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@param[in] channelMask Mask 
*				Indicates which channels will use presampling 
*				with Vdd.
*				Predefined values:
*				- CHSmask - Mask for ALL standard channels
*				- CH0Smask - Mask for standard channel no.0
*				- CH1Smask - Mask for standard channel no.1
*				- CH2Smask - Mask for standard channel no.2
*				- CH3Smask - Mask for standard channel no.3
*				- CH4Smask - Mask for standard channel no.4
*				- CH5Smask - Mask for standard channel no.5
*				- CH6Smask - Mask for standard channel no.6
*				- CH7Smask - Mask for standard channel no.7
*				- CH8Smask - Mask for standard channel no.8
*				- ch.9 - DO NOT USE - internally connected
*				- ch.10 - DO NOT USE - internally connected
*				- CH11Smask - Mask for standard channel no.11
*				- CH12Smask - Mask for standard channel no.12
*				- CH13Smask - Mask for standard channel no.13
*				- CH14Smask - Mask for standard channel no.14
*				- ch.15 - DO NOT USE - internally connected to TSENS_0			
*	@remarks 	Function in the same time enables presampling phase for the 
*				specified channels. Presampling values cannot be set separately 
*				for different channels so the presampling voltage should be 
*				either positive or negative (ground). Channel masks should be combined arbitrarily 
*				using logical OR (|) operator. ADCx must be initialized before using of this function 
*				(see ADC_Init function for details).
*	@par Code sample
*			ADC_SetVSSforPresampling(0, CH2Smask | CH3Smask);
*			- Command enables presampling for channels 2 and 3 of the ADC0 and 
*			sets Vss as the presampling voltage at the same time.
********************************************************************************/
void ADC_SetVSSforPresampling(uint8_t nbADC, vuint32_t channelMask){
	volatile struct ADC_tag *p_ADC;
	
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	
	p_ADC->PSCR.B.PREVAL0 = VSS;		//set Vdd/Vss voltage for presampling		
	p_ADC->PSR0.R = channelMask;								//activate presampling for masked channels
	p_ADC->PSCR.B.PRECONV = 0;							//presampling + sampling + conversion

}

/***************************************************************************//*!
*   @brief The function ADC_DisableSampleBypass enables the sample phase 
*			for the ADCx => presampling + sampling + conversion.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function disables bypassing of the sampling phase so the s
*					conversion consists of the following phases: 
*					(presampling) + sampling + conversion. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_DisableSampleBypass(1);
*			- Command enables sampling phase (disables sampling bypass) for 
*			the ADC1 conversions.
********************************************************************************/
void ADC_DisableSampleBypass(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	p_ADC->PSCR.B.PRECONV = 0;		//presampling + sampling + conversion
}

/***************************************************************************//*!
*   @brief The function ADC_EnableSampleBypass bypasses the sample phase of the ADCx => presampling + conversion.
*	@par Include 
*					ADC.h
* 	@par Description 
*					This function enables bypassing of the sampling phase so the 
*					conversion consists of the following phases: 
*					(presampling) + conversion. 
* 	@param[in] nbADC 
*				Number of ADC module (0 or 1).
*	@remarks 	ADCx must be initialized before using of this function (see ADC_Init function for 
*				details).
*	@par Code sample
*			ADC_EnableSampleBypass(0);
*			- Command enables sampling phase for the ADC0 conversions.
********************************************************************************/
void ADC_EnableSampleBypass(uint8_t nbADC){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
	
	p_ADC->PSCR.B.PRECONV = 1;		//presampling + conversion
}

/***************************************************************************//*!
*   @brief Computes/sets register for sampling phase duration of ADCx.
*	@par Include: 
*					ADC.h
* 	@par Description: 
*					Function computes register (INPSAMP) setting depending on 
*					the module clock frequency (sysClk) and on the desired 
*					sampling phase duration.
* 	@param[in] nbADC - Number of ADC module (0 or 1).
*	@param[in] sysClk - Frequency of the system clock.
*	@param[in] samplingTime - 	Desired sampling time. Time is limited by 
*								its maximal and minimal values depending on 
*								the clock frequency and ADCLKSEL bit 
*								(ADCLKSEL = 0 by default).
*	@remarks 	Values of the frequency should be added in basic units [Hz] 
*				as well as in their multiples, but everytime with respect to 
*				the units of sampling time. The ratio of their multiples has 
*				to be everytime equal to 1. E.g. [Hz]/[s], [kHz]/[ms], 
*				[MHz]/[us] etc. Minimal and maximal time possible to set depend 
*				on the clock frequency and on the ADCLKSEL bit setting 
*				(see external Excel file for these values).
********************************************************************************/
void ADC_SetSamplingTime(uint8_t nbADC, uint32_t sysClk, float samplingTime){
		volatile struct ADC_tag *p_ADC;
		
		float Tck;
		uint32_t INSAMPvalue = 0;
		
		//pointer settings	
		switch(nbADC){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;	
		}
		
		
		if(p_ADC->MCR.B.ADCLKSEL == 1){		//timing period Tck
			Tck = (float)2.0/sysClk;
		}
		else{
			Tck = (float)1.0/sysClk;
		}
		
		INSAMPvalue = (samplingTime/Tck) + 1;
		
		if((INSAMPvalue >= 8) && (INSAMPvalue<= 255)){		//if value is in the borders
			p_ADC->CTR0.B.INPSAMP = INSAMPvalue;							//sampling time
		}
		else{
			if(INSAMPvalue < 8){						//if smaller than set the smallest possible value
				p_ADC->CTR0.B.INPSAMP = 8;
			}
			else{														//if greater than set the greatest possible value
				p_ADC->CTR0.B.INPSAMP = 255;
			}
			
			
		}		
	
}














