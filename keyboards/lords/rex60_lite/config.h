/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x0002
#define DEVICE_VER      0x0001
#define MANUFACTURER   	'Lord\'s Boards'
#define PRODUCT         Rex60 Lite 

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15 

#define MATRIX_ROW_PINS { GP29, GP28, GP27, GP26, GP25}
#define MATRIX_COL_PINS { GP0, GP1, GP2, GP3, GP4, GP5, GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13, GP14 }

#define LED_CAPS_LOCK_PIN GP23

#define DIODE_DIRECTION COL2ROW

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
#define RP2040_FLASH_GENERIC_03H
