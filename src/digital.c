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
 ** \brief Implementacion del modulo digital
 **
 ** \addtogroup Digital Digital.c
 ** \brief Capa de abstracción de hardware para Entradas y Salidas Digitales
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "digital.h"
#include "chip.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

//! Estructura que solo almacena informacion sobre las Salidas Digitales
struct digital_output_s {
    uint8_t port; //!< Numero de Puerto GPIO al que esta conectada la Salida Digital
    uint8_t pin;  //!< Numero de Pin GPIO al que esta conectada la Salida Digital
    bool ocupado; //!< Booleano que indica si la estructura esta vacia o no
};

//! Estructura que solo almacena informacion sobre las Entradas Digitales
struct digital_input_s {
    uint8_t port;    //!< Numero de Puerto GPIO al que esta conectada la Entrada Digital
    uint8_t pin;     //!< Numero de Pin GPIO al que esta conectada la Entrada Digital
    bool ocupado;    //!< Booleano que indica si la estructura esta vacia o no
    bool inverted;   //!< Boolenao que indica si la entrada sera pullup o pulldown
    bool last_state; //!< Booleano que almacena el ultimo estado en el que se encontro la entrada
};

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

digital_output_t DigitalOutputCreate(uint8_t port, uint8_t pin) {
    digital_output_t output = NULL;
    static struct digital_output_s instancias[NUMBER_OF_OUTPUTS] = {0};
    for (int i = 0; i < NUMBER_OF_OUTPUTS; i++) {
        if (!instancias[i].ocupado) {
            instancias[i].ocupado = true;
            output = &instancias[i];
            output->ocupado = true;
            output->port = port;
            output->pin = pin;
            Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
            Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, output->port, output->pin, true);
            break;
        }
    }
    return output;
}

digital_input_t DigitalInputCreate(uint8_t port, uint8_t pin, bool inverted) {
    digital_input_t input = NULL;
    static struct digital_input_s instancias[NUMBER_OF_INPUTS] = {0};
    for (int i = 0; i < NUMBER_OF_INPUTS; i++) {
        if (!instancias[i].ocupado) {
            instancias[i].ocupado = true;
            input = &instancias[i];
            input->ocupado = true;
            input->inverted = inverted;
            input->port = port;
            input->pin = pin;
            Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, input->port, input->pin, false);
            break;
        }
    }
    return input;
}

//                  ENTRADAS

bool DigitalInputGetState(digital_input_t input) {
    return (input->inverted ^ Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->port, input->pin));
}

// void DigitalInputHasChanged(digital_input_t input){
//     bool current_state = DigitalInputPressed(input);
//     bool state = ( current_state != input->last_state );
//     input->last_state = current_state;
//     return state;
// }

bool DigitalInputHasActivated(digital_input_t input) {
    bool current_state = DigitalInputGetState(input);
    bool state = (current_state && !input->last_state);
    input->last_state = current_state;
    return state;
}

// void DigitalInputHasDeactivated(digital_input_t input){
//     bool current_state = DigitalInputPressed(input);
//     bool state = ( !current_state &&  !input->last_state );
//     input->last_state = current_state;
//     return state;
// }

//                  SALIDAS

void DigitalOutputActivate(digital_output_t output) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, true);
}

void DigitalOutputDeactivate(digital_output_t output) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
}

void DigitalOutputToggle(digital_output_t output) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, output->port, output->pin);
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
