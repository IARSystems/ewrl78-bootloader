/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

/*!
  \file    main-app.c
  \brief   Implements the main_app() function for the bootloader example
  \version 20240820
*/

/* All functions/vars in this module goes into the APPCODE section */
#pragma default_function_attributes = @ ".app"

#include "flash.h"
#include "timer.h"
#include "uart.h"

#define LED_ON  (0)
#define LED_OFF (1)

void main_app(void)
{
    uint8_t menu_selection = '\0';

    uint16_t max_value = 1000;
    uint16_t min_value = 100;
    uint32_t initial_timeout = ((max_value - min_value) / 2) + min_value;
    uint32_t timeout = initial_timeout;
    uint8_t threshold = ((max_value - min_value) / 20) % 0xFF;

    TIMER_delay(750);

    UART_init();

    /* Init USER LED */
    P7 = 0x80;
    PM7 = 0x00;
    UART_send_string("\n\r*** Welcome to the app V1!\n\rPress:\n\r[b]: enter bootloader mode\n\r[+]: raise speed\n\r[*]: default speed\n\r[-]: lower speed\n\r");
    while(1)
    {
        P7_bit.no7 ^= 1;

        UART_recv_byte_timeout(25, &menu_selection);
        switch(menu_selection)
        {
        case '+':
            if (timeout - threshold > min_value)
                timeout -= threshold;
            break;
        case '-':
            if (timeout + threshold < max_value)
                timeout += threshold;
            break;
        case '*':
            timeout = initial_timeout;
            break;
        case 'b':
            P7_bit.no7 = LED_OFF;
            FLASH_jump2boot();
            break;
        default:
            break;
        }
        TIMER_delay(timeout);
    }
}

/* End of the boot sector */
#pragma default_function_attributes =
