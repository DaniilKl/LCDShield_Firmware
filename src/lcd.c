#include "lcd.h"

/*File name: lcd.c;
* Author: Daniil;
* Platform: Windows 10 x64;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: Unfortunately I have not tested this driver with 8-pin interface because R/W pin is connected to the ground (0 state), therefore BF 
*						(Busy Flag) and read mode is not tested as well. If you are using LCD with 8 bit interface make sure you have defined _LCD_8BIT_INTERFACE.
* Description: Source file with functions for brief acquaintance with the LCD using STM boards. I hope I manage to cover all of states but
* 						 unfortunately I have only 6 pins from 11 available. I'm going to unsolder it soon. For more detailed explanation check the documentation
* 						 for HD44780U controller. I have also six buttons on my shield: one RESET button (which is connected to the Reset pin on my STM board) 
*							 and five function buttons named as: SELECT, LEFT, DOWN, UP and RIGHT (all of the are connected to the one analog pin), so I decided to 
*							 write a few simple functions for it which use ADC module in poll mode.
* Last update: 27.03.2023.
*/

//Auxiliary functions:
static GPIO_PinState Int2GPIO_PinState(uint8_t x, uint8_t y){ return (x & y) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET; }

//LCD functions:
void LCD_Init(LCDInitStructTypedef LCDStruct) {
	//Initialization process according to documentation for HD44780U controller:
	HAL_Delay(50);

	HAL_GPIO_WritePin(LCDStruct.RS_Port, LCDStruct.RS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCDStruct.E_Port, LCDStruct.E_Pin, GPIO_PIN_RESET);
	LCD_Write(LCDStruct, 0x3);
	HAL_Delay(5);
	LCD_Write(LCDStruct, 0x3);
	HAL_Delay(1);
	LCD_Write(LCDStruct, 0x3);

	LCD_Write(LCDStruct, 0x2);
	LCD_WriteCommand(LCDStruct, LCDStruct.InitSettings);
	LCD_WriteCommand(LCDStruct, 0x08);
	LCD_WriteCommand(LCDStruct, LCD_CLEAN);
	LCD_WriteCommand(LCDStruct, LCDStruct.MoveDirection);
	LCD_WriteCommand(LCDStruct, LCDStruct.DispSettings);
	LCD_WriteCommand(LCDStruct, LCD_CLEAN);
}

void LCD_Write(LCDInitStructTypedef LCDStruct, uint8_t dtw) {
	HAL_GPIO_WritePin(LCDStruct.E_Port, LCDStruct.E_Pin, GPIO_PIN_RESET);
	
#ifdef _LCD_8BIT_INTERFACE
	HAL_GPIO_WritePin(LCDStruct.D0_Port, LCDStruct.D0_Pin, Int2GPIO_PinState(dtw, 0x01));
	HAL_GPIO_WritePin(LCDStruct.D1_Port, LCDStruct.D1_Pin, Int2GPIO_PinState(dtw, 0x02));
	HAL_GPIO_WritePin(LCDStruct.D2_Port, LCDStruct.D2_Pin, Int2GPIO_PinState(dtw, 0x04));
	HAL_GPIO_WritePin(LCDStruct.D3_Port, LCDStruct.D3_Pin, Int2GPIO_PinState(dtw, 0x08));
#endif //#ifdef _LCD_8BIT_INTERFACE
	HAL_GPIO_WritePin(LCDStruct.D4_Port, LCDStruct.D4_Pin, Int2GPIO_PinState(dtw, 0x10));
	HAL_GPIO_WritePin(LCDStruct.D5_Port, LCDStruct.D5_Pin, Int2GPIO_PinState(dtw, 0x20));
	HAL_GPIO_WritePin(LCDStruct.D6_Port, LCDStruct.D6_Pin, Int2GPIO_PinState(dtw, 0x40));
	HAL_GPIO_WritePin(LCDStruct.D7_Port, LCDStruct.D7_Pin, Int2GPIO_PinState(dtw, 0x80));


	HAL_GPIO_WritePin(LCDStruct.E_Port, LCDStruct.E_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(LCDStruct.E_Port, LCDStruct.E_Pin, GPIO_PIN_RESET);
	HAL_Delay(2);
}

#ifndef _LCD_8BIT_INTERFACE
void LCD_WriteData(LCDInitStructTypedef LCDStruct, uint8_t dtw) {
	HAL_GPIO_WritePin(LCDStruct.RS_Port, LCDStruct.RS_Pin, GPIO_PIN_SET);
	
	LCD_Write(LCDStruct, dtw & 0xf0);
	LCD_Write(LCDStruct, dtw << 4);
}
void LCD_WriteCommand(LCDInitStructTypedef LCDStruct, uint8_t dtw) {
	HAL_GPIO_WritePin(LCDStruct.RS_Port, LCDStruct.RS_Pin, GPIO_PIN_RESET);
	
	LCD_Write(LCDStruct, dtw & 0xf0);
	LCD_Write(LCDStruct, dtw << 4);
}
#endif //#ifndef _LCD_8BIT_INTERFACE

#ifdef _LCD_8BIT_INTERFACE
void LCD_WriteData(LCDInitStructTypedef LCDStruct, uint8_t dtw) {
	HAL_GPIO_WritePin(LCDStruct.RS_Port, LCDStruct.RS_Pin, GPIO_PIN_SET);
	LCD_Write(LCDStruct, dtw);
}
void LCD_WriteCommand(LCDInitStructTypedef LCDStruct, uint8_t dtw) {
	HAL_GPIO_WritePin(LCDStruct.RS_Port, LCDStruct.RS_Pin, GPIO_PIN_RESET);
	LCD_Write(LCDStruct, dtw);
}
#endif //#ifdef _LCD_8BIT_INTERFACE

void LCD_WriteText(LCDInitStructTypedef LCDStruct, uint8_t *ttw) {
	while (*ttw)
		LCD_WriteData(LCDStruct, *ttw++);
}

void LCD_SetCursorPos(LCDInitStructTypedef LCDStruct, uint8_t position){
	LCD_WriteCommand(LCDStruct, 0x80 | position);
}

void LCD_MoveCursorRight(LCDInitStructTypedef LCDStruct){
	LCD_WriteCommand(LCDStruct, SHIFT | SHIFT_CURSOR | SHIFT_RIGHT);
}

void LCD_MoveCursorLeft(LCDInitStructTypedef LCDStruct){
	LCD_WriteCommand(LCDStruct, SHIFT | SHIFT_CURSOR | SHIFT_LEFT);
}

void LCD_WriteCGRAM5x8(LCDInitStructTypedef LCDStruct, uint8_t pattern[8], uint8_t CGRAMcell_num){
	LCD_WriteCommand(LCDStruct, 0x40 | (8*CGRAMcell_num));
	for (uint8_t i =0; i<8;i++)
		LCD_WriteData( LCDStruct, pattern[i]);
	LCD_WriteCommand(LCDStruct, LCD_HOME);
}
void LCD_WriteCGRAM5x10(LCDInitStructTypedef LCDStruct, uint8_t pattern[10], uint8_t CGRAMcell_num){
	LCD_WriteCommand(LCDStruct, 0x40 | (16*CGRAMcell_num));
	for (uint8_t i =0; i<10;i++)
		LCD_WriteData( LCDStruct, pattern[i]);
	LCD_WriteCommand(LCDStruct, LCD_HOME);
}
void LCD_ReadCGRAM5x8(LCDInitStructTypedef LCDStruct, uint8_t CGRAMcell_num){
	LCD_WriteData( LCDStruct, CGRAMcell_num);
}
void LCD_ReadCGRAM5x10(LCDInitStructTypedef LCDStruct, uint8_t CGRAMcell_num){
	LCD_WriteData( LCDStruct, CGRAMcell_num << 1);
}


//Buttons functions:
void Buttons_CheckLimits(LCDInitStructTypedef LCDStruct, ADC_HandleTypeDef *ADC_HandleStructure){
	LCD_WriteCommand(LCDStruct, LCD_CLEAN);
	uint32_t value = HAL_ADC_GetValue(ADC_HandleStructure);
	uint8_t tab[11] = {0};
	sprintf(tab,"%d", value);
	LCD_WriteText(LCDStruct, tab);
}

uint8_t Buttons_Check(LCDInitStructTypedef LCDStruct, ADC_HandleTypeDef *ADC_HandleStructure){
	uint32_t value = HAL_ADC_GetValue(ADC_HandleStructure);
	if (value>=RIGHT_LOWLIMIT && value<=RIGHT_UPLIMIT)
		return RIGHT;
	else if (value>=UP_LOWLIMIT && value <= UP_UPLIMIT)
		return UP;
	else if (value>=DOWN_LOWLIMIT && value <=DOWN_UPLIMIT)
		return DOWN;
	else if (value>=LEFT_LOWLIMIT && value <=LEFT_UPLIMIT)
		return LEFT;
	else if (value>=SELECT_LOWLIMIT && value <=SELECT_UPLIMIT)
		return SELECT;
	else
		return NOTHING_IS_PRESSED;
}
