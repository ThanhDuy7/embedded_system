/*
 * timer.c
 *
 *  Created on: Nov 29, 2024
 *      Author: Thanh Duy
 */
#include "timer.h"
#include "lcd.h"

enum timeElements currentEle = SEC;
enum timeElements nextEle = MIN;
enum timeMode currentMode = MODIFY;
uint8_t changeMode = 0;
uint8_t alarm = 0;
uint8_t count_LED_debug = 0;
uint16_t timeElements[]= {23,8,23,5,5,15,43}; //year,month,date,day,hour,min,sec
uint16_t scheduleElements[]= {0,0,0,0,0,0,0}; //year,month,date,day,hour,min,sec
uint8_t curMode = 0;
uint8_t confirmChangeTime = 0;
uint8_t setAlarmFlag = 0;
uint8_t holdCounter = 0;

char *modes[] = { "NORMAL", "MODIFY", "SCHEDULE"};

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
void button_press() {
	if (button_count[0]%20 == 1 ) {
		curMode = (curMode + 1) % 3;
	  confirmChangeTime = 1;
	}
	  if (curMode != 0) {
		  if (confirmChangeTime) {
			  currentEle = SEC;
			  nextEle = currentEle;
			  storeTime();
			  confirmChangeTime = 0;
		  }
			  if (button_count[3]%20 == 1) {
				  holdCounter = 0;
				  switch (currentEle){
				  case SEC:
					  timeElements[6]++;
					  if (timeElements[6] >= 60) {
						  timeElements[6] = 0;
					  }
					  nextEle = MIN;
					  break;
				  case MIN:
					  timeElements[5]++;
					  if (timeElements[5] >= 60) {
						  timeElements[5] = 0;
					  }
					  nextEle = HOUR;
					  break;
				  case HOUR:
					  timeElements[4]++;
					  if (timeElements[4] >= 24) {
						  timeElements[4] = 0;
					  }
					  nextEle = DAY;
					  break;
				  case DAY:
					  timeElements[3]++;
					  if (timeElements[3] >= 8) {
						  timeElements[3] = 1;
					  }
					  nextEle = DATE;
					  break;
				  case DATE:
					  timeElements[2]++;
					  if (timeElements[2] >= 32) {
						  timeElements[2] = 1;
					  }
					  nextEle = MONTH;
					  break;
				  case MONTH:
					  timeElements[1]++;
					  if (timeElements[1] >= 13) {
						  timeElements[1] = 1;
					  }
					  nextEle = YEAR;
					  break;
				  case YEAR:
					  timeElements[0]++;
					  if (timeElements[0] >= 100) {
						  timeElements[0] = 1;
					  }
					  break;
				  }

			  }
			  if (button_count[3] >= 40) { // Long press behavior
				  holdCounter++;
				  if (holdCounter >= 4) { // 4 * 50ms = 200ms
					  holdCounter = 0; // Reset hold counter
					  switch (currentEle) {
						  case SEC:
							  timeElements[6]++;
							  if (timeElements[6] >= 60) timeElements[6] = 0;
							  break;
						  case MIN:
							  timeElements[5]++;
							  if (timeElements[5] >= 60) timeElements[5] = 0;
							  break;
						  case HOUR:
							  timeElements[4]++;
							  if (timeElements[4] >= 24) timeElements[4] = 0;
							  break;
						  case DAY:
							  timeElements[3]++;
							  if (timeElements[3] >= 8) timeElements[3] = 1;
							  break;
						  case DATE:
							  timeElements[2]++;
							  if (timeElements[2] >= 32) timeElements[2] = 1;
							  break;
						  case MONTH:
							  timeElements[1]++;
							  if (timeElements[1] >= 13) timeElements[1] = 1;
							  break;
						  case YEAR:
							  timeElements[0]++;
							  if (timeElements[0] >= 100) timeElements[0] = 1;
							  break;
					  }
				  }
			  }
			  if (button_count[12]%20 == 1) {
				  if (curMode != 2) {
					  updateTime();
				  }
				  if (nextEle == currentEle && currentEle != YEAR) {
					  switch(currentEle) {
					  case SEC:
						  nextEle = MIN;
						  break;
					  case MIN:
						  nextEle = HOUR;
						  break;
					  case HOUR:
						  nextEle = DAY;
						  break;
					  case DAY:
						  nextEle = DATE;
						  break;
					  case DATE:
						  nextEle = MONTH;
						  break;
					  case MONTH:
						  nextEle = YEAR;
						  break;

					  }
				  }

				  if (currentEle == YEAR) {
					  if (curMode == 2) {
						  setAlarmFlag = 1;
						  for (int i = 0; i < 7; i++){
							  scheduleElements[i] = timeElements[i];
						  }
					  }
					  confirmChangeTime = 1;
					  curMode = 0;

				  }
				  currentEle = nextEle;


			  }

			  timeEdit(currentEle);

	  } else displayTime();
	  lcd_ShowStr(30, 50, modes[curMode], WHITE, RED, 24,0);
	  	  if (setAlarmFlag) {
	  		  lcd_ShowStr(30, 185, "alarm at" , WHITE, RED, 24,0);
	  		  lcd_ShowIntNum(130, 185, 2000 + scheduleElements[0], 2, GREEN, BLACK, 24);
	  		  lcd_ShowStr(160, 185, "year" , WHITE, RED, 24,0);
	  		  lcd_ShowIntNum(130, 210, scheduleElements[1], 2, GREEN, BLACK, 24);
	  		  lcd_ShowStr(160, 210, "month" , WHITE, RED, 24,0);
	  		  lcd_ShowIntNum(130, 235, scheduleElements[2], 2, GREEN, BLACK, 24);
	  		  lcd_ShowStr(160, 235, "date" , WHITE, RED, 24,0);
	  		  lcd_ShowStr(160, 260, (scheduleElements[3] == 1) ? "MONDAY"
	  				  	  	  : (scheduleElements[3] == 2) ? "TUESDAY"
	  						  : (scheduleElements[3] == 3) ? "WEDNESDAY"
	  						  :(scheduleElements[3] == 4) ? "THURSDAY"
	  						  : (scheduleElements[3] == 5) ? "FRIDAY"
	  						  :(scheduleElements[3] == 6) ? "SATURDAY"
	  						  : "SUNDAY", WHITE, RED, 24,0);
	  		  lcd_ShowIntNum(20, 260, scheduleElements[4], 2, GREEN, BLACK, 24);
	  		  lcd_ShowStr(50, 260, ":" , WHITE, RED, 24,0);
	  		  lcd_ShowIntNum(65, 260, scheduleElements[5], 2, GREEN, BLACK, 24);
	  		  lcd_ShowStr(100, 260, ":" , WHITE, RED, 24,0);
	  		  lcd_ShowIntNum(110, 260, scheduleElements[6], 2, GREEN, BLACK, 24);
	  		  if (checkAlarm()) setAlarmFlag = 0;
	  	 }
}
