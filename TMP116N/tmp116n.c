/**
  **********************************************************************************************
  * @file    tmp116n.c
  * @brief   This file contains all the functions for
  *          the tmp116n
  **********************************************************************************************
  * @attention
  *
  * Copyright (c) Embedded Area.
  * All rights reserved.
  *
  **********************************************************************************************
  */


/* Includes ----------------------------------------------------------------------------------*/
#include "i2c.h"
#include "tmp116n.h"


/* Global Variables --------------------------------------------------------------------------*/
TMP116N_ts	Tmp116;

/* Externs -----------------------------------------------------------------------------------*/


/* Static function  --------------------------------------------------------------------------*/
static int TMP116N_LL(TMP116N_ts *me, TMP_OperationTypeDef_e Tmp_Operation, uint8_t RegAddr){
	uint8_t retval[2]={0};
	TMP_StatusTypeDef_e status = TMP_OK;

	switch(Tmp_Operation)
	{
		case TMP_RESET:
				TMP116N_Write(TMP116N_REG_TEMP,				0x8000);
				TMP116N_Write(TMP116N_REG_CONF,				0x0220);
				TMP116N_Write(TMP116N_REG_LIMIT_HIGH,			0x6000);
				TMP116N_Write(TMP116N_REG_LIMIT_LOW,			0x8000);
				TMP116N_Write(TMP116N_REG_EEPROM_UNLOCK,		0x0000);
				TMP116N_Write(TMP116N_REG_EEPROM1,			0x0000);
				TMP116N_Write(TMP116N_REG_EEPROM2,			0x0000);
				TMP116N_Write(TMP116N_REG_EEPROM3,			0x0000);
				TMP116N_Write(TMP116N_REG_EEPROM4,			0x0000);
				TMP116N_Write(TMP116N_REG_DEVICE_ID,			0x1116);
			break;
		
		case TMP_READ:
				status  = (TMP_StatusTypeDef_e)HAL_I2C_Mem_Read(me->i2chandle, me->DeviceAddress, RegAddr, I2C_MEMADD_SIZE_8BIT, retval, 2, me->ProgramTimeOut);
				if (status != TMP_OK ) 
				{
					#if TMP_DEBUG
						printHS((HAL_StatusTypeDef)status);	
					#endif
					return 0;
				}
				return retval[0]<<8 | retval[1];
		
		case TMP_SHUTDOWN:
				TMP116N_Write(TMP116N_REG_CONF,	0x0400);
			break;
		
		case TMP_INTERRUPT:
				TMP116N_setLowTH();
				#if TMP_DEBUG
					DBG_PRINT("\ntmp116n_setLowTH set as %f", convertRawTemperature2Temperature(TMP116N_LL(me, TMP_READ, TMP116N_REG_LIMIT_LOW)));
				#endif
				TMP116N_setHighTH();
				#if TMP_DEBUG
					DBG_PRINT("\ntmp116n_setHighTH set as %f\n", convertRawTemperature2Temperature(TMP116N_LL(me, TMP_READ, TMP116N_REG_LIMIT_HIGH)));
				#endif
				TMP116N_LL(me, TMP_READ, TMP116N_REG_CONF); // to clear alert bit
				TMP116N_Write(TMP116N_REG_CONF,0x0380); // 0x0400 shutdown mode
			break;
		
		default: break;
	}
	return 0;
}

/* Global Functions  ---------------------------------------------------------*/
/**
	* @brief			TMP116N_Init Init Task
	* @param[in]			me			: Handle Pointer
	* @param[in]			i2cHandler		: Register Address
	* @param[in]			deviceAddress		: Target Device Address
	* @param[in]			TimeOut			: Program TimeOut
	* @retval			value					
  */
TMP_StatusTypeDef_e TMP116N_Init(TMP116N_ts *me,  I2C_HandleTypeDef *i2cHandler, uint8_t deviceAddress, uint16_t TimeOut){
	me->i2chandle = i2cHandler;
	me->DeviceAddress = deviceAddress;
	me->ProgramTimeOut = TimeOut;
	
	
	return TMP116N_isConnected(me);
}


/**
	* @brief			TMP116N_isConnected checks the device is connected or not. 
	* @param[in]			me			: Handle pointer
	* @retval			FlagStatus					
  */
TMP_StatusTypeDef_e TMP116N_isConnected(TMP116N_ts *me){
	if(HAL_I2C_IsDeviceReady(me->i2chandle, me->DeviceAddress, 3, me->ProgramTimeOut) == HAL_OK)
		return TMP_OK;
	return TMP_ERROR;
}


/**
	* @brief			tmp116n_setLowTH sets the low thresh hold value to device for the interrupt.
	* @param[in]			none
	* @retval			none					
  */
void TMP116N_setLowTH(void){
	// 1 LSB equals 7.8125 m°C.( 2^-7)
	TMP116N_Write(TMP116N_REG_LIMIT_LOW, TEMPERATURE_LOW_TH <<7);
}


/**
	* @brief			TMP116N_setHighTH sets the high thresh hold value to device for the interrupt.
	* @param[in]			none
	* @retval			none					
  */
void TMP116N_setHighTH(void){
	// 1 LSB equals 7.8125 m°C.( 2^-7)
	TMP116N_Write(TMP116N_REG_LIMIT_HIGH, TEMPERATURE_HIGH_TH <<7);
}


/**
	* @brief			TMP116N_Read_2_Byte reads the 2 bytes of temperature value  from device. 
	* @param[in]			me			: Handle pointer
	* @param[in]			RegAddr			: Register Address
	* @retval			value					
  */
uint16_t TMP116N_Read_2_Byte(TMP116N_ts *me, uint8_t RegAddr){
	return TMP116N_LL(me, TMP_READ, RegAddr); 
}


/**
	* @brief			TMP116N_Write writes the data to the given register address.
	* @param[in]			RegAddr			: Register Address
	* @param[in]			pData			: Data to writing at the given register address.
	* @retval			TMP_StatusTypeDef_e					
  */
TMP_StatusTypeDef_e TMP116N_Write(uint8_t RegAddr, int16_t pData ){

	TMP_StatusTypeDef_e status = TMP_OK;
	pData = ((pData &0x00FF)<<8 | (pData &0xFF00)>> 8);
	status  = (TMP_StatusTypeDef_e)HAL_I2C_Mem_Write(&TMP116N_I2C, TMP116N_I2C_SAD, RegAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&pData, 2, 1000);
	if (status!=HAL_OK ) printHS((HAL_StatusTypeDef)status);	
	return status;
}


/**
	* @brief			TMP116N_readTemperatureRaw reads the raw temperature value  from device. 
	* @param[in]			me			: Handle pointer
	* @retval			value					
  */
uint16_t TMP116N_readTemperatureRaw(TMP116N_ts *me){
	return TMP116N_LL(me, TMP_READ, TMP116N_REG_TEMP);
}


/**
	* @brief			TMP116N_ReadTemperature reads the temperature value  from device. 
	* @param[in]			me			: Handle pointer
	* @retval			value					
  */
float TMP116N_ReadTemperature(TMP116N_ts *me){
	return TMP116N_convertRawTemp(TMP116N_LL(me, TMP_READ, TMP116N_REG_TEMP));
}


/**
	* @brief			TMP116N_convertRawTemp converts the raw value came from device. 
	* @param[in]			rawTemp			: The parameter value that comes from the device.
	* @retval			value					
  */
float TMP116N_convertRawTemp(int16_t rawTemp){
	return (float)(rawTemp)/128.0f;
}


/**
	* @brief			TMP116N_Reset resets the device. 
	* @param[in]			me			: Handle pointer
	* @retval			value					
  */
int	TMP116N_Reset(TMP116N_ts *me){
	return TMP116N_LL(me, TMP_RESET, 0);
}

