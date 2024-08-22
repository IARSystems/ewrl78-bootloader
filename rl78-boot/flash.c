/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

/* All functions in this module goes into the .boot section */
#pragma default_function_attributes = @ ".boot"
#pragma default_variable_attributes = @ ".bootdata"

#include "xmodem.h"
#include "flash.h"

/* Function pointer for jumping to the user app */
typedef void (*fun_ptr)(void);

/* Functions */
extern void main_app(void);
extern void main_boot(void);

/* Global vars */
const fsl_descriptor_t __far d = { .fsl_auto_status_check_u08 = 0x01,
                                   .fsl_flash_voltage_u08 = 0x00,
                                   .fsl_frequency_u08 = 0x20 };
void FLASH_init(void)
{
    fsl_u08 status;

    status = FSL_Init((const fsl_descriptor_t __far *)&d);
    if (FSL_OK != status)
    {
        while(1);
    }
    FSL_Open();
    FSL_PrepareFunctions();
    FSL_PrepareExtFunctions();
}

void FLASH_close(void)
{
    FSL_Close();
}

fsl_u08 FLASH_write(fsl_u32 address, fsl_u08 *data, fsl_u08 num_words)
{
    fsl_u08 status;

    if (0 == address % FLASH_BLOCK_SIZE)
    {
        if (FSL_OK != FSL_BlankCheck(FLASH_BLOCK_ALIGNED(address) / FLASH_BLOCK_SIZE))
        {
            status = FSL_Erase(FLASH_BLOCK_ALIGNED(address)/FLASH_BLOCK_SIZE);
            if (FSL_OK != status)
                while(1);
        }
    }

    fsl_write_t write = { .fsl_data_buffer_p_u08 = data,
                          .fsl_destination_address_u32 = FLASH_32BIT_ALIGNED(address),
                          .fsl_word_count_u08 = num_words };

    status = FSL_Write(&write);
    if (FSL_OK != status)
        while(1);

    return status;
}

fsl_u08 FLASH_write_packet(fsl_u32 address, xm_packet_t *packet)
{
    fsl_u08 status;

    if (0 == address % FLASH_BLOCK_SIZE)
    {
        if (FSL_OK != FSL_BlankCheck(FLASH_BLOCK_ALIGNED(address) / FLASH_BLOCK_SIZE))
        {
            status = FSL_Erase(FLASH_BLOCK_ALIGNED(address)/FLASH_BLOCK_SIZE);
            if (FSL_OK != status)
                while(1);
        }
    }

    fsl_write_t write = { .fsl_data_buffer_p_u08 = packet->payload,
                          .fsl_destination_address_u32 = FLASH_32BIT_ALIGNED(address),
                          .fsl_word_count_u08 = sizeof(packet->payload)/4 };

    status = FSL_Write(&write);
    if (FSL_OK != status)
        while(1);

    return status;
}

void FLASH_jump2app(void)
{
  fun_ptr jump2app = (fun_ptr)((void *)&main_app);
  jump2app();
}

void FLASH_jump2boot(void)
{
  fun_ptr jump2boot = (fun_ptr)((void *)&main_boot);
  jump2boot();
}

/* End of the .boot section functions */
#pragma default_function_attributes =
#pragma default_variable_attributes =