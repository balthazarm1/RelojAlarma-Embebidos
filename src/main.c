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

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

void ActivarAlarma(clock_t reloj){
    // DigitalOutputActivate(board->buzzer);
    // alarma_sonando = true; -> ver
}

void CambiarModo(modo_t valor){
    modo = valor;
    switch (modo) {
    case SIN_CONFIGURAR:
        DisplayFlashDigits(board->display, 0, 3, 200);
        break;
    case MOSTRANDO_HORA:
        DisplayFlashDigits(board->display, 0, 0, 0);
        break;
    case AJUSTANDO_MINUTOS_ACTUALES:
        DisplayFlashDigits(board->display, 2, 3, 200);
        break;
        case AJUSTANDO_HORAS_ACTUALES:
        DisplayFlashDigits(board->display, 0, 1, 200);
        break;
    case AJUSTANDO_MINUTOS_ALARMA:
        DisplayFlashDigits(board->display, 2, 3, 200);
        DisplayToggleDot(board->display, 0);
        DisplayToggleDot(board->display, 1);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);
        break;
    case AJUSTANDO_HORAS_ALARMA:
        DisplayFlashDigits(board->display, 0, 1, 200);
        DisplayToggleDot(board->display, 0);
        DisplayToggleDot(board->display, 1);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);
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

/* === Public function implementation ========================================================= */

int main(void) {
    uint8_t hora_entrada[4];
                        //recordar dejorlo en 1000
    reloj = ClockCreate(1000, ActivarAlarma); //ver esta funcion activar alarma con los test
    board = BoardCreate();

    SysTick_Init(1000);
    CambiarModo(SIN_CONFIGURAR);

    while (true) {        
        if (DigitalInputHasActivated(board->aceptar)) {
            if ( modo == MOSTRANDO_HORA){
                if ( !ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))){
                    ClockAlarmToggle(reloj);//falta un toggle dot?
                }
            }else if ( modo == AJUSTANDO_MINUTOS_ACTUALES){
                CambiarModo(AJUSTANDO_HORAS_ACTUALES);
            }else if ( modo == AJUSTANDO_HORAS_ACTUALES){
                ClockSetTime(reloj, hora_entrada, sizeof(hora_entrada));
                CambiarModo(MOSTRANDO_HORA);
            }else if ( modo == AJUSTANDO_MINUTOS_ALARMA){
                CambiarModo(AJUSTANDO_HORAS_ALARMA);
            }else if ( modo == AJUSTANDO_HORAS_ALARMA){
                ClockSetAlarm(reloj, hora_entrada, sizeof(hora_entrada));
                CambiarModo(MOSTRANDO_HORA);
            }
        }

        if (DigitalInputHasActivated(board->cancelar)) {
            if ( modo == MOSTRANDO_HORA){
                if (ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada))){
                    ClockAlarmToggle(reloj);
                }
            }else {
                if ( ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada))) {
                    CambiarModo(MOSTRANDO_HORA);
                }else{
                    CambiarModo(SIN_CONFIGURAR);
                }
            }
        }

        if (DigitalInputHasActivated(board->set_alarma)) {
            CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
            ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada));
            DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            DisplayToggleDot(board->display, 0);
            DisplayToggleDot(board->display, 1);
            DisplayToggleDot(board->display, 2);
            DisplayToggleDot(board->display, 3);
        }

        if (DigitalInputHasActivated(board->set_hora)) {
            CambiarModo(AJUSTANDO_MINUTOS_ACTUALES);
            ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada));
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
                DisplayToggleDot(board->display, 0);
                DisplayToggleDot(board->display, 1);
                DisplayToggleDot(board->display, 2);
                DisplayToggleDot(board->display, 3);
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
                DisplayToggleDot(board->display, 0);
                DisplayToggleDot(board->display, 1);
                DisplayToggleDot(board->display, 2);
                DisplayToggleDot(board->display, 3);
            }
        }

        //DisplayRefresh(board->display);

        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }

/*         ClockGetTime(reloj, hora, sizeof(hora));
        __asm volatile("cpsid i");
        DisplayWriteBCD(board->display, hora, sizeof(hora));
        __asm volatile("cpsie i"); */
    }
}

void SysTick_Handler(void) {
    static uint16_t contador = 0;
    uint8_t hora[4];

    DisplayRefresh(board->display);
    ClockTick(reloj);

    contador = (contador + 1) % 1000;
    if ( modo <= MOSTRANDO_HORA){
        ClockGetTime(reloj, hora, sizeof(hora));
        DisplayWriteBCD(board->display, hora, sizeof(hora));
        if ( contador > 500 ){
            DisplayToggleDot(board->display, 1);
        }
        if ( ClockGetAlarm(reloj, hora, sizeof(hora))){
            DisplayToggleDot(board->display, 0);
            DisplayToggleDot(board->display, 1);
            DisplayToggleDot(board->display, 2);
            DisplayToggleDot(board->display, 3);
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
