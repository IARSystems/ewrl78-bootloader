/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

/* All functions in this module goes into the .boot section */
#pragma default_function_attributes = @ ".boot"
#pragma default_variable_attributes = @ ".bootdata"

#include "uart.h"

#define UART_send R_UART2_Send
#define UART_recv R_UART2_Receive

/* Global control flags */
volatile bool g_uart_sent = false;
volatile bool g_uart_rcvd = false;
extern volatile uint32_t g_timer_ticks_ms;

void UART_init(void)
{
    R_UART2_Start();
}

bool UART_send_string(const uint8_t str[])
{
    uint8_t nul = '\0';
    size_t length = 0;
    g_uart_sent = false;
    while (nul != str[length]) ++length;
    UART_send((uint8_t*)str, length);
    while (!g_uart_sent);
    g_uart_sent = false;
    return true;
}

bool UART_send_byte(const uint8_t c)
{
    g_uart_sent = false;
    UART_send((uint8_t*)&c, 1);
    while (!g_uart_sent);
    g_uart_sent = false;
    return true;
}

bool UART_recv_byte(uint8_t *c)
{
    UART_recv(c, 1);
    while (!g_uart_rcvd);
    g_uart_rcvd = false;
    return true;
}

bool UART_recv_byte_timeout(uint32_t timeout_ms, uint8_t *data)
{
	g_timer_ticks_ms = timeout_ms;

    R_TAU0_Channel0_Start();
    (void)UART_recv(data, 1);
    while (!g_uart_rcvd && g_timer_ticks_ms > 0);
    R_TAU0_Channel0_Stop();
    if (g_uart_rcvd)
    {
        g_uart_rcvd = false;
        return true;
    }
    return false;
}

bool UART_recv_packet(xm_packet_t *p)
{
    (void)UART_recv((uint8_t*)&p->id, sizeof(p->id));
    while (!g_uart_rcvd);
    g_uart_rcvd = false;
    (void)UART_recv((uint8_t*)&p->id_cmpl, sizeof(p->id_cmpl));
    while (!g_uart_rcvd);
    g_uart_rcvd = false;
    (void)UART_recv((uint8_t*)&p->payload, sizeof(p->payload));
    while (!g_uart_rcvd);
    g_uart_rcvd = false;
    (void)UART_recv((uint8_t*)&p->checksum, sizeof(p->checksum));
    while (!g_uart_rcvd);
    g_uart_rcvd = false;
    return true;
}

/* End of the .boot section functions */
#pragma default_function_attributes =
#pragma default_variable_attributes =