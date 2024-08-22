/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

/* All functions in this module goes into the .boot section */
#pragma default_function_attributes = @ ".boot"
#pragma default_variable_attributes = @ ".bootdata"

#include "uart.h"
#include "xmodem.h"
#include "flash.h"
#include "timer.h"

/* Global variables */
xm_packet_t packet;

bool XMODEM_packet_verify(xm_packet_t *p, uint8_t expected_id)
{
    uint8_t checksum = 0;

    /* Calculate payload checksum */
    for (uint8_t idx = 0; idx < sizeof(packet.payload); idx++)
    {
        checksum += packet.payload[ idx ];
    }
    /* Check if the packet is invalid - <SOH>, blk#, 255 - blk#, (data), checksum */
    if (!((packet.preamble == XM_SOH) &&
          (packet.id == expected_id) &&
              (packet.id_cmpl == (0xFF - packet.id)) &&
                  (packet.checksum == checksum)))
    {
        return false;
    }
    return true;
}

xm_status_t XMODEM_flash(uint32_t flash_initial_address)
{
    uint32_t address = FLASH_BLOCK_ALIGNED(flash_initial_address);
    uint8_t expected_id = 1;     // XMODEM block number initiates @ 1
    uint8_t retry = RETRIES;

    uint8_t start_condition = true;
    uint8_t status = XM_STATUS_OK;
    bool ret = false;

    FLASH_init();

    while (1)
    {
        /* (Re-)initialize RX retries */
        retry = RETRIES;
        ret = false;
        /* Attempts to get RX data byte */
        while (retry && !ret)
        {
            /* If this is the start of the XMODEM packet */
            if (start_condition)
            {
                /* Sends a <NAK> to the transmitter */
                UART_send_byte(XM_NAK);
            }
            /* Waits for <SOH> or <EOT>) */
            ret = UART_recv_byte_timeout(5 * TIMEOUT, &packet.preamble);
            if (ret)
            {
                break;
            }
            retry--;
        }
        start_condition = false;
        /* Timed out? */
        if (!ret)
        {
            return XM_STATUS_TIMEOUT;
        }
        switch(packet.preamble)
        {
        case XM_SOH:
            break;
        case XM_EOT:
            UART_send_byte(XM_ACK);
            return XM_STATUS_OK;
            break;
        default:
            return XM_STATUS_COMMS_ERROR;
            break;
        }
        ret = UART_recv_packet(&packet);
        if (!XMODEM_packet_verify(&packet, expected_id))
        {
            /* Send <NAK> and loop back to the start of the while loop */
            UART_send_byte(XM_NAK);
            continue;
        }
        status = FLASH_write_packet(address, &packet);
        if (FSL_OK == status)
        {
            /* If programming passed, go to the next flash address */
            address += sizeof(packet.payload);
            expected_id++;
            UART_send_byte(XM_ACK);
        }
        else
        {
            /* Programming failed */
            UART_send_byte(XM_NAK);
            /* cancel XMODEM download */
            UART_send_byte(XM_CAN);
            return XM_STATUS_PROG_FAIL;
        }
    }
}

/* End of the .boot section functions */
#pragma default_function_attributes =
#pragma default_variable_attributes =
