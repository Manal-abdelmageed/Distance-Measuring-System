/*
 * Mini_Project4.c
 *
 *  Created on: Oct 19, 2023
 *      Author: hp
 */
#include "icu.h"
#include "lcd.h"
#include "ultrasonic.h"
#include "std_types.h"
#include "gpio.h"
#include <avr/io.h>

int main() {
	/*initiate the needed drivers*/
	LCD_init();
	Ultrasonic_init();
	/*enable interrupts*/
	SREG |= (1 << 7);
	while (1) {
		/*display the needed formula on the LCD specifying the cursor location to prevent overwriting */
		LCD_moveCursor(0, 0);
		LCD_displayString("Distance = ");
		LCD_moveCursor(0, 14);
		LCD_displayString("cm");
		/*get the distance from the red distance function then display it*/
		uint16 distance = Ultrasonic_readDistance();
		LCD_moveCursor(0, 11);
		LCD_intgerToString(distance);
	}
	return 0;
}
