/*
 * databuffer.h
 *
 *  Created on: Jan 14, 2020
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_DATABUFFER_H_
#define DRIVERS_DATABUFFER_H_

#include "ele417server.h"

typedef enum { SERVER, WEB, LCD, SD, CRC} logtype_t;

void setLog(logtype_t logt,datatype_t datat);
void resetLog(logtype_t logt,datatype_t datat);
uint16_t getLog(logtype_t logt);
char* getBuffer(datatype_t);


#endif /* DRIVERS_DATABUFFER_H_ */
