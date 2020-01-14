#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "address.h"
#include "font6x8.h"
#include "font12x16.h"
#include "ssd1306_lib.h"
#include "ssd1306_i2c_lib.h"

volatile uint32_t i;
unsigned char data [2];
unsigned char *TI_transmit_field;
unsigned char *dataBuffer;

//******************************************************************************************************************************************
void ssd1306Init() {
    sendCommand(SSD1306_DISPLAY_OFF);                                   /* Entire Display OFF */
    sendCommand(SSD1306_SET_DISPLAY_CLOCK_DIV);                         /* Set Display Clock Divide Ratio and Oscillator Frequency */
    sendCommand(0x80);                                                  /* Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended */
    sendCommand(SSD1306_SET_MULTIPLEX_RATIO);                           /* Set Multiplex Ratio */
    sendCommand(0x3F);                                                  /* 64 COM lines */
    sendCommand(SSD1306_SET_DISPLAY_OFFSET);                            /* Set display offset */
    sendCommand(0x00);                                                  /* 0 offset */
    sendCommand(SSD1306_SET_START_LINE | 0x00);                         /* Set first line as the start line of the display */
    sendCommand(SSD1306_SET_CHARGE_PUMP);                               /* Charge pump */
    sendCommand(0x14);                                                  /* Enable charge dump during display on */
    sendCommand(SSD1306_MEMORY_ADDRESS_MODE);                           /* Set memory addressing mode */
    sendCommand(SSD1306_SET_LCOL_START_ADDRESS);                        /* Horizontal addressing mode */
    sendCommand(SSD1306_SEGMENT_REMAP | 0x01);                          /* Set segment remap with column address 127 mapped to segment 0 */
    sendCommand(SSD1306_COM_SCAN_INVERSE);                              /* Set com output scan direction, scan from com63 to com 0 */
    sendCommand(SSD1306_SET_COM_PINS_CONFIG);                           /* Set com pins hardware configuration */
    sendCommand(0x12);                                                  /* Alternative com pin configuration, disable com left/right remap */
    sendCommand(SSD1306_SET_CONTRAST);                                  /* Set contrast control */
    sendCommand(0x80);                                                  /* Set Contrast to 128 */
    sendCommand(SSD1306_SET_PRECHARGE_PERIOD);                          /* Set pre-charge period */
    sendCommand(0xF1);                                                  /* Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK */
    sendCommand(SSD1306_SET_VCOM_DESELECT_LVL);                         /* Set Vcomh deselect level */
    sendCommand(0x40);                                                  /* Vcomh deselect level */
    sendCommand(SSD1306_ENTIRE_DISPLAY_RESUME);                         /* Entire display ON, resume to RAM content display */
    sendCommand(SSD1306_NORMAL_DISPLAY);                                /* Set Display in Normal Mode, 1 = ON, 0 = OFF */
    sendCommand(SSD1306_DISPLAY_ON);                                    /* Display on in normal mode */
}
//******************************************************************************************************************************************
void sendCommand (unsigned char command) {
    data[0] = 0x00;
    data[1] = command;

    sendData(data, 2);
}
//******************************************************************************************************************************************
void sendData (unsigned char *params, unsigned char flag) {
    TI_transmit_field = params;
    i2c_init ();
    i2c_transmit (TI_transmit_field,flag);
}
//******************************************************************************************************************************************
void setCursor (unsigned char x, unsigned char p) {
    sendCommand(SSD1306_SET_LCOL_START_ADDRESS | (x & 0x0F));
    sendCommand(SSD1306_SET_HCOL_START_ADDRESS | (x >> 4));
    sendCommand(SSD1306_SET_PAGE_START_ADDRESS | p);
}
//******************************************************************************************************************************************
void darwPixel (unsigned char x, unsigned char y, unsigned char clear) {

    if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT)) return;
    setCursor(x, y >> 3);
    data[0] = SSD1306_DATA_MODE;
    if (clear)
        data[1] = (1 << (y & 7));
    else
        data[1] = ~(1 << (y & 7));
    sendData(data, 2);
}
//******************************************************************************************************************************************
void fillDisplay(unsigned char param) {
  unsigned char page, x;

  dataBuffer = malloc(129);
  dataBuffer[0] = SSD1306_DATA_MODE;
  for (page = 0; page < 8; page++) {
      setCursor(0, page);
    for (x = 0; x < 128; x++) {
        dataBuffer[x + 1] = param;
    }
    sendData(dataBuffer, 129);
  }
  free(dataBuffer);
}
//******************************************************************************************************************************************
void drawImage(unsigned char x, unsigned char y, unsigned char sx,
                       unsigned char sy, const unsigned char img[],
                       unsigned char invert) {
  unsigned int j, t;
  unsigned char i, p, p0, p1, n, n1, b;

  if (((x + sx) > SSD1306_WIDTH) || ((y + sy) > SSD1306_HEIGHT) ||
      (sx == 0) || (sy == 0)) return;

  // Total bytes of the image array
  if (sy % 8)
    t = (sy / 8 + 1) * sx;
  else
    t = (sy / 8) * sx;
  p0 = y / 8;                 // first page index
  p1 = (y + sy - 1) / 8;      // last page index
  n = y % 8;                  // offset form begin of page

  n1 = (y + sy) % 8;
  if (n1) n1 = 8 - n1;

  j = 0;                      // bytes counter [0..t], or [0..(t+sx)]
  dataBuffer = malloc(sx + 1);       // allocate memory for the buf
  dataBuffer[0] = SSD1306_DATA_MODE; // fist item "send data mode"
  for (p=p0; p<(p1+1); p++) {
      setCursor(x, p);
    for (i=x; i<(x+sx); i++) {
      if (p == p0) {
        b = (img[j] << n) & 0xFF;
      } else if ((p == p1) && (j >= t)) {
        b = (img[j - sx] >> n1) & 0xFF;
      } else {
        b = ((img[j - sx] >> (8 - n)) & 0xFF) | ((img[j] << n) & 0xFF);
      };
      if (invert)
          dataBuffer[i - x + 1] = b;
      else
          dataBuffer[i - x + 1] = ~b;
      j++;
    }
    sendData(dataBuffer, sx + 1); // send the buf to display
  }
  free(dataBuffer);
}
//******************************************************************************************************************************************
void draw6x8Str(unsigned char x, unsigned char p, const char str[],
                        unsigned char invert, unsigned char underline) {
  unsigned char i, j, b, buf[FONT6X8_WIDTH + 1];
  unsigned int c;

  i = 0;
  buf[0] = SSD1306_DATA_MODE; // fist item "send data mode"
  while (str[i] != '\0') {
    if (str[i] > 191)
      c = (str[i] - 64) * FONT6X8_WIDTH;
    else
      c = str[i] * FONT6X8_WIDTH;
    if (x > (SSD1306_WIDTH - FONT6X8_WIDTH - 1))
    {
      x = 0;
      p++;
    };
    if (p > 7) p = 0;
    setCursor(x, p);
    for (j = 0; j < FONT6X8_WIDTH; j++)
    {
      if (underline)
        b = font6x8[(unsigned int)(c + j)] | 0x80;
      else
        b = font6x8[(unsigned int)(c + j)];
      if (invert)
        buf[j + 1] = b;
      else
        buf[j + 1] = ~b;
    };
    sendData(buf, FONT6X8_WIDTH + 1); // send the buf to display
    x += FONT6X8_WIDTH;
    i++;
  };
}
//******************************************************************************************************************************************
void draw12x16Str(unsigned char x, unsigned char y, const char str[],
                          unsigned char invert) {
  unsigned char i;
  unsigned int c;

  i = 0;
  while (str[i] != '\0') {
    if (str[i] > 191)
      c = (str[i] - 64) * FONT12X16_WIDTH * 2;
    else
      c = str[i] * FONT12X16_WIDTH * 2;
    drawImage(x, y, 12, 16, (unsigned char *) &font12x16[c], invert);
    i++;
    x += 12;
  };
}
/******************************************************************************************************************************************
void dispData(){

    switch(typeID)
    {
       case : 01
       {
           char str temperature[];
           char str weather_status[];
           temperature = typeID_02 ;
           weather_status = typeID_06 ;

           switch(weather_status)
           {
               fillDisplay (0x00);
               draw6x8Str(31 , 2 , "TEMPERATURE" , 1 , 0 );
               draw6x8Str(61 , 3 , "&" , 1 , 0 );
               draw6x8Str(22 , 4 , "WEATHER STATUS" , 1 , 0 );
               draw6x8Str(16 , 5 , "----------------" , 1 , 0 );

               case : RAIN
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, rain, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(55 , 34 , weather_status , 1 );
               }
               break;

               case : SNOW
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, snow, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(55 , 34 , weather_status , 1 );
               }
               break;

               case : STORM
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, storm, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(49 , 34 , weather_status , 1 );
               }
               break;

               case : FOGG
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, foggy, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(55 , 34 , weather_status , 1 );
               }
               break;

               case : PARTLY CLOUD
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, partly_cloudy, 0);
                   draw12x16Str(43 , 4 , temperature , 1 );
                   draw12x16Str(49 , 24 , "PARTLY" , 1 );
                   draw12x16Str(49 , 44 , weather_status , 1 );
               }
               break;

               case : WIND
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, wind, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(55 , 34 , weather_status , 1 );
               }
               break;

               case : CLOUD
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, cloudy, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(49 , 34 , weather_status , 1 );
               }
               break;

               case : SUN
               {
                   fillDisplay (0x00);
                   drawImage(0, 0, 42, 64, sunny, 0);
                   draw12x16Str(43 , 14 , temperature , 1 );
                   draw12x16Str(55 , 34 , weather_status , 1 );
               }
               break;
           }
       }
       break;

       case : 02
       {
           char str air_pressure[];
           air_pressure = typeID_02;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, air_pressure, 0);
           draw6x8Str(48 , 0 , "AIR PRESSURE" , 1 , 0 );
           draw6x8Str(42 , 1 , "--------------" , 1 , 0 );
           draw12x16Str(43 , 24 , air_pressure , 1 );
       }
       break;

       case : 03
       {
           char str humidity[];
           humidity = typeID_03;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, humidity, 0);
           draw6x8Str(60 , 0 , "HUMIDITY" , 1 , 0 );
           draw6x8Str(54 , 1 , "----------" , 1 , 0 );
           draw12x16Str(61 , 24 , humidity , 1 );
       }
       break;

       case : 04
       {
           char str wind_speed[];
           wind_speed = typeID_04;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, wind_speed, 0);
           draw6x8Str(54 , 0 , "WIND SPEED" , 1 , 0 );
           draw6x8Str(48 , 1 , "------------" , 1 , 0 );
           draw12x16Str(43 , 24 , wind_speed , 1 );
       }
       break;

       case : 05
       {
           char str cloudiness[];
           cloudiness = typeID_05;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, cloudiness, 0);
           draw6x8Str(54 , 0 , "CLOUDINESS" , 1 , 0 );
           draw6x8Str(48 , 1 , "------------", 1 , 0 );
           draw12x16Str(61 , 24 , cloudiness , 1 );
       }
       break;

       case : 07 || 08
       {
           char str cloudiness[];
           cloudiness = typeID_05;

           fillDisplay (0x00);
           draw6x8Str(43 , 2 , "CURRENT" , 1 , 0 );
           draw6x8Str(31 , 3 , "TIME & DATE" , 1 , 0 );
           draw6x8Str(25 , 4 , "-------------" , 1 , 0 );
           fillDisplay (0x00);

           drawImage(0, 0, 42, 64, uptime, 0);
           draw12x16Str(42 , 11 , "11" , 1 );
           draw6x8Str(66 , 1 , "." , 1 , 0 );
           draw6x8Str(66 , 2 , "." , 1 , 0 );
           draw12x16Str(72 , 11 , "46" , 1 );
           draw6x8Str(96 , 1 , "." , 1 , 0 );
           draw6x8Str(96 , 2 , "." , 1 , 0 );
           draw12x16Str(102 , 11 , "05" , 1 );

           draw12x16Str(42 , 33 , "12" , 1 );
           draw6x8Str(66 , 5 , "." , 1 , 0 );
           draw12x16Str(72 , 33 , "01" , 1 );
           draw6x8Str(96 , 5 , "." , 1 , 0 );
           draw12x16Str(102 , 33 , "20" , 1 );
       }
       break;

       case : 09
       {
           char str uptime[];
           uptime = typeID_09;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, uptime, 0);
           draw6x8Str(67 , 0 , "UPTIME" , 1 ,0 );
           draw6x8Str(61 , 1 , "--------" , 1 ,0 );
           draw12x16Str(61 , 21 , uptime , 1 );
           draw12x16Str(49 , 42 , uptime , 1 );
       }
       break;

       case : 0A
       {
           char str sTemp[];
           sTemp = typeID_0A;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, system_temperature, 0);
           draw6x8Str(66 , 0 , "SYSTEM" , 1 ,0 );
           draw6x8Str(51 , 1 , "TEMPERATURE" , 1 ,0 );
           draw6x8Str(45 , 2 , "-------------" , 1 ,0 );
           draw12x16Str(49 , 32 , sTemp , 1 );
       }
       break;

       case : 0B
       {
           char str clients[];
           clients = typeID_0B;

           fillDisplay (0x00);
           drawImage(0, 0, 42, 64, number_of_clients, 0);
           draw6x8Str(51 , 0 , "CONNECTED #" , 1 ,0 );
           draw6x8Str(54 , 1 , "OF CLIENTS" , 1 ,0 );
           draw6x8Str(45 , 2 , "-------------", 1 ,0 );
           draw12x16Str(73 , 32 , clients , 1 );
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
*/
























