/*
 * ele417server.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_ELE417SERVER_H_
#define DRIVERS_ELE417SERVER_H_

typedef enum{
    TEMPERATURE = 0x0001,
    AIRPRESSURE = 0x0002,
    HUMIDITY = 0x0004,
    WINDSPEED = 0x0008,
    CLOUDINESS = 0x0010,
    WEATHERSTATUS = 0x0020,
    TIME = 0x0040,
    DATE = 0x0080,
    UPTIME = 0x0100,
    SYSTEMTEMP = 0x0200,
    CLIENTNUM = 0x0400,
    TEST = 0x0800
} datatype_t;

bool init_417server();
bool getData(datatype_t type);

#endif /* DRIVERS_ELE417SERVER_H_ */
