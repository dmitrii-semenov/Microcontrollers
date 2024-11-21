/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <string.h>

#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
uint8_t strcmp_custom(const char *str1, const char *str2) {
	uint8_t res = 0;
	for (int i = 0; i < 20; i++) {
		if (str1[i] != str2[i]) {
			res = 1;
		}
	}

	if (res==1) {
		return 1;
	}
	else {
		return 0;
	}
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif

    uint32_t DWT1,DWT2,status;
    char password_stored[20] = "1234";
    char input[20];

    while (1)
    {
    	PRINTF("\r\nEnter password: ");
    	SCANF("%s", input);

    	DWT1 = DWT -> CYCCNT;
    	status = strcmp_custom(input,password_stored);
    	DWT2 = DWT -> CYCCNT;
    	PRINTF("\r\nCycles in function: %d", DWT2-DWT1);

    	PRINTF("\r\ninput: %s", input);
    	if(status==0) {
    		PRINTF("\r\ninput correct\n");}
    	else {
    		PRINTF("\r\ninput incorrect\n");}
    }
}
