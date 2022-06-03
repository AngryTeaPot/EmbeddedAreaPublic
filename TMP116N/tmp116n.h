/**
	******************************************************************************
	* @file    tmp116n.h
	* @brief   This file contains all the function prototypes for
	*          the tmp116n.c file
	******************************************************************************
	* @attention
	*
	* Copyright (c) Embedded Area.
	* All rights reserved.
	*
	*
	******************************************************************************
	*/
#ifndef __TMP116N_H__
#define __TMP116N_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


/* Defines  ------------------------------------------------------------------*/
#define TMP116N_I2C			(hi2c1)
#define TMP116N_I2C_SAD			(0x48<<1)
#define TMP116N_REG_TEMP		0x00
#define TMP116N_REG_CONF		0x01
#define TMP116N_REG_LIMIT_HIGH		0x02
#define TMP116N_REG_LIMIT_LOW		0x03
#define TMP116N_REG_EEPROM_UNLOCK 	0x04
#define TMP116N_REG_EEPROM1		0x05
#define TMP116N_REG_EEPROM2		0x06
#define TMP116N_REG_EEPROM3		0x07
#define TMP116N_REG_EEPROM4		0x08
#define TMP116N_REG_DEVICE_ID		0x0F
#define TEMPERATURE_LOW_TH		(-45)
#define TEMPERATURE_HIGH_TH		100

#define TMP_DEBUG										0

/* Macros  -------------------------------------------------------------------*/
#define	__SET_BITS(__VALUE__, __NAME__, __DATA__)\
(uint16_t)((__VALUE__ & ~(__NAME__##_MSK << __NAME__##_POS))| \
					(__DATA__ << __NAME__##POS))
			
#define	__SET_BIT(__VALUE__, __NAME__)\
(uint16_t)(__VALUE__ | (1U << __NAME__##_BIT))

#define	__CLR_BIT(__VALUE__, __NAME__)\
(uint16_t)(__VALUE__ & ~(1U << __NAME__##_BIT))

#define	__GET_BIT(__VALUE__, __NAME__)\
		((__VALUE__ &(1U << __NAME__##_BIT)) == (1U << __NAME__##_BIT))


/* Enums  --------------------------------------------------------------------*/
/**
  * @brief  RTC_MK Status structures definition
  */
typedef enum
{
  TMP_OK       = 0x00,
  TMP_ERROR    = 0x01,
  TMP_BUSY     = 0x02,
  TMP_TIMEOUT  = 0x03
} TMP_StatusTypeDef_e;

/**
  * @brief  RTC_MK Operation types
  */
typedef enum
{
	TMP_RESET    		= 0x00,
	TMP_READ 		= 0x01,
	TMP_SHUTDOWN		= 0x02,
	TMP_INTERRUPT		= 0x03,
	TMP_CCONVERSION 	= 0x04
} TMP_OperationTypeDef_e;



/* Sructs  -------------------------------------------------------------------*/
typedef struct{
	I2C_HandleTypeDef *i2chandle;
	uint8_t		DeviceAddress;
	uint16_t 	ProgramTimeOut;
}TMP116N_ts;


/* Externs  ------------------------------------------------------------------*/
extern TMP116N_ts	Tmp116;

/* Prototypes  ---------------------------------------------------------------*/
/**
	* @brief			TMP116N_Init Init Task
	* @param[in]			me	: Handle Pointer
	* @param[in]			i2cHandler	: Register Address
	* @param[in]			deviceAddress	: Target Device Address
	* @param[in]			TimeOut		: Program TimeOut
	* @retval			value					
  */
TMP_StatusTypeDef_e TMP116N_Init(TMP116N_ts *me,  I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint16_t TimeOut);


/**
	* @brief			TMP116N_isConnected checks the device is connected or not. 
	* @param[in]			me	: Handle pointer
	* @retval			FlagStatus					
  */
TMP_StatusTypeDef_e TMP116N_isConnected(TMP116N_ts *me);


/**
	* @brief			tmp116n_setLowTH sets the low thresh hold value to device for the interrupt.
	* @param[in]			none
	* @retval			none					
  */
void TMP116N_setLowTH(void);


/**
	* @brief			TMP116N_setHighTH sets the high thresh hold value to device for the interrupt.
	* @param[in]			none
	* @retval			none					
  */
void TMP116N_setHighTH(void);


/**
	* @brief			TMP116N_Read_2_Byte reads the 2 bytes of temperature value  from device. 
	* @param[in]			me			: Handle pointer
	* @param[in]			RegAddr	: Register Address
	* @retval			value					
  */
uint16_t TMP116N_Read_2_Byte(TMP116N_ts *me, uint8_t RegAddr);


/**
	* @brief			TMP116N_Write writes the data to the given register address.
	* @param[in]			RegAddr	: Register Address
	* @param[in]			pData	  : Data to writing at the given register address.
	* @retval			TMP_StatusTypeDef_e					
  */
TMP_StatusTypeDef_e TMP116N_Write(uint8_t RegAddr, int16_t pData );


/**
	* @brief			TMP116N_readTemperatureRaw reads the raw temperature value  from device. 
	* @param[in]			me	: Handle pointer
	* @retval			value					
  */
uint16_t TMP116N_readTemperatureRaw(TMP116N_ts *me);


/**
	* @brief			TMP116N_ReadTemperature reads the temperature value  from device. 
	* @param[in]			me	: Handle pointer
	* @retval			value					
  */
float TMP116N_ReadTemperature(TMP116N_ts *me);


/**
	* @brief			TMP116N_convertRawTemp converts the raw value came from device. 
	* @param[in]			rawTemp : The parameter value that comes from the device.
	* @retval			value					
  */
float TMP116N_convertRawTemp(int16_t rawTemp );


/**
	* @brief			TMP116N_Reset resets the device. 
	* @param[in]			me	: Handle pointer
	* @retval			value					
  */
int	TMP116N_Reset(TMP116N_ts *me);
#endif
