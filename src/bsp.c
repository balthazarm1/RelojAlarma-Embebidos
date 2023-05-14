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
 ** \brief Implimentacion de board support package para la EDU-­CIAA-­NXP
 **
 ** \addtogroup Bsp Bsp.c
 ** \brief Implementacion bsp para EDU-­CIAA-­NXP
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "chip.h"
#include "ciaa.h"
#include "digital.h"
#include "bsp.h"
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

//! Estructura con informacion vacia sobre las Entradas y Salidas de la placa
static struct board_s board = {0};  //esto puede modificarse para que sea mas de 1 placa

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

board_t BoardCreate(void){
    Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, true);
    board.ledrgb_rojo=DigitalOutputCreate(LED_R_GPIO, LED_R_BIT);

    Chip_SCU_PinMuxSet(LED_G_PORT, LED_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_G_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, true);
    board.ledrgb_verde=DigitalOutputCreate(LED_G_GPIO, LED_G_BIT);

    Chip_SCU_PinMuxSet(LED_B_PORT, LED_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_B_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, true);
    board.ledrgb_azul=DigitalOutputCreate(LED_B_GPIO, LED_B_BIT);

    /******************/
    Chip_SCU_PinMuxSet(LED_1_PORT, LED_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_1_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, true);
    board.led_rojo=DigitalOutputCreate(LED_1_GPIO, LED_1_BIT);

    Chip_SCU_PinMuxSet(LED_2_PORT, LED_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_2_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, true);
    board.led_amarillo=DigitalOutputCreate(LED_2_GPIO, LED_2_BIT);

    Chip_SCU_PinMuxSet(LED_3_PORT, LED_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_3_FUNC);
    // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, false);
    // Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, true); 
    board.led_verde=DigitalOutputCreate(LED_3_GPIO, LED_3_BIT);

    /******************/
    Chip_SCU_PinMuxSet(TEC_1_PORT, TEC_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_1_FUNC);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_1_GPIO, TEC_1_BIT, false);
    board.TEC_1=DigitalInputCreate(TEC_1_GPIO, TEC_1_BIT, true);


    Chip_SCU_PinMuxSet(TEC_2_PORT, TEC_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_2_FUNC);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_2_GPIO, TEC_2_BIT, false);
    board.TEC_2=DigitalInputCreate(TEC_2_GPIO, TEC_2_BIT, true);

    Chip_SCU_PinMuxSet(TEC_3_PORT, TEC_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_3_FUNC);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_3_GPIO, TEC_3_BIT, false);
    board.TEC_3=DigitalInputCreate(TEC_3_GPIO, TEC_3_BIT, true);

    Chip_SCU_PinMuxSet(TEC_4_PORT, TEC_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_4_FUNC);
    //Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, TEC_4_GPIO, TEC_4_BIT, false);
    board.TEC_4=DigitalInputCreate(TEC_4_GPIO, TEC_4_BIT, true);

    return &board;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */