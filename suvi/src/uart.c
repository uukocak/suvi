/*
 * uart.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Umut Utku Kocak
 */
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "delay.h"
#include "uart.h"

bool _wait_for_tx_available(uint16_t timeout_val);
bool _wait_for_rx_available(uint16_t timeout_val);

bool uart_send_data(char tx_buffer[], uint16_t tx_timeout, bool aCRLF)
{
    uint16_t buffer_len = strlen(tx_buffer);
    uint16_t buffer_idx = 0;

    for(buffer_idx=0; buffer_idx < buffer_len; buffer_idx++)
    {
        if(!uart_send_char(tx_buffer[buffer_idx], tx_timeout))
            return false;
    }

    if(aCRLF == true) //append CRLF
    {
        if(!uart_send_char(0x0D, tx_timeout))
            return false;
        if(!uart_send_char(0x0A, tx_timeout))
            return false;
    }

    return true;
}

char* uart_receive_data(uint16_t rx_timeout)
{
    static char rec_buff[50];
    char rec_char;
    uint16_t buff_index = 0;
    do
    {
        rec_char = uart_receive_char(rx_timeout);
        rec_buff[buff_index] = rec_char;
        buff_index++;
    }while(rec_char != 0x0D);

    return &rec_buff[0];
}

void uart_init(uint16_t UART_CONFIG)
{
    // Configure UART pins
    #if defined (__MSP430FR2355__)
    //P1SEL0 |= BIT6 | BIT7;          // set 2-UART pin as second function
    P4SEL0 |= BIT2 | BIT3;
    #elif defined (__MSP430FR2433__)
    P1SEL0 |= BIT4 | BIT5;          // set 2-UART pin as second function
    #else
    #error "Failed to match a default include file"
    #endif

    // Configure UART
    UCA1CTLW0 |= UCSWRST;       // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL_2;      // Chose SMCLK as clock source

    // Baud Rate calculation for 115200 with SMCLK (1MHz)
    // 1000000/115200 = 8.68
    // 1000000/115200 - INT(1000000/115200)=0.68
    // UCBRSx value = 0xD6 (See UG)

    // Baud Rate calculation for 9600 with SMCLK (1MHz)
    // 1000000/(9600) = N = 104.1667
    // while N > 16 | UCOS16 = 1;
    // UCBRx = INT(N/16) = INT(6.5104) = 6
    // UCBRFx = INT([(N/16) � INT(N/16)] � 16) = INT((6.5104 - 6) x 16 ) = INT(8.1664) = 8
    // UCBRSx = UGtable(N-INT(N)) = UGtable(0.1667) = 0x11

    UCA1BR0 = 6;
    UCA1BR1 = 0;
    UCA1MCTLW = 0x2200 | UCOS16 | UCBRF_13;

    UCA1IE |= UART_CONFIG;
    UCA1CTLW0 &= ~UCSWRST;          // Initialize eUSCI
    UCA1IE &= ~UCRXIE;               // Disable USCI_A0 RX interrupt

}

bool uart_send_char(char tx_char, uint16_t tx_timeout)
{
    if(_wait_for_tx_available(tx_timeout))
    {
        // if tx available initiate transmission

        UCA1TXBUF = tx_char;
        return true;
    }
    else
    {
        // Timeout
        return false;
    }
}


char uart_receive_char(uint16_t rx_timeout)
{
    if(_wait_for_rx_available(rx_timeout))
    {
        // if rx available initiate transmission
        return UCA1RXBUF;
    }
    else
    {
        // Timeout
        return false;
    }
}

bool _wait_for_tx_available(uint16_t timeout_val)
{
    while(!(UCA1IFG & UCTXIFG))
    {
        if (timeout_val > 0)
            timeout_val--;
        else
            return false; // Timeout
    }

    return true; // TX available
}

bool _wait_for_rx_available(uint16_t timeout_val)
{
    while(!(UCA1IFG & UCRXIFG))
    {
        if (timeout_val == 1)
            return false; // Timeout
        else if(timeout_val == 0) //no timeout
            timeout_val = 0;
        else
            timeout_val--;
    }

    return true; // TX available
}

/*  ===========================
| UCSYNC | UCMODEx | MODE
    0        00      UART
    1        00      SPI(3)
    1     01 or 10   SPI(4)
    1        11      I2C
================================*/
