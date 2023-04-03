/*File name: main.c;
* Author: Daniil;
* Platform: Windows 10 x64;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: -
* Description: -
* Last update: 27.03.2023.
*/

#include "main.h"

//I do not like any mess in the main function. So an initialization is in static inline functions.
static __INLINE void SysInit(ADC_HandleTypeDef *ADC_HandleStructure);
static __INLINE void LCDInit(LCDInitStructTypedef *LCDInitStructure);

int main(void){
	ADC_HandleTypeDef ADC_HandleStructure = {0};
	SysInit(&ADC_HandleStructure);	//System initialization function;
	
	LCDInitStructTypedef LCDInitStructure = {0};
	LCDInit(&LCDInitStructure); //LCD initialization;
	
	HAL_ADC_Start(&ADC_HandleStructure); //ADC start.
	
	
	//Simple example:
	char str[]="Hello world!";
	LCD_WriteText(LCDInitStructure, str);
	HAL_Delay(500);
	LCD_WriteCommand(LCDInitStructure, LCD_CLEAN);
	
	while(1){
		/*Here is a test of your ADC initialization/configuration. If you have done it right LCD should display different values 
		when you press any of the buttons on LCD shield (apart of RESET button of course).*/
		Buttons_CheckLimits(LCDInitStructure, &ADC_HandleStructure);
		HAL_Delay(250);
	}
}


static __INLINE void SysInit(ADC_HandleTypeDef *ADC_HandleStructure)
{
	HAL_Init(); //HAL initialization.
	
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	//Clocks enabling:
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	//GPIOA port initialization:
	GPIO_InitStructure.Pin = LCD_RSPin | LCD_D7Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LED_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC pin initialization:
	GPIO_InitStructure.Pin = LCD_BUTTONSPin;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	//GPIOB port initialization:
	GPIO_InitStructure.Pin = LCD_D4Pin | LCD_D5Pin | LCD_D6Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIOC port initialization:
	GPIO_InitStructure.Pin = LCD_EPin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//ADC1 initialization:
	__HAL_RCC_ADC1_CLK_ENABLE(); //ADC1 clock enabling.
	
	ADC_HandleStructure->Instance = ADC1;
	ADC_HandleStructure->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC_HandleStructure->Init.Resolution = ADC_RESOLUTION_12B;
	ADC_HandleStructure->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_HandleStructure->Init.ScanConvMode = DISABLE;
	ADC_HandleStructure->Init.EOCSelection = ADC_EOC_SEQ_CONV;
	ADC_HandleStructure->Init.ContinuousConvMode = ENABLE;
	ADC_HandleStructure->Init.NbrOfConversion = 1;
	ADC_HandleStructure->Init.DiscontinuousConvMode = DISABLE;
	ADC_HandleStructure->Init.NbrOfDiscConversion = 1;
	ADC_HandleStructure->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC_HandleStructure->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_HandleStructure->Init.DMAContinuousRequests = DISABLE;
	HAL_ADC_Init(ADC_HandleStructure);
	
	//ADC channel 0 configuration:
	ADC_ChannelConfTypeDef ADC_ChannelConfStructure = {0};
	
	ADC_ChannelConfStructure.Channel = ADC_CHANNEL_0;
	ADC_ChannelConfStructure.Rank = 1;
	ADC_ChannelConfStructure.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC_ChannelConfStructure.Offset = 0;
	HAL_ADC_ConfigChannel(ADC_HandleStructure, &ADC_ChannelConfStructure);
}

static __INLINE void LCDInit(LCDInitStructTypedef *LCDInitStructure){
	//LCD data pins initialization:
	LCDInitStructure->D4_Port = LCD_D4Port;
	LCDInitStructure->D5_Port = LCD_D5Port;
	LCDInitStructure->D6_Port = LCD_D6Port;
	LCDInitStructure->D7_Port = LCD_D7Port;
	LCDInitStructure->D4_Pin = LCD_D4Pin;
	LCDInitStructure->D5_Pin = LCD_D5Pin;
	LCDInitStructure->D6_Pin = LCD_D6Pin;
	LCDInitStructure->D7_Pin = LCD_D7Pin;
	
	//LCD control pinns initialization:
	LCDInitStructure->E_Port = LCD_EPort;
	LCDInitStructure->RS_Port = LCD_RSPort;
	LCDInitStructure->E_Pin = LCD_EPin;
	LCDInitStructure->RS_Pin = LCD_RSPin;
	
	LCDInitStructure->MoveDirection = SET_CUR_MOVE_DIR | INC_CURSOR | NO_DISP_SHIFT; //Cursor move direction: right. No display shifting. 
	LCDInitStructure->InitSettings = SET_DISP_START_SETT | INT_DATA_LENGTH_4B | TWO_LINES | FONT_5X8; //4-pin LCD interface. Two lines. Font size: 5x8 pixels.
	LCDInitStructure->DispSettings = SET_DISP_SETT | TURN_DISP_ON | TURN_CURSOR_ON | TURN_CURSOR_BLINK_ON; //Display ON. Cursor ON. Cursor blinking mode ON.
	LCD_Init(*LCDInitStructure);
}