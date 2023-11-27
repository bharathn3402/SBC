/*******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ********************************************************************************
 *
 * $File Name:       Main.c$
 * @file             Main.c
 *
 * $Date:            July-3rd-2015$
 * @date             July-3rd-2015$
 *
 * $Version:         0.9$
 * @version          0.9
 *
 * Description      FS65xx (PwSBC) DEMO source file
 * @brief           FS65xx (PwSBC) DEMO source file
 *
 * --------------------------------------------------------------------
 * $Name:  $ test0
 *******************************************************************************/
/****************************************************************************/
/*
 *  @mainpage FS65xx (PwSBC) DEMO for MPC5744P
 *
 *  @section Intro Introduction
 *
 *	This DEMO SW is dedicated to evaluation and debug purposes of the FS65xx
 *	(PwSBC) device using enhanced Evaluation Board (EVM). DEMO SW uses
 *	MPC5744P Basic Drivers and the PwSBC driver.
 *
 *  The key features of this package are the following:
 *	- Power management
 *	- Event handling
 *	- Interrupt handling
 *	- Windowed real-time WD refresh with control computations
 *	- ADC handling
 *	- UART communication
 *	- Interrupt priority handling
 *  	- Communication with the Graphical User Interface (GUI)
 *
 *  For more information about the functions and configuration items see these
 *  documents:
 *
 ******************************************************************************
 *
 * @attention
 *
 *******************************************************************************/
/*==================================================================================================
 *   Project              : 	FS65xx drivers
 *   Platform             : 	MPC5744P
 *   Dependencies         : 	MPC5744P - Basic SW drivers.
 *				MPC5744P - MC33907_8 (PwSBC) driver
 *				enhanced Evaluation Board (EVM)
 *   All Rights Reserved.
 ==================================================================================================*/

/*==================================================================================================
  Revision History:
  Modification     Function
  Author (core ID)              Date D/M/Y       Name		  Description of Changes
  B35993		 	      13/08/2014       ALL		  Driver created
  BRNY001			      26/10/2015       ALL		  Driver adapted to FS65xx
  ---------------------------   ----------    ------------  ------------------------------------------
  ==================================================================================================*/

#include "derivative.h"
#include "FS65xx_driver.h"
#include "MPC5744P_drv.h"
#include "DSPI.h"
#include "ME.h"
#include "LINFLEX.h"
#include "ADC.h"
#include "FCCU.h"
#include "SIUL.h"
#include "PIT.h"
#include "CAN.h"

#define FORCE_FS65_INIT

__attribute__ ((section(".text")))
extern void xcptn_xmpl(void);

/**********************************************************************/
/* Functions tested with the MPC5744P-257DC + the FS6522 demo board   */
/**********************************************************************/

int main()
{

    uint32_t error_code;
    uint8_t index;

/* Enable all PBridge Masters for Reads, Writes, and Master Privilege Mode. */
    AIPS_0.MPRA.R = 0x77777777;
    AIPS_1.MPRA.R = 0x77777777;

/* Configure FCCU */
    FCCU_SetProtocol(FCCU_BI_STABLE);		//set Bi-Stable protocol if not in the SAFE mode

/* Init MODE ENTRY module */
    MC_MODE_INIT();                             /* (pll_init) Setup the MCU clocks and modes */

/* Init interrupt controller */
    xcptn_xmpl ();              /* Configure and Enable Interrupts */

/* INIT SIUL */
    //PA0 interfaces IRQ signal from FS65xx
    SIUL_DigitalInput(SIUL_PA0,173,0,1);	//MPC5744P:P8[1] to FS65:J37[12]
    SIUL_EnableExtIntFallingEdge(1);
    SIUL_EnableExtInt(1);

    //PA1 connected to LED2
    SIUL_DigitalIO(SIUL_PA1,0);				//MPC5744P:P8[2] to MPC5744P:P7[1]
    SIUL_ClearPad(SIUL_PA1);
    //SIUL_ToggleIO(SIUL_PA1);

    //PA2 connected to LED3
    SIUL_DigitalIO(SIUL_PA2,0);				//MPC5744P:P8[3]
    SIUL_ClearPad(SIUL_PA2);
    //SIUL_ToggleIO(SIUL_PA2);

/* Init CAN */
    CAN_Init(0);
    CAN_ConfigurePads(0);  //PB0 = CAN0_TX (MPC5744P:J17[5] to FS65:J37[18])
    					   //PB1 = CAN0_RX (MPC5744P:J17[2] to FS65:J37[19])

/* Init ADC0 */
    ADCstruct.scanVoltage.R = 0x8F;				//Scan 2.5V reference, wide voltages and temperature
    ADC_Init(ADC_NB, ADC_MASK, 0, ONE_SHOT);	//PB7 (P9[1]) and PB8(P9[2]) as analog inputs:
    ADC_SetInt(ADC_NB, EOC_FLAG, ADC_MASK);		//Enable EOC interrupt

/* Init DSPI */
    DSPI_Init(DSPI_NB, MASTER, DSPI_CLK, 1000000, 0);		//DSPI initialization as a MASTER, RFDF interrupt flag
    //PC7 (P10[8]) = SIN_0
    //PC6 (P10[7]) = SOUT
    //PC5 (P10[6]) = SCK
    //PC4 (P10[5]) = CS0

/* Init PIT module for watchdog refresh */
    PIT_Init();
   	PIT_Setup(PIT_WD_CH, PIT_CLK/1000000, 3000);  //3msec refresh period
   	PIT_EnableInt(PIT_WD_CH);

/* if power-on reset, FS65xx initialization*/

#ifndef FORCE_FS65_INIT
    if ( (MC_RGM.DES.B.F_VOR_DEST==1) || (MC_RGM.DES.B.F_POR==1) )
    {
#endif

	  //Clear VOR and POR flags
	  MC_RGM.DES.B.F_VOR_DEST = 1;
	  MC_RGM.DES.B.F_POR      = 1;
	  //Init FS65xx
	  FS65_Init();
	  // Start WD refresh
	  PIT_EnableChannel(PIT_WD_CH);
	  //Configure non-init registers
	  FS65_Config_NonInit();

#ifndef FORCE_FS65_INIT
    }
    else
    {
      // Start WD refresh
      PIT_EnableChannel(PIT_WD_CH);
	  //Reset was not caused by FS65xx
	  //Get FS65xx status
	  FS65_GetStatus();
	  //Configure non-init registers
	  FS65_Config_NonInit();
    }
#endif

 /* Long duration Timer configuration */
    //Configuration for Func 1 : generate an INT pulse after 15sec
    error_code = FS65_SetLDTNormalMode();
    error_code = FS65_SelectLDTOperation(LDT_FUNCTION_1);
    error_code = FS65_ConfAfterRunValue(0x0000000F);
    FS65_UpdateRegisterContent(LDT_AFTER_RUN_1_ADR);
    FS65_UpdateRegisterContent(LDT_AFTER_RUN_2_ADR);
    error_code = FS65_SetRTCReg();

    SIUL_ToggleIO(SIUL_PA1);
    error_code = FS65_StartLDTCounter();

/* Start infinite loop */
   for (index=0; index <= 8; index++)
    {
	  ///Launch and configure ADC conversion
	  ADC_StartNormalConversion(ADC_NB, ADC_MASK);

	  //wait 10msec
	  PIT_wait_micsec(10000);

	  //Send CAN_Frame
	  CAN_Send(0, 0, 0xA0A0A0A0A0A0A0A0, 0x15555555);

   }

  /*
   *  Go to LPOFF mode with automatic wake-up after #1msec
   */
   FS65_UpdateRegisterContent(MODE_ADR);
   FS65_UpdateRegisterContent(DIAG_SF_ERR_ADR);
   FS65_UpdateRegisterContent(DIAG_SF_ERR_ADR);
   FS65_UpdateRegisterContent(RELEASE_FSxB_ADR);

   if (INTstruct.DIAG_SF_ERR.B.FLT_ERR == 0)
   {
  	 SIUL_ToggleIO(SIUL_PA2);
  	 FS65_SetLPOFFmode_autoWU();
  }

   for (;;)
   {}

}
