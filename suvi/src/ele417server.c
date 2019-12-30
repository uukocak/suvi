/*
 * main.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Umut Utku Kocak
 */

#include <stdbool.h>
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
