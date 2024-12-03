/*
 * timer.h
 *
 *  Created on: Nov 29, 2024
 *      Author: Thanh Duy
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"
#include "ds3231.h"
#include "button.h"

extern uint16_t timeElements[7];
extern uint16_t scheduleElements[7];
extern enum timeElements {
	SEC,
	MIN,
	HOUR,
	DAY,
	DATE,
	MONTH,
	YEAR
};
extern enum timeMode {
	NORMAL,
	MODIFY,
	SCHEDULE
};
uint8_t checkAlarm();
void storeTime();
void timeEdit(enum timeElements);
void displayTime();
void change_time();
void button_press();

#endif /* INC_TIMER_H_ */
