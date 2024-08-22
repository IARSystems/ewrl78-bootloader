/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

#pragma once

#include <stdbool.h>

#include "applilet3_src/r_cg_macrodriver.h"
#include "applilet3_src/r_cg_timer.h"
#include "applilet3_src/r_cg_serial.h"

#include "xmodem.h"

/**
 * @brief   Initializes the UART.
 * @param   none.
 * @return  none.
 */
void UART_init(void);

/**
 * @brief   Sends a string to UART.
 * @param   str[]: Array of the data.
 * @return  status: Report success/failure.
 */
bool UART_send_string(const uint8_t str[]);

/**
 * @brief   Sends a single char to UART.
 * @param   *c: The char.
 * @return  status: Report success/failure.
 */
bool UART_send_byte(const uint8_t c);

/**
 * @brief   Receives a single char to UART.
 * @param   *c: A pointer to the char.
 * @return  status: Report success/failure.
 */
bool UART_recv_byte(uint8_t *c);

/**
 * @brief   Receives a Byte, with timeout
 * @param   timeout_ms: Timeout (in milliseconds).
 * @param   *data: 8-bit pointer to data buffer.
 * @return  status: Report success/failure.
 */
bool UART_recv_byte_timeout(uint32_t timeout_ms, uint8_t *data);

/**
 * @brief   Receives a XMODEM packet.
 * @param   *p: The buffer to accomodate the XMODEM packet.
 * @return  status: Report success/failure.
 */
bool UART_recv_packet(xm_packet_t *p);