/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "koneko40_ortho.h"

#define OPTION_BYTES_BASE 0x40022020

#define FLASH_STATUS_REGISTER_OFFSET 0x10

volatile uint32_t * FLASH_STATUS_REGISTER = (volatile uint32_t *)(OPTION_BYTES_BASE + FLASH_STATUS_REGISTER_OFFSET); 

#define FLASH_STATUS_REGISTER_BUSY (1 << 16)

#define FLASH_CONTROL_REGISTER_OFFSET 0x14
volatile uint32_t * FLASH_CONTROL_REGISTER = (volatile uint32_t *)(OPTION_BYTES_BASE + FLASH_CONTROL_REGISTER_OFFSET);
#define FLASH_CONTROL_REGISTER_OPTION_MOD_START (1 << 17)
#define FLASH_CONTROL_REGISTER_OPTION_BYTES_LOADING (1 << 27)

#define FLASH_KEY_REGISTER_OFFSET 0x04
volatile uint32_t * FLASH_KEY_REGISTER = (volatile uint32_t *)(OPTION_BYTES_BASE + FLASH_KEY_REGISTER_OFFSET);

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB

#define FLASH_OPTION_KEY_REGISTER_OFFSET 0xC
volatile uint32_t * FLASH_OPTION_KEY_REGISTER = (volatile uint32_t *)(OPTION_BYTES_BASE + FLASH_OPTION_KEY_REGISTER_OFFSET);

#define OPTION_KEY1 0x08192A3B
#define OPTION_KEY2 0x4C5D6E7F

#define FLASH_OPTION_REGISTER_OFFSET 0x20
volatile uint32_t * FLASH_OPTION_REGISTER = (volatile uint32_t *)(OPTION_BYTES_BASE + FLASH_OPTION_REGISTER_OFFSET);
#define FLASH_OPTION_REGISTER_NBOOT0 (1 << 27)
#define FLASH_OPTION_REGISTER_NSWBOOT0 (1 << 26)
#define FLASH_OPTION_REGISTER_NBOOT1 (1 << 23)

void write_flash_register(volatile uint32_t * address, uint32_t value) {
    *address = value;
    while (((*FLASH_STATUS_REGISTER) & FLASH_STATUS_REGISTER_BUSY) != 0) {}
}

void set_flash_register_bit(volatile uint32_t * address, uint32_t bit) {
    *address |= bit;
    while (((*FLASH_STATUS_REGISTER) & FLASH_STATUS_REGISTER_BUSY) != 0) {}
}

void unset_flash_register_bit(volatile uint32_t * address, uint32_t bit) {
    *address &= ~bit;
    while (((*FLASH_STATUS_REGISTER) & FLASH_STATUS_REGISTER_BUSY) != 0) {}
}

void unlock_flash(void) {
    write_flash_register(FLASH_KEY_REGISTER, FLASH_KEY1);
    write_flash_register(FLASH_KEY_REGISTER, FLASH_KEY2);

    write_flash_register(FLASH_OPTION_KEY_REGISTER, OPTION_KEY1);
    write_flash_register(FLASH_OPTION_KEY_REGISTER, OPTION_KEY2);
}

void write_options(void) {
    unset_flash_register_bit(FLASH_CONTROL_REGISTER, FLASH_OPTION_REGISTER_NBOOT0);
    unset_flash_register_bit(FLASH_CONTROL_REGISTER, FLASH_OPTION_REGISTER_NSWBOOT0);
    set_flash_register_bit(FLASH_CONTROL_REGISTER, FLASH_OPTION_REGISTER_NBOOT1);

    set_flash_register_bit(FLASH_CONTROL_REGISTER, FLASH_CONTROL_REGISTER_OPTION_MOD_START);
    set_flash_register_bit(FLASH_CONTROL_REGISTER, FLASH_CONTROL_REGISTER_OPTION_BYTES_LOADING);
}

void keyboard_post_init_kb(void) {
    while (((*FLASH_STATUS_REGISTER) & FLASH_STATUS_REGISTER_BUSY) != 0) {}
    if ((*(volatile uint32_t *)FLASH_OPTION_REGISTER & FLASH_OPTION_REGISTER_NSWBOOT0) == 0) {
        return;
    }
    else {
        unlock_flash();
        write_options();
    }
}
