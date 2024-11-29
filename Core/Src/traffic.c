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
uint16_t redCycle = 7;
uint16_t greenCycle = 5;
uint16_t yellowCycle = 2;
uint16_t counter = 5;
uint16_t light = 7;
uint8_t count_LED_debug = 0;
uint8_t ledState = 0;
void traffic_init (uint16_t curMode,uint16_t cycle) {
	if (curMode == 0) {
		currentMode = NORMAL;
	} else if (curMode == 1) {
		currentMode = RED_CONFIG;
		redCycle = cycle;
		counter = redCycle - yellowCycle; //green cycle
		greenCycle = counter;
		light = redCycle;
		currentState = RED1_GREEN2;
		nextState = currentState;
		//lcd_ShowIntNum(160, 160, defaultLight, 2, WHITE, BLUE, 32);
	} else if (curMode == 2) {
		currentMode = GREEN_CONFIG;
		greenCycle = cycle;
		redCycle = greenCycle + yellowCycle;
		light = redCycle;
		counter = greenCycle;
		currentState = RED1_GREEN2;
		nextState = currentState;
		//lcd_ShowIntNum(160, 160, defaultLight, 2, WHITE, BLUE, 32);
	} else if (curMode == 3) {
		currentMode = YELLOW_CONFIG;
		yellowCycle = cycle;
		redCycle = greenCycle + yellowCycle;
		light = redCycle;
		counter = greenCycle;
		currentState = RED1_GREEN2;
		nextState = currentState;
		//lcd_ShowIntNum(160, 160, defaultLight, 2, WHITE, BLUE, 32);
	}

}
void run_traffic() {

	count_LED_debug = (count_LED_debug + 1)%20;
	if (count_LED_debug%10 == 0) {
		turnOnTrafficLight(currentState);
		displayNumb();
	}
	if(count_LED_debug == 0) {
		switch(currentState) {
			case RED1_GREEN2:
				//turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = RED1_YELLOW2;
					counter = light;
				}
				break;
			case RED1_YELLOW2:
				//turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = GREEN1_RED2;
					counter = redCycle;
					light = greenCycle;
				}
				break;
			case GREEN1_RED2:
				//turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (light <= 0) {
					nextState = YELLOW1_RED2;
					//ncounter = defaultLight - defaultCounter;
					light = counter;
				}
				break;
			case YELLOW1_RED2:
				//turnOnTrafficLight(currentState);
				counter--;
				light--;
				if (counter <= 0) {
					nextState = RED1_GREEN2;
					counter = greenCycle;
					light = redCycle;
				}
				break;
		}
		currentState = nextState;
	}

}

void turnOnTrafficLight(enum traffic_light lightmode) {

	if (currentMode != NORMAL) {
			ledState = !ledState; // Toggle LED state
	}

	// light coordinate
	const uint16_t RED1_coordinate[2] = {40,250};
	const uint16_t RED2_coordinate[2] = {150,280};
	const uint16_t GREEN1_coordinate[2] = {80,250};
	const uint16_t GREEN2_coordinate[2] = {150,250};
	const uint16_t YELLOW1_coordinate[2] = {120,250};
	const uint16_t YELLOW2_coordinate[2] = {150,220};

	switch(lightmode) {
		case RED1_GREEN2:
			lcd_DrawCircle(RED1_coordinate[0],RED1_coordinate[1], (currentMode==RED_CONFIG) ? (ledState ? RED :WHITE ): RED,10,1);
			lcd_DrawCircle(GREEN1_coordinate[0],GREEN1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(YELLOW1_coordinate[0],YELLOW1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(RED2_coordinate[0],RED2_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(GREEN2_coordinate[0],GREEN2_coordinate[1],(currentMode==GREEN_CONFIG) ? (ledState ? GREEN :WHITE ): GREEN,10,1);
			lcd_DrawCircle(YELLOW2_coordinate[0],YELLOW2_coordinate[1],WHITE,10,1);
			break;
		case RED1_YELLOW2:
			lcd_DrawCircle(RED1_coordinate[0],RED1_coordinate[1],(currentMode==RED_CONFIG) ? (ledState ? RED :WHITE ): RED,10,1);
			lcd_DrawCircle(GREEN1_coordinate[0],GREEN1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(YELLOW1_coordinate[0],YELLOW1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(RED2_coordinate[0],RED2_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(GREEN2_coordinate[0],GREEN2_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(YELLOW2_coordinate[0],YELLOW2_coordinate[1],(currentMode==YELLOW_CONFIG) ? (ledState ? YELLOW :WHITE ): YELLOW,10,1);
			break;
		case GREEN1_RED2:
			lcd_DrawCircle(RED1_coordinate[0],RED1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(GREEN1_coordinate[0],GREEN1_coordinate[1],(currentMode==GREEN_CONFIG) ? (ledState ? GREEN :WHITE ): GREEN,10,1);
			lcd_DrawCircle(YELLOW1_coordinate[0],YELLOW1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(RED2_coordinate[0],RED2_coordinate[1],(currentMode==RED_CONFIG) ? (ledState ? RED :WHITE ): RED,10,1);
			lcd_DrawCircle(GREEN2_coordinate[0],GREEN2_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(YELLOW2_coordinate[0],YELLOW2_coordinate[1],WHITE,10,1);
			break;
		case YELLOW1_RED2:
			lcd_DrawCircle(RED1_coordinate[0],RED1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(GREEN1_coordinate[0],GREEN1_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(YELLOW1_coordinate[0],YELLOW1_coordinate[1],(currentMode==YELLOW_CONFIG) ? (ledState ? YELLOW :WHITE ): YELLOW,10,1);
			lcd_DrawCircle(RED2_coordinate[0],RED2_coordinate[1],(currentMode==RED_CONFIG) ? (ledState ? RED :WHITE ): RED,10,1);
			lcd_DrawCircle(GREEN2_coordinate[0],GREEN2_coordinate[1],WHITE,10,1);
			lcd_DrawCircle(YELLOW2_coordinate[0],YELLOW2_coordinate[1],WHITE,10,1);
			break;
	}
}
void displayNumb() {
	lcd_ShowStr(30, 80, "Red Cycle", WHITE, RED, 24, 0);
	lcd_ShowIntNum(150, 80, redCycle, 2, WHITE, RED, 24);

	lcd_ShowStr(30, 110, "Green Cycle", WHITE, GREEN, 24, 0);
	lcd_ShowIntNum(150, 110, greenCycle, 2, WHITE, GREEN, 24);

	lcd_ShowStr(30, 140, "Yellow Cycle", BLACK, YELLOW, 24, 0);
	lcd_ShowIntNum(150, 140, yellowCycle, 2, BLACK, YELLOW, 24);

	lcd_ShowIntNum(40, 280, light, 2,WHITE,BLUE, 32);
	lcd_ShowIntNum(180, 250, counter, 2, WHITE, BLUE, 32);
}
