#ifndef _MAIN_
#define _MAIN_

/* File name: main.h;
 * Author: Daniil;
 * Compiler: armclang;
 * Language: C;
 * Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
 * Warnings: -
 * Description: main.h and main.c files consist two examples: an example of using LCD firmware and an example of using menu middleware for LCD.
 *							The LCD firmware example is compiled automatically. If you want to compile menu example - define MENU_MIDDLEWARE_EXAMPLE.
 * Last update: 07.04.2023.
 * */

//#define MENU_MIDDLEWARE_EXAMPLE

#include <stm32f4xx_hal.h>
#include "lcd.h"
#include "stm32f4xx_it.h"

#ifndef MENU_MIDDLEWARE_EXAMPLE
	#define LCD_FIRMWARE_EXAMPLE
#else
	#include <stdlib.h>
	#include <LCDmenu.h>
#endif //#ifndef MENU_MIDDLEWARE_EXAMPLE


//Data LCD pins definitions:
#define LCD_D4Port GPIOB
#define LCD_D5Port GPIOB
#define LCD_D6Port GPIOB
#define LCD_D7Port GPIOA
#define LCD_D4Pin GPIO_PIN_5
#define LCD_D5Pin GPIO_PIN_4
#define LCD_D6Pin GPIO_PIN_10
#define LCD_D7Pin GPIO_PIN_8

//Control LCD pins definitions:
#define LCD_EPort GPIOC
#define LCD_RSPort GPIOA
#define LCD_EPin GPIO_PIN_7
#define LCD_RSPin GPIO_PIN_9

//Led pin definition:
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA

//Buttons LCD pin definition:
#define LCD_BUTTONSPort GPIOA
#define LCD_BUTTONSPin GPIO_PIN_0


int main(void);

#endif