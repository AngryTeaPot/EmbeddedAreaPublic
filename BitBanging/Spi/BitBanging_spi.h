#ifndef __BITBANGING_SPI_H__
#define	__BITBANGING_SPI_H__


#include "main.h"


/*				DEFINES				*/

/*
 *Spi general ports
*/
#define SPI_PORT	GPIOE
	
/*
 *Spi general pins
*/
#define SPI_CLK		GPIO_PIN_15
#define SPI_MOSI	GPIO_PIN_7
#define SPI_MISO	GPIO_PIN_12
#define SPI_CS		GPIO_PIN_10

#define CPOL		1


/*				MACROS				*/
#define	GPIO_OUTPUT_HIGH(port,pin)		HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
#define	GPIO_OUTPUT_LOW(port,pin)		HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
#define GPIO_INPUT_READ(port,pin)		HAL_GPIO_ReadPin(port, pin);
// MicroSecond Delay
#define DELAYUS(x)		{\
							uint32_t m = (x * 30);\
							while(m > 0 ){ m--;}  \
						}


/*				FUNTION PROTOTYPES				*/
uint8_t BitBangSpi_sendByte(uint8_t	Tx_data);
void BitBangSpi_Init(void);

#endif
