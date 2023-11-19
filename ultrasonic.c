/*
 * ultrasonic.c
 *
 *  Created on: Oct 15, 2023
 *      Author: hp
 */
#include "gpio.h"
#include "icu.h"
#include "std_types.h"
#include "common_macros.h"
#include "ultrasonic.h"
#include <util/delay.h>
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint32 distance = 0;
void Ultrasonic_init(void) {
	ICU_ConfigType ultrasonic_settings = { PRESCALER, RAISING };
	ICU_init(&ultrasonic_settings);
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(TRIGGER_PORT, TRIGGER_PIN, PIN_OUTPUT);
}
void Ultrasonic_Trigger(void) {
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_HIGH);
	_delay_us(15);
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_LOW);
}
uint16 Ultrasonic_readDistance(void) {
	Ultrasonic_Trigger();
	while (g_timeHigh == 0)
		;
	/*get the distance by multiplying the timer value by the constant given in the PDF with adding 2 to cancel the sensor error*/
	distance = CONST * g_timeHigh + 2 ;
	return distance;
}
void Ultrasonic_edgeProcessing(void) {
	g_edgeCount++;
	if (g_edgeCount == 1) {
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		ICU_clearTimerValue();
		/* Detect falling edge */
		ICU_setEdgeDetectionType(FALLING);
	} else if (g_edgeCount == 2) {
		/* Store the High time value */
		g_timeHigh = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RAISING);
		g_edgeCount = 0;
	}
}
