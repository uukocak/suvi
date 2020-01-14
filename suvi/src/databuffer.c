/*
 * databuffer.c
 *
 *  Created on: Jan 14, 2020
 *      Author: Umut Utku Kocak
 */
#include <databuffer.h>
#include <stdint.h>

struct _databuffer_t{
    char temperature[12];
    char airpressure[12];
    char humidity[12];
    char windspeed[12];
    char cloudiness[12];
    char weatherstatus[12];
    char time[12];
    char date[12];
    char uptime[12];
    char systemtemp[12];
    char clientnum[12];
    uint16_t serlog;
    uint16_t weblog;
    uint16_t lcdlog;
    uint16_t sdlog;
}_databuffer;


void setLog(logtype_t logt,datatype_t datat)
{
    switch(logt)
    {
    case SERVER:
        _databuffer.serlog |= datat;
        break;
    case WEB:
        _databuffer.weblog |= datat;
        break;
    case LCD:
        _databuffer.lcdlog |= datat;
        break;
    case SD:
        _databuffer.sdlog |= datat;
        break;
    }

}
void resetLog(logtype_t logt,datatype_t datat)
{
    switch(logt)
    {
    case SERVER:
        _databuffer.serlog &= ~datat;
        break;
    case WEB:
        _databuffer.weblog &= ~datat;
        break;
    case LCD:
        _databuffer.lcdlog &= ~datat;
        break;
    case SD:
        _databuffer.sdlog &= ~datat;
        break;
    }

}
uint16_t getLog(logtype_t logt)
{
    uint16_t tempLog;
    switch(logt)
    {
    case SERVER:
        tempLog =  _databuffer.serlog;
        break;
    case WEB:
        tempLog =  _databuffer.weblog;
        break;
    case LCD:
        tempLog =  _databuffer.lcdlog;
        break;
    case SD:
        tempLog =  _databuffer.sdlog;
        break;
    default:
        tempLog =  0;
        break;
    }

    return tempLog;

}

char* getBuffer(datatype_t type)
{
    char* tempBufferp;
    switch(type)
    {
        case TEMPERATURE:
            tempBufferp = (char*) &(_databuffer.temperature);
            break;
        case AIRPRESSURE:
            tempBufferp = (char*) &(_databuffer.airpressure);
            break;
        case HUMIDITY:
            tempBufferp = (char*) &(_databuffer.humidity);
            break;
        case WINDSPEED:
            tempBufferp = (char*) &(_databuffer.windspeed);
            break;
        case CLOUDINESS:
            tempBufferp = (char*) &(_databuffer.cloudiness);
            break;
        case WEATHERSTATUS:
            tempBufferp = (char*) &(_databuffer.weatherstatus);
            break;
        case TIME:
            tempBufferp = (char*) &(_databuffer.time);
            break;
        case DATE:
            tempBufferp = (char*) &(_databuffer.date);
            break;
        case UPTIME:
            tempBufferp = (char*) &(_databuffer.uptime);
            break;
        case SYSTEMTEMP:
            tempBufferp = (char*) &(_databuffer.systemtemp);
            break;
        case CLIENTNUM:
            tempBufferp = (char*) &(_databuffer.clientnum);
            break;
    }

    return tempBufferp;
}
