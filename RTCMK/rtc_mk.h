/**
	******************************************************************************
	* @file    rtc_mk.h
	* @brief   This file contains all the function prototypes for
	*          the rtc_mk.c file
	******************************************************************************
	* @attention
	*
	* Copyright (c) Embedded Area.
	* All rights reserved.
	*
	*
	******************************************************************************
	*/

#ifndef __RTC_MK_H__
#define __RTC_MK_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


/* Defines  ------------------------------------------------------------------*/
#define ABRTCMK_I2C               	(hi2c4)

#define ABRTCMK_I2C_SAD             	(0x32<<1)

#define ABRTCMK_REG_SECONDS		0x00
#define ABRTCMK_REG_MINUTES		0x01
#define ABRTCMK_REG_HOURS		0x02
#define ABRTCMK_REG_WEEKDAY		0x03
#define ABRTCMK_REG_DATE		0x04
#define ABRTCMK_REG_MONTH		0x05
#define ABRTCMK_REG_YEAR		0x06


#define ABRTCMK_MINUTES_ALARM		0x07
#define ABRTCMK_HOUR_ALARM		0x08
#define ABRTCMK_WEEK_AND_DAY_ALARM	0x09

#define ABRTCMK_TIMER_COUNTER		0x0A
#define ABRTCMK_SELECT_REGISTER   	0x0B
#define ABRTCMK_FLAG_REGISTER		0x0C
#define ABRTCMK_CTRL_REGISTER		0x0D




/* Enums  --------------------------------------------------------------------*/
typedef enum {
	stopTimer  =0,
	startTimer =1
}timerState_e;

/**
  * @brief  RTC_MK Status structures definition
  */
typedef enum
{
  RTC_OK       = 0x00,
  RTC_ERROR    = 0x01,
  RTC_BUSY     = 0x02,
  RTC_TIMEOUT  = 0x03
} RTC_StatusTypeDef_e;

/**
  * @brief  RTC_MK Operation types
  */
typedef enum
{
	RTC_WRITE		= 0x00,
	RTC_READ		= 0x01,
	RTC_RESET		= 0x02,
	RTC_GETTIME		= 0x03,
	RTC_SETTIME		= 0x04
} RTC_OperationTypeDef_e;


/* Sructs  -------------------------------------------------------------------*/
typedef struct{
	I2C_HandleTypeDef *i2chandle;
	uint8_t		DeviceAddress;
	uint16_t 	ProgramTimeOut;
}RTCMK_ts;

/* Externs  ------------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c4;
extern RTCMK_ts	Rtcmk;


/* Prototypes  ---------------------------------------------------------------*/

/**
	* @brief		RTCMK_Init Task. 
	* @param[in]		me		:	Handle pointer
	* @param[in]		i2cHandler	:	Handles the I2C instance pointer	
	* @param[in]		deviceAddress	:	The Device address that defines pre-def ==> ABRTCMK_I2C_SAD
	* @param[in]		TimeOut		:	The time that maintains tring
	* @retval		FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Init(RTCMK_ts *me, I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint16_t TimeOut);


/**
	* @brief			RTCMK_isConnected checks the device is connected or not. 
	* @param[in]			me		: Handle pointer
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_isConnected(RTCMK_ts *me);


/**
	* @brief  			RTCMK_Reset resets the device. 
	* @param[in]			me		: Handle pointer
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Reset(RTCMK_ts *me);


/**
	* @brief			RTCMK_Reset resets the device. 
	* @param[in]			me		: Handle pointer
	* @param[in]			TimeStruct_t	: The time struct where the time get stored into.
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_GetTime(RTCMK_ts *me, TimeStruct_t *sZaman);


/**
	* @brief  			RTCMK_Reset resets the device. 
	* @param[in]			me		: Handle pointer
	* @param[in]			TimeStruct_t	: The time struct which will set the device time .
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_SetTime(RTCMK_ts *me, TimeStruct_t *timeStr);


/**
	* @brief  			RTCMK_Reset resets the device. 
	* @param[in]			RegAddr		: Register Address
	* @param[in]			val		: The Buffer that will stored into the read register value
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Read(uint8_t RegAddr,uint8_t* val);


/**
	* @brief			RTCMK_Reset resets the device. 
	* @param[in]			RegAddr		: Register Address
	* @param[in]			pData		: The data that will write into the register 
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Write(uint8_t RegAddr, uint8_t pData);


/**
	* @brief  			RTCMK_Timer_Init Init of Every timerCnt's interrupt occur.  
	* @param[in]			timerCnt	: The period of interrupt. If its setted 4, every 4 second interrupt occur
	* @param[in]			timerState	: Start or Stop State
	* @retval			None
  */
void RTCMK_Timer(uint8_t timerCnt, timerState_e timerState);


/**
	* @brief			RTCMK_Timer_Init Init of Every timerCnt ms interrupt occur. 
	* @param[in]			timerCnt	: The period of interrupt. If its setted 2, every 5 milisecond interrupt occur
	* @retval			None
  */
void RTCMK_Timer_Init(uint8_t timerCnt);


/**
	* @brief			RTCMK_Clock_init Inits the clock 1 hz show. 
	* @param[in]			None
	* @retval			None
  */
void RTCMK_Clock_init(void);


/**
	* @brief			RTCMK_printTime_24HourFormat prints the the related time struct in 24 hour format. 
	* @param[in]			time		: The time struct that will print 
	* @retval			None					
  */
void RTCMK_printTime_24HourFormat(TimeStruct_t time);



#endif

