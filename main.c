/*File name: main.c;
* Author: Daniil;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: -
* Description: main.h and main.c files consist two examples: an example of using LCD firmware and an example of using menu middleware for LCD.
*							 The LCD firmware example is compiled automatically. If you want to compile menu example - define MENU_MIDDLEWARE_EXAMPLE.
* Last update: 07.04.2023.
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
	
	#ifdef MENU_MIDDLEWARE_EXAMPLE
	//Here is an example for LCDmenu.h middleware:
	
	//First menu initialization:
	MenuStructureTypedef Menu_1 = {0}; //Declaration
	Menu_1.MenuList = (uint8_t**)malloc(sizeof(uint8_t)*4*17); //Allocation
	
	//Initialization:
	Menu_1.MenuList[0] = "****************";
	Menu_1.MenuList[1] = "Start";
	Menu_1.MenuList[2] = "Options";
	Menu_1.MenuList[3] = "****************";
	Menu_1.AmountOfMenuLines = 4; //Amount of menu lines in MenuList variable
	
	//Menu_1 startup position settings:
	Menu_1.LinesToDisplay[0] = 1;
	Menu_1.LinesToDisplay[1] = 2;
	Menu_1.CurrentLinePos = 0;
	
	//Option menu initialization:
	MenuStructureTypedef Menu_Options = {0}; //Declaration
	Menu_Options.MenuList = (uint8_t**)malloc(sizeof(uint8_t*)*5*17); //Allocation
	
	//Initialization:
	Menu_Options.MenuList[0] = "****************";
	Menu_Options.MenuList[1] = "Option 1";
	Menu_Options.MenuList[2] = "Option 2";
	Menu_Options.MenuList[3] = "Return";
	Menu_Options.MenuList[4] = "****************";
	Menu_Options.AmountOfMenuLines = 5; //Amount of menu lines in MenuList variable
	
	//Menu_Options startup position settings:
	Menu_Options.LinesToDisplay[0] = 1;
	Menu_Options.LinesToDisplay[1] = 2;
	Menu_Options.CurrentLinePos = 0;
	
	uint8_t MenuToDisplay = 0; //0(false) - display "Menu_1", 1(true) - display "Menu_Options"
	
	Menu_Display(&Menu_1, LCDInitStructure); //Displays menu
	#endif //#ifdef MENU_MIDDLEWARE_EXAMPLE
	
	while(1){
		#ifdef LCD_FIRMWARE_EXAMPLE
		/*Here is a test of your ADC initialization/configuration. If you have done it right LCD should display different values 
		when you press any of the buttons on LCD shield (apart of RESET button of course).*/
		Buttons_CheckLimits(LCDInitStructure, &ADC_HandleStructure);
		HAL_Delay(250);
		#endif //#ifdef LCD_FIRMWARE_EXAMPLE
		
		#ifdef MENU_MIDDLEWARE_EXAMPLE
		
		//Checking for buttons on LCD:
		if ((Buttons_Check(LCDInitStructure, &ADC_HandleStructure) == UP)){ //If UP button is pressed
			switch (MenuToDisplay) {
				case 0:
					Menu_MoveUP(&Menu_1); //Moves cursor up
					Menu_Display(&Menu_1, LCDInitStructure);
					break;
				case 1:
					Menu_MoveUP(&Menu_Options); //Moves cursor up
					Menu_Display(&Menu_Options, LCDInitStructure);
					break;
			}
		}
		else if ((Buttons_Check(LCDInitStructure, &ADC_HandleStructure) == DOWN)){ //If DOWN button is pressed
			switch (MenuToDisplay) {
				case 0:
					Menu_MoveDOWN(&Menu_1); //Moves cursor down
					Menu_Display(&Menu_1, LCDInitStructure);
					break;
				case 1:
					Menu_MoveDOWN(&Menu_Options); //Moves cursor down
					Menu_Display(&Menu_Options, LCDInitStructure);
					break;
			}
		}
		else if ((Buttons_Check(LCDInitStructure, &ADC_HandleStructure) == SELECT)){ /*If SELECT button is pressed 
																				and certain condition is true - change "MenuToDisplay" variable value*/
			switch (MenuToDisplay){
				case 0:
					if (Menu_ReturnPos(Menu_1) == 2){ //If "Options" line in Menu_1 is selected
						MenuToDisplay = 1; //Display Menu_Options
						
						//Return Menu_Options to initial position state in case it was changed before:
						Menu_Options.LinesToDisplay[0] = 1;
						Menu_Options.LinesToDisplay[1] = 2;
						Menu_Options.CurrentLinePos = 0;
						
						Menu_Display(&Menu_Options, LCDInitStructure); //Display Menu_Options
					}
					break;
				case 1:
					if (Menu_ReturnPos(Menu_Options) == 3){ //If "Return" line in Menu_Options is selected
						MenuToDisplay = 0; //Display Menu_1
						
						//Return Menu_1 to initial position state in case it was changed before:
						Menu_1.LinesToDisplay[0] = 1;
						Menu_1.LinesToDisplay[1] = 2;
						Menu_1.CurrentLinePos = 0;
						
						Menu_Display(&Menu_1, LCDInitStructure); //Display Menu_1
					}
					break;
			}
		}
		
		#endif //#ifdef MENU_MIDDLEWARE_EXAMPLE
	}
}


static __INLINE void SysInit(ADC_HandleTypeDef *ADC_HandleStructure){
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