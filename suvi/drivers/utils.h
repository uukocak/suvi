/*
 * utils.h
 *
 *  Created on: Dec 31, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_UTILS_H_
#define DRIVERS_UTILS_H_

#if defined (__MSP430FR2355__)
#define REDLED_O P1OUT
#define REDLED_D P1DIR
#define REDLED_I P1IN
#define REDLED_BIT BIT0
#define GREENLED_O P6OUT
#define GREENLED_D P6DIR
#define GREENLED_I P6IN
#define GREENLED_BIT BIT6
#define BTN1_O P4OUT
#define BTN1_D P4DIR
#define BTN1_I P4IN
#define BTN1_BIT BIT1
#define BTN2_O P2OUT
#define BTN2_D P2DIR
#define BTN2_I P2IN
#define BTN2_BIT BIT3
#elif defined (__MSP430FR2433__)
#define REDLED_O P1OUT
#define REDLED_D P1DIR
#define REDLED_I P1IN
#define REDLED_BIT BIT0
#define GREENLED_O P1OUT
#define GREENLED_D P1DIR
#define GREENLED_I P1IN
#define GREENLED_BIT BIT1
#define BTN1_O P2OUT
#define BTN1_D P2DIR
#define BTN1_I P2IN
#define BTN1_BIT BIT3
#define BTN2_O P2OUT
#define BTN2_D P2DIR
#define BTN2_I P2IN
#define BTN2_BIT BIT7
#else
#error "Failed to match a default include file"
#endif

typedef enum {SET, RESET, TOGGLE, READ} led_t;
typedef enum {BUTTON1, BUTTON2} btn_t;

bool inline inRange(uint16_t low, uint16_t high, uint16_t x);

bool redLED(led_t CMD);
bool greenLED(led_t CMD);
void initLED(void);
void initBUTTON(void);
bool readBUTTON(btn_t button);

#endif /* DRIVERS_UTILS_H_ */
