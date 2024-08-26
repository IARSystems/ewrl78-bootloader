/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

/*!
  \file    main-boot.c
  \brief   Implements the main_boot() function for the bootloader example
  \version 20240820
*/

/* All functions/vars in this module goes into the .boot section */
#pragma default_function_attributes = @ ".boot"
#pragma default_variable_attributes = @ ".bootdata"

#include <stdbool.h>

#include "flash.h"
#include "timer.h"
#include "uart.h"
#include "xmodem.h"

/* User messages */
uint8_t str_version[] = "\n\r\n\r**************************\n\r*** RL78 bootloader V1 ***";
uint8_t str_menu[] = "\n\rChoose action\n\r[1]: Download new firmware...\n\r[2]: Jump back to the app...";
uint8_t str_command[] = "\n\rCOMMAND>";
uint8_t str_dl_boot[] = "\n\rINPUT: Transfer boot+app.bin (XMODEM)...";
uint8_t str_error_command[] = "\n\rERROR: Unrecognized command.";
uint8_t str_crc_fail[] = "\n\rERROR: Checksum failed.";
uint8_t str_upd_fail[] = "\n\rERROR: XMODEM_flash() update failed.";
uint8_t str_fw_upd_ok[] = "\n\rSTATUS: XMODEM_flash() update OK.";
uint8_t str_dbg_restart[] = "\n\rINPUT: Restart debugger! (Ctrl+Shift+R)";
uint8_t str_upd_timeout[] = "\n\rSTATUS: XMODEM timed out.";

void main_boot(void)
{
    xm_status_t ret = XM_STATUS_OK;
    uint8_t menu_selection = '\0';
    uint32_t __far * p = (uint32_t __far *)(0x00000);

    UART_init();

    UART_send_string(str_version);
    UART_send_string(str_menu);

    while (1U)
    {
        UART_send_string(str_command);
        UART_recv_byte(&menu_selection);
        switch(menu_selection)
        {
        case '1':
            UART_send_byte(menu_selection);
            UART_send_string(str_dl_boot);
            ret = XMODEM_flash(FLASH_BOOT1_START_ADDRESS);
            switch(ret)
            {
            case XM_STATUS_OK:
                UART_send_string(str_fw_upd_ok);
                // The Debugger needs manual restart (Ctrl+Shift+R)
                if (*p == 0x000100D0)
                {
                    UART_send_string(str_dbg_restart);
                }
                FSL_InvertBootFlag();
                FSL_ForceReset();
                break;
            case XM_STATUS_TIMEOUT:
                UART_send_string(str_upd_timeout);
                break;
            default:
                UART_send_string(str_upd_fail);
            }
            break;
        case '2':
            FLASH_jump2app();
            break;
        case '?':
            UART_send_string(str_menu);
            break;
        default:
            UART_send_string(str_error_command);
        }
        TIMER_delay(200);
    }
}

/* End of the .boot section */
#pragma default_function_attributes =
#pragma default_variable_attributes =
