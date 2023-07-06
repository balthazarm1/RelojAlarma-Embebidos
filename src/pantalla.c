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
** \date 05/06/23
** \brief Implimentacion del modulo pantalla
**
** \addtogroup Pantalla Pantalla.c
** \brief HAL del controlador de la pantalla de 7 segmentos
** @{ */

/* === Headers files inclusions =============================================================== */

#include "pantalla.h"
#include "bsp.h"
#include "digital.h"
#include "poncho.h"
#include <stdbool.h>
#include <string.h>

/* === Macros definitions ====================================================================== */

#ifndef DISPLAY_MAX_SEGMENTS
/**
 * @brief Define el numero maximo de segmentos del display
 *
 * Este parametro define el numero de maximo de segmentos en caso que no se haya definido antes una
 * cantidad especifica \ref display_s "display_s"
 */
#define DISPLAY_MAX_SEGMENTS 8
#endif

/**
 * @brief Define el numero maximo de segmentos del display
 *
 * Este parametro define el numero de displays en caso que no se haya definido antes una
 * cantidad especifica \ref display_s "display_s"
 */
#define NUMBER_OF_DISPLAYS 1

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

//! Estructura que solo almacena informacion sobre los displays
struct display_s {
    uint8_t digits;                       //! Entero que representa cuantos digitos tendra el display al crearlo
    uint8_t digits_active;                //! Entero que representa cuantos digitos estara activos en el display
    uint8_t flashing_from;                //! Entero que guarda cual sera el primer digito que se desea parpadear
    uint8_t flashing_to;                  //! Entero que guarda cual sera el ultimo digito que se desea parpadear
    uint16_t flashing_count;              //! Entero que guarda cuanto tiempo se lleva parpadeando
    uint16_t flashing_factor;             //! Entero que guarda cuantos ciclos se debera hacer el parpadeo
    uint8_t memory[DISPLAY_MAX_SEGMENTS]; //! Entero que almacenara los numeros que se querran mostrar en el display
    struct display_driver_s driver[1];    //! Estructura que solo almacena las funciones de callback del display
};

//! Entero que utilizada las mascaras de los segmentos para mostrar los numeros que se soliciten en el display
static const uint8_t IMAGES[] = {
    SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_E_MASK | SEGMENT_F_MASK, //!< 0
    SEGMENT_B_MASK | SEGMENT_C_MASK,                                                                     //!< 1
    SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_D_MASK | SEGMENT_E_MASK | SEGMENT_G_MASK,                  //!< 2
    SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_G_MASK,                  //!< 3
    SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_F_MASK | SEGMENT_G_MASK,                                   //!< 4
    SEGMENT_A_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_F_MASK | SEGMENT_G_MASK,                  //!< 5
    SEGMENT_A_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_E_MASK | SEGMENT_F_MASK | SEGMENT_G_MASK, //!< 6
    SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK,                                                    //!< 7
    SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_E_MASK | SEGMENT_F_MASK |
        SEGMENT_G_MASK,                                                                                 //!< 8
    SEGMENT_A_MASK | SEGMENT_B_MASK | SEGMENT_C_MASK | SEGMENT_D_MASK | SEGMENT_F_MASK | SEGMENT_G_MASK /*!< 9 */
};

/* === Private function declarations =========================================================== */

//! Puntero a la estructura display_s que se encarga de alocar los display de forma estatica
static display_t DisplayAllocate(void);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static display_t DisplayAllocate(void) {
    static struct display_s instances[NUMBER_OF_DISPLAYS] = {0};
    return &instances[0];
}

/* === Public function implementation ========================================================= */

display_t DisplayCreate(uint8_t digits, display_driver_t driver) {
    display_t display;
    display = DisplayAllocate();

    if (display) {
        display->digits = digits;
        display->digits_active = digits - 1;
        display->flashing_from = 0;
        display->flashing_to = 0;
        display->flashing_count = 0;
        display->flashing_factor = 0;
        memcpy(display->driver, driver, sizeof(display->driver));
        memset(display->memory, 0, sizeof(display->memory));
        display->driver->DisplayTurnOff();
    }

    return display;
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size) {
    for (int index = 0; index < size; index++) {
        if (index >= display->digits)
            break;
        display->memory[index] &= SEGMENT_P;
        display->memory[index] |= IMAGES[number[index]];
    }
}

void DisplayRefresh(display_t display) {
    uint8_t segments;

    display->driver->DisplayTurnOff();
    display->digits_active = (display->digits_active + 1) % display->digits;

    segments = display->memory[display->digits_active];
    if (display->flashing_factor) {
        if (display->digits_active == 0){   //si comienza un nuevo barrido
            display->flashing_count = (display->flashing_count + 1) % display->flashing_factor;
        }
        if ((display->digits_active >= display->flashing_from) && (display->digits_active <= display->flashing_to)){
            if(display->flashing_count > (display->flashing_factor / 2)){
                segments = 0;
            }
        }
    }

    display->driver->SegmentsTurnOn(segments);
    display->driver->DigitsTurnOn(display->digits_active);
}

void    DisplayFlashDigits(display_t display, uint8_t from, uint8_t to, uint16_t frequency){
    display->flashing_count = 0;
    display->flashing_factor = frequency;
    display->flashing_from = from;
    display->flashing_to = to;
}

bool DisplayToggleDot(display_t display, uint8_t position){
    display->memory[position] ^= SEGMENT_P; 
    return (display->memory[position] & SEGMENT_P);
}

void DisplayActivateDot(display_t display, uint8_t position) {
    if (!DisplayToggleDot(display, position)) {
        DisplayToggleDot(display, position);
    }
}

void DisplayDeactivateDot(display_t display, uint8_t position) {
    if (DisplayToggleDot(display, position)) {
        DisplayToggleDot(display, position);
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */