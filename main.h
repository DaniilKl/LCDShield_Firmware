#ifndef _MAIN_
#define _MAIN_

/* File name: main.h;
 * Author: Daniil;
 * Platform: Windows 10 x64;
 * Compiler: armclang;
 * Language: C;
 * Hardware used in testing: STM32 NUCLEO-F401RE, LCD Keypad Shield for ARDUINO, LCD1602 with HCD44780UA00 chip.
 * Warnings: -
 * Description: -
 * */

#include <stm32f4xx_hal.h>
#include "lcd.h"
#include "stm32f4xx_it.h"

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