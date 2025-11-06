/*
 * userApp.c
 *
 *  Created on: Oct 29, 2025
 *      Author: ferry
 */
#include "main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart3, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}

void userApp() {
	printf("Starting my app\r\n\n");
	char rx_buffer[50];
	while (1) {
		// Receive data from Raspberry Pi -non blocking
		HAL_UART_Receive(&huart3, (uint8_t*)rx_buffer, sizeof(rx_buffer), HAL_MAX_DELAY);
		printf("Received: %s\n", rx_buffer);
	}
}
