/**
  **********************************************************************************************
  * @file    rtc_mk.c
  * @brief   This file provides code for the RTC_MK
  **********************************************************************************************
  * @attention
  *
  * Copyright (c) Embedded Area.
  * All rights reserved.
  *
  **********************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"


/* Global Variables --------------------------------------------------------------------------*/
RTCMK_ts	Rtcmk;

uint8_t TIMESOURCE ;


/* Static Functions  ---------------------------------------------------------*/

/**
  *@brief		RTCMK_LL funtion that generic funcs to do every operation types
  *@param[in]		me: RTCMK_ts handle pointer
  *@param[in]		TimeStruct_t: write address
  *@param[in]		RTC_OperationTypeDef_e: Operation Type
  *@retval		RTC_StatusTypeDef_e 
  */
static RTC_StatusTypeDef_e RTCMK_LL(RTCMK_ts *me,  TimeStruct_t *sZaman, RTC_OperationTypeDef_e Rtc_Operation){
	
	RTC_StatusTypeDef_e status = RTC_OK;
	uint8_t val;
	
	switch(Rtc_Operation)
	{
		case	RTC_RESET:

				status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read (me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_CTRL_REGISTER, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
				if(status != RTC_OK) 
						return status;
				val |= 0x80;
				status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_CTRL_REGISTER, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
				if(status != RTC_OK) 
						return status;
				HAL_Delay(10);	
				val &= (~0x80);
				status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_CTRL_REGISTER, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
				return status;
		
				
		case	RTC_GETTIME:
			
				status = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_DATE, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
				if(status == RTC_OK) 
					sZaman->date = bcd2bin(val & 0x3F); 
				else 
					return status ; 

				status = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_MONTH, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
				if(status == RTC_OK) 
					sZaman->month = bcd2bin(val & 0x1F); 
				else 
					return status ; 
				
				sZaman->year_H 	= 20;
				
				status = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_YEAR, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000); 
				if(status == RTC_OK) 
					sZaman->year_L = bcd2bin(val); 
				else 
					return status ; 

				status = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_HOURS, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
				if(status == RTC_OK) 
					sZaman->hours = bcd2bin(val & 0x3F); 
				else 
					return status ; 
						
				status = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_MINUTES, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000); 
				if(status == RTC_OK) 
					sZaman->minutes = bcd2bin(val & 0x7F); 
				else 
					return status ; 
				 
				status = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);  
				if(status == RTC_OK) 
					sZaman->seconds = bcd2bin(val & 0x7F); 
					return status;	
		
		case	RTC_SETTIME:
			
					val = bin2bcd(sZaman->date);
					status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_DATE,		I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
					if(status != RTC_OK) 
						return status;
					
					val = bin2bcd(sZaman->month) ;
					status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_MONTH,		I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);		
					if(status != RTC_OK) 
						return status;
					
					val = bin2bcd(sZaman->year_L);
					status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_YEAR,		I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
					if(status != RTC_OK) 
						return status;
					
					val = bin2bcd(sZaman->hours);
					status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_HOURS,		I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
					if(status != RTC_OK) 
						return status;
					
					val = bin2bcd(sZaman->minutes);
					status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_MINUTES, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
					if(status != RTC_OK) 
						return status;
					
					val = bin2bcd(sZaman->seconds);
					status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(me->i2chandle, ABRTCMK_I2C_SAD, ABRTCMK_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, &val, 1, 1000);
					return status;
		default: return RTC_ERROR;
	}
}

/* Global Functions  ---------------------------------------------------------*/
/**
	* @brief		RTCMK_Init Task. 
	* @param[in]		me		:	Handle pointer
	* @param[in]		i2cHandler	:	Handles the I2C instance pointer	
	* @param[in]		deviceAddress	:	The Device address that defines pre-def ==> ABRTCMK_I2C_SAD
	* @param[in]		TimeOut		:	The time that maintains tring
	* @retval		FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Init(RTCMK_ts *me, I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint16_t TimeOut){
	uint8_t regVal;	

	me->i2chandle = i2cHandler;
	me->DeviceAddress = deviceAddress;
	me->ProgramTimeOut = TimeOut;
	
	
	/*
	 *		CLKOUT Freq set to 1 Hz
	*/ 
	uint8_t status = HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_SELECT_REGISTER, ABRTCMK_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, &regVal, 1, 1000);  
	if(status != RTC_OK) 
 		return (RTC_StatusTypeDef_e)status;
	
	regVal |= (0x30);
	status = HAL_I2C_Mem_Read(me->i2chandle, ABRTCMK_SELECT_REGISTER, ABRTCMK_REG_SECONDS, I2C_MEMADD_SIZE_8BIT, &regVal, 1, 1000);  
	if(status != RTC_OK) 
		return (RTC_StatusTypeDef_e)status;
				
	return	RTCMK_isConnected(me);
}


/**
	* @brief			RTCMK_isConnected checks the device is connected or not. 
	* @param[in]			me		: Handle pointer
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_isConnected(RTCMK_ts *me){
	if(HAL_I2C_IsDeviceReady(me->i2chandle, me->DeviceAddress, 3, me->ProgramTimeOut) == HAL_OK)
		return RTC_OK;
	return RTC_ERROR;
}


/**
	* @brief  			RTCMK_Reset resets the device. 
	* @param[in]			me		: Handle pointer
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Reset(RTCMK_ts *me){
	return	RTCMK_LL(me, 0, RTC_RESET);
}


/**
	* @brief			RTCMK_Reset resets the device. 
	* @param[in]			me		: Handle pointer
	* @param[in]			TimeStruct_t	: The time struct where the time get stored into.
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_GetTime(RTCMK_ts *me, TimeStruct_t *sZaman){
	return RTCMK_LL(me, 0, RTC_GETTIME);
}


/**
	* @brief  			RTCMK_Reset resets the device. 
	* @param[in]			me		: Handle pointer
	* @param[in]			TimeStruct_t	: The time struct which will set the device time .
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_SetTime(RTCMK_ts *me, TimeStruct_t *timeStr){
	return RTCMK_LL(me, 0, RTC_SETTIME);
}


/**
	* @brief  			RTCMK_Reset resets the device. 
	* @param[in]			RegAddr		: Register Address
	* @param[in]			val		: The Buffer that will stored into the read register value
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Read(uint8_t RegAddr,uint8_t* val){
	RTC_StatusTypeDef_e status = RTC_OK;
	status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Read(&ABRTCMK_I2C, ABRTCMK_I2C_SAD, RegAddr, I2C_MEMADD_SIZE_8BIT, val, 1, 1000);
	if (status!=HAL_OK ) printf((HAL_StatusTypeDef)status);	
	return status;
}


/**
	* @brief			RTCMK_Reset resets the device. 
	* @param[in]			RegAddr		: Register Address
	* @param[in]			pData		: The data that will write into the register 
	* @retval			FlagStatus					
  */
RTC_StatusTypeDef_e RTCMK_Write(uint8_t RegAddr, uint8_t pData ){
	RTC_StatusTypeDef_e status = RTC_OK;
	status  = (RTC_StatusTypeDef_e)HAL_I2C_Mem_Write(&ABRTCMK_I2C, ABRTCMK_I2C_SAD, RegAddr, I2C_MEMADD_SIZE_8BIT, &pData, 1, 1000);
	if (status!=HAL_OK ) printf((HAL_StatusTypeDef)status);	
	return status;
}


/**
	* @brief  			RTCMK_Timer_Init Init of Every timerCnt's interrupt occur.  
	* @param[in]			timerCnt	: The period of interrupt. If its setted 4, every 4 second interrupt occur
	* @param[in]			timerState	: Start or Stop State
	* @retval			None
  */
void RTCMK_Timer(uint8_t timerCnt, timerState_e timerState){
	uint8_t regVal;
	if(timerState == startTimer){
		// Set TF,TE,TIE to “0” to prevent incorrect operation
		RTCMK_Read(ABRTCMK_FLAG_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_FLAG_REGISTER ,regVal&(~0x04) );		
		RTCMK_Read(ABRTCMK_CTRL_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_CTRL_REGISTER ,regVal&(~0x0C) );
		
		// Set timer count register
		RTCMK_Write(ABRTCMK_TIMER_COUNTER ,timerCnt);
		
		//Set source clock to 1 second
		RTCMK_Read(ABRTCMK_SELECT_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_SELECT_REGISTER ,regVal| 0x08 );
		
		//Set source clock to 1 minute
//		abrtcmk_Read(ABRTCMK_SELECT_REGISTER,&regVal);
//		abrtcmk_Write(ABRTCMK_SELECT_REGISTER ,regVal| 0x0C );
		
		
		// Set TE,TIE to “1” to start the timer and enable the /INT output. When the count
		//reaches zero, enters wait state for fixed- cycle timer interrupt
		RTCMK_Read(ABRTCMK_CTRL_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_CTRL_REGISTER ,regVal|0x0C);
	}
	else if(timerState == stopTimer){
		RTCMK_Read(ABRTCMK_FLAG_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_FLAG_REGISTER ,regVal&(~0x04) );		
		RTCMK_Read(ABRTCMK_CTRL_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_CTRL_REGISTER ,regVal&(~0x0C) );
	}
}


/**
	* @brief			RTCMK_Timer_Init Init of Every timerCnt ms interrupt occur. 
	* @param[in]			timerCnt	: The period of interrupt. If its setted 2, every 5 milisecond interrupt occur
	* @retval			None
  */
void RTCMK_Timer_Init(uint8_t timerCnt){
		uint8_t regVal = 0x00;
	
		/* Set TF,TE,TIE to “0” to prevent incorrect operation*/	
		RTCMK_Read(ABRTCMK_FLAG_REGISTER		,&regVal);
		RTCMK_Write(ABRTCMK_FLAG_REGISTER 	,regVal&(0xFB) );	
	
		/* Set TF,TE,TIE to “0” to prevent incorrect operation*/
		regVal = 0x00;
		RTCMK_Read(ABRTCMK_CTRL_REGISTER		,&regVal);
		RTCMK_Write(ABRTCMK_CTRL_REGISTER 	,regVal&(0xF3) );
	
		/*
		 * Below 2 abrtcmk_Write lines managing the clock cycle and counter. 
		 * RtcMk configured as a 1 cycle 2,4ms and counter = 2 so 2,4ms * 2 = 4,8ms 
		 * every 4,8ms interrupt occur
		*/
		regVal = 0x00;
		RTCMK_Write(ABRTCMK_TIMER_COUNTER 	,regVal|(timerCnt) ); // TIMER COUNTER = 2
	
		regVal = 0x00;
		RTCMK_Read(ABRTCMK_SELECT_REGISTER	,&regVal);
		RTCMK_Write(ABRTCMK_SELECT_REGISTER ,regVal&(0xFF) ); // 4096HZ
	
		/* 
		 * Set TE,TIE to “1” to start the timer and enable the /INT output. 
		 *  When the count reaches zero, enters wait state for fixed- cycle timer interrupt 
		*/
		regVal = 0x00;
		RTCMK_Read(ABRTCMK_CTRL_REGISTER		,&regVal);
		RTCMK_Write(ABRTCMK_CTRL_REGISTER 	,regVal&(0xFF) );
}

/**
	* @brief			RTCMK_Clock_init Inits the clock 1 hz show. 
	* @param[in]			None
	* @retval			None
  */
void RTCMK_Clock_init(void){
		uint8_t regVal;
		// CLKOUT Freq set to 1 Hz
		RTCMK_Read(ABRTCMK_SELECT_REGISTER,&regVal);
		RTCMK_Write(ABRTCMK_SELECT_REGISTER ,regVal|(0x30) );	
}

/**
	* @brief			RTCMK_printTime_24HourFormat prints the the related time struct in 24 hour format. 
	* @param[in]			time		: The time struct that will print 
	* @retval			None					
  */
void RTCMK_printTime_24HourFormat(TimeStruct_t time){
	printf(CGREEN " Current time: %0.2d:%0.2d:%0.2d Date: %0.2d/%0.2d/%0.2d%0.d\n",	
	  time.hours, time.minutes,time.seconds,				
		time.date,	time.month, time.year_H, time.year_L);
}




