/*
 * ele417server.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_ELE417SERVER_H_
#define DRIVERS_ELE417SERVER_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum{
    WEATHERSTATUS = 0x0020,
    TEMPERATURE = 0x0001,
    AIRPRESSURE = 0x0002,
    HUMIDITY = 0x0004,
    WINDSPEED = 0x0008,
    CLOUDINESS = 0x0010,
    DATE = 0x0080,
    TIME = 0x0040,
    UPTIME = 0x0100,
    SYSTEMTEMP = 0x0200,
    CLIENTNUM = 0x0400,
    TEST = 0x0800,
    TIME_DATE,
    BATTERY = 0x1000,
    INIT = 0x8000
} datatype_t;

typedef enum{
    IDLE_STATE,
    START_STATE,
    RESP_STATE,
    LEN_STATE,
    RECEIVE_STATE,
    END_STATE,
    DISABLE_STATE
} state_e;

struct{
    uint8_t RESP_LEN;
    uint8_t buff_index;
    uint16_t check;
    bool package_ready;
    bool isNR;
    char string2send[50];
}uart_phase;


bool init_417server();
bool getData(datatype_t type);
state_e uartReceive(char * rec_buff);

#endif /* DRIVERS_ELE417SERVER_H_ */
