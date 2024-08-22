/*
  Copyright (c) 2024, IAR Systems AB.
  SPDX-License-Identifier: MIT
*/

/* All functions in this module goes into the .boot section */
#pragma default_function_attributes = @ ".boot"
#pragma default_variable_attributes = @ ".bootdata"

#include "timer.h"

/* Global control flags */
extern volatile uint32_t g_timer_ticks_ms;

void TIMER_delay(uint32_t delay_ms)
{
    g_timer_ticks_ms = delay_ms;

    R_TAU0_Channel0_Start();
    while (g_timer_ticks_ms > 0);
    R_TAU0_Channel0_Stop();
}

/* End of the .boot section functions */
#pragma default_function_attributes =
#pragma default_variable_attributes =
