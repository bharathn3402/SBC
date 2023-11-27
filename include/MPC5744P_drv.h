/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       FS65xx_drv.h
* @file             FS65xx_drv.h
*
* $Date:            Mar-08-2016
* @date             Mar-08-2016
*
* $Version:         0.1
* @version          0.1
*
* Description:      FS65 header file for MCU
* @brief            FS65 header file for MCU
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage FS65xx driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains PowerSBC driver allowing to control MC33FS65xx and MC33FS45xx.
*	FS65xx driver was created as a demo code to enable our customers
*	to start their development quickly and to demonstrate functionality of the PowerSBC
*	chip. Presented driver is not AUTOSAR nor ISO26262 compliant. This driver package
*	needs so-called MPC5744P Basic software drivers, which present the basic layer
*	for this application.
*
*  The key features of this package are the following:
*  - Offer driver allowing to write and read into and from FS65xx registers
*  - Switch FS65xx modes (low power, normal etc.)
*  - Treat all interrupt sources (events)
*  - Supervise voltages presented in the FS65xx interface
*
*  For more information about the functions and configuration items see these documents:
*
*******************************************************************************
*
* @attention
*
*******************************************************************************/
/*==================================================================================================
*   Project              : FS65xx
*   Platform             : MPC5744P
*   Dependencies         : MPC5744P - Basic SW drivers.
*   All Rights Reserved.
==================================================================================================*/

/*==================================================================================================
Revision History:
                             Modification     Function
Author (core ID)              Date D/M/Y       Name		  		Description of Changes
BRNY001					      08/04/2016 	   ALL		  		Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

#ifndef _FS65XX_DRV_H_
#define _FS65XX_DRV_H_

#include "MPC5744P.h"

/*==================================================================================================
*   Configurable parameters
*	User shall change configuration in this section regarding needs of the application.
==================================================================================================*/
#define MCU_SYS_CLK		200000000		///defines system clock frequency in Hz
#define MCU_PLL0_CLK		160000000		///defines frequency of the PLL0 clock
#define DSPI_CLK		MCU_SYS_CLK/4	///defined by the MC_CGM.SC_DC0.R setting
#define LINFLEX_CLK		MCU_SYS_CLK/2	///given by the HALFSYS_CLK distribution
#define ADC_CLK			MCU_PLL0_CLK/2	///defined by the MC_CGM.AC0_DC2.R register setting
#define DSPI_CLK		MCU_SYS_CLK/4	///defined by the MC_CGM.SC_DC0.R setting
#define PIT_CLK			MCU_SYS_CLK/4	///defined by the MC_CGM.SC_DC0.R setting

#define	DSPI_NB	0					///defines number of DSPI module
#define	DSPI_CS	1			///defines Chip Select
#define	DSPI_BR	5000000				///defines DSPI baud rate in b/s

#define	ADC_NB	0	///defines number of ADC module to be used
#define	ADC_CH	0	///defines number of used ADC channel
#define	ADC_MASK	3	///Channel mask created from the channel number ADC_MASK = 2^ADC_CH
#define	ADC_SOURCE_VCCA				///defines that Vcca is used as a reference voltage for ADC
//#define	ADC_SOURCE_CALIB	4.9		///defines voltage used as a reference for ADC if the default Vcca is not used

#define	PIT_WD_CH	0					///defines PIT channel number used for the Watchdog refresh
#define	PIT_FS_DELAY_CH	1				///defines PIT channel number used for delay between two fail safe commands
#define	PIT_UART_CH	2					///defines PIT channel number used for sending data via UART
#define	PIT_WD_PERIOD     	0.00225		///defines WD refresh period in seconds
#define PIT_WD_PERIOD1		0.00075
#define PIT_WD_PERIOD2		0.0015
#define PIT_WD_PERIOD3		0.00225
#define PIT_WD_PERIOD4		0.003
#define PIT_WD_PERIOD6		0.0045
#define PIT_WD_PERIOD8		0.006
#define PIT_WD_PERIOD12		0.009
#define PIT_WD_PERIOD16		0.012
#define PIT_WD_PERIOD24		0.018
#define PIT_WD_PERIOD32		0.024
#define PIT_WD_PERIOD64		0.048

#define PIT_WD_PERIOD128	0.096
#define PIT_WD_PERIOD256	0.192
#define PIT_WD_PERIOD512	0.384
#define PIT_WD_PERIOD1024	0.768

/****************************************************************************\
* SIUL parameters
\****************************************************************************/
#define	SIUL_INT_EIRQ	SIUL_EIRQ0	///defines a pad used for external interruption by its mask (EIRQ0 - EIRQ31)

/****************************************************************************\
* UART parameters
\****************************************************************************/
#define	LINFLEX_UART_NB	1			///defines number of the LINFlex module used for the UART communication
#define	LINFLEX_UART_BR	921600 ///defines Baud rate in Bd used for the UART <=> PC communication
#define	MSG_FRAME_SIZE	6	///defines number of bytes for each frame of the UART communication (1 - 4)
#define LINFLEX_UART_RX_BUFFER_SIZE 1	///defines size of the receive UART buffer

/****************************************************************************\
* PIT parameters
\****************************************************************************/
#define	PIT_UART_PERIOD	0.0043		///defines period for sending data via UART to PC in seconds
#define PIT_FS_DELAY	0.000004	///delay between two fail safe commands in seconds

/****************************************************************************\
* ADC parameters
\****************************************************************************/
#define	ADC_RESOLUTION	4095	///ADC resolution 2^12
#define	ADC_RATIO	(ADC_SOURCE_CALIB / ADC_RESOLUTION)	///Ratio necessary for ADC computations

/****************************************************************************\
* INTC parameters
\****************************************************************************/
#define	INT_CEIL_PRIORITY	12	///ceil priority has to be equal to the highest priority of interrupts sharing DSPI to communicate with FS65xx
#define	INT_WD_PRIORITY	12	///priority for WD refresh interrupt caused by PIT
#define	INT_SIUL_PRIORITY	10	///priority for interrupt caused by INT pin
#define	INT_UART_RX_PRIORITY	8	///priority for commands receiving from PC
#define	INT_ADC_PRIORITY	6	///priority for end of conversion of ADC

#define	INT_CEIL_UART_PRIORITY	8	///ceil UART priority has to be equal to the highest priority of interrupts sharing UART to communicate with PC

/************************************************************************/
// 	Software defines DO NOT MODIFY Following section
/************************************************************************/
//Initialize clocks value - DO NOT MODIFY IT - Set it up with Hardware theoritical value
#define	fFXOSC      16000000
#define	fSXOSC      32000
#define	fFIRC       16000000
#define	fSIRC       128000

//Related to EVB
#define LED1        68 //PCR[68]
#define LED2        69 //PCR[69]
#define LED3        70 //PCR[70]
#define LED4        71 //PCR[71]

//Declaration for SPI state Machine
#define STOPPED     0
#define RUNNING     1

//use for CMU
#define FIRC_clk    0
#define SIRC_clk    1
#define SXOSC_clk   2

//use for Mode Entry
#define	ME_RESET	0
#define	ME_TEST		1
#define	ME_SAFE		2
#define	ME_DRUN		3
#define	ME_RUN0		4
#define	ME_RUN1		5
#define	ME_RUN2		6
#define	ME_RUN3		7
#define	ME_HALT0	8
#define	ME_STOP0	10
#define	ME_STANDBY0	13

//Pad Pull
#define 	SIUL_PULL_UP	0x00030000
#define 	SIUL_PULL_DOWN	0x00010000
#define 	SIUL_PULL_DIS	0x00000000

//Pad dir
#define  	SIUL_INPUT	0x00080000
#define 	SIUL_OUTPUT	0x02000000
#define     SIUL_BIDIR  0x02080000

//Pad function
#define 	SIUL_GPIO	0x0
#define 	SIUL_ALT1	0x1
#define 	SIUL_ALT2	0x2
#define 	SIUL_ALT3	0x3
#define 	SIUL_ALT4	0x4

//Safe mode control
#define 	SIUL_SMC	0x00800000

//Pad type
#define 	SIUL_DIGITAL	0x00000000
#define 	SIUL_ANALOG 	0x00400000

//Pad slew rate
#define 	SIUL_SLEW_ENB 	0x00000000
#define 	SIUL_SLEW_DIS	0x20000000
#define 	SIUL_DRIVE_HALF 0x00000000
#define 	SIUL_DRIVE_FULL 0x1000000

//Output type
#define 	SIUL_NORMAL	0x00000000
#define 	SIUL_OPEND	0x01000000

// external interruption masks EIRQx

#define	SIUL_EIRQ0	0x1
#define	SIUL_EIRQ1	0x2
#define	SIUL_EIRQ2	0x4
#define	SIUL_EIRQ3	0x8
#define	SIUL_EIRQ4	0x10
#define	SIUL_EIRQ5	0x20
#define	SIUL_EIRQ6	0x40
#define	SIUL_EIRQ7	0x80
#define	SIUL_EIRQ8	0x100
#define	SIUL_EIRQ9	0x200
#define	SIUL_EIRQ10	0x400
#define	SIUL_EIRQ11	0x800
#define	SIUL_EIRQ12	0x1000
#define	SIUL_EIRQ13	0x2000
#define	SIUL_EIRQ14	0x4000
#define	SIUL_EIRQ15	0x8000
#define	SIUL_EIRQ16	0x10000
#define	SIUL_EIRQ17	0x20000
#define	SIUL_EIRQ18	0x40000
#define	SIUL_EIRQ19	0x80000
#define	SIUL_EIRQ20	0x100000
#define	SIUL_EIRQ21	0x200000
#define	SIUL_EIRQ22	0x400000
#define	SIUL_EIRQ23	0x800000
#define	SIUL_EIRQ24	0x1000000
#define	SIUL_EIRQ25	0x2000000
#define	SIUL_EIRQ26	0x4000000
#define	SIUL_EIRQ27	0x8000000
#define	SIUL_EIRQ28	0x10000000
#define	SIUL_EIRQ29	0x20000000
#define	SIUL_EIRQ30	0x40000000
#define	SIUL_EIRQ31	0x80000000

// pad list Name versus MSCR[SSS]

#define	SIUL_PA0	0
#define	SIUL_PA1	1
#define	SIUL_PA2	2
#define	SIUL_PA3	3
#define	SIUL_PA4	4
#define	SIUL_PA5	5
#define	SIUL_PA6	6
#define	SIUL_PA7	7
#define	SIUL_PA8	8
#define	SIUL_PA9	9
#define	SIUL_PA10	10
#define	SIUL_PA11	11
#define	SIUL_PA12	12
#define	SIUL_PA13	13
#define	SIUL_PA14	14
#define	SIUL_PA15	15
#define	SIUL_PB0	16
#define	SIUL_PB1	17
#define	SIUL_PB2	18
#define	SIUL_PB3	19
#define	SIUL_PB4	20
#define	SIUL_PB5	21
#define	SIUL_PB6	22
#define	SIUL_PB7	23
#define	SIUL_PB8	24
#define	SIUL_PB9	25
#define	SIUL_PB10	26
#define	SIUL_PB11	27
#define	SIUL_PB12	28
#define	SIUL_PB13	29
#define	SIUL_PB14	30
#define	SIUL_PB15	31
#define	SIUL_PC0	32
#define	SIUL_PC1	33
#define	SIUL_PC2	34
#define	SIUL_PC3	35
#define	SIUL_PC4	36
#define	SIUL_PC5	37
#define	SIUL_PC6	38
#define	SIUL_PC7	39
#define	SIUL_PC8	40
#define	SIUL_PC9	41
#define	SIUL_PC10	42
#define	SIUL_PC11	43
#define	SIUL_PC12	44
#define	SIUL_PC13	45
#define	SIUL_PC14	46
#define	SIUL_PC15	47
#define	SIUL_PD0	48
#define	SIUL_PD1	49
#define	SIUL_PD2	50
#define	SIUL_PD3	51
#define	SIUL_PD4	52
#define	SIUL_PD5	53
#define	SIUL_PD6	54
#define	SIUL_PD7	55
#define	SIUL_PD8	56
#define	SIUL_PD9	57
#define	SIUL_PD10	58
#define	SIUL_PD11	59
#define	SIUL_PD12	60
#define	SIUL_PD13	61
#define	SIUL_PD14	62
#define	SIUL_PD15	63
#define	SIUL_PE0	64
#define	SIUL_PE1	65
#define	SIUL_PE2	66
#define	SIUL_PE3	67
#define	SIUL_PE4	68
#define	SIUL_PE5	69
#define	SIUL_PE6	70
#define	SIUL_PE7	71
#define	SIUL_PE8	72
#define	SIUL_PE9	73
#define	SIUL_PE10	74
#define	SIUL_PE11	75
#define	SIUL_PE12	76
#define	SIUL_PE13	77
#define	SIUL_PE14	78
#define	SIUL_PE15	79
#define	SIUL_PF0	80
#define	SIUL_PF1	81
#define	SIUL_PF2	82
#define	SIUL_PF3	83
#define	SIUL_PF4	84
#define	SIUL_PF5	85
#define	SIUL_PF6	86
#define	SIUL_PF7	87
#define	SIUL_PF8	88
#define	SIUL_PF9	89
#define	SIUL_PF10	90
#define	SIUL_PF11	91
#define	SIUL_PF12	92
#define	SIUL_PF13	93
#define	SIUL_PF14	94
#define	SIUL_PF15	95
#define	SIUL_PG0	96
#define	SIUL_PG1	97
#define	SIUL_PG2	98
#define	SIUL_PG3	99
#define	SIUL_PG4	100
#define	SIUL_PG5	101
#define	SIUL_PG6	102
#define	SIUL_PG7	103
#define	SIUL_PG8	104
#define	SIUL_PG9	105
#define	SIUL_PG10	106
#define	SIUL_PG11	107
#define	SIUL_PG12	108
#define	SIUL_PG13	109
#define	SIUL_PG14	110
#define	SIUL_PG15	111
#define	SIUL_PH0	112
#define	SIUL_PH1	113
#define	SIUL_PH2	114
#define	SIUL_PH3	115
#define	SIUL_PH4	116
#define	SIUL_PH5	117
#define	SIUL_PH6	118
#define	SIUL_PH7	119
#define	SIUL_PH8	120
#define	SIUL_PH9	121
#define	SIUL_PH10	122
#define	SIUL_PH11	123
#define	SIUL_PH12	124
#define	SIUL_PH13	125
#define	SIUL_PH14	126
#define	SIUL_PH15	127
#define	SIUL_PI0	128
#define	SIUL_PI1	129
#define	SIUL_PI2	130
#define	SIUL_PI3	131
#define	SIUL_PI4	132
#define	SIUL_PI5	133
#define	SIUL_PI6	134
#define	SIUL_PI7	135
#define	SIUL_PI8	136
#define	SIUL_PI9	137
#define	SIUL_PI10	138
#define	SIUL_PI11	139
#define	SIUL_PI12	140
#define	SIUL_PI13	141
#define	SIUL_PI14	142
#define	SIUL_PI15	143
#define	SIUL_PJ0	144
#define	SIUL_PJ1	145
#define	SIUL_PJ2	146
#define	SIUL_PJ3	147
#define	SIUL_PJ4	148
#define	SIUL_PJ5	149
#define	SIUL_PJ6	150
#define	SIUL_PJ7	151
#define	SIUL_PJ8	152
#define	SIUL_PJ9	153

// pad list Name versus IMCR[SSS]
#define SIUL_IN_PA0     	173
#define SIUL_IN_PA0_DSPI2	48
#define SIUL_IN_PA3		49
#define SIUL_IN_PB3     165
#define SIUL_IN_PB7     165
#define SIUL_IN_PC7     41
#define SIUL_IN_PD12    166
#define SIUL_IN_PB1		32
#define SIUL_IN_PA15	33
#define SIUL_IN_PF15	34
#define SIUL_IN_PA8		44
#define SIUL_IN_PA2		47
#define SIUL_IN_PD7		50
#define SIUL_IN_PD6	51
#define	SIUL_IN_PC11	52

#endif
