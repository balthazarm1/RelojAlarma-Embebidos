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
#ifndef BSP_H
#define BSP_H

/**
 ** \author Balthazar Martin
 ** \date 05/06/23
 ** \brief Declaraciones publicas del modulo bsp
 **
 ** \addtogroup bsp bsp.h
 ** \brief Declaraciones publicas para implementar Entradas y Salidas Digitales
 ** @{ */

/* === Headers files inclusions =============================================================== */

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

#include "digital.h"
#include "pantalla.h"

/* === Public data type declarations =========================================================== */

//! Estructura publica para definir las Entradas y Salidas con las que trabajara nuestra placa
typedef struct board_s {
    digital_output_t led_1;
    digital_output_t led_2;
    digital_output_t led_3;
    digital_output_t buzzer;
    digital_input_t set_hora;
    digital_input_t set_alarma;
    digital_input_t incrementar;
    digital_input_t decrementar;
    digital_input_t aceptar;
    digital_input_t cancelar;
    display_t display;
} const * board_t;

/* === Public variable definitions ============================================================= */

/* === Public function declarations ============================================================ */

//! Funcion que implementara las Entradas y Salidas de la placa
board_t BoardCreate(void);

void SysTick_Init(uint16_t ticks);

/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

/** @} End of module definition for doxygen */

#endif /* BSP_H */