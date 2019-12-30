/*
 * uart.h
 *
 *  Created on: Oct 30, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include <stdbool.h>
#include <stdint.h>

bool uart_send_data(char tx_buffer[], uint16_t tx_timeout, bool aCRLF);
bool uart_send_char(char tx_char, uint16_t tx_timeout);
char* uart_receive_data(uint16_t rx_timeout);
char uart_receive_char(uint16_t rx_timeout);
void uart_init(uint16_t UART_CONFIG);

#endif /* DRIVERS_UART_H_ */
