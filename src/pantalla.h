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
#ifndef PANTALLA_H
#define PANTALLA_H

/**
 ** \author Balthazar Martin
 ** \date 05/06/23
 ** \brief Declaraciones publicas del controlador de la pantalla
 **
 ** \addtogroup pantalla pantalla.h
 ** \brief Declaraciones publicas para crear HAL del controlador de la pantalla de 7 segmentos
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "poncho.h"

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

/* === Public data type declarations =========================================================== */

//! Estructura anonima con un puntero para almacenar datos del display con campos desconocidos
typedef struct display_s * display_t;

//! F.Callback del controlador de la pantalla para apagar a la misma
typedef void (*display_screen_off_t)(void);

//! F.Callback del controlador de la pantalla para encender los segmentos de la misma
typedef void (*display_segments_on_t)(uint8_t segments);

//! F.Callback del controlador de la pantalla para encender los digitos de la misma
typedef void (*display_digits_on_t)(uint8_t digits);

//! Estructura que solo almacena las funciones del controlador del display
typedef struct display_driver_s {
    display_screen_off_t DisplayTurnOff;
    display_segments_on_t SegmentsTurnOn;
    display_digits_on_t DigitsTurnOn;
} const * const display_driver_t;

/* === Public variable definitions ============================================================= */

/* === Public function declarations ============================================================ */

//! Funcion que recibe los campos del display para crear y devolver un puntero a la misma
display_t DisplayCreate(uint8_t digits, //!< Entero que indicara cuantos digitos dispondra el display al crearlo
                        display_driver_t driver /*!< Puntero a su controlador donde se le asignara las funciones*/);

//! Funcion que permira la conversion de decimal a binario para mostrar en el display los numeros que se soliciten
void DisplayWriteBCD(display_t display, //!< Puntero a la estructura del display
                     uint8_t * number,  //!< Entero que se expresara como un arreglo que indicara que numeros se desean
                                        //!< mostrar en cada digito del display
                     uint8_t size /*!< Entero que indica cuantos digitos se mostraran en el display*/);

//! Funcion que permitira refrescar el display
void DisplayRefresh(display_t display /*!< Puntero a la estructura del display*/);

//! Completar
void DisplayFlashDigits(display_t display, //!< Puntero a la estructura del display
                        uint8_t from, //!< Posicion del primer digito que se desea parpadear
                        uint8_t to, //!< Posicion del ultimo digito que se desea parpadear
                        uint16_t frequency /*!< Factor de division de frequencia de parpadeo*/);

//!
void DisplayToggleDot(display_t display, //!< Puntero a la estructura del display 
                      uint8_t position /*!< Posicion del punto que se desea parpadear*/);

/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

/** @} End of module definition for doxygen */

#endif /* PANTALLA_H */