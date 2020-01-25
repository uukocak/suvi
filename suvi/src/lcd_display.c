/*
 * lcd_display.c
 *
 *  Created on: Jan 14, 2020
 *      Author: Ahmet Vehbi Genç
 */

#include <msp430.h>
#include <string.h>
#include <ssd1306_lib.h>
#include "lcd_display.h"


void dispData(datatype_t typeID, char * string1, char* string2 , int crc_status){

    //crc_status -> 0 no error
    //crc_status -> 1 first string
    //crc_status -> 2 second string
    //crc_status -> 3 fist & second error

switch(typeID)
{
     case WEATHERSTATUS:
         fillDisplay (0x00);
         draw6x8Str(31 , 2 , "TEMPERATURE" , 1 , 0 );
         draw6x8Str(61 , 3 , "&" , 1 , 0 );
         draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
         draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
         break;

     case TEMPERATURE:
     {
        if(!strcmp(string2,"Rain")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, rainy, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(55 , 37 , "RAINY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, rainy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(55 , 37 , "RAINY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else if(!strcmp(string2,"Snow")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, snowy, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(55 , 37 , "SNOWY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, snowy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(55 , 37 , "SNOWY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else if(!strcmp(string2,"Storm")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, stormy, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(49 , 37 , "STORMY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, stormy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(49 , 37 , "STORMY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else if(!strcmp(string2,"Fogg")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, foggy, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(55 , 37 , "FOGGY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, foggy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(55 , 37 , "FOGGY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else if(!strcmp(string2,"Partly Cloud")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, partly_cloudy, 0);
                draw12x16Str(43 , 3 , string1 , 1 );
                draw6x8Str(109 , 0 , "o" , 1 , 0 );
                draw12x16Str(115 , 3 , "C" , 1 );
                draw12x16Str(49 , 24 , "PARTLY" , 1 );
                draw12x16Str(49 , 45 , "CLOUDY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, partly_cloudy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(49 , 27 , "PARTLY" , 1 );
                draw12x16Str(49 , 45 , "CLOUDY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 3 , string1 , 1 );
                draw6x8Str(109 , 0 , "o" , 1 , 0 );
                draw12x16Str(115 , 3 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else if(!strcmp(string2,"Wind")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, windy, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(55 , 37 , "WINDY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, windy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(55 , 34 , "WINDY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else if(!strcmp(string2,"Clouds")){
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, cloudy, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(49 , 37 , "CLOUDY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, cloudy, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(49 , 34 , "CLOUDY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }
        else {
            if(crc_status == 0){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, sunny, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw12x16Str(55 , 37 , "SUNNY" , 1 );
            }
            else if(crc_status == 1){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, sunny, 0);
                draw6x8Str(52 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(46 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );
                draw12x16Str(55 , 34 , "SUNNY" , 1 );
            }
            else if(crc_status == 2){
                fillDisplay (0x00);
                drawImage(0, 0, 42, 64, temperature, 0);
                draw12x16Str(43 , 11 , string1 , 1 );
                draw6x8Str(109 , 1 , "o" , 1 , 0 );
                draw12x16Str(115 , 11 , "C" , 1 );
                draw6x8Str(43 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
                draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );
            }
            else{
                fillDisplay (0x00);
                draw6x8Str(31 , 0 , "TEMPERATURE" , 1 , 0 );
                draw6x8Str(25 , 1 , "-------------" , 1 , 0 );
                draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
                draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
                draw6x8Str(16 , 5 , "----------------" , 1 , 0 );
                draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
            }
        }

    }
    break;

    case AIRPRESSURE:
    {

        if(crc_status == 0){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, air_pressure, 0);
            draw6x8Str(48 , 0 , "AIR PRESSURE" , 1 , 0 );
            draw6x8Str(42 , 1 , "--------------" , 1 , 0 );
            draw12x16Str(43 , 24 , string1 , 1 );
            draw12x16Str(91 , 24 , "hpa" , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, air_pressure, 0);
            draw6x8Str(48 , 0 , "AIR PRESSURE" , 1 , 0 );
            draw6x8Str(42 , 1 , "--------------" , 1 , 0 );
            draw6x8Str(55 , 3 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case HUMIDITY:
    {

        if(crc_status == 0){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, humidity, 0);
            draw6x8Str(60 , 0 , "HUMIDITY" , 1 , 0 );
            draw6x8Str(54 , 1 , "----------" , 1 , 0 );
            draw12x16Str(61 , 24 , "%" , 1 );
            draw12x16Str(85 , 24 , string1 , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, humidity, 0);
            draw6x8Str(60 , 0 , "HUMIDITY" , 1 , 0 );
            draw6x8Str(54 , 1 , "----------" , 1 , 0 );
            draw6x8Str(55 , 3 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case WINDSPEED:
    {


        if(crc_status == 0){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, wind_speed, 0);
            draw6x8Str(54 , 0 , "WIND SPEED" , 1 , 0 );
            draw6x8Str(48 , 1 , "------------" , 1 , 0 );
            draw12x16Str(43 , 24 , string1 , 1 );
            draw12x16Str(91 , 24 , "m/s" , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, wind_speed, 0);
            draw6x8Str(54 , 0 , "WIND SPEED" , 1 , 0 );
            draw6x8Str(48 , 1 , "------------" , 1 , 0 );
            draw6x8Str(55 , 3 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case CLOUDINESS:
    {
        if(crc_status == 0){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, cloudiness, 0);
            draw6x8Str(54 , 0 , "CLOUDINESS" , 1 , 0 );
            draw6x8Str(48 , 1 , "------------", 1 , 0 );
            draw12x16Str(61 , 24 , "%" , 1 );
            draw12x16Str(85 , 24 , string1 , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, cloudiness, 0);
            draw6x8Str(54 , 0 , "CLOUDINESS" , 1 , 0 );
            draw6x8Str(48 , 1 , "------------", 1 , 0 );
            draw6x8Str(55 , 3 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case TIME_DATE:
        fillDisplay (0x00);
        draw6x8Str(43 , 2 , "CURRENT" , 1 , 0 );
        draw6x8Str(31 , 3 , "TIME & DATE" , 1 , 0 );
        draw6x8Str(25 , 4 , "-------------" , 1 , 0 );
    break;

    case TIME:
    {

        if(crc_status == 0){

            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, uptime, 0);
            draw12x16Str(42 , 11 , strtok(string1,":") , 1 );
            draw6x8Str(66 , 1 , "." , 1 , 0 );
            draw6x8Str(66 , 2 , "." , 1 , 0 );
            draw12x16Str(72 , 11 , strtok(NULL,":") , 1 );
            draw6x8Str(96 , 1 , "." , 1 , 0 );
            draw6x8Str(96 , 2 , "." , 1 , 0 );
            draw12x16Str(102 , 11 , strtok(NULL,":") , 1 );

            draw12x16Str(102 , 33 , strtok(string2,"/") , 1 );
            draw6x8Str(66 , 5 , "." , 1 , 0 );
            draw12x16Str(72 , 33 , strtok(NULL,"/")  , 1 );
            draw6x8Str(96 , 5 , "." , 1 , 0 );
            draw12x16Str(42 , 33 , strtok(NULL,"/")  , 1 );
        }
        else if(crc_status == 1){                                               //düzenlenecek!!!!

            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, current_date, 0);
            draw6x8Str(49 , 0 , "CURRENT TIME" , 1 , 0 );
            draw6x8Str(43 , 1 , "--------------" , 1 , 0 );
            draw6x8Str(55 , 2 , "CRC ERROR!" , 1 , 0 );

            draw12x16Str(102 , 33 , strtok(string2,"/") , 1 );
            draw6x8Str(66 , 5 , "." , 1 , 0 );
            draw12x16Str(72 , 33 , strtok(NULL,"/")  , 1 );
            draw6x8Str(96 , 5 , "." , 1 , 0 );
            draw12x16Str(42 , 33 , strtok(NULL,"/")  , 1 );
        }
        else if(crc_status == 2){                                               //düzenlenecek!!!!

            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, current_time, 0);
            draw12x16Str(42 , 11 , strtok(string1,":") , 1 );
            draw6x8Str(66 , 1 , "." , 1 , 0 );
            draw6x8Str(66 , 2 , "." , 1 , 0 );
            draw12x16Str(72 , 11 , strtok(NULL,":") , 1 );
            draw6x8Str(96 , 1 , "." , 1 , 0 );
            draw6x8Str(96 , 2 , "." , 1 , 0 );
            draw12x16Str(102 , 11 , strtok(NULL,":") , 1 );

            draw6x8Str(49 , 4 , "CURRENT DATE" , 1 , 0 );
            draw6x8Str(43 , 5 , "--------------" , 1 , 0 );
            draw6x8Str(55 , 6 , "CRC ERROR!" , 1 , 0 );

        }
        else{
            fillDisplay (0x00);
            draw6x8Str(28 , 0 , "CURRENT TIME" , 1 , 0 );
            draw6x8Str(22 , 1 , "--------------" , 1 , 0 );
            draw6x8Str(34 , 2 , "CRC ERROR!" , 1 , 0 );
            draw6x8Str(28 , 4 , "CURRENT DATE" , 1 , 0 );
            draw6x8Str(22 , 5 , "--------------" , 1 , 0 );
            draw6x8Str(34 , 6 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case UPTIME:
    {
        if(crc_status == 0){                                               //düzenlenecek!!!!
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, uptime, 0);
            draw6x8Str(67 , 0 , "UPTIME" , 1 ,0 );
            draw6x8Str(61 , 1 , "--------" , 1 ,0 );
            draw12x16Str(55 , 21 , strtok(string1,"d") , 1 );
            draw12x16Str(103 , 21 , "d" , 1 );
            draw12x16Str(49 , 42 , strtok(NULL,"m") , 1 );
            draw12x16Str(109 , 42 , "m" , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, uptime, 0);
            draw6x8Str(67 , 0 , "UPTIME" , 1 ,0 );
            draw6x8Str(61 , 1 , "--------" , 1 ,0 );
            draw6x8Str(55 , 3 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case SYSTEMTEMP:
    {

        if(crc_status == 0){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, system_temperature, 0);
            draw6x8Str(66 , 0 , "SYSTEM" , 1 ,0 );
            draw6x8Str(51 , 1 , "TEMPERATURE" , 1 ,0 );
            draw6x8Str(45 , 2 , "-------------" , 1 ,0 );
            draw12x16Str(43 , 35 , string1 , 1 );
            draw6x8Str(109 , 4 , "o" , 1 , 0 );
            draw12x16Str(115 , 35 , "C" , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, system_temperature, 0);
            draw6x8Str(66 , 0 , "SYSTEM" , 1 ,0 );
            draw6x8Str(51 , 1 , "TEMPERATURE" , 1 ,0 );
            draw6x8Str(45 , 2 , "-------------" , 1 ,0 );
            draw12x16Str(55 , 4 , "CRC ERROR!" , 1 );
        }
    }
    break;

    case CLIENTNUM:
    {

        if(crc_status == 0){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, number_of_clients, 0);
            draw6x8Str(51 , 0 , "CONNECTED #" , 1 ,0 );
            draw6x8Str(54 , 1 , "OF CLIENTS" , 1 ,0 );
            draw6x8Str(45 , 2 , "-------------", 1 ,0 );
            draw12x16Str(73 , 32 , string1 , 1 );
        }
        else if(crc_status == 1){
            fillDisplay (0x00);
            drawImage(0, 0, 42, 64, number_of_clients, 0);
            draw6x8Str(51 , 0 , "CONNECTED #" , 1 , 0 );
            draw6x8Str(54 , 1 , "OF CLIENTS" , 1 , 0 );
            draw6x8Str(45 , 2 , "-------------", 1 , 0 );
            draw6x8Str(55 , 4 , "CRC ERROR!" , 1 , 0 );
        }
    }
    break;

    case BATTERY:
    {
        fillDisplay (0x00);
        drawImage(29, 0, 71, 64, battery, 0);
        draw12x16Str(40 , 42 , "%" , 1 );
        draw12x16Str(64 , 42 , string1 , 1 );
    }
    break;

    case INIT:
    {
        fillDisplay (0x00);
        drawImage(40, 0, 48, 48, hacettepe_logo, 0);
        draw6x8Str(4 , 6 , "HACETTEPE UNIVERSITY" , 1 , 0 );

        fillDisplay (0x00);
        draw6x8Str(13 , 0 , "ELE417 - EMBEDDED" , 1 , 0 );
        draw6x8Str(25 , 1 , "SYSTEM DESIGN" , 1 , 0 );
        draw6x8Str(7 , 2 , "-------------------" , 1 , 0 );
        draw6x8Str(1 , 4 , "- SUMEYRA DURAK" , 1 , 0 );
        draw6x8Str(1 , 5 , "- UMUT UTKU KOCAK" , 1 , 0 );
        draw6x8Str(1 , 6 , "- IBRAHIM KAYMAK" , 1 , 0 );
        draw6x8Str(1 , 7 , "- AHMET VEHBI GENC" , 1 , 0 );
    }
    break;

    default :
    {
        fillDisplay (0x00);
        draw12x16Str(4 , 24 , "LOADING..." , 1 );
    }
    break;
}

}



