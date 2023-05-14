/************************************************************************************************
Copyright (c) 2023, Balthazar Martin <balthazarm8@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/**
 ** \author Balthazar Martin
 ** \date 15/05/23
 ** \brief Implimentacion publica para testear Entradas y Salidas Digitales
 **
 ** \addtogroup tp4 TP4-main.c
 ** \brief Cuarto Trabajo Practico
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "chip.h"
#include "digital.h"
#include "bsp.h"
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {

    int divisor = 0;
    //bool current_state, last_state = false;

    board_t board = BoardCreate();

    while (true) {
        if (DigitalInputGetState(board->TEC_1)/*Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_1_GPIO, TEC_1_BIT  == 0 */) {
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, true);
            DigitalOutputActivate(board->ledrgb_azul);
        } else {
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, false);
            DigitalOutputDeactivate(board->ledrgb_azul);
        }

        //current_state = (DigitalInputGetState(TEC_2) /*Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_2_GPIO, TEC_2_BIT) == 0*/);
        if ( DigitalInputHasActivated(board->TEC_2) /*(current_state) && ( !last_state crear funcion?  )*/ ) {
            //Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT);
            DigitalOutputToggle(board->led_rojo);
        }
        //last_state = current_state;

        if (DigitalInputGetState(board->TEC_3)/*Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_3_GPIO, TEC_3_BIT) == 0*/) {
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, true);
            DigitalOutputActivate(board->led_amarillo);
        }
        if (DigitalInputGetState(board->TEC_4)/*Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_4_GPIO, TEC_4_BIT) == 0*/) {
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, false);
            DigitalOutputDeactivate(board->led_amarillo);
        }

        divisor++;
        if (divisor == 5) {
            divisor = 0;
            //Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT);
            DigitalOutputToggle(board->led_verde);
        }

        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
