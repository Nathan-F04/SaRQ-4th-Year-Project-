/*
 * userApp.c
 *
 *  Created on: Dec 8, 2023
 *      Author: Niall.OKeeffe@atu.ie
 */

#include "userApp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
//--------------------------------------------------------------
//used for real time stats, do not delete code from this section
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim2;
extern volatile unsigned long ulHighFrequencyTimerTicks;
void configureTimerForRunTimeStats(void) {
	ulHighFrequencyTimerTicks = 0;
	HAL_TIM_Base_Start_IT(&htim7);
}

unsigned long getRunTimeCounterValue(void) {
	return ulHighFrequencyTimerTicks;
}
//end of real time stats code
//----------------------------------------------------------------

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
// _write function used for printf
int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}

uint16_t servo(uint16_t anglePassed);

void userApp() {
	printf("Starting application\r\n\n");
	char rx_buffer[1]; //number + carraige return and newline
	uint8_t msg = 0;

	//pwm starts here and ccrms etc
	uint16_t anglePassed = 0;
	uint32_t CCR_Return = 0;
	TIM2->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	while (1) {
		HAL_UART_Receive(&huart3, (uint8_t*)rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
		printf("Received: %s\r\n", rx_buffer);
		//Read code from atoi
		msg = atoi(rx_buffer);
		//Reset before looping again
		memset(rx_buffer, 0, sizeof(rx_buffer));
		//Case statement to determine movement
		switch(msg){
		//Forward - prerequiste vals and writes
		case 1:
			//printf("1 received, going forward\r\n");
			//Get CCR so that each task can assaign based on its own CCR and timer
			//Set angle passed for each based on ik
			anglePassed = 100;
			CCR_Return = servo(anglePassed);
			TIM2->CCR1 = CCR_Return;
			HAL_Delay(500);
			//repeat for all servos
			break;
		//Reverse
		case 2:
			printf("2 received, going in reverse\r\n");
			break;
		//Left
		case 3:
			printf("3 received, turning left\r\n");
			break;
		//Right
		case 4:
			printf("4 received, turning right\r\n");
			break;
		default:
			printf("Invalid number received\r\n");
			break;
		}
		//reset msg
		msg = 0;
	}
}

uint16_t servo(uint16_t anglePassed) {
	//Servo function calculated CCR value and returns it, angle is 220 as it is the servos max.
	uint16_t Min_ARR = 139, Max_ARR = 1044, AngleRange = 220;
	return ((Max_ARR - Min_ARR) * anglePassed) / AngleRange + Min_ARR;
}
