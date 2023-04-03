#ifndef _LCDDEFINES_H_
#define _LCDDEFINES_H_
 
/*File name: lcddefines.h;
* Author: Daniil;
* Platform: Windows 10 x64;
* Compiler: armclang;
* Language: C;
* Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
* Warnings: -
* Description: Header file with commands definitions. This file consists of 5 sections which contain deffirent commands for LCD display. 
*  						 More detailed description is placed above every section. First 4 sections have their "main definitions" which must be merged 
*							 eith options from the same section. For further details about definitions in this file read the documentation for HD44780U 
* 						 controller.
* Last uadte: 27.03.2023.
*/


//Initialization section:
//The main definition:
#define SET_DISP_START_SETT 0x20
//Sets size of the display interface (amount of used data pins):
#define INT_DATA_LENGTH_8B 0x10 //8-pin interface;
#define INT_DATA_LENGTH_4B 0x00 //4-pin interface;
//Amount of lines to show:
#define TWO_LINES 0x08 //Two line option;
#define ONE_LINE 0x00	//One line option;
//Height of the font:
#define FONT_5X10 0x04 //10 pixels;
#define FONT_5X8 0x00 //8 pixels.
/*Example: LCD_WriteCommand(SET_DISP_START_SETT | INT_DATA_LENGTH_4B | TWO_LINES | FONT_5X8) //Gives you: 4-bit interface, two lines to 
					 show and 8-pixel height.
*/


/* Second section:
* Sets cursor move direction and specifies display shift. These operations are performed during data write and read.
*/
//The main definition:
#define SET_CUR_MOVE_DIR 0x04
//Cursor move directions:
#define INC_CURSOR 0x02 //Forward;
#define DIC_CURSOR 0x00 //Backward;
//Display shift:
#define DISP_SHIFT 0x01 //Display shift enabled;
#define NO_DISP_SHIFT 0x00 //Display shift diabled;
/*Example: LCD_WriteCommand(SET_CUR_MOVE_DIR | INC_CURSOR | DISP_SHIFT) //Gives you: cursor increments while tiping with display shift enabled.
*/


/* Third section:
* Sets entire display on/off, cursor on/off, and blinking of
* cursor position character.
*/
//The main definition:
#define SET_DISP_SETT 0x08
//Turning display ON/OFF:
#define TURN_DISP_ON 0x04
#define TURN_DISP_OFF 0x00
//Turning cursor ON/OFF:
#define TURN_CURSOR_ON 0x02
#define TURN_CURSOR_OFF 0x00
//Enabling cursor blinking:
#define TURN_CURSOR_BLINK_ON 0x01
#define TURN_CURSOR_BLINK_OFF 0x00
/*Example: LCD_WriteCommand(SET_DISP_SETT | TURN_DISP_ON | TURN_CURSOR_ON | TURN_CURSOR_BLINK_ON) //Turns on the display, turns on cursor, 
					 and enables cursor blinking:*/


/* Fourth section; Cursor/Display shift:
* Moves cursor and shifts display without changing DDRAM contents.
*/
//The main definition:
#define SHIFT 0x10
//Shift display:
#define SHIFT_DISP 0x08
//Shift cursor:
#define SHIFT_CURSOR 0x00
//Shifting direction:
#define SHIFT_RIGHT 0x04 //Right;
#define SHIFT_LEFT 0x00 //Left.
/*Example: LCD_WriteCommand(SHIFT | SHIFT_DISP | SHIFT_RIGHT) //Shifts display right. */


/* Fifth section:
* This LCD commands must not be merged with each other! And this section does not have any "main definitions".
*/
#define LCD_CLEAN 0x01 //Clears entire display and sets DDRAM address 0 in address counter.
#define LCD_HOME 0x02	/*Sets DDRAM address 0 in address counter. Also returns display from being shifted to original position. DDRAM 
												* contents remain unchanged. */
#define LCD_HOME2 0xC0 //Places cursor on the start position on second line.


#endif //#ifndef _LCDDEFINES_H_
