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
 ** \addtogroup digital digital.h
 ** \brief Capa de abstracción de hardware para Entradas y Salidas Digitales
 ** @{ */

/* === Headers files inclusions ================================================================ */

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

#include <stdint.h>
#include <stdbool.h>

#ifndef NUMBER_OF_OUTPUTS
    /**
     * @brief Define el numero de salidas 
     *
     * Este parametro define el numero de salidas en caso que no se haya definido antes una
     * cantidad especifica \ref digital_output_s "digital_output_s"
     */
    #define NUMBER_OF_OUTPUTS 4
#endif

#ifndef NUMBER_OF_INPUTS
    /**
     * @brief Define el numero de entradas 
     *
     * Este parametro define el numero de entradas en caso que no se haya definido antes una
     * cantidad especifica \ref digital_input_s "digital_input_s"
     */
    #define NUMBER_OF_INPUTS 4
#endif
/* === Public data type declarations =========================================================== */

//! Estructura anonima para almacenar datos de las Salidas Digitales con campos desconocidos
typedef struct digital_output_s * digital_output_t;

//! Estructura anonima para almacenar datos de las Entradas Digitales con campos desconocidos
typedef struct digital_input_s * digital_input_t;

/* === Public variable declarations ============================================================ */

/* === Public function declarations ============================================================ */

//! Funcion para recibir datos de las Entradas Digitales y crearlas de forma Estatica
digital_input_t DigitalInputCreate(uint8_t port, //!< Numero de Puerto GPIO al que esta conectada la Entrada Digital
                                    uint8_t pin, //!< Numero de Pin GPIO al que esta conectada la Entrada Digital
                                    bool inverted /*!< Boolenao que indica si la entrada sera pullup o pulldown */);


//! Funcion para recibir datos de las Salidas Digitales y crearlas de forma Estatica
digital_output_t DigitalOutputCreate(uint8_t port, //!<  Numero de Puerto GPIO al que esta conectada la Salida Digital
                                     uint8_t pin /*!< Numero de Pin GPIO al que esta conectada la Salida Digital */);

/** 
 * @brief Funcion que chequea si la entrada esta activa o inactiva
 * 
 * @param input Puntero a la estructura de las Entradas Digitales
 * @return true La entrada esta activa
 * @return false La entrada esta inactiva
 */
bool DigitalInputGetState (digital_input_t input /*!< */ );

/** 
 * @brief Funcion que chequea si la entrada fue activada
 * 
 * @param input Puntero a la estructura de las Entradas Digitales
 * @return true La entrada esta activada
 * @return false La entrada no esta activada
 */
bool DigitalInputHasActivated(digital_input_t input /*!< */ );

// void DigitalInputHasDeactivated(digital_input_t input);

// void DigitalInputHasChanged(digital_input_t input);

//! Funcion que activa la Salida Digital
void DigitalOutputActivate (digital_output_t output /*!< */ );

//! Funcion que desactiva la Salida Digital
void DigitalOutputDeactivate (digital_output_t output /*!< */ );

//! Funcion que conmuta la Salida Digital
void DigitalOutputToggle (digital_output_t output /*!< */ );
    
/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

/** @} End of module definition for doxygen */

#endif /* ALUMNO_H */