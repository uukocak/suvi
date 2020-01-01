/*
 * utils.c
 *
 *  Created on: Dec 31, 2019
 *      Author: Umut Utku Kocak
 */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <utils.h>

bool inline inRange(uint16_t low, uint16_t high, uint16_t x)
{
    return  ((x-low) <= (high-low));
}

bool redLED(led_t CMD)
{
    switch(CMD)
    {
    case SET:
        REDLED_O |= REDLED_BIT;
        break;
    case RESET:
        REDLED_O &= ~REDLED_BIT;
        break;
    case TOGGLE:
        REDLED_O ^= REDLED_BIT;
        break;
    case READ:
        break;
    }

    return (bool)(REDLED_I & REDLED_BIT);
}

bool greenLED(led_t CMD)
{
    switch(CMD)
    {
    case SET:
        GREENLED_O |= GREENLED_BIT;
        break;
    case RESET:
        GREENLED_O &= ~GREENLED_BIT;
        break;
    case TOGGLE:
        GREENLED_O ^= GREENLED_BIT;
        break;
    case READ:
        break;
    }

    return (bool)(GREENLED_I & GREENLED_BIT);
}

void initLED(void)
{
    REDLED_D |= REDLED_BIT;
    GREENLED_D |= GREENLED_BIT;
    REDLED_O &= ~REDLED_BIT;
    GREENLED_O &= ~GREENLED_BIT;
}

bool readBUTTON(btn_t button)
{
    if(button == BUTTON1)
        return (bool)(BTN1_I & BTN1_BIT);
    else if(button == BUTTON2)
        return (bool)(BTN2_I & BTN2_BIT);
    else
        return -1;
}

void initBUTTON(void)
{
    BTN1_D &= ~BTN1_BIT;
    BTN1_O |= BTN1_BIT;
    BTN2_D &= ~BTN2_BIT;
    BTN2_O |= BTN2_BIT;
}


