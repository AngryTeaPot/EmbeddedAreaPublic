#include "BitBanging_spi.h"
#include "main.h"

/*				FUNCTIONS				*/
/**
  * @brief  Inits SPI. This function inits chip select and clk pins
  */
void BitBangSpi_Init(void){
	GPIO_OUTPUT_HIGH(SPI_PORT,SPI_CS);
	if(CPOL)
		GPIO_OUTPUT_HIGH(SPI_PORT,SPI_CLK);
}

/**
  * @brief  Reads the 1 bit data from GPIO
  * @retval Related GPIO output
  */
uint8_t BitBangSpi_bitRead(){

	return GPIO_INPUT_READ(SPI_PORT, SPI_MISO);
}

/**
  * @brief  Writes an 1 bit data.
  * @param  bit: variable that configures pin output
  */
uint8_t BitBangSpi_bitWrite(uint8_t bit){
	
	uint8_t result = 0;
	
	if (bit)
	{
		// Write '1' bit
		GPIO_OUTPUT_HIGH(SPI_PORT, SPI_MOSI);
	}
	else
	{
		// Write '0' bit
		GPIO_OUTPUT_LOW(SPI_PORT, SPI_MOSI);
	}
	return result;
}

/**
  * @brief  Writes an 1 byte data.
  * @retval Received 1 byte value from GPIO
  */
uint8_t BitBangSpi_byteWrite(uint8_t data){
	
	uint8_t result = 0;
	
	// Loop to write each bit in the byte, LS-bit first
	for (uint8_t loop = 0; loop < 8; loop++)
	{
		if(CPOL){
			GPIO_OUTPUT_LOW(SPI_PORT, SPI_CLK);}
		else
			GPIO_OUTPUT_HIGH(SPI_PORT, SPI_CLK);

		BitBangSpi_bitWrite(data & 0x80);
		// shift the data byte for the next bit
		result  |= (BitBangSpi_bitRead() << loop);

		if(CPOL){
			GPIO_OUTPUT_HIGH(SPI_PORT, SPI_CLK);}
		else
			GPIO_OUTPUT_LOW(SPI_PORT, SPI_CLK);

		data = data << 1;
	}

	return result;
}

/**
  * @brief  Sends the 1 byte data via GPIO
  * @param  Tx_data: Variable to the transmits data address
  * @retval Related GPIO output
  */
uint8_t BitBangSpi_sendByte(uint8_t	Tx_data)
{
	// Incoming data buffer
	uint8_t Rx_data = 0;

	// To Start the Communication make sure that Chip Select is low.
	GPIO_OUTPUT_LOW(SPI_PORT,SPI_CS);

	// Writing data through to the IO pins
	Rx_data = BitBangSpi_byteWrite(Tx_data);

	// To Finish the Communication make sure that Chip Select is low.
	GPIO_OUTPUT_HIGH(SPI_PORT,SPI_CS);

	return Rx_data;
}



