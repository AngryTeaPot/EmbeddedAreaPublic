/**
 ********************************************************************************
 * @file    atsha204a.h
 * @date    25.07.2022
 * @brief
 ********************************************************************************
 */

#ifndef __ATSHA204A_H__
#define __ATSHA204A_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define MAX_READ_WRITE_SIZE						40


#define ATSHA204_I2C_TX	HAL_I2C_Master_Transmit
#define ATSHA204_I2C_RX	HAL_I2C_Master_Receive

#define ZONE_LOCK_CONFIG 						0
#define ZONE_LOCK_DATA_OTP						1

#define CONFIG_SIZE 							84
#define CONFIG_BASE_ADDRESS						0



/************************************
 * TYPEDEFS
 ************************************/

typedef enum
{
  ATSHA204_OK       							= 0x00U,
  ATSHA204_ERROR    							= 0x01U,
  ATSHA204_BUSY     							= 0x02U,
  ATSHA204_BUSY_TIMEOUT  						= 0x03U
} ATSHA204_StatusTypeDef;

typedef enum {
	ATSHA204_RESET 								= 0x00,
	ATSHA204_SLEEP 								= 0x01,
	ATSHA204_IDLE								= 0x02,
	ATSHA204_COMMAND							= 0x03,
	ATSHA204_WAKE
}ATSHA204_Mode_te;

typedef enum {
	CONFIG_ZONE 								= 0x00,
	OTP_ZONE 									= 0x01,
	DATA_ZONE									= 0x02
}ATSHA204_Zone_te;

typedef enum {
	COMMAND_DERIVEKEY 							= 0x1C,
	COMMAND_DEVREV								= 0x30,
	COMMAND_GENDIG								= 0x15,
	COMMAND_HMAC								= 0x11,
	COMMAND_CHECKMAC							= 0x28,
	COMMAND_LOCK								= 0x17,
	COMMAND_MAC									= 0x08,
	COMMAND_NONCE								= 0x16,
	COMMAND_PAUSE								= 0x01,
	COMMAND_RANDOM								= 0x1B,
	COMMAND_READ								= 0x02,
	COMMAND_UPDATEEXTRA							= 0x20,
	COMMAND_WRITE 								= 0x12
}ATSHA204_Commands_te;

typedef enum {
	ATSHA204_SUCCESFUL_COMMAND_EXECUTION		= 0x00,
	ATSHA204_CHECKMAC_MISCOMPARE				= 0x01,
	ATSHA204_PARSE_ERROR						= 0x03,
	ATSHA204_EXECUTION_ERROR					= 0x0F,
	ATSHA204_AFTER_WAKE_FIRST_COMMAND			= 0x11,
	ATSHA204_CRC_COMMUNICATIONS_ERROR			= 0xFF
}Status_Error_te;

typedef struct
{
	uint8_t functionCommand_u8;				// Command type : ATSHA204_Commands_te
	uint8_t count_u8;						// Packet size. Include Count, Opcode, Param1, Param2, Data, and CRC
	uint8_t opcode_u8;						// ATSHA204 operation code
	union
	{
		struct
		{
			uint8_t _bit0_1   					: 2;
			uint8_t _bit2   					: 1;
			uint8_t _bit3   					: 1;
			uint8_t _bit4   					: 1;
			uint8_t _bit5   					: 1;
			uint8_t _bit6   					: 1;
			uint8_t _bit7   					: 1;
		}bits;
		uint8_t all_u8;
	}param1;								// First Parameter. One byte always present
	uint16_t 	param2_u16;					// Second parameter. Two bytes always presen
	uint8_t  	pData[32]; 					// Optional Data based on Command being called //Duruma göre pData[] da olabilir.
	uint8_t 	crcl;						// Includes Count, Opcode, Param1, Param2 and Data. Does not include Function Command
	uint8_t 	crch;
}Command_t;


typedef struct
{
	union
	{
		struct
		{
			uint8_t _bit0_3   					: 4;
			uint8_t _bit4   					: 1;
			uint8_t _bit5   					: 1;
			uint8_t _bit6   					: 1;
			uint8_t _bit7   					: 1;
			uint8_t _bit8_11   					: 4;
			uint8_t _bit12_15   				: 4;
		}bits;
		uint16_t all_u16;
	}param2;								// First Parameter. One byte always present

}SlotConfig_t;

typedef struct
{
	I2C_HandleTypeDef *hi2c;
	uint16_t DevAddress;
	uint16_t Timeout;

	uint8_t readBuffer[MAX_READ_WRITE_SIZE];				/*	ReadBuffer neden 100? */
	uint8_t wake_data[4];
	uint8_t config_zone_data[88];
	struct _config
	{
		uint8_t serialNum[9];
		uint8_t slotConf[32];
		uint8_t lockValue;
		uint8_t lockConfig;
	}_configData;
	uint8_t slotData[MAX_READ_WRITE_SIZE];
} ATSHA204_Device;

typedef struct {
	uint16_t address;
	uint16_t size;
	ATSHA204_Zone_te zone;
	uint8_t *pData;
	uint8_t byte;
}ATSHA204_Variable_t;


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
ATSHA204_StatusTypeDef ATSHA_GetSerialNumber(ATSHA204_Device *me);
void ATSHA204_Init(ATSHA204_Device *me, I2C_HandleTypeDef *hi2c, uint16_t devaddress, uint16_t timeout);
ATSHA204_StatusTypeDef ATSHA204_GetConfig(ATSHA204_Device *me);
ATSHA204_StatusTypeDef ATSHA204_Read(ATSHA204_Device *me, ATSHA204_Zone_te zone, uint16_t address, uint16_t size);
ATSHA204_StatusTypeDef ATSHA204_Write(ATSHA204_Device *me, ATSHA204_Zone_te zone, uint16_t address, uint8_t *pData, uint16_t size);
#ifdef __cplusplus
}
#endif

#endif
