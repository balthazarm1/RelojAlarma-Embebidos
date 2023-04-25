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
 ** \date ??/04/23
 ** \brief Implementacion del modulo digital
 **
 ** \addtogroup Alumno Alumnos.c
 ** \brief Modulo para gestion de alumnos
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "digital.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "chip.h"

/* === Macros definitions ====================================================================== */


/* === Private data type declarations ========================================================== */
//! Estructura SOLO para almacenar datos del alumno
struct digital_output_s {
    uint8_t port; //!< Almacena el apellido del alumno
    uint8_t pin;  //!< Booleano que indica si la estructura esta vacia o no
    bool ocupado;
};

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

//! Casi equivalente a Chip_SCU_PinMuxSet
digital_output_t DigitalOutputCreate(uint8_t port, uint8_t pin){
    digital_output_t output = NULL;
    static struct digital_output_s instancias[NUMBER_OF_OUTPUTS] = {0};
    for (int i = 0 ; i < NUMBER_OF_OUTPUTS ; i++){
        if(!instancias[i].ocupado){
            instancias[i].ocupado=true;
            output = &instancias[i];
            output->ocupado= true;
            output->port = port;
            output->pin = pin;
            Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
            Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, output->port, output->pin, true);
            break;
        }
    }
    return output;
}
;
//! Casi equivalente a Chip_GPIO_SetPinState
void DigitalOutputActivate (digital_output_t output){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, true);
}

//! Casi equivalente a Chip_GPIO_SetPinState
void DigitalOutputDeactivate (digital_output_t output){
    // eslint-disable-next-line
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
}

//! Casi equivalente a Chip_GPIO_SetPinState
void DigitalOutputToggle (digital_output_t output){
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, output->port, output->pin);
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
