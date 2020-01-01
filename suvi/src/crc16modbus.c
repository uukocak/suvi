/*
 * crc16modbus.c
 *
 *  Created on: Dec 29, 2019
 *      Author: Umut Utku Kocak
 */
#include <msp430.h>
#include <stdint.h>
#include "crc16modbus.h"

uint16_t calculateCRC(uint8_t * data, uint8_t datalen)
{
    uint16_t crc = CRC_INITIAL;
    uint8_t i = 0;
    uint8_t j = 0;

    for(j = 0; j < datalen; j++)
    {
        crc ^= (uint16_t)*(data+j);
        for (i = 0; i < 8; ++i) {
            if (crc & 1)
                crc = (crc >> 1) ^ CRC_POLY;
            else
                crc = (crc >> 1);
        }
    }

    return crc;
}
