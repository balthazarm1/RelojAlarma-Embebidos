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

#ifndef DIGITAL_H
#define DIGITAL_H

/**
 ** \author Balthazar Martin
 ** \date ??/04/23
 ** \brief Declaraciones publicas del modulo digital
 **
 ** \addtogroup alumno digital.h
 ** \brief Modulo para
 ** @{ */

/* === Headers files inclusions ================================================================ */

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

#include <stdint.h>

/**
 * @brief Define el tamaño de los campos de texto
 *
 * Este parametro define el tamaño de los campos de texto de la
 * estructura \ref alumno_s "alumno_s"
 */
#define FIELD_SIZE 50


#ifndef NUMBER_OF_OUTPUTS
    /**
     * @brief Define el numero de salidas 
     *
     * Este parametro define el numero de salidas en caso que no se haya definido antes una
     * cantidad especifica \ref alumno_s "alumno_s"
     */
    #define NUMBER_OF_OUTPUTS 4
#endif
/* === Public data type declarations =========================================================== */

//! Estructura anonima para almacenar datos del alumno con campos desconocidos
typedef struct digital_output_s * digital_output_t;

/* === Public variable declarations ============================================================ */

/* === Public function declarations ============================================================ */

//! Casi equivalente a Chip_SCU_PinMuxSet
digital_output_t DigitalOutputCreate(uint8_t port, //!< Puntero que recibe el apellido del alumno
                                   uint8_t pin /*!< Entero que recibe el documento del alumno */);

//! Casi equivalente a Chip_GPIO_SetPinState
void DigitalOutputActivate (digital_output_t output);

//! Casi equivalente a Chip_GPIO_SetPinState
void DigitalOutputDeactivate (digital_output_t output);

//! Casi equivalente a Chip_GPIO_SetPinState
void DigitalOutputToggle (digital_output_t output);
    
/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

/** @} End of module definition for doxygen */

#endif /* ALUMNO_H */