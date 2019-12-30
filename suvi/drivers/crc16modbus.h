/*
 * crc16modbus.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Umut Utku Kocak
 */

#ifndef DRIVERS_CRC16MODBUS_H_
#define DRIVERS_CRC16MODBUS_H_

#include <stdint.h>

#define CRC_POLY 0xA001
#define CRC_INITIAL 0xFFFF

uint16_t calculateCRC(uint8_t * data, uint8_t datalen);

#endif /* DRIVERS_CRC16MODBUS_H_ */
