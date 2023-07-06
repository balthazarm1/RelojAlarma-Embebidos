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
 ** \date 05/05/23
 ** \brief Implimentacion publica del main del Reloj
 **
 ** \addtogroup tp8 TP4-main.c
 ** \brief Sexto Trabajo Practico
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "bsp.h"
#include "reloj.h"
#include <stdbool.h>
#include <stdlib.h>

/* === Macros definitions ====================================================================== */

#ifndef TICKS_PER_SEC
#define TICKS_PER_SEC 1000
#endif

#ifndef FREQ_PARPADEO
#define FREQ_PARPADEO 200
#endif

#ifndef TIEMPO_POSPONER
#define TIEMPO_POSPONER 5
#endif

/* === Private data type declarations ========================================================== */

typedef enum {
    SIN_CONFIGURAR,
    MOSTRANDO_HORA,
    AJUSTANDO_MINUTOS_ACTUALES,
    AJUSTANDO_HORAS_ACTUALES,
    AJUSTANDO_MINUTOS_ALARMA,
    AJUSTANDO_HORAS_ALARMA,
} modo_t;

/* === Private variable declarations =========================================================== */

static const uint8_t LIMITE_MINUTOS[] = {5, 9};

static const uint8_t LIMITE_HORAS[] = {2, 3};

/* === Private function declarations =========================================================== */

void ActivarAlarma(clock_t reloj);

void CambiarModo(modo_t valor);

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]);

void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]);

/* === Public variable definitions ============================================================= */

static board_t board;

static clock_t reloj;

static modo_t modo;

static bool alarma_zumba;

static int contador_timeout = 0;

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

void ActivarAlarma(clock_t reloj){
    DigitalOutputActivate(board->buzzer);
    alarma_zumba = true;
}

void CambiarModo(modo_t valor){
    modo = valor;
    switch (modo) {
    case SIN_CONFIGURAR:
        DisplayFlashDigits(board->display, 0, 3, FREQ_PARPADEO);
        DisplayActivateDot(board->display, 1);
        DisplayDeactivateDot(board->display, 0);
        DisplayDeactivateDot(board->display, 2);
        DisplayDeactivateDot(board->display, 3);
        break;
    case MOSTRANDO_HORA:
        DisplayFlashDigits(board->display, 0, 0, 0);
        DisplayDeactivateDot(board->display, 0);
        DisplayDeactivateDot(board->display, 1);
        DisplayDeactivateDot(board->display, 2);
        DisplayDeactivateDot(board->display, 3);
        break;
    case AJUSTANDO_MINUTOS_ACTUALES:
        DisplayFlashDigits(board->display, 2, 3, FREQ_PARPADEO);
        DisplayDeactivateDot(board->display, 0);
        DisplayDeactivateDot(board->display, 1);
        DisplayDeactivateDot(board->display, 2);
        DisplayDeactivateDot(board->display, 3);
        break;
        case AJUSTANDO_HORAS_ACTUALES:
        DisplayFlashDigits(board->display, 0, 1, FREQ_PARPADEO);
        DisplayDeactivateDot(board->display, 0);
        DisplayDeactivateDot(board->display, 1);
        DisplayDeactivateDot(board->display, 2);
        DisplayDeactivateDot(board->display, 3);
        break;
    case AJUSTANDO_MINUTOS_ALARMA:
        DisplayFlashDigits(board->display, 2, 3, FREQ_PARPADEO);
        DisplayActivateDot(board->display, 0);
        DisplayActivateDot(board->display, 1);
        DisplayActivateDot(board->display, 2);
        DisplayActivateDot(board->display, 3);
        break;
    case AJUSTANDO_HORAS_ALARMA:
        DisplayFlashDigits(board->display, 0, 1, FREQ_PARPADEO);
        DisplayActivateDot(board->display, 0);
        DisplayActivateDot(board->display, 1);
        DisplayActivateDot(board->display, 2);
        DisplayActivateDot(board->display, 3);
        break;
    default:
        break;
    }
}

void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    numero[1]++;
    if ((numero[0] >= limite[0]) && numero[1] > limite[1]) {
        numero[1] = 0;
        numero[0] = 0;
    }
    if (numero[1] > 9) {
        numero[1] = 0;
        numero[0]++;
    }
}

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    numero[1]--;
    if (numero[1] > 9) {
        numero[1] = 9; 
        numero[0]--;
    }
    if ((numero[0] >= limite[0]) && numero[1] >= limite[1]) { 
        numero[0] = limite[0]; 
        numero[1] = limite[1]; 
    }
}

bool CountDown(int segundos, bool estado, int numero_entradas, const digital_input_t input[]) {
    contador_timeout = segundos;
    bool condicion = estado;

    if (condicion) {
        while (contador_timeout > 0 && condicion) {
            for (int index = 0; index < numero_entradas; index++) {
                condicion = DigitalInputGetState(input[index]);
                if (!condicion) {
                    return false;
                }
            }
        }
    } else {
        while (contador_timeout > 0 && !condicion) {
            for (int index = 0; index < numero_entradas; index++) {
                condicion = DigitalInputGetState(input[index]);
                if (condicion) {
                    return false;
                }
            }
        }
    }
    return true;
}

/* === Public function implementation ========================================================= */

int main(void) {
    uint8_t hora_entrada[4];
                        //recordar dejorlo en 1000
    reloj = ClockCreate(1000, ActivarAlarma); //ver esta funcion activar alarma con los test
    board = BoardCreate();

    SysTick_Init(TICKS_PER_SEC);
    CambiarModo(SIN_CONFIGURAR);

    while (true) {        
        if (DigitalInputHasActivated(board->aceptar)) {
            if (!alarma_zumba){
                if ( modo == MOSTRANDO_HORA){
                    if (!ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))){
                        ClockAlarmActivate(reloj);
                        DisplayActivateDot(board->display, 3);
                    }
                }else if ( modo == AJUSTANDO_MINUTOS_ACTUALES){
                    CambiarModo(AJUSTANDO_HORAS_ACTUALES);
                }else if ( modo == AJUSTANDO_HORAS_ACTUALES){
                    ClockSetTime(reloj, hora_entrada, sizeof(hora_entrada));
                    CambiarModo(MOSTRANDO_HORA);
                    if (ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))){
                        DisplayActivateDot(board->display, 3);
                    }
                }else if ( modo == AJUSTANDO_MINUTOS_ALARMA){
                    CambiarModo(AJUSTANDO_HORAS_ALARMA);
                }else if ( modo == AJUSTANDO_HORAS_ALARMA){
                    ClockSetAlarm(reloj, hora_entrada, sizeof(hora_entrada));
                    CambiarModo(MOSTRANDO_HORA);
                    ClockAlarmActivate(reloj);
                    DisplayActivateDot(board->display, 3);
                }
            }else {
                DigitalOutputDeactivate(board->buzzer);
                ClockPostponeAlarm(reloj, TIEMPO_POSPONER);
                alarma_zumba = false;
            }
        }

        if (DigitalInputHasActivated(board->cancelar)) {
            if ( !alarma_zumba){
                if ( modo == MOSTRANDO_HORA){
                    if (ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))){
                        ClockStopAlarm(reloj);
                        DisplayDeactivateDot(board->display, 3);
                    }
                }else {
                    if ( ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada))) {
                        CambiarModo(MOSTRANDO_HORA);
                    }else{
                        CambiarModo(SIN_CONFIGURAR);
                    }
                }
            }else {
                ClockStopAlarm(reloj);
                DigitalOutputDeactivate(board->buzzer);
                alarma_zumba = false;
            };
        }

        if (CountDown(3, true, 1, &(board->set_hora)) ) {
            CambiarModo(AJUSTANDO_MINUTOS_ACTUALES);
            ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada));
            DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada)); 
        }

        if (CountDown(3, true, 1, &(board->set_alarma)) && ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada))){
            CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
            ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada));
            DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
        }

        if (DigitalInputHasActivated(board->incrementar)) {
            if ( (modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_MINUTOS_ALARMA) ){
                IncrementarBCD(&hora_entrada[2], LIMITE_MINUTOS);
            } else if ( (modo == AJUSTANDO_HORAS_ACTUALES) || (modo == AJUSTANDO_HORAS_ALARMA)){
                IncrementarBCD(hora_entrada, LIMITE_HORAS);
            }

            if( (modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_HORAS_ACTUALES)){
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada)); 
            } else if ( (modo == AJUSTANDO_HORAS_ALARMA) || (modo == AJUSTANDO_MINUTOS_ALARMA)){
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            }
        }

        if (DigitalInputHasActivated(board->decrementar)) {
            if ( (modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_MINUTOS_ALARMA) ){
                DecrementarBCD(&hora_entrada[2], LIMITE_MINUTOS);
            } else if ( (modo == AJUSTANDO_HORAS_ACTUALES) || (modo == AJUSTANDO_HORAS_ALARMA)){
                DecrementarBCD(hora_entrada, LIMITE_HORAS);
            }

            if( (modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_HORAS_ACTUALES)){
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada)); 
            } else if ( (modo == AJUSTANDO_HORAS_ALARMA) || (modo == AJUSTANDO_MINUTOS_ALARMA)){
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            }
        }

        if (CountDown(30, false, 6, (digital_input_t[]){board->aceptar, board->cancelar,
                                                       board->set_hora, board->set_alarma,
                                                       board->incrementar, board->decrementar})) {
            if (modo > MOSTRANDO_HORA) {
                if (ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada))) {
                    CambiarModo(MOSTRANDO_HORA);
                } else {
                    CambiarModo(SIN_CONFIGURAR);
                }
            }
        }

        for (int index = 0; index < 20; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }
    }
}

void SysTick_Handler(void) {
    uint8_t hora[6];
    int ticks;

    DisplayRefresh(board->display);
    ticks = ClockTick(reloj);

    if (ticks == TICKS_PER_SEC/2 || ticks == 0) {
        if ( modo <= MOSTRANDO_HORA){
            ClockGetTime(reloj, hora, sizeof(hora));
            DisplayWriteBCD(board->display, hora, sizeof(hora));
            if (modo == MOSTRANDO_HORA){
                DisplayToggleDot(board->display, 1);
            }
        }
        if ((ticks == 0) && contador_timeout) {
            contador_timeout--;
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
