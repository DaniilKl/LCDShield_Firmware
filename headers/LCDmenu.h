#ifndef __LCDMENU_H_
#define __LCDMENU_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "lcd.h"
#include "main.h"


/*File name: LCDmenu.h;
* Author: Daniil;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: -
* Description: Menu middleware for LCD.
* Last update: 27.03.2023.
*/

#ifndef MAX_LINE_SIZE
#define MAX_LINE_SIZE 16 //Determines how many chars are visible per LCD line at a time.
#endif //#ifndef MAX_LINE_SIZE


//Menu structure typedef which contains main data.
typedef struct{
	uint8_t **MenuList;/*
						* Must be initialized by dynamically allocated data, so you can have unlimited amount of menu lines. This variable points to a table of pointers which point
						* to each menu line. Size of the menu lines is unlimited. And, eah... it is a pretty controvercial idea to do some dynamic allocation in embedded, be careful.
						* */
	uint8_t AmountOfMenuLines;// Must be initialized with 2.
	uint8_t CurrentLinePos;//This variable indicates which line is chosen at every time slice. I miplemented this middleware for CurrentLinePos = [0,1], because 
												 //I have only two lines available on my LCD. Must be initialized with 0 or 1;
	uint8_t LinesToDisplay[2];// Lines which are currently displayed on LCD.
}MenuStructureTypedef;


void Menu_Display(MenuStructureTypedef *MenuStruct, LCDInitStructTypedef LCDStruct); /*Displays menu on LCD depending on ChangesFlag.*/
//Move cursor UP/DOWN:
void Menu_MoveUP(MenuStructureTypedef *MenuStruct);
void Menu_MoveDOWN(MenuStructureTypedef *MenuStruct);

#ifdef TWO_OR_MORE_MENULINES //This function work currently when there is more than 1 menu lines.
//Moves cursor to a certain line:
void Menu_Move(MenuStructureTypedef *MenuStruct, uint8_t Position);
#endif //#ifdef TWO_OR_MORE_MENULINES

uint8_t Menu_ReturnPos(MenuStructureTypedef MenuStruct); /* As I mentioned above returns your current vertical position determined by number of menu
														  * line (I reccomend you to check the logic in LCDmenu.hpp file).*/


#endif //#ifndef __LCDMENU_H_
