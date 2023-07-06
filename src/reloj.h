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

#ifndef RELOJ_H
#define RELOJ_H

/**
 ** \author Balthazar Martin
 ** \date 14/06/23
 ** \brief Declaraciones publicas / Libreria del modulo reloj
 **
 ** \addtogroup reloj reloj.h
 ** \brief Declaraciones publicas para las funciones del reloj
 ** @{ */

/* === Headers files inclusions ================================================================ */

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

#include <stdbool.h>
#include <stdint.h>

//! Define el tamaño de los vectores hora y alarma
#define TIME_SIZE 6

#define CONTROLAR_REBALSE_MIN(DECENA, UNIDAD, SIGUIENTE)                                           \
    if (UNIDAD > 9) {                                                                              \
        DECENA = DECENA + UNIDAD / 9;                                                              \
        UNIDAD = 0 + ((UNIDAD / 9) * UNIDAD - 10);                                                 \
    }                                                                                              \
    if (DECENA > 5) {                                                                              \
        DECENA = DECENA - 6;                                                                       \
        SIGUIENTE += 1;                                                                            \
    }

#define CONTROLAR_REBALSE_HOR(DECENA, UNIDAD)                                                      \
    if (UNIDAD > 9) {                                                                              \
        UNIDAD = 0;                                                                                \
        DECENA += 1;                                                                               \
    }                                                                                              \
    if ((UNIDAD >= 4) & (DECENA >= 2)) {                                                           \
        UNIDAD = 0;                                                                                \
        DECENA = 0;                                                                                \
    }

/* === Public data type declarations =========================================================== */

//! Estructura anonima para almacenar datos y funciones del reloj con campos desconocidos
typedef struct clock_s * clock_t;

//! Funcion de Callback de la estructura reloj
typedef void (*clock_event_t)(clock_t reloj);

/* === Public variable declarations ============================================================ */

/* === Public function declarations ============================================================ */

/**
 ** @brief Funcion para crear e iniciar el reloj
 **
 ** @param tics_por_segundo Indica cuantos ticks(pulsos) se necesitaran para que el reloj cuente un
 *seg
 ** @param handler Puntero a la funcion de CallBack del reloj para activar la alarma
 **
 ** @return Devuelve un puntero al reloj creado
 **/
clock_t ClockCreate(int tics_por_segundo, clock_event_t handler);

/**
 ** @brief Funcion para consultar la hora actual del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 ** @param hora Arreglo que tomara la hora actual del reloj
 ** @param size Tamaño del arreglo de la hora
 **
 ** @return true La hora es valida para consultarla
 ** @return false La hora no fue configurada y no se la puede consultar
 **/
bool ClockGetTime(clock_t reloj, uint8_t * hora, int size);

/**
 ** @brief Funcion para configurar y fijar la hora actual del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 ** @param hora Arreglo de la hora con que se configurara la hora del reloj
 ** @param size Tamaño del arreglo de la hora
 **/
void ClockSetTime(clock_t reloj, const uint8_t * hora, int size);

/**
 ** @brief Funcion para consultar la alarma actual del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 ** @param hora Arreglo que tomara la alarma actual del reloj
 ** @param size Tamaño del arreglo de la hora
 **
 ** @return true La hora es valida para consultarla
 ** @return false La alarma no fue configurada y no se la puede consultar
 **/
bool ClockGetAlarm(clock_t reloj, uint8_t * hora, int size);

/**
 ** @brief Funcion para configurar y fijar la alarma actual del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 ** @param hora Arreglo de la hora con que se configurara la alarma del reloj
 ** @param size Tamaño del arreglo de la hora
 **/
void ClockSetAlarm(clock_t reloj, const uint8_t * hora, int size);

/**
 ** @brief Funcion para habilitar o deshabilitar la alarma del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 **/
bool ClockAlarmToggle(clock_t reloj);

void ClockAlarmActivate(clock_t reloj);

void ClockAlarmDeactivate(clock_t reloj);

/**
 ** @brief Funcion para desactivar la alarma una vez que suene cuando fue pospuesta
 **
 ** @param reloj Puntero al reloj que fue creado
 **/
void ClockStopAlarm(clock_t reloj);

/**
 ** @brief Funcion que pospondra la alarma del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 ** @param tiempo Entero que indica cuantos minutos se desean posponer
 **/
void ClockPostponeAlarm(clock_t reloj, uint8_t tiempo);

/**
 ** @brief Funcion que verificara por cada minuto que pase si la alarma sonara o no
 **
 ** @param reloj Puntero al reloj que fue creado
 **/
void CheckAlarmActive(clock_t reloj);

/**
 ** @brief Funcion para actualizar la cuenta interna del reloj
 **
 ** @param reloj Puntero al reloj que fue creado
 ** @return Devuelve el tick al que esta actualmente
 **/
int ClockTick(clock_t reloj);

/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

/** @} End of module definition for doxygen */

#endif /* RELOJ_H */