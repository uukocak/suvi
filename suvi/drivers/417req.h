/*
 * 417req.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_417REQ_H_
#define DRIVERS_417REQ_H_

#define REQ_TEMP 0x01       // Temperature
#define RESP_TEMP_LEN 5     // ex. 13.36

#define REQ_AIRP 0x02       // Air pressure
#define RESP_AIRP_LEN 4     // ex. 1016

#define REQ_HUMI 0x03       // Humidity (%)
#define RESP_HUMI_LEN 2     // ex. 50

#define REQ_WINDS 0x04      // WindSpeed(m/s)
#define RESP_WINDS_LEN 2    // ex. 4.5

#define REQ_CLOUD 0x05      // Cloudiness (%)
#define RESP_CLOUD_LEN 2    // ex. 30

#define REQ_WEATHER 0x06    // Weather
#define RESP_WEATHER_LEN 4  // ex. rain

#define REQ_TIME 0x07       // Time
#define RESP_TIME_LEN 5     // ex. 16:05:12

#define REQ_DATE 0x08       // Date
#define RESP_DATE_LEN 10    // ex. 2019/11/20

#define REQ_UPTIME 0x09     // Uptime
#define RESP_UPTIME_LEN 10  // ex. 1w4d10h15m

#define REQ_SYSTEMP 0x0A    // System Temperature
#define RESP_SYSTEMP_LEN 4  // ex. 40.5

#define REQ_CLNUM 0x0B      // Number of clients
#define RESP_CLNUM_LEN 2    // ex. 10

#define REQ_TEST 0xFF       // Test
#define RESP_TEST_LEN 2     // ex. hi

#define REQBYTE 0xEE        //Request initial byte

#endif /* DRIVERS_417REQ_H_ */
