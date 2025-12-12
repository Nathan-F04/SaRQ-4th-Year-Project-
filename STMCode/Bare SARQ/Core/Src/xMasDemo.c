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
#include "stm32l4xx_hal.h"
//--------------------------------------------------------------
//used for real time stats, do not delete code from this section
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;

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

//uint16_t servo(uint16_t anglePassed);
void servo(uint16_t anglePassed, uint8_t num, TIM_HandleTypeDef *htim);
//uint16_t servo(uint16_t anglePassed);

void userApp() {
	printf("Starting application\r\n\n");
	char rx_buffer[1]; //number + carraige return and newline
	uint8_t msg = 0;

	//pwm starts here and ccrms etc
	//TIM 2
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

	//TIM 3
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	//TIM 4
	TIM4->CCR3 = 0;
	TIM4->CCR4 = 0;
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	//TIM 5
	TIM5->CCR1 = 0;
	TIM5->CCR4 = 0;
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

	//TIM 16
	TIM16->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	TIM2->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	while (1) {
		HAL_UART_Receive(&huart3, (uint8_t*)rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
		msg = 1;
		printf("Received: %s\r\n", rx_buffer);
		//Read code from atoi
		msg = atoi(rx_buffer);
		//Reset before looping again
		memset(rx_buffer, 0, sizeof(rx_buffer));
		//Case statement to determine movement
		switch (msg) {
		//Forward - prerequiste vals and writes
		case 1:
			printf("1 received, going forward\r\n");
			//Get CCR so that each task can assaign based on its own CCR and timer
			//Set angle passed for each based on ik

			//Servo 1 (servos named based on pinout order for now) TIM16 CH1
			servo(0, 1, &htim16);
			printf("Servo TIM16 CH1 angle is 0\r\n\n");
			servo(180, 1, &htim16);
			printf("Servo angle TIM16 CH1 is 180\r\n\n");

			//Servo 2 TIM4 CH4
			servo(0, 4, &htim4);
			printf("Servo angle TIM4 CH4 is 0\r\n\n");
			servo(180, 4, &htim4);
			printf("Servo angle TIM4 CH4 is 180\r\n\n");

			//Servo 3 TIM2 CH1
			servo(0, 1, &htim2);
			printf("Servo angle TIM2 CH1 is 0\r\n\n");
			servo(180, 1, &htim2);
			printf("Servo angle TIM2 CH1 is 180\r\n\n");

			//Servo 4 TIM3 CH1
			servo(0, 1, &htim3);
			printf("Servo angle TIM3 CH1 is 0\r\n\n");
			servo(180, 1, &htim3);
			printf("Servo angle TIM3 CH1 is 180\r\n\n");

			//Servo 5 TIM3 CH2
			servo(0, 2, &htim3);
			printf("Servo angle TIM3 CH2 is 0\r\n\n");
			servo(180, 2, &htim3);
			printf("Servo angle TIM3 CH2 is 180\r\n\n");

			//Servo 6 TIM2 CH3
			servo(0, 3, &htim2);
			printf("Servo angle TIM2 CH3 is 0\r\n\n");
			servo(180, 3, &htim2);
			printf("Servo angle TIM2 CH3 is 180\r\n\n");

			//Servo 7 TIM3 CH4
			servo(0, 4, &htim3);
			printf("Servo angle TIM3 CH4 is 0\r\n\n");
			servo(180, 4, &htim3);
			printf("Servo angle TIM3 CH4 is 180\r\n\n");

			//Servo 8 TIM5 CH4
			servo(0, 4, &htim5);
			printf("Servo angle TIM5 CH4 is 0\r\n\n");
			servo(180, 4, &htim5);
			printf("Servo angle TIM5 CH4 is 180\r\n\n");

			//Servo 9 TIM3 CH3
			servo(0, 3, &htim3);
			printf("Servo angle TIM3 CH3 is 0\r\n\n");
			servo(180, 3, &htim3);
			printf("Servo angle TIM3 CH3 is 180\r\n\n");

			//Servo 10 TIM4 CH3
			servo(0, 3, &htim4);
			printf("Servo angle TIM4 CH3 is 0\r\n\n");
			servo(180, 3, &htim4);
			printf("Servo angle TIM4 CH3 is 180\r\n\n");

			//Servo 11 TIM5 CH1
			servo(0, 1, &htim5);
			printf("Servo angle TIM5 CH1 is 0\r\n\n");
			servo(180, 1, &htim5);
			printf("Servo angle TIM5 CH1 is 180\r\n\n");


			//Servo 12 TIM2 CH2
			servo(0, 2, &htim2);
			printf("Servo angle TIM2 CH2 is 0\r\n\n");
			servo(180, 2, &htim2);
			printf("Servo angle TIM2 CH2 is 180\r\n\n");
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
		//msg = 0;
	}
}

void servo(uint16_t anglePassed, uint8_t channel, TIM_HandleTypeDef *htim) {
	//Servo function calculated CCR value and returns it, angle is 210 as it is the servos max.
	uint32_t CCR_Return = 0;
	uint16_t Min_ARR = 1120, Max_ARR = 8495, AngleRange = 210;
	CCR_Return = ((Max_ARR - Min_ARR) * anglePassed) / AngleRange + Min_ARR;
	if (htim->Instance == TIM2) {
		switch (channel) {
		case 1:
			TIM2->CCR1 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 2:
			TIM2->CCR2 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 3:
			TIM2->CCR3 = CCR_Return;
			HAL_Delay(2000);
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM3) {
		switch (channel) {
		case 1:
			TIM3->CCR1 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 2:
			TIM3->CCR2 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 3:
			TIM3->CCR3 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 4:
			TIM3->CCR4 = CCR_Return;
			HAL_Delay(2000);
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM4) {
		switch (channel) {
		case 3:
			TIM4->CCR3 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 4:
			TIM4->CCR4 = CCR_Return;
			HAL_Delay(2000);
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM5) {
		switch (channel) {
		case 1:
			TIM5->CCR1 = CCR_Return;
			HAL_Delay(2000);
			break;
		case 4:
			TIM5->CCR4 = CCR_Return;
			HAL_Delay(2000);
			break;
		default:
			break;
		}
	}
	if (htim->Instance == TIM16) {
		TIM15->CCR1 = CCR_Return;
		HAL_Delay(2000);
	}
}
