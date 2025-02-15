/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (c) 2015-2021 Renesas Electronics Corporation and/or its affiliates
 * The MIT License (MIT)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "arch.h"
#include "spi.h"
#include "spi_flash.h"
#include "user_periph_setup.h"
#include "datasheet.h"
#include "system_library.h"
#include "rwip_config.h"
#include "gpio.h"
#include "uart.h"
#include "syscntl.h"
#include "arch_console.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static const spi_cfg_t spi_cfg = {
.spi_ms = SPI_MS_MODE_MASTER,
.spi_cp = SPI_CP_MODE_0,
.spi_speed = SPI_SPEED_MODE_4MHz,
.spi_wsz = SPI_MODE_8BIT,
.spi_cs = SPI_CS_0,
.cs_pad.port = SPI_EN_PORT,
.cs_pad.pin = SPI_EN_PIN,
#if defined (__DA14531__)
.spi_capture = SPI_MASTER_EDGE_CAPTURE,

#endif
};


// WE HAVE THIS CHIP!!!!!
static const spi_flash_cfg_t spi_flash_cfg = {
.dev_index = AT25DF021A_DEV_INDEX,
.jedec_id = AT25DF021A_JEDEC_ID,
.chip_size = AT25DF021A_CHIP_SIZE,
};



/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
    RESERVE_GPIO(UART2_RX, UART2_RX_PORT, UART2_RX_PIN, PID_UART2_RX);
		RESERVE_GPIO(UART2_TX, UART2_TX_PORT, UART2_TX_PIN, PID_UART2_TX);
		RESERVE_GPIO(BT_STATE, BT_STATE_PORT, BT_STATE_PIN, PID_GPIO);
		RESERVE_GPIO(SPI_EN, SPI_EN_PORT, SPI_EN_PIN, PID_SPI_EN);
		RESERVE_GPIO(SPI_CLK, SPI_CLK_PORT, SPI_CLK_PIN, PID_SPI_CLK);
		RESERVE_GPIO(SPI_DO, SPI_DO_PORT, SPI_DO_PIN, PID_SPI_DO);
		RESERVE_GPIO(SPI_DI, SPI_DI_PORT, SPI_DI_PIN, PID_SPI_DI);
}

#endif
void set_pad_functions(void)
{
/*
    i.e. to set P0_1 as Generic purpose Output:
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false);
*/
    // Configure UART2 TX Pad
    GPIO_ConfigurePin(UART2_RX_PORT, UART2_RX_PIN, INPUT, PID_UART2_RX, false);
		GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
		GPIO_ConfigurePin(BT_STATE_PORT, BT_STATE_PIN, OUTPUT, PID_GPIO, false);


   	GPIO_ConfigurePin(SPI_EN_PORT, SPI_EN_PIN, OUTPUT, PID_SPI_EN, true);    // FLASH DEMO says "true"
		GPIO_ConfigurePin(SPI_DO_PORT, SPI_DO_PIN, OUTPUT, PID_SPI_DO, false);
		GPIO_ConfigurePin(SPI_DI_PORT, SPI_DI_PIN, INPUT, PID_SPI_DI, false);
		GPIO_ConfigurePin(SPI_CLK_PORT, SPI_CLK_PIN, OUTPUT, PID_SPI_CLK, false);
}

//#if defined (CFG_PRINTF_UART2)
// Configuration struct for UART2
static const uart_cfg_t uart_cfg = {
    .baud_rate = UART2_BAUDRATE,
    .data_bits = UART2_DATABITS,
    .parity = UART2_PARITY,
    .stop_bits = UART2_STOPBITS,
    .auto_flow_control = UART2_AFCE,
    .use_fifo = UART2_FIFO,
    .tx_fifo_tr_lvl = UART2_TX_FIFO_LEVEL,
    .rx_fifo_tr_lvl = UART2_RX_FIFO_LEVEL,
    .intr_priority = 2,
};
//#endif

void periph_init(void)
{
	
		GPIO_Disable_HW_Reset();
	
#if defined (__DA14531__)
    // In Boost mode enable the DCDC converter to supply VBAT_HIGH for the used GPIOs
    syscntl_dcdc_turn_on_in_boost(SYSCNTL_DCDC_LEVEL_3V0);
#else
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

    // ROM patch
    patch_func();
		// uart config
    uart_initialize(UART2, &uart_cfg);
		// spi configuration
		spi_flash_configure_env(&spi_flash_cfg);
		spi_initialize(&spi_cfg);
    // Set pad functionality
    set_pad_functions();
    // Enable the pads
    GPIO_set_pad_latch_en(true);
		GPIO_SetInactive(BT_STATE_PORT, BT_STATE_PIN);
		//spi_flash_release_from_power_down();
		//spi_flash_configure_memory_protection(SPI_FLASH_MEM_PROT_NONE);
		//spi_flash_auto_detect(&a);
}
