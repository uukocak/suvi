/*
 * delay.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Umut Utku Kocak
 */

#include <stdint.h>

uint16_t CLK_FREQ; //Clock freq. in Mhz

void delay_init(uint16_t fclk)
{
    CLK_FREQ = fclk;
}

void delay_us(uint16_t us)
{
    while (us)
    {
        __delay_cycles(1); // 1 for 1 Mhz set 16 for 16 MHz
        us--;
    }
}

void delay_ms(uint16_t ms)
{
    while (ms)
    {
        __delay_cycles(1000); //1000 for 1MHz and 16000 for 16MHz
        ms--;
    }
}
