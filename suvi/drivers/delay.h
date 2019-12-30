/*
 * delay.h
 *
 *  Created on: Oct 30, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_DELAY_H_
#define DRIVERS_DELAY_H_

#include <stdint.h>

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void delay_init(uint16_t fclk);

#endif /* DRIVERS_DELAY_H_ */
