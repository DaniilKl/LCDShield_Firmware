#ifndef _LCD_H
#define _LCD_H

#include "stm32f4xx_hal.h"
#include "lcddefines.h"


/*File name: lcd.h;
* Author: Daniil;
* Platform: Windows 10 x64;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: Unfortunately I have not tested this driver with 8-pin interface because R/W pin is connected to the ground (0 state), therefore BF 
*						(Busy Flag) and read mode is not tested as well. If you are using LCD with 8 bit interface make sure you have defined _LCD_8BIT_INTERFACE.
* Description: Header file with functions for brief acquaintance with the LCD using STM boards. I hope I manage to cover all of states but
* 						 unfortunately I have only 6 pins from 11 available. I'm going to unsolder it soon. For more detailed explanation check the documentation
* 						 for HD44780U controller. I have also six buttons on my shield: one RESET button (which is connected to the Reset pin on my STM board) 
*							 and five function buttons named as: SELECT, LEFT, DOWN, UP and RIGHT (all of the are connected to the one analog pin), so I decided to 
*							 write a few simple functions for it which use ADC module in poll mode.
* Last update: 27.03.2023.
*/

//Wornings:
#ifndef STM32F401xE
	#warning This LCD firmware has not been tested on this board. Some bugs may appear.
#endif //#ifndef STM32F40xE

#ifndef _LCD_8BIT_INTERFACE
#define _LCD_4BIT_INTERFACE
#endif

//LCDInitStructTypedef
typedef struct{
	//Only for 8 data pins connected:
#ifdef _LCD_8BIT_INTERFACE
	GPIO_TypeDef *D0_Port;
	GPIO_TypeDef *D1_Port;
	GPIO_TypeDef *D2_Port; 
	GPIO_TypeDef *D3_Port;
	uint16_t D0_Pin; //DB4 on LCD shield;
	uint16_t D1_Pin; //DB5 on LCD shield;
	uint16_t D2_Pin; //DB6 on LCD shield;
	uint16_t D3_Pin; //DB7 on LCD shield;
#endif //#ifdef _LCD_8BIT_INTERFACE
	
	GPIO_TypeDef *D4_Port;
	GPIO_TypeDef *D5_Port;
	GPIO_TypeDef *D6_Port;
	GPIO_TypeDef *D7_Port;
	uint16_t D4_Pin; //DB0 on LCD shield;
	uint16_t D5_Pin; //DB1 on LCD shield;
	uint16_t D6_Pin; //DB2 on LCD shield;
	uint16_t D7_Pin; //DB3 on LCD shield.
	
	//Control pins:
	GPIO_TypeDef *RS_Port;
	GPIO_TypeDef *E_Port;
	uint16_t RS_Pin;
	uint16_t E_Pin;

	uint8_t InitSettings; //Check the first section in lcddefines.h file. 
	uint8_t MoveDirection; //Cursor or display move directions; check the second section in lcddefines.h file.
	uint8_t DispSettings; //Display settings; check the third section in lcddefines.h file.
}LCDInitStructTypedef;


//Functions for LCD:
void LCD_Init(LCDInitStructTypedef LCDStruct); //Initializes LCD display;
void LCD_Write(LCDInitStructTypedef LCDStruct, uint8_t dtw);/*
													* Writes 4 (for 4 most significant
													* pin connected only) or 8 (for all 8 data pins
													* connected, with _LCD_8BIT_INTERFACE defined) bits
													* depending on your pin configuration.
													* */
void LCD_WriteData(LCDInitStructTypedef LCDStruct, uint8_t dtw); //Writes a char;
void LCD_WriteCommand(LCDInitStructTypedef LCDStruct, uint8_t dtw); //Writes a command;
void LCD_WriteText(LCDInitStructTypedef LCDStruct, uint8_t *ttw); //Writes a string;
void LCD_SetCursorPos(LCDInitStructTypedef LCDStruct, uint8_t position);/*
 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	* Sets cursor position from available DDRAM range
 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	* (in my case 0-80 (in decimal)) without changing DDRAM data.
 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	* As an argument takes position from available range of DDRAM, for
 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	* example: I want to move cursor to second position on 1'st line
 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	* therefore i insert 0x01.
 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	* */
void LCD_MoveCursorRight(LCDInitStructTypedef LCDStruct); //Increments cursor position in DDRAM.
void LCD_MoveCursorLeft(LCDInitStructTypedef LCDStruct); //Decrements cursor position in DDRAM.

//Functions for using patterns:
/*To underastand patterns try this example:
	uint8_t pattern[8]={0b10101, //Place it in your main function after initialization, replacing "LCDInitStruct" 
											0b01010, //in the functions below by your LCD structure.
											0b01111,
											0b01101,
											0b01111,
											0b11110,
											0b11111,
											0b01010};
	LCD_WriteCGRAM5x8(LCDInitStruct, pattern, 0x00);
	LCD_ReadCGRAM5x8(LCDInitStruct, 0x00);
	HAL_Delay(1000);
	LCD_WriteCommand(LCDInitStruct,LCD_CLEAN);
	LCD_WriteCommand(LCDInitStruct,LCD_HOME);
*/
void LCD_WriteCGRAM5x8(LCDInitStructTypedef LCDStruct, uint8_t pattern[8], uint8_t CGRAMcell_num);/*
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* You can write your own pattern by editing CGRAM cells. HD44780U
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* provides eight cells for 5x8 patterns and 4 cells for 5x10 patterns.
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* This function provides writing 5x8 patterns which are located in the
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* addresses from 0x00 to 0x07. So by referring to it you can read your
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* own patterns. As an arguments this function takes number of cell where
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* you want to write your pattern (from available, for example: 0x02
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* (in hexadecimal) or 2 (in decimal)) and, of course, the pattern which is
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* specified by the table with size 8.
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	* */
void LCD_WriteCGRAM5x10(LCDInitStructTypedef LCDStruct, uint8_t pattern[10], uint8_t CGRAMcell_num);/*
																  * For writing 5x10 patterns.
																  * */
void LCD_ReadCGRAM5x8(LCDInitStructTypedef LCDStruct, uint8_t CGRAMcell_num);//Reads and displays a pattern with size 5x8. CGRAMcell_num = 0...7.
void LCD_ReadCGRAM5x10(LCDInitStructTypedef LCDStruct, uint8_t CGRAMcell_num);//Reads and displays a pattern with size 5x10. CGRAMcell_num = 0...3.


//Definitions for buttons:
//Limits to determine which button is pressed (example), can be redefined.
#define RIGHT_LOWLIMIT 0
#define RIGHT_UPLIMIT 200

#define UP_LOWLIMIT 400
#define UP_UPLIMIT 800

#define DOWN_LOWLIMIT 1300
#define DOWN_UPLIMIT 1700

#define LEFT_LOWLIMIT 2200
#define LEFT_UPLIMIT 2600

#define SELECT_LOWLIMIT 3500
#define SELECT_UPLIMIT 3900

//Auxiliary definitions:
#define RIGHT 0b00000001
#define UP 0b00000010
#define DOWN 0b00000100
#define LEFT 0b00001000
#define SELECT 0b00010000
#define NOTHING_IS_PRESSED 0b00100000

//Functions for buttons:
void Buttons_CheckLimits(LCDInitStructTypedef LCDStruct, ADC_HandleTypeDef *ADC_HandleStructure);/*
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	*Checks limits returned by ADC and prints it on LCD.
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	*/
uint8_t Buttons_Check(LCDInitStructTypedef LCDStruct, ADC_HandleTypeDef *ADC_HandleStructure);/*
 	 	 	 	 	 	 	 	 	 	 	   * Returns inf. about which button is pressed in one byte. Only one pressed button in every
 	 	 	 	 	 	 	 	 	 	 	   * moment of time  is possible, because buttons with higher limits overwrite buttons with lower
 	 	 	 	 	 	 	 	 	 	 	   * limits. Result is encrypted into six bits started from  LSB. So, here are described all
 	 	 	 	 	 	 	 	 	 	 	   * possible combinations:
 	 	 	 	 	 	 	 	 	 	 	   * 0b00000001 - RIGHT button is pressed;
 	 	 	 	 	 	 	 	 	 	 	   * 0b00000010 - UP button is pressed;
 	 	 	 	 	 	 	 	 	 	 	   * 0b00000100 - DOWN button is pressed;
 	 	 	 	 	 	 	 	 	 	 	   * 0b00001000 - LEFT button is pressed;
 	 	 	 	 	 	 	 	 	 	 	   * 0b00010000 - SELECT button is pressed.
 	 	 	 	 	 	 	 	 	 	 	   * 0b00100000 - Nothing is pressed.
 	 	 	 	 	 	 	 	 	 	 	   */



#endif //#ifndef _LCD_H
