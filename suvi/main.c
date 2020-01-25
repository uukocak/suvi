/*
 * main.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Umut Utku Kocak
 */
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "delay.h"
#include "crc16modbus.h"
#include "databuffer.h"
#include "utils.h"
#include "ele417server.h"
#include "lcd_display.h"
#include "ssd1306_lib.h"

//TODO: Arrange global variables
char printNum[80];
//unsigned char num = 5;
const char temp[200]={NULL};

char temp_string1[16] = "39";
char temp_string2[16] = "39";

uint16_t dummy = 0;

typedef enum{
    START_PHASE,
    REQUEST_PHASE,
    RECEIVE_PHASE,
    WEB_PHASE,
    LCD_PHASE,
    SD_PHASE,
    IDLE_PHASE,
    END_PHASE
}phase_t;

// State machine variables
phase_t current_phase = IDLE_PHASE;
phase_t next_phase;

// Datatype array
const datatype_t type_array[13] = {WEATHERSTATUS, TEMPERATURE, AIRPRESSURE, HUMIDITY, WINDSPEED, CLOUDINESS, DATE, TIME, UPTIME, SYSTEMTEMP, CLIENTNUM, BATTERY};

//Field selection number array
const char num_array[13] = {'6','1','2','3','4','5','8','7','9',':',';','0'};
uint8_t type_index = 0;

//Timer period array
const uint16_t timer_period[3] = {34576, 2048, 512};
uint8_t period_index = 0;

//Global flags and ADC_val buffer
uint16_t ADC_val;
bool start_flag = false;

//Function prototype
void adc_init();

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //  Disable the GPIO power-on default high-impedance mode ???

    initLED();
    initBUTTON();
    adc_init();
    ssd1306Init();     // initialize the OLED
    uart_init(0); //Default settings for UCA0CTLW0 register
    delay_init(1); // Default 1Mhz clock selected

    P6DIR |= BIT1 | BIT2 | BIT0;
    P6OUT &= ~(BIT1 | BIT2 | BIT0);

    P2DIR &= BIT3;
    P4DIR &= BIT1;

    P2REN |= BIT3;
    P4REN |= BIT1;

    P2OUT |= BIT3;
    P4OUT |= BIT1;

    P2IE |= BIT3;
    P4IE |= BIT1;

    P2IES |= BIT3;
    P4IES |= BIT1;

    fillDisplay (0x00);
    dispData(INIT,"0","0",0);  //Update screen

    //Reset ESP8266
    P6OUT |= (BIT2);
    delay_ms(100);
    P6OUT &= ~(BIT2);

    //Set timer
    TB0CTL = TBSSEL__ACLK | MC_1 | ID_3 | TBCLR;
    TB0CCTL0 = CCIE;
    TB0CCR0 = timer_period[1];

    __bis_SR_register(GIE);
    datatype_t cdatatype = TEMPERATURE;
    uint16_t check;
    uint8_t crc_result;
    char * temp_p;
    float bat_val;

    while(1)
    {

        switch(current_phase)
        {
            case IDLE_PHASE:
                if(start_flag)
                {
                    start_flag = false;

                    TB0CTL = TBSSEL__ACLK | MC_1 | ID_3 | TBCLR;
                    TB0CCR0 = timer_period[period_index];
                    TB0CCTL0 &= ~CCIE;
                    ADCIE &= ~ADCIE0;

                    next_phase = START_PHASE;
                }
                else
                {
                    next_phase = IDLE_PHASE;
                }
                break;

            case START_PHASE:
                cdatatype = type_array[type_index];                 //Set current datatype

                next_phase = REQUEST_PHASE;
                break;

            case REQUEST_PHASE:
                if(cdatatype == BATTERY)
                {
                    next_phase = LCD_PHASE;
                }
                else
                {
                    getData(cdatatype);                 //Request data
                    UCA1IE |= UCRXIE;                   //Enable uart interrupt.
                    next_phase = RECEIVE_PHASE;
                }
                break;

            case RECEIVE_PHASE:
                uartReceive(getBuffer(cdatatype));    //Receive byte , record state

                if(!uart_phase.package_ready)
                {
                    next_phase = RECEIVE_PHASE;
                }
                else
                {
                    UCA1IE &= ~UCRXIE;                                  //Disable uart interrupt.
                    setLog(SERVER,cdatatype);                           //Update server data log

                    sprintf(temp_string1,"%s",getBuffer(cdatatype));
                    check = 0;
                    check = (uint16_t)(temp_string1[uart_phase.RESP_LEN-4] << 8);
                    check |= (uint16_t)temp_string1[uart_phase.RESP_LEN-3];
                    if(check == calculateCRC((uint8_t *)temp_string1, uart_phase.RESP_LEN-4))
                        resetLog(CRC,cdatatype);
                    else
                        setLog(CRC,cdatatype);

                    temp_p = getBuffer(cdatatype);
                    *(temp_p + uart_phase.RESP_LEN - 4) = 0x00;

                    next_phase = WEB_PHASE;
                }
                break;

            case WEB_PHASE:
                if(uart_phase.isNR)
                {
                    next_phase = END_PHASE;
                }
                else
                {
                    sprintf(uart_phase.string2send,"WEB%c%s",num_array[type_index],getBuffer(cdatatype));
                    uart_send_data(uart_phase.string2send, 9876, false);
                    setLog(WEB,cdatatype);                                   //Update web print data log
                    next_phase = LCD_PHASE;
                }

                break;

            case LCD_PHASE:

                if(cdatatype == TEMPERATURE)
                {
                    sprintf(temp_string1,"%s",getBuffer(TEMPERATURE));
                    sprintf(temp_string2,"%s",getBuffer(WEATHERSTATUS));

                    if(getLog(CRC) & TEMPERATURE)
                        crc_result = getLog(CRC) & WEATHERSTATUS ? 3 : 1;
                    else
                        crc_result = getLog(CRC) & WEATHERSTATUS ? 2 : 0;

                    dispData(TEMPERATURE,temp_string1,temp_string2,crc_result);  //Update screen
                    setLog(LCD,TEMPERATURE|WEATHERSTATUS);
                }
                else if(cdatatype == WEATHERSTATUS)
                {
                    dispData(WEATHERSTATUS,"","",0);  //Update screen
                }
                else if(cdatatype == TIME)
                {
                    sprintf(temp_string1,"%s",getBuffer(TIME));
                    sprintf(temp_string2,"%s",getBuffer(DATE));

                    if(getLog(CRC) & TIME)
                        crc_result = getLog(CRC) & DATE ? 3 : 1;
                    else
                        crc_result = getLog(CRC) & DATE ? 2 : 0;

                    dispData(TIME,temp_string1,temp_string2,crc_result);  //Update screen
                    setLog(LCD,TIME|DATE);
                }
                else if(cdatatype == DATE)
                {
                    dispData(TIME_DATE,"","",0);  //Update screen
                }
                else if(cdatatype == BATTERY)
                {
                    bat_val = ((((float)ADC_val)*(3.3))/1024);
                    bat_val = ((bat_val - 1.4671)/0.1982)*100;
                    sprintf(temp_string1,"%d",(uint16_t)bat_val);
                    dispData(BATTERY,temp_string1,"0",0);  //Update screen
                }
                else
                {
                    sprintf(temp_string1,"%s",getBuffer(cdatatype));
                    crc_result = getLog(CRC) & cdatatype;
                    dispData(cdatatype,temp_string1,temp_string2,crc_result);  //Update screen
                    setLog(LCD,cdatatype);    //Update lcd print data log
                }

                next_phase = SD_PHASE;
                break;

            case SD_PHASE:
                //updateSD();                                               //Update sd card
                setLog(SD,cdatatype);                                       //Update web print data log

                next_phase = END_PHASE;
                break;

            case END_PHASE:
                if(type_index < 11)
                   type_index++;
                else
                   type_index = 0;

                TB0CTL = TBSSEL__ACLK | MC_1 | ID_3 | TBCLR;
                TB0CCTL0 = CCIE;
                TB0CCR0 = timer_period[period_index];

                ADCIE |= ADCIE0;

                next_phase = IDLE_PHASE;
                break;

            default:
                next_phase = IDLE_PHASE;
                break;
        }

        current_phase = next_phase;
        if((current_phase == RECEIVE_PHASE) || (current_phase == IDLE_PHASE) )
            __bis_SR_register(GIE | LPM3_bits);                     //Enter low power with interrupts enabled

        __no_operation();
    }

}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer_B(void)
{
    start_flag = true;                                  //Start sequence
    __bic_SR_register_on_exit(LPM3_bits);                             //Out lowpower on exit
    __no_operation();
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI1RX_ISR(void)
{
    dummy = UCA1RXBUF;
    __bic_SR_register_on_exit(LPM3_bits);                    //Out lowpower on exit
}

#pragma vector=PORT4_VECTOR
__interrupt void BTN1_ISR(void)
{
    if(period_index < 2)
        period_index++;


    if(period_index == 2)
    {
        redLED(RESET);
        greenLED(SET);
    }
    else if(period_index == 1 )
    {
        redLED(RESET);
        greenLED(RESET);
    }
    else
    {
        redLED(SET);
        greenLED(RESET);
    }
    delay_ms(20);
    P4IFG = 0;

}

#pragma vector=PORT2_VECTOR
__interrupt void BTN2_ISR(void)
{
    if(period_index > 0)
        period_index--;

    if(period_index == 2)
    {
        redLED(RESET);
        greenLED(SET);
    }
    else if(period_index == 1 )
    {
        redLED(RESET);
        greenLED(RESET);
    }
    else
    {
        redLED(SET);
        greenLED(RESET);
    }

    delay_ms(20);
    P2IFG = 0;

}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
    ADC_val = ADCMEM0;
}

void adc_init()
{

    P1SEL0 |= BIT5;
    P1SEL1 |= BIT5;

    ADCCTL0 |= ADCSHT_2 | ADCON;              // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP | ADCSSEL_1;            // ADCCLK = MODOSC; sampling timer
    ADCCTL2 |= ADCRES;                        // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_5;                    // A5 ADC input select; Vref=AVCC
    ADCIE |= ADCIE0;                          // Enable ADC conv complete interrupt
    ADCCTL0 |= ADCENC | ADCSC;                // Sampling and conversion start

}
