/*
 * button_press.c
 *
 *  Created on: Dec 3, 2024
 *      Author: Thanh Duy
 */
#include "button_press.h"

uint16_t curMode = 0;
char *modes[] = { "NORMAL", "RED_CONFIG", "GREEN_CONFIG", "YELLOW_CONFIG" };
uint16_t cycle = 10;
uint16_t newCycle = 10;
uint8_t confirmChangeCycle = 0;

void button_press(){
	if (button_count[0]%20 == 1 ) {
		curMode = (curMode + 1) % 4;
			  confirmChangeCycle = 1;
		  }
		  if (curMode != 0) {
			  if (confirmChangeCycle) {
				  switch(curMode){
				  case 1:
					  newCycle = redCycle;
					  break;
				  case 2:
					  newCycle = greenCycle;
					  break;
				  case 3:
					  newCycle = yellowCycle;
					  break;
				  default:
					  newCycle = redCycle;
					  break;
				  }
			  }

			  if (button_count[1]%20 == 1) {
				  if (newCycle >= 99)
					  newCycle = 1;
				  else newCycle++;
				  confirmChangeCycle = 0;

			  }
			  if (button_count[3]%20 == 1) {
				  if (newCycle <= 0)
					  newCycle = 99;
				  else newCycle--;
				  confirmChangeCycle = 0;
			  }

			  if (button_count[2]%20 == 1){
				  cycle = newCycle;
				  traffic_init(curMode,cycle);
				  confirmChangeCycle = 1;
			  }


		  }
		  lcd_ShowStr(30, 50, modes[curMode], WHITE, RED, 24,0);
		  	  lcd_ShowStr(30, 170, "modified", WHITE, BLUE, 24, 0);
		  	  lcd_ShowIntNum(150, 170, newCycle, 2, WHITE, BLUE, 32);
}
