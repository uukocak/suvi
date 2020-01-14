/*
 * main.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Umut Utku Kocak
 */

#include <stdbool.h>
#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "delay.h"
#include "uart.h"
#include "417req.h"
#include "ele417server.h"

bool init_417server()
{
    char txBuffer[50];
    sprintf(txBuffer, "SERhi");
    uart_send_data(txBuffer, 9876, true);
    return true;
}

bool getData(datatype_t type)
{
    char txBuffer[50];

    while(type)
    {
        if(type & TEMPERATURE)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_TEMP);
            type &= ~TEMPERATURE;
        }
        else if(type & AIRPRESSURE)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_AIRP);
            type &= ~AIRPRESSURE;
        }
        else if(type & HUMIDITY)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_HUMI);
            type &= ~HUMIDITY;
        }
        else if(type & WINDSPEED)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_WINDS);
            type &= ~WINDSPEED;
        }
        else if(type & CLOUDINESS)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_CLOUD);
            type &= ~CLOUDINESS;
        }
        else if(type & WEATHERSTATUS)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_WEATHER);
            type &= ~WEATHERSTATUS;
        }
        else if(type & TIME)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_TIME);
            type &= ~TIME;
        }
        else if(type & DATE)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_DATE);
            type &= ~DATE;
        }
        else if(type & UPTIME)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_UPTIME);
            type &= ~UPTIME;
        }
        else if(type & SYSTEMTEMP)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_SYSTEMP);
            type &= ~SYSTEMTEMP;
        }
        else if(type & CLIENTNUM)
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_CLNUM);
            type &= ~CLIENTNUM;
        }
        else
        {
            sprintf(txBuffer, "SER%c%c", REQBYTE, REQ_TEST);
            type &= ~REQ_TEST;
            break;
        }

        uart_send_data(txBuffer, 9876, false);
    }

    return true;
}

state_e uartReceive(char * rec_buff)
{
    static state_e c_state = IDLE_STATE;
    static state_e n_state = IDLE_STATE;

    uint16_t LOCAL_RXBUF = UCA1RXBUF;
    switch(c_state)
    {
        case IDLE_STATE:
            uart_phase.package_ready = false;
            if(LOCAL_RXBUF == 0x00AA)
                n_state = START_STATE;
            else
                n_state = IDLE_STATE;
            break;

        case START_STATE:
            if((LOCAL_RXBUF >= 0x0001) && (LOCAL_RXBUF <= 0x000B))
            {
                uart_phase.isNR = false;
                n_state = LEN_STATE;
            }
            else if (LOCAL_RXBUF == 0x0040)
            {
                uart_phase.package_ready = true;
                uart_phase.isNR = true;
                n_state = IDLE_STATE;
            }
            else
            {
                n_state = IDLE_STATE;
            }
            break;

        case LEN_STATE:
            uart_phase.RESP_LEN = LOCAL_RXBUF;
            uart_phase.buff_index = 0;
            n_state = RECEIVE_STATE;
            break;

        case RECEIVE_STATE:
            if(uart_phase.buff_index < uart_phase.RESP_LEN-3)
            {
                rec_buff[uart_phase.buff_index] = LOCAL_RXBUF;
                uart_phase.buff_index++;
                n_state = RECEIVE_STATE;
            }
            else
            {
                rec_buff[uart_phase.buff_index] = LOCAL_RXBUF;
                uart_phase.buff_index = 0;
                uart_phase.package_ready = true;
                n_state = IDLE_STATE;
            }
            break;

        case END_STATE:
            n_state = IDLE_STATE;
            break;
    }

    c_state = n_state;
    return c_state;
}
