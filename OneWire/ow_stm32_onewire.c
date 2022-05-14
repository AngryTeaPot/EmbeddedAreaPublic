#include "string.h"
#include "ow_stm32_onewire.h"

#if FREERTOS
    #include "FreeRTOS.h"
    #include "task.h"
#endif


int A,B,C,D,E,F,G,H,I,J;

/*
 *	Global Variables That uses while searching device
*/
uint8_t  ROM_NO[8] ;

/*
 *	CRC8 variable
*/
unsigned char crc8;

int OW_ReadBit(void);

static unsigned char dscrc_table[] = {
        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
      157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
       35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
      190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
       70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
      219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
      101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
      248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
      140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
       17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
      175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
       50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
      202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
       87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
      233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
      116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

//--------------------------------------------------------------------------
// Calculate the CRC8 of the byte value provided with the current 
// global 'crc8' value. 
// Returns current global crc8 value
//
unsigned char docrc8(unsigned char value)
{   
   crc8 = dscrc_table[crc8 ^ value];
   return crc8;
}

/*
 *	Configures the related GPIO port pin modes.
*/
void OW_GPIO_Config( GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,uint32_t GPIO_Mode,uint32_t GPIO_PuPd,uint32_t GPIO_Speed){
	GPIO_InitTypeDef  gpio_prms;
	gpio_prms.Pin 	= GPIO_Pin;
	gpio_prms.Mode 	= GPIO_Mode;
	gpio_prms.Pull 	= GPIO_PuPd;	
	(GPIO_Mode == GPIO_MODE_OUTPUT_PP ) ? gpio_prms.Speed = GPIO_Speed : 0;
	
	HAL_GPIO_Init(GPIOx, &gpio_prms);
}

/*
 *	FOR SET-UP First of all OW_OneWire_Init();
 *			  				OW_SetSpeed();
 *							OW_TouchReset(); must be called.
*/
void OW_OneWire_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	
	memset(ROM_NO, 0 , 8);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* Init GPIO pin */
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	ONEWIRE_IO_Init(GPIOx, &GPIO_InitStruct);

	ONEWIRE_HIGH(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	/* Save settings */
	OW_SetSpeed(STANDARD_SPEED);
}

//-----------------------------------------------------------------------------
// Set the 1-Wire timing to 'standard' (standard=1) or 'overdrive' (standard=0).
//
void OW_SetSpeed(int standard){
	// Adjust tick values depending on speed
	
	if (standard)
	{
			// Standard Speed
			A = 8  ;	  // 6us  min5  - max15 us    * 30
			B = 64 ;      // 64us (65 - A)us          * 30
			C = 60 ;      // 60us min60 - max120 us   * 30
			D = 12 ;      // 10us (65 + B)us          * 30
			E = 9  ;      // 9  * 4			          * 30
			F = 55 ;      // 55 * 4			          * 30
			G = 0  ;      //                		  * 30
			H = 480;      // using in touch reset     * 30
			I = 70 ;      // using in touch reset     * 30
			J = 410;
	}
	else
	{
			// Overdrive Speed
			A = 2  ;
			B = 8  ;
			C = 8  ;
			D = 2  ;
			E = 1  ;
			F = 7  ;
			G = 2  ;
			H = 70 ;
			I = 8  ;
			J = 40 ;
	}
}

//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
// 
//
int OW_TouchReset(void)
{
	int result;
	
	ONEWIRE_DELAY(G);
	ONEWIRE_LOW(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	ONEWIRE_DELAY(H);
	ONEWIRE_HIGH(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	ONEWIRE_INPUT(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	ONEWIRE_DELAY(I);
	result = ONEWIRE_READ(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin) ^ 0x01; // Sample for presence pulse from slave
	ONEWIRE_DELAY(J); // Complete the reset sequence recovery

	return result; // Return sample presence pulse result
}


void OW_WriteBit(uint8_t bit)
{
	if (bit)
	{
		// Write '1' bit
		ONEWIRE_LOW(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
		ONEWIRE_DELAY(A);
		ONEWIRE_HIGH(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
		ONEWIRE_DELAY(B); // Complete the time slot and 10us recovery
	}
	else
	{
		// Write '0' bit
		ONEWIRE_LOW(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
		ONEWIRE_DELAY(C);
		ONEWIRE_HIGH(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
		ONEWIRE_DELAY(D); // Complete the time slot and 10us recovery
	}
}


int OW_ReadBit(void)
{
	uint8_t result = 0;
	ONEWIRE_OUTPUT(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	ONEWIRE_LOW(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	ONEWIRE_DELAY(A); 
    ONEWIRE_INPUT(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	ONEWIRE_DELAY(E); // 9
	result = ONEWIRE_READ(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin); // Sample the bit value from the slave
	ONEWIRE_DELAY(F); // 55  Complete the time slot and 10us recovery

	return result;
}


void OW_WriteByte(uint8_t data)
{
	int loop;

	// Loop to write each bit in the byte, LS-bit first
	for (loop = 0; loop < 8; loop++)
	{
		OW_WriteBit(data & 0x01);
		// shift the data byte for the next bit
		data = data >> 1;
	}
	ONEWIRE_HIGH(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
}

uint8_t cmndcntr = 0 ;
uint8_t OW_ReadByte(void)
{
	uint8_t loop, result = 1;

	for (loop = 0; loop < 8; loop++)
	{
		// if result is one, then set MS bit
		if (OW_ReadBit())
			result = result | 0x80; // b10000000

		// shift the result to get it ready for the next bit
		result = result >> 1;

	}
  result = result << 1;

	if(cmndcntr % 2 == 0  && cmndcntr < 5 ){
		result = result | 0x01;;
	}
	ONEWIRE_DELAY(A+E);
	return result;
}

void OW_SkipRom(void)
{
	uint8_t presence = OW_TouchReset();
	ONEWIRE_OUTPUT(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	OW_WriteByte(ONEWIRE_CMD_SKIPROM);  // read rom command 
}

void OW_ReadRom(void)
{
	// 1-Wire reset
	uint8_t presence = OW_TouchReset();
	
	ONEWIRE_OUTPUT(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
	
	// issue the read rom command
	OW_WriteByte(ONEWIRE_CMD_READROM);  // read rom command 
	
	for (uint8_t i=0;	i<8;	i++)
	{
		 ROM_NO[i] = OW_ReadByte();
		 cmndcntr++;
	}
	
	ONEWIRE_OUTPUT(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin); 
}


int OW_ReadRomCommand(void)
{
	int rslt,i;
	OW_OneWire_Init(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);
    
#if FREERTOS
	/*
	 *	Because of the FreeRTOS context switching ONEWIRE_DELAY does not work accured.
	 *	taskENTER_CRITICAL(); used here.
	*/
	taskENTER_CRITICAL();
#endif
    
    OW_ReadRom();

	printh(" Family Code:: %02X \n Card id    :: ", ROM_NO[0]);
	// print device found
	for (i = 6; i >= 1; i--)
		 printh("%02X", ROM_NO[i]);
	
	printh("\n Crc Value  :: %02X", ROM_NO[7]);
    
#if FREERTOS

	/*
	 *	After the search completes taskEXIT_CRITICAL();
	*/
	taskEXIT_CRITICAL();
#endif
	ONEWIRE_HIGH(KART_ID_1WIRE_S1_GPIO_Port, KART_ID_1WIRE_S1_Pin);

	return 1;
}



