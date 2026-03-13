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

#define CCReg1 1
#define CCReg2 2
#define CCReg3 3
#define CCReg4 4

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

	//Messages for the RPI
	char tx_buffer[] = "Command executed\r\n";
	char tx_err_buffer[] = "Invalid number sent\r\n";
	char tx_start_buffer[] = "Starting the RPI\r\n";

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

	//Starting the RPI
	HAL_UART_Transmit(&huart3, (uint8_t*)tx_start_buffer, strlen(tx_start_buffer), HAL_MAX_DELAY);

	while (1) {
		HAL_UART_Receive(&huart3, (uint8_t*)rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
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
//			//Get CCR so that each task can assaign based on its own CCR and timer
//			//Set angle passed for each based on ik
//
//			//Rear right Leg
//			//Elbow TIM16 CH1 rear right
			servo(61, CCReg1, &htim16);
			printf("Servo TIM16 CH1 angle is 0\r\n\n");
//
//			//Shoulder TIM4 CH4 rear right
//			servo(90, CCReg4, &htim4);
//			printf("Servo angle TIM4 CH4 is 0\r\n\n");
//
//			//Rear right Hip TIM2 CH1
//			servo(90, CCReg1, &htim2);
//			printf("Servo angle TIM2 CH1 is 0\r\n\n");
//
//			//Rear left leg
//			//Elbow rear left TIM3 CH1 rear left
//			servo(61, CCReg1, &htim3);
//			printf("Servo angle TIM3 CH1 is 0\r\n\n");
//
//			//Shoulder TIM3 CH2 rear left
//			servo(90, CCReg2, &htim3);
//			printf("Servo angle TIM3 CH2 is 0\r\n\n");
//
//			//Hip TIM2 CH3 rear left
//			servo(90, CCReg3, &htim2);
//			printf("Servo angle TIM2 CH3 is 0\r\n\n");
//
//
//			//Front right leg
//			//Elbow TIM3 CH4 front right
//			servo(61, CCReg4, &htim3);
//			printf("Servo angle TIM3 CH4 is 0\r\n\n");
//
//			//Shoulder TIM5 CH4 front right
//			servo(90, CCReg4, &htim5);
//			printf("Servo angle TIM5 CH4 is 0\r\n\n");
//
//			//Hip TIM3 CH3 front right
//			servo(90, CCReg3, &htim3);
//			printf("Servo angle TIM3 CH3 is 0\r\n\n");
//
//			//Front left
//
//			//Elbow TIM5 CH1 front left
//			printf("Setting servo angle TIM5 CH1 is 0\r\n\n");
//			servo(61, CCReg1, &htim5);
//
//			//shoulder TIM4 CH3 front left
//			printf("Setting servo angle TIM4 CH3 is 0\r\n\n");
//			servo(90, CCReg3, &htim4);
//
//
//			//Hip TIM2 CH2 front left
//			printf("Setting servo angle TIM2 CH2 is 0\r\n\n");
//			servo(90, CCReg2, &htim2);
//			servo(100, CCReg2, &htim2);
//
//			//Send a command complete message to the RPI
//			printf("Sending command complete to the RPI\r\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
			//Reverse
		case 2:
			printf("2 received, going in reverse\r\n");

			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
			//Left
		case 3:
			printf("3 received, turning left\r\n");


			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
			//Right
		case 4:
			printf("4 received, turning right\r\n");

			//Send a command complete message to the RPI
			printf("Sending command complete to the RPI\r\n");
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_buffer, strlen(tx_buffer), HAL_MAX_DELAY);
			break;
		default:
			printf("Invalid number received, sending a message to RPI\r\n");

			//Send a command incomplete message to the RPI
			HAL_UART_Transmit(&huart3, (uint8_t*)tx_err_buffer, strlen(tx_err_buffer), HAL_MAX_DELAY);
			break;
		}
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
		TIM16->CCR1 = CCR_Return;
		HAL_Delay(2000);
	}
}
