/*
 * board.h
 *
 * TI AM335x boards information header
 *
 * Copyright (C) 2011, Texas Instruments, Incorporated - http://www.ti.com/
 *
 * Modified by Biagio Montaruli <biagio-mkr@libero.it> in order to configure
 * U-Boot for Arduino Tre board https://www.arduino.cc/en/Main/ArduinoBoardTre
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50

static inline int board_is_arduinotre(void)
{
        return board_ti_is("TREBOARD");
}

/*
 * We have three pin mux functions that must exist.  We must be able to enable
 * uart0, for initial output and i2c0 to read the main EEPROM.  We then have a
 * main pinmux function that can be overridden to enable all other pinmux that
 * is required on the board.
 */
void enable_uart0_pin_mux(void);
void enable_uart1_pin_mux(void);
void enable_uart2_pin_mux(void);
void enable_uart3_pin_mux(void);
void enable_uart4_pin_mux(void);
void enable_uart5_pin_mux(void);
void enable_i2c0_pin_mux(void);
void enable_board_pin_mux(void);
#endif
