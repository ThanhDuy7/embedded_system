/*
 * timer.c
 *
 *  Created on: Nov 29, 2024
 *      Author: Thanh Duy
 */
#include "timer.h"
#include "lcd.h"

enum timeElements currentEle = SEC;
enum timeMode currentMode = MODIFY;
uint8_t changeMode = 0;
uint8_t alarm = 0;
uint8_t count_LED_debug = 0;
uint16_t timeElements[]= {23,8,23,5,5,15,43}; //year,month,date,day,hour,min,sec
uint16_t scheduleElements[]= {0,0,0,0,0,0,0}; //year,month,date,day,hour,min,sec

void storeTime() {
	timeElements[0] = ds3231_year;
	timeElements[1] = ds3231_month;
	timeElements[2] = ds3231_date;
	timeElements[3] = ds3231_day;
	timeElements[4] = ds3231_hours;
	timeElements[5] = ds3231_min;
	timeElements[6] = ds3231_sec;
}
uint8_t checkAlarm() {
	if (scheduleElements[6] == ds3231_sec && scheduleElements[5] == ds3231_min
		&& scheduleElements[4] == ds3231_hours && scheduleElements[3] == ds3231_day
		&& scheduleElements[2] == ds3231_date && scheduleElements[1] == ds3231_month
		&& scheduleElements[0]== ds3231_year){
		lcd_DrawRectangle(0, 0, 240, 30, RED);
		return 1;
	}
	return 0;
}
void timeEdit(enum timeElements currentElement) {
	currentEle = currentElement;
	count_LED_debug = (count_LED_debug + 1)%10;
	if (count_LED_debug == 0) {

		if (currentMode != NORMAL) {
			changeMode = !changeMode;
		}
		lcd_ShowIntNum(70, 100, timeElements[4], 2, (currentEle == HOUR) ? (changeMode ? GREEN : BLACK) : GREEN, BLACK, 24);
		lcd_ShowIntNum(110, 100, timeElements[5], 2, (currentEle == MIN) ? (changeMode ? GREEN : BLACK) : GREEN, BLACK, 24);
		lcd_ShowIntNum(150, 100, timeElements[6], 2, (currentEle == SEC) ? (changeMode ? GREEN : BLACK) : GREEN, BLACK, 24);
		lcd_ShowIntNum(20, 130, timeElements[3], 2, (currentEle == DAY) ? (changeMode ? YELLOW : BLACK) : YELLOW, BLACK, 24);
		lcd_ShowIntNum(70, 130, timeElements[2], 2, (currentEle == DATE) ? (changeMode ? YELLOW : BLACK) : YELLOW, BLACK, 24);
		lcd_ShowIntNum(110, 130, timeElements[1], 2, (currentEle == MONTH) ? (changeMode ? YELLOW : BLACK) : YELLOW, BLACK, 24);
		lcd_ShowIntNum(150, 130, timeElements[0], 2, (currentEle == YEAR) ? (changeMode ? YELLOW : BLACK) : YELLOW, BLACK, 24);

	}
}
void displayTime(){

	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}
