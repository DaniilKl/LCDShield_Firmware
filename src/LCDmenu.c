#include "LCDmenu.h"

/*File name: LCDmenu.c;
* Author: Daniil;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: -
* Description: Menu middleware for LCD.
* Last update: 07.04.2023.
*/

void Menu_Display(MenuStructureTypedef *MenuStruct, LCDInitStructTypedef LCDStruct){
	LCD_WriteCommand(LCDStruct, LCD_CLEAN);
	LCD_WriteText(LCDStruct, *(MenuStruct->MenuList+MenuStruct->LinesToDisplay[0]));
	LCD_WriteCommand(LCDStruct, LCD_HOME2);
	LCD_WriteText(LCDStruct, *(MenuStruct->MenuList+MenuStruct->LinesToDisplay[1]));
	if (MenuStruct->CurrentLinePos)
		LCD_WriteCommand(LCDStruct, LCD_HOME2);
	else
		LCD_WriteCommand(LCDStruct, LCD_HOME);
}

void Menu_MoveUP(MenuStructureTypedef *MenuStruct){
	if (MenuStruct->CurrentLinePos && (MenuStruct->LinesToDisplay[0] >= 0))
		MenuStruct->CurrentLinePos=false;
	else if (MenuStruct->LinesToDisplay[0]>0) {
		MenuStruct->LinesToDisplay[0]--;
		MenuStruct->LinesToDisplay[1]--;
	}
	else
		return;
}

void Menu_MoveDOWN(MenuStructureTypedef *MenuStruct){
	if (!MenuStruct->CurrentLinePos && (MenuStruct->LinesToDisplay[1] <= MenuStruct->AmountOfMenuLines - 1))
		MenuStruct->CurrentLinePos=true;
	else if (MenuStruct->LinesToDisplay[1]<MenuStruct->AmountOfMenuLines-1){
		MenuStruct->LinesToDisplay[0]++;
		MenuStruct->LinesToDisplay[1]++;
	}
	else
		return;
}

#ifdef TWO_OR_MORE_MENULINES
void Menu_Move(MenuStructureTypedef *MenuStruct, uint8_t Position){
	if (Position == 0){
		MenuStruct->LinesToDisplay[0] = 0;
		MenuStruct->LinesToDisplay[1] = 1;
		MenuStruct->CurrentLinePos = 0;
	}
	else {
		MenuStruct->LinesToDisplay[0] = Position - 1;
		MenuStruct->LinesToDisplay[1] = Position;
		MenuStruct->CurrentLinePos = 1;
	}
}
#endif //#ifdef TWO_OR_MORE_MENULINES

uint8_t Menu_ReturnPos(MenuStructureTypedef MenuStruct){
	return MenuStruct.LinesToDisplay[MenuStruct.CurrentLinePos];
}

