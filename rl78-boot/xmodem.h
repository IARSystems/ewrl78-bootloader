/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

#pragma once

#include <stdbool.h>

#include "applilet3_src/r_cg_macrodriver.h"
#include "applilet3_src/r_cg_timer.h"

#include "FSL/IAR_210/lib/fsl.h"

typedef enum {
    XM_SOH = 0x01,
    XM_EOT = 0x04,
    XM_ACK = 0x06,
    XM_NAK = 0x15,
    XM_CAN = 0x18,
} xm_control_t;

typedef enum {
    XM_STATUS_OK= 0x00,
    XM_STATUS_ADDRESS_ERROR= 0x01,
    XM_STATUS_COMMS_ERROR = 0x02,
    XM_STATUS_TIMEOUT = 0x04,
    XM_STATUS_PROG_FAIL = 0x08
} xm_status_t;

typedef struct {
    uint8_t preamble;
    uint8_t id;
    uint8_t id_cmpl;
    uint8_t payload[128];
    uint8_t checksum;
} xm_packet_t;

#define TIMEOUT    1000
#define RETRIES    10

/**
 * @brief   Verifies a XMODEM packet integrity.
 * @param   p: A pointer to the packet.
 * @param   expected_id: expected packet id#.
 * @return  status: Report success/failure.
 */
bool XMODEM_packet_verify(xm_packet_t *p, uint8_t expected_id);

/**
 * @brief   Flashes the MCU using the XMODEM protocol.
 * @param   flash_initial_address: A 32-bit aligned address.
 * @return  xm_status_t: Report success/failure.
 */
xm_status_t XMODEM_flash(uint32_t flash_initial_address);
