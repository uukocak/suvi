/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Umut Utku Kocak
 */
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"
#include "delay.h"
#include "ele417server.h"
#include "crc16modbus.h"
#include "utils.h"

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

    initLED();
    initBUTTON();

    P6DIR |= (BIT2); // 2-> ESP nRST mosfet
    P6OUT &= ~(BIT2); // 2-> ESP nRST mosfet

    //2433 TA , 2355 TB
    TB0CTL = TBSSEL1 | MC_1 | ID_3;
    TB0CCTL0 = CCIE;
    TB0CCR0 = 65535;

    __bis_SR_register(GIE);

    while(1)
    {
        if(package_ready)
        {
            //greenLED(SET);
            c_state = DISABLE_STATE;
        }
        else
        {
            //greenLED(RESET);
            //c_state = IDLE_STATE;
        }

        if(readBUTTON(BUTTON1))
        {
            P6OUT |= BIT2;
            greenLED(SET);
        }
        else
        {
            P6OUT &= ~(BIT2);
            greenLED(RESET);
        }
        delay_ms(10);

    }
}

//2433 TA , 2355 TB
#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer_B(void)
{
    if(afterscaler < 3)
        afterscaler++;
    else
    {
        redLED(TOGGLE);
        if(package_ready)
        {
            check = (uint16_t)(rec_buff[RESP_LEN-2] << 8);
            check |= (uint16_t)rec_buff[RESP_LEN-1];
            if(check == calculateCRC((uint8_t *)rec_buff, RESP_LEN))
                //P6OUT &= ~BIT6;

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

}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    switch(c_state)
    {
    case IDLE_STATE:
        if((UCA0RXBUF >= 0x0001) && (UCA0RXBUF <= 0x000B))
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

    redLED(RESET);
    c_state = n_state;

}


