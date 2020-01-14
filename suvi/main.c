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

#include "uart.h"
#include "delay.h"
#include "ele417server.h"
#include "crc16modbus.h"
#include "databuffer.h"
#include "utils.h"
#include "lcd_display.h"
#include "ssd1306_lib.h"

//TODO: Arrange global variables

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

phase_t current_phase = IDLE_PHASE;
phase_t next_phase;

datatype_t type_array[11] = {TEMPERATURE, AIRPRESSURE, HUMIDITY, WINDSPEED, CLOUDINESS, WEATHERSTATUS, TIME, DATE, UPTIME, SYSTEMTEMP, CLIENTNUM};
uint8_t type_index = 10;

bool start_flag = false;

bool inRange(uint16_t low, uint16_t high, uint16_t x);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //  Disable the GPIO power-on default high-impedance mode ???

    uart_init(0); //Default settings for UCA0CTLW0 register
    delay_init(1); // Default 1Mhz clock selected

    initLED();
    initBUTTON();

    P6DIR |= BIT1 | BIT2 | BIT0;
    P6OUT &= ~(BIT1 | BIT2 | BIT0);

    ssd1306Init();              // initialize the OLED
    fillDisplay (0x00);

    //P6DIR |= (BIT2); // 2-> ESP nRST mosfet
    //P6OUT &= ~(BIT2); // 2-> ESP nRST mosfet

    //2433 TA , 2355 TB
    TB0CTL = TBSSEL__SMCLK | MC_2 | ID_3 | TBCLR;
    TB0CCTL0 = CCIE;
    TB0CCR0 = 65535;

    __bis_SR_register(GIE);
    datatype_t cdatatype = TEMPERATURE;
    state_e uart_status;

    while(1)
    {
        switch(current_phase)
        {
            case IDLE_PHASE:
                if(start_flag)
                {
                    start_flag = false;
                    //exp disable timer
                    TB0CTL = TBSSEL__SMCLK | MC_0 | ID_3 | TBCLR;
                    //TB0CCTL0 &= ~CCIE;
                    TB0CCR0 = 65535;
                    //exp
                    TB0CCTL0 &= ~CCIE;                //EXP: disable timer interrupt
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
                getData(cdatatype);                                     //Request data
                UCA1IE |= UCRXIE;                                       //Enable uart interrupt.

                next_phase = RECEIVE_PHASE;
                break;

            case RECEIVE_PHASE:
                uart_status = uartReceive(getBuffer(cdatatype));    //Receive byte , record state

                if(!uart_phase.package_ready)
                {
                    next_phase = RECEIVE_PHASE;
                }
                else
                {
                    UCA1IE &= ~UCRXIE;                                  //Disable uart interrupt.
                    setLog(SERVER,cdatatype);                           //Update server data log
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
                    sprintf(uart_phase.string2send,"WEB%c%s",type_index+49,getBuffer(cdatatype));
                    uart_phase.string2send[uart_phase.RESP_LEN] = 0x00;                 //Discard crc bytes
                    uart_send_data(uart_phase.string2send, 9876, false);
                    setLog(WEB,cdatatype);                                   //Update web print data log
                    next_phase = LCD_PHASE;
                }
                break;

            case LCD_PHASE:
                dispData(TEMPERATURE,getBuffer(cdatatype),"Snow",0);  //Update screen
                setLog(LCD,cdatatype);                                    //Update lcd print data log
                delay_ms(500);  //Debug purposes , delete later

                next_phase = SD_PHASE;
                break;

            case SD_PHASE:
                //updateSD();                                               //Update sd card
                setLog(SD,cdatatype);                                       //Update web print data log
                delay_ms(500);  //Debug purposes , delete later

                next_phase = END_PHASE;
                break;

            case END_PHASE:
                if(type_index < 10)
                   type_index++;
                else
                    type_index = 0;

                //exp enable timer
                TB0CTL = TBSSEL__SMCLK | MC_2 | ID_3 | TBCLR;
                TB0CCTL0 = CCIE;
                TB0CCR0 = 65535;
                //exp
                //TB0CCTL0 = CCIE;                //EXP: enable timer interrupt

                next_phase = IDLE_PHASE;
                break;

            default:
                next_phase = IDLE_PHASE;
                break;
        }

        current_phase = next_phase;
        if((current_phase == RECEIVE_PHASE) || (current_phase == IDLE_PHASE) )
            __bis_SR_register(GIE | LPM0_bits);                     //Enter low power with interrupts enabled

        __no_operation();
    }
}

//TODO: Define TA for 2433, TB for 2355

//2433 TA , 2355 TB
#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer_B(void)
{
    start_flag = true;                                  //Start sequence
    __bic_SR_register_on_exit(LPM0_bits);                             //Out lowpower on exit
    __no_operation();
}

//TODO: Define A1 for 2355 , A0 for 2433
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI1RX_ISR(void)
{
    dummy = UCA1RXBUF;
    __bic_SR_register_on_exit(LPM0_bits);                    //Out lowpower on exit
}





