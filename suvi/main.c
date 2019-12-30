/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Umut Utku Kocak
 */
#include <msp430fr2433.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"
#include "delay.h"
#include "ele417server.h"
#include "crc16modbus.h"

#define wait_time 1100

bool package_ready = false;
uint8_t afterscaler = 0;

uint16_t dummy;
uint8_t RESP_LEN = 0;
uint8_t buff_index = 0;
char rec_buff[50];
uint16_t check = 0;
char string2send[50];

typedef enum{
    IDLE_STATE,
    RESP_STATE,
    LEN_STATE,
    RECEIVE_STATE,
    DISABLE_STATE
}state_e;

state_e c_state = IDLE_STATE;
state_e n_state = IDLE_STATE;

datatype_t type_array[11] = {TEMPERATURE, AIRPRESSURE, HUMIDITY, WINDSPEED, CLOUDINESS, WEATHERSTATUS, TIME, DATE, UPTIME, SYSTEMTEMP, CLIENTNUM};
int8_t type_index = 0;

bool inRange(uint16_t low, uint16_t high, uint16_t x);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //  Disable the GPIO power-on default high-impedance mode ???

    uart_init(0); //Default settings for UCA0CTLW0 register
    delay_init(1); // Default 1Mhz clock selected

    P1DIR |= BIT0 | BIT1;    //set P1.0 , P1.1 output (LEDs)
    P2DIR &= ~(BIT3 | BIT7); //set P2.3 , P2.7 input (Buttons)
    P2REN |= BIT3;    //enable P2.3 , P2.7 pull up/pull down resistors
    P2OUT |= BIT3;    //set P2.3 , P2.7 pull-up resistor

    P1OUT &= ~( BIT0 | BIT1 );
    P2IE |= BIT3;
    P2IFG &= ~BIT3;

    TA0CTL = TASSEL1 | MC_1 | ID_3;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 65535;

    __bis_SR_register(GIE);

    while(1)
    {
        if(package_ready)
        {
            P1OUT |= BIT1;
            c_state = DISABLE_STATE;
        }
        else
        {
            P1OUT &= ~BIT1;
            //c_state = IDLE_STATE;
        }

        delay_ms(10);

    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_A(void)
{
    if(afterscaler < 3)
        afterscaler++;
    else
    {
        P1OUT ^= BIT0;  // Toggle P1.0
        if(package_ready)
        {
            check = (uint16_t)(rec_buff[RESP_LEN-2] << 8);
            check |= (uint16_t)rec_buff[RESP_LEN-1];
            if(check == calculateCRC((uint8_t *)rec_buff, RESP_LEN))
                P1OUT &= ~BIT1;

            sprintf(string2send,"WEB%s",(char*)rec_buff);
            string2send[RESP_LEN-1] = 0x00;

            uart_send_data(string2send, 9876, false);
            package_ready = false;

            c_state = IDLE_STATE;
        }
        else
        {
            if(type_index < 11)
            {
                getData(type_array[type_index]);
                type_index++;
            }
            else
            {
                getData(type_array[0]);
                type_index = 1;
            }


        }

        afterscaler = 0;

    }

    P2IFG &= ~BIT3; // P1.3 interrupt flag cleared
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    switch(c_state)
    {
    case IDLE_STATE:
        if(inRange(0x0001,0x000B,UCA0RXBUF))
            n_state = LEN_STATE;
        else
            n_state = IDLE_STATE;
        break;

    case LEN_STATE:
        RESP_LEN = UCA0RXBUF;
        buff_index = 0;
        n_state = RECEIVE_STATE;
        break;

    case RECEIVE_STATE:
        if(buff_index < RESP_LEN-3)
        {
            rec_buff[buff_index] = UCA0RXBUF;
            buff_index++;
            n_state = RECEIVE_STATE;
        }
        else
        {
            rec_buff[buff_index] = UCA0RXBUF;
            package_ready = true;
            buff_index = 0;
            n_state = IDLE_STATE;
        }
        break;

    case DISABLE_STATE:
        n_state = DISABLE_STATE;
        break;
    }

    dummy = UCA0RXBUF;

    P1OUT &= ~BIT0;
    c_state = n_state;

}

bool inRange(uint16_t low, uint16_t high, uint16_t x)
{
    return  ((x-low) <= (high-low));
}
