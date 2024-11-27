/*
 * traffic.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Thanh Duy
 */

#ifndef INC_TRAFFIC_H_
#define INC_TRAFFIC_H_

#include "main.h"

extern enum traffic_mode {
	NORMAL = 0,
	RED_CONFIG,
	YELLOW_CONFIG,
	GREEN_CONFIG
};
extern enum traffic_light {
	RED1_GREEN2,
	RED1_YELLOW2,
	GREEN1_RED2,
	YELLOW1_RED2
};
void turnOnTrafficLight(enum traffic_light lightmode);
void displayNumb(uint16_t light,uint16_t counter);
void traffic_init(uint16_t curMode,uint16_t cycle);
void run_traffic();
#endif /* INC_TRAFFIC_H_ */
