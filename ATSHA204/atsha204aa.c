/**
 ********************************************************************************
 * @file    atsha204a.c
 * @date    25.07.2022
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "atsha204aa.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static Command_t command = {0};
ATSHA204_Variable_t atsha204_variable = {0};
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static ATSHA204_StatusTypeDef ATSHA204_SendI2C_Command(ATSHA204_Device *pATSHA204a, uint8_t *packet, uint32_t packetSize);
static ATSHA204_StatusTypeDef ATSHA204_LL(ATSHA204_Mode_te MODE, ATSHA204_Device *pATSHA204a, ATSHA204_Commands_te COMMAND_FUNC);
static ATSHA204_StatusTypeDef ATSHA204_Commands(ATSHA204_Device *pATSHA204a, ATSHA204_Commands_te COMMAND_FUNC);
static void ATSHA204_Calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc);
/************************************
 * STATIC FUNCTIONS
 ************************************/

/**
  * @func	ATSHA204_Calculate_crc
  *	@brief	Calculate CRC
  *	@param	length					: Length of the Command_t struct without crc2 and function command
  *	@param	data					: The data pointer that crc will be calculated
  *	@param	crc						: CRC bytes pointer
  * @retval	none
  */
static void ATSHA204_Calculate_crc(uint8_t length, uint8_t *data, uint8_t *crc)
{
	uint8_t counter;
	uint16_t crc_register = 0;
	uint16_t polynom = 0x8005;
	uint8_t shift_register;
	uint8_t data_bit, crc_bit;

	for (counter = 0; counter < length; counter++)
	{
		for (shift_register = 0x01; shift_register > 0x00; shift_register <<= 1)
		{
			data_bit = (data[counter] & shift_register) ? 1 : 0;
			crc_bit = crc_register >> 15;
			crc_register <<= 1;
			if (data_bit != crc_bit)
				crc_register ^= polynom;
		}
	}
	crc[0] = (uint8_t)(crc_register & 0x00FF);
	crc[1] = (uint8_t)(crc_register >> 8);
}


/**
  * @func	ATSHA204_Commands
  *	@brief	Determines which operations to be performed according to the command type.
  *	@param	pATSHA204a				: ATSHA204 device pointer
  *	@param	COMMAND_FUNC			: Specifies the command type
  * @retval	status 					: Return ATSHA204 status.
  */
static ATSHA204_StatusTypeDef ATSHA204_Commands(ATSHA204_Device *pATSHA204a, ATSHA204_Commands_te COMMAND_FUNC) {
	ATSHA204_StatusTypeDef status = ATSHA204_OK;
	command.functionCommand_u8 = ATSHA204_COMMAND;

	switch(COMMAND_FUNC) {

		case COMMAND_DERIVEKEY:
		break;

		case COMMAND_DEVREV:
		break;

		case COMMAND_GENDIG:
		break;

		case COMMAND_HMAC:
		break;

		case COMMAND_CHECKMAC:
		break;

		case COMMAND_LOCK:
			command.count_u8				= 7 + sizeof(command.pData);
			command.opcode_u8				= COMMAND_LOCK;
			command.param1.bits._bit0_1  	= (atsha204_variable.zone == 0) ? ZONE_LOCK_CONFIG : ZONE_LOCK_DATA_OTP;
			command.param1.bits._bit2		= 0;
			command.param1.bits._bit3		= 0;
			command.param1.bits._bit4		= 0;
			command.param1.bits._bit5		= 0;
			command.param1.bits._bit6		= 0;
			command.param1.bits._bit7		= 1;
			command.param2_u16				= 0;
			ATSHA204_Calculate_crc(command.count_u8-2, (uint8_t*)&command.count_u8, (uint8_t*)&command.crcl);
			status = ATSHA204_SendI2C_Command(pATSHA204a, (uint8_t*)&command, sizeof(command));
		break;

		case COMMAND_MAC:
		break;

		case COMMAND_NONCE:
		break;

		case COMMAND_PAUSE:
		break;

		case COMMAND_RANDOM:
		break;

		case COMMAND_READ:
			command.count_u8				= 7 + sizeof(command.pData);
			command.opcode_u8				= COMMAND_READ;
			command.param1.bits._bit0_1  	= atsha204_variable.zone;
			command.param1.bits._bit2		= 0;
			command.param1.bits._bit3		= 0;
			command.param1.bits._bit4		= 0;
			command.param1.bits._bit5		= 0;
			command.param1.bits._bit6		= 0;
			command.param1.bits._bit7		= (atsha204_variable.byte == 0) ? 0 : 1;
			command.param2_u16				= atsha204_variable.address;
			ATSHA204_Calculate_crc(command.count_u8-2, (uint8_t*)&command.count_u8, (uint8_t*)&command.crcl);
			status = ATSHA204_SendI2C_Command(pATSHA204a, (uint8_t*)&command, sizeof(command));
		break;

		case COMMAND_UPDATEEXTRA:
		break;

		case COMMAND_WRITE:
			command.count_u8 				= 7 + sizeof(command.pData);;
			command.opcode_u8 				= COMMAND_WRITE;
			command.param1.bits._bit0_1  	= atsha204_variable.zone;
			command.param1.bits._bit2		= 0;
			command.param1.bits._bit3		= 0;
			command.param1.bits._bit4		= 0;
			command.param1.bits._bit5		= 0;
			command.param1.bits._bit6		= 0;
			command.param1.bits._bit7		= (atsha204_variable.byte == 0) ? 0 : 1;
			command.param2_u16				= atsha204_variable.address;
			uint8_t paramSize = (atsha204_variable.byte == 0) ? 4 : 32;
			for(uint32_t i = 0; i < paramSize; i++)	{
				command.pData[i] = *(atsha204_variable.pData+i);
			}
			ATSHA204_Calculate_crc(command.count_u8-2, (uint8_t*)&command.count_u8, (uint8_t*)&command.crcl);
			status = ATSHA204_SendI2C_Command(pATSHA204a, (uint8_t*)&command, sizeof(command));
		break;

		default:
		return ATSHA204_ERROR;

	}
	memset(&command.functionCommand_u8,0,12);
	return status;
}

/**
  * @func	ATSHA204_LL
  *	@brief	Read data from flash memory
  *	@param	MODE					: Specifies the ATSHA204 device mode.
  *	@param	pATSHA204a				: ATSHA204 device pointer
  *	@param	COMMAND_FUNC			: Specifies the command type
  * @retval	status 					: Return ATSHA204 status.
  */
static ATSHA204_StatusTypeDef ATSHA204_LL(ATSHA204_Mode_te MODE, ATSHA204_Device *pATSHA204a, ATSHA204_Commands_te COMMAND_FUNC) {
	ATSHA204_StatusTypeDef status = ATSHA204_ERROR;
	uint8_t wakeupData[40] = {0};
	switch(MODE) {

		case ATSHA204_RESET:
		break;

		case ATSHA204_SLEEP:
			status = (ATSHA204_StatusTypeDef)HAL_I2C_Master_Transmit(pATSHA204a->hi2c, pATSHA204a->DevAddress, (uint8_t*)ATSHA204_SLEEP, 1, 1000);
			HAL_Delay(5);
			return(status);
		break;

		case ATSHA204_IDLE:
		break;

		case ATSHA204_COMMAND:
			return (ATSHA204_StatusTypeDef)ATSHA204_Commands(pATSHA204a, COMMAND_FUNC);
		break;

		case ATSHA204_WAKE:
			status = (ATSHA204_StatusTypeDef)HAL_I2C_Master_Transmit(pATSHA204a->hi2c, pATSHA204a->DevAddress, wakeupData, sizeof(wakeupData), 1000);
			HAL_Delay(5); // waiting until the tWHI completed. It is enough to more than 2.5ms.
			status = (ATSHA204_StatusTypeDef)HAL_I2C_Master_Receive(pATSHA204a->hi2c, pATSHA204a->DevAddress, pATSHA204a->wake_data, sizeof(pATSHA204a->wake_data), 1000);
			return status;
		break;

		default:
		return status;
	}
	return status;
}

/**
  * @func	ATSHA204_SendI2C_Command
  *	@brief	Read data from flash memory
  *	@param	*pATSHA204a				: ATSHA204 Device pointer
  *	@param	*packet					: Transmitting data packet pointer
  *	@param	packetSize				: Transmitting data packet size
  * @retval	status 					: Returns ATSHA204 status
  */
	ATSHA204_StatusTypeDef status = ATSHA204_ERROR;
static ATSHA204_StatusTypeDef ATSHA204_SendI2C_Command(ATSHA204_Device *pATSHA204a, uint8_t *packet, uint32_t packetSize) {
	ATSHA204_LL(ATSHA204_WAKE, pATSHA204a, 0);
	status = (ATSHA204_StatusTypeDef)HAL_I2C_Master_Transmit(pATSHA204a->hi2c, pATSHA204a->DevAddress, packet, packetSize, 1000);
	HAL_Delay(5);
	status = (ATSHA204_StatusTypeDef)HAL_I2C_Master_Receive(pATSHA204a->hi2c, pATSHA204a->DevAddress, pATSHA204a->readBuffer, packetSize, 1000);
	ATSHA204_LL(ATSHA204_SLEEP, pATSHA204a, 0);
	return status;
}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
  * @func	ATSHA204_Init
  *	@brief	Initialize ATSHA204 device parameter.
  *	@param	me						: ATSHA204 device pointer
  *	@param	hi2c					: Specifies ATSHA204 device I2C interface.
  *	@param	devaddress				: Represent ATSHA204 device address
  *	@param	timeout					: Timeout value of the SHA204 device.
  * @retval	none
  */
void ATSHA204_Init(ATSHA204_Device *me, I2C_HandleTypeDef *hi2c, uint16_t devaddress, uint16_t timeout) {
	me->hi2c 			= hi2c;
	me->DevAddress 		= devaddress;
	me->Timeout			= timeout;
}

/**
  * @func	ATSHA204_DeriveKey
  *	@brief	The device combines the current value of a key with the Nonce stored in TempKey using SHA-256, and places the result
  *			into the target key slot.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_DeriveKey(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_DevRev
  *	@brief	DevRev command returns a single four-byte word representing the revision number of device. Software should not depend
  *			on this value as it may change from time to time.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_DevRev(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_GenDig
  *	@brief	The GenDig command uses SHA-256 to combine a stored value with the contents of TempKey, which must have been valid
  *			prior to the execution of this command. The stored value can come from one of the data slots, either of the OTP pages,
  *			either of the first two pages of the Configuration zone, or retrieved from the hardware transport key array.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_GenDig(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_HMAC
  *	@brief	Computes a HMAC/SHA-256 digest of a key stored in the device, a challenge, and other information on the device.
  *			The output of this command is the output of the HMAC algorithm computed over this key and message. If the message
  *			includes the serial number of device, the response is said to be diversified.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_HMAC(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_CheckMac
  *	@brief	The CheckMac command calculates a MAC response that would have been generated on a CryptoAuthentication device and
  *			compares that with an input value. It returns a boolean to indicate the success or failure of the comparison.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_CheckMac(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_Lock
  *	@brief	Write either LockConfig or LockData to 0x00, thereby changing the permissions in the designation zone.
  *			Thia command fails if the designated zone is already locked.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
ATSHA204_StatusTypeDef ATSHA204_Lock(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	atsha204_variable.zone		= zone;
	return (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_COMMAND, me, COMMAND_LOCK);
}

/**
  * @func	ATSHA204_MAC
  *	@brief	The CheckMac command calculates a MAC response that would have been generated on a Crypto Authentication
  *			device and compares with an input value.
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_MAC(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_Nonce
  *	@brief	This function generates a nonce for use by a subsequent GenDig, MAC, HMAC, Read, or Write command by combining
  *			an internally generated random number with an input value from the system
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_Nonce(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_Pause
  *	@brief	All devices on the bus for which the configuration selector byte does not math the input selector parameter
  *			will go into the idle state
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_Pause(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_Random
  *	@brief	This function generates a random number for use by the system
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: Specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_Random(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}


/**
  * @func	ATSHA204_Read
  *	@brief	Read word from one of the memory zones of the device
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: Specifies zones types as Data zone, Configuration zone or OTP zone
  * @param	address					: specifies number of bits to read
  * @param	address					: specifies number of bits to read
  * @retval	none
  */
ATSHA204_StatusTypeDef ATSHA204_Read(ATSHA204_Device *me, ATSHA204_Zone_te zone, uint16_t address, uint16_t size) {
	atsha204_variable.address 		= address;
	atsha204_variable.zone			= zone;
	atsha204_variable.size			= size;
	ATSHA204_StatusTypeDef status 	= ATSHA204_ERROR;
	atsha204_variable.byte			= 0;
	uint16_t counter = 0;
	uint8_t readBytes = 4;

	while(size > 3) {
		atsha204_variable.byte 	= (size >= 32)	?	1	:	0;
		readBytes 				= (size >= 32)	?	32	:	4;
		status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_WAKE, me, 0);
		status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_COMMAND ,me, COMMAND_READ);
		status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_SLEEP, me, 0);
		if(zone == CONFIG_ZONE) {
			for(uint8_t k = 0; k < readBytes; k++)	{
				me->config_zone_data[k+counter] = me->readBuffer[k+1];
			}
			counter += (size >= 32)	?	32	:	4;
		} else  {
			for(uint8_t k = 0; k < readBytes; k++)	{
				me->slotData[k+counter] = me->readBuffer[k+1];
			}
			counter += (size >= 32)	?	32	:	4;
		}
		atsha204_variable.address 		+= (size >= 32) ? 	8 	: 	1;
		size						 	-= (size >= 32) ? 	32 	: 	4;
	}
	return status;
}

/**
  * @func	ATSHA204_UpdateExtra
  *	@brief	This command is used to update the values of the two "extra" bytes within the configuration zone
  *			after the configuration zone has been blocked
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: Specifies zones types as Data zone, Configuration zone or OTP zone
  * @retval	none
  */
void ATSHA204_UpdateExtra(ATSHA204_Device *me, ATSHA204_Zone_te zone) {
	__NOP();
}

/**
  * @func	ATSHA204_Write
  *	@brief	Read data from flash memory
  *	@param	me						: ATSHA204 device pointer
  *	@param	zone					: Specifies zones types as Data zone, Configuration zone or OTP zone
  *	@param	address					: Address of first word to be written within the zone.
  *	@param	pData					: Data pointer to be written within the zone
  *	@param	size					: Data pointer size
  * @retval	none
  */
ATSHA204_StatusTypeDef ATSHA204_Write(ATSHA204_Device *me, ATSHA204_Zone_te zone, uint16_t address, uint8_t *pData, uint16_t size) {
	ATSHA204_StatusTypeDef status 	= ATSHA204_ERROR;
	atsha204_variable.address 		= address;
	atsha204_variable.size			= size;
	atsha204_variable.zone			= zone;
	atsha204_variable.pData			= pData;
	atsha204_variable.byte			= 0;

	while(size > 3)	{
		atsha204_variable.byte		=	(size >= 32) ? 1	:	0;
		status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_COMMAND, me, COMMAND_WRITE);
		atsha204_variable.address 	+= 	(size >= 32) ? 8 	: 	1;
		atsha204_variable.pData		+= 	(size >= 32) ? 32 	: 	4;
		size 						-= 	(size >= 32) ? 32	:	4;
	}
	return status;
}

/**
  * @func	ATSHA204_GetConfig
  *	@brief	Read Configuration Datas from devices
  *	@param	me						: ATSHA204 device pointer
  * @retval	status					: Returns ATSHA204 status
  */
ATSHA204_StatusTypeDef ATSHA204_GetConfig(ATSHA204_Device *me) {
	ATSHA204_StatusTypeDef status = ATSHA204_OK;
	atsha204_variable.byte		= 0;
	status = ATSHA_GetSerialNumber(me);
	status = ATSHA204_Read(me, CONFIG_ZONE, CONFIG_BASE_ADDRESS, CONFIG_SIZE);

	for (uint8_t i = 0; i < 32; i++)
		me->_configData.slotConf[i] = me->config_zone_data[i + 20];

	me->_configData.lockValue 	= me->config_zone_data[86];
	me->_configData.lockConfig 	= me->config_zone_data[87];
	return status;
}

/**
  * @func	ATSHA_GetSerialNumber
  *	@brief	Read Serial number from device
  *	@param	me						: ATSHA204 device pointer
  * @retval	status					: Returns ATSHA204 status
  */
ATSHA204_StatusTypeDef ATSHA_GetSerialNumber(ATSHA204_Device *me) {
	ATSHA204_StatusTypeDef status = ATSHA204_ERROR;
	uint8_t counter = 0;
	atsha204_variable.byte		= 0;
	memset(me->readBuffer, 0, MAX_READ_WRITE_SIZE);

	status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_WAKE, me, 0);
	status = ATSHA204_Read(me, CONFIG_ZONE, 0, 4);
	status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_SLEEP, me, 0);
	for(uint8_t k = 0; k < 4; k++)	{
		me->_configData.serialNum[k+counter] = me->readBuffer[k+1];
	}
	counter += 4;
	status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_WAKE, me, 0);
	status = ATSHA204_Read(me, CONFIG_ZONE, 2, 4);
	status = (ATSHA204_StatusTypeDef)ATSHA204_LL(ATSHA204_SLEEP, me, 0);
	for(uint8_t k = 0; k < 4; k++)	{
		me->_configData.serialNum[k+counter] = me->readBuffer[k+1];
	}
	return status;
}
