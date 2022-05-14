#ifndef OW_ONEWIRE_H
#define OW_ONEWIRE_H 100

#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "utility.h"



// definitions
#define FALSE 0
#define TRUE  1
#define FREERTOS

#define OW_DEBUG	1

/* OneWire delay */
#define ONEWIRE_DELAY(x)				delays(x * 30)

//GPIO_MODE_OUTPUT_PP
/* Pin  settings */
#define ONEWIRE_LOW(port,pin)		HAL_GPIO_WritePin(port,pin,GPIO_PIN_RESET)
#define ONEWIRE_HIGH(port,pin)		HAL_GPIO_WritePin(port,pin,GPIO_PIN_SET)
#define ONEWIRE_INPUT(port,pin)		OW_GPIO_Config(port, pin, GPIO_MODE_INPUT, GPIO_PULLUP,0)
#define ONEWIRE_OUTPUT(port,pin)	OW_GPIO_Config(port, pin, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,GPIO_SPEED_FREQ_VERY_HIGH)

#define ONEWIRE_READ	HAL_GPIO_ReadPin
#define ONEWIRE_IO_Init HAL_GPIO_Init

/* OneWire ROM commands */
#define ONEWIRE_CMD_RSCRATCHPAD			0xBE
#define ONEWIRE_CMD_WSCRATCHPAD			0x4E
#define ONEWIRE_CMD_CPYSCRATCHPAD		0x48
#define ONEWIRE_CMD_RECEEPROM			0xB8
#define ONEWIRE_CMD_RPWRSUPPLY			0xB4
#define ONEWIRE_CMD_SEARCHROM			0xF0
#define ONEWIRE_CMD_READROM				0x33
#define ONEWIRE_CMD_MATCHROM			0x55
#define ONEWIRE_CMD_SKIPROM				0xCC


typedef struct{
	uint8_t familyCode;
	uint8_t romID[6];
	uint8_t crcReadRom;	
}ReadRom_command_t;

typedef enum{
	OVERDRIVE_SPEED = 0,
	STANDARD_SPEED
}OW_SPEED;

int OW_ReadRomCommand(void);
void OW_SetSpeed(int standard);

#ifdef __cplusplus
}
#endif

#endif

