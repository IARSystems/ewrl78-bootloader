/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

#pragma once

#include "applilet3_src/r_cg_macrodriver.h"
#include "FSL/IAR_210/lib/fsl.h"

#include "xmodem.h"

#define FLASH_BLOCK_SIZE (0x400)
#define FLASH_APP_START_ADDRESS (0x2000)
#define FLASH_BOOT1_START_ADDRESS (0x1000)

#define FLASH_BLOCK_ALIGNED(address) (address & 0x00FFFC00)
#define FLASH_32BIT_ALIGNED(address) (address & 0x00FFFFFC)

/**
 * @brief   Initializes Code Flash Library.
 * @param   none.
 * @return  none.
 */
void FLASH_init(void);

/**
 * @brief   Shuts down Code Flash Library.
 * @param   none.
 * @return  none.
 */
void FLASH_close(void);

/**
 * @brief   Writes to the Code Flash.
 * @param   address: A 32-bit aligned Code Flash address.
 * @param   *data: A 8-bit pointer to the data buffer.
 * @param   num_words: The number of 32-bit words to write.
 * @return  fsl_u08: FSL status.
 */
fsl_u08 FLASH_write(fsl_u32 address, fsl_u08 *data, fsl_u08 num_words);

/**
 * @brief   Writes a XMODEM packet to the Code Flash.
 * @param   address: A 32-bit aligned Code Flash address.
 * @param   *packet: A pointer to a xm_packet_t packet.
 * @return  fsl_u08: FSL status.
 */
fsl_u08 FLASH_write_packet(fsl_u32 address, xm_packet_t *packet);

/**
 * @brief   Jumps to the user application.
 * @param   void
 * @return  void
 */
void FLASH_jump2app(void);

/**
 * @brief   Jumps to the user bootloader.
 * @param   void
 * @return  void
 */
void FLASH_jump2boot(void);