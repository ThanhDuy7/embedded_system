/*
 * traffic.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Thanh Duy
 */


#include "traffic.h"
#include "lcd.h"
enum traffic_mode currentMode = NORMAL;
enum traffic_light currentState = RED1_GREEN2;
enum traffic_light nextState = RED1_GREEN2;
uint16_t defaultCounter = 5;
uint16_t defaultLight = 7;
uint16_t redCycle = 7;
uint16_t greenCycle = 5;
uint16_t yellowCycle = 2;
uint16_t counter = 5;
uint16_t light = 7;
uint8_t count_LED_debug = 0;
void traffic_init (uint16_t curMode,uint16_t cycle) {
	if (curMode == 0) {
		defaultCounter = 5;
		defaultLight = 7;
		redCycle = 7;
		greenCycle = 5;
		counter = 5;
		light = 7;
	}
	if (curMode == 1) {
		currentMode = RED_CONFIG;
		redCycle = cycle;
		defaultLight = redCycle;
		defaultCounter = defaultLight - yellowCycle;
		light = defaultLight;
		counter = defaultCounter;
		lcd_ShowIntNum(160, 160, defaultLight, 2, WHITE, BLUE, 32);
	} else if (curMode == 2) {
		currentMode = GREEN_CONFIG;
		greenCycle = cycle;
		defaultLight = greenCycle + yellowCycle;
		defaultCounter = greenCycle;
		light = defaultLight;
		counter = defaultCounter;
		lcd_ShowIntNum(160, 160, defaultLight, 2, WHITE, BLUE, 32);
	} else if (curMode == 3) {
		currentMode = YELLOW_CONFIG;
		yellowCycle = cycle;
		defaultLight = greenCycle + yellowCycle;
		defaultCounter = greenCycle;
		light = defaultLight;
		counter = defaultCounter;
		lcd_ShowIntNum(160, 160, defaultLight, 2, WHITE, BLUE, 32);
	}

}
void run_traffic() {

	count_LED_debug = (count_LED_debug + 1)%20;

	if(count_LED_debug == 0) {
		//displayNumb(light);
		switch(currentState) {
			case RED1_GREEN2:
				turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = RED1_YELLOW2;
					counter = light;
				}
				break;
			case RED1_YELLOW2:
				turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = GREEN1_RED2;
					counter = defaultCounter;
					light = defaultCounter;
				}
				break;
			case GREEN1_RED2:
				turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = YELLOW1_RED2;
					counter = defaultLight - defaultCounter;
					light = counter;
				}
				break;
			case YELLOW1_RED2:
				turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = RED1_GREEN2;
					counter = defaultCounter;
					light = defaultLight;
				}
				break;
		}
		currentState = nextState;
	}

}

void turnOnTrafficLight(enum traffic_light lightmode) {
	lcd_Clear(WHITE);
	displayNumb(light,counter);
	switch(lightmode) {
		case RED1_GREEN2:
			lcd_DrawCircle(40,250,RED,10,1);
			lcd_DrawCircle(150,120,GREEN,10,1);
			break;
		case RED1_YELLOW2:
			lcd_DrawCircle(40,250,RED,10,1);
			lcd_DrawCircle(150,160,YELLOW,10,1);
			break;
		case GREEN1_RED2:
			lcd_DrawCircle(80,250,GREEN,10,1);
			lcd_DrawCircle(150,200,RED,10,1);
			break;
		case YELLOW1_RED2:
			lcd_DrawCircle(120,250,YELLOW,10,1);
			lcd_DrawCircle(150,200,RED,10,1);
			break;
	}
}
void displayNumb(uint16_t light, uint16_t counter) {
	lcd_ShowIntNum(40, 280, light, 2, WHITE, BLUE, 32);
	lcd_ShowIntNum(180, 160, counter, 2, WHITE, BLUE, 32);
}
