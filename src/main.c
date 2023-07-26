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
 ** \date 26/07/23
 ** \brief Implimentacion publica del main del Reloj
 **
 ** \addtogroup tp9 TP9-main.c
 ** \brief Noveno Trabajo Practico
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "FreeRTOS.h"
#include "bsp.h"
#include "event_groups.h"
#include "reloj.h"
#include "task.h"
#include <stdbool.h>
#include <stdlib.h>

/* === Macros definitions ====================================================================== */

#ifndef TICKS_PER_SEC
#define TICKS_PER_SEC 1000
#endif

#ifndef FREQ_PARPADEO
#define FREQ_PARPADEO 250
#endif

#ifndef TIEMPO_POSPONER
#define TIEMPO_POSPONER 5
#endif

#define EVENT_SETHORA_ON (1 << 0)
#define EVENT_SETALARM_ON (1 << 1)
#define EVENT_DEC_ON (1 << 2)
#define EVENT_INC_ON (1 << 3)
#define EVENT_CAN_ON (1 << 4)
#define EVENT_ACP_ON (1 << 5)

#define EVENT_SETHORA_OFF (1 << 6)
#define EVENT_SETALARM_OFF (1 << 7)
#define EVENT_DEC_OFF (1 << 8)
#define EVENT_INC_OFF (1 << 9)
#define EVENT_CAN_OFF (1 << 10)
#define EVENT_ACP_OFF (1 << 11)

#define EVENT_TIMEOUT_FINISHED (1 << 12)

/* === Private data type declarations ========================================================== */

typedef enum {
    SIN_CONFIGURAR,
    MOSTRANDO_HORA,
    AJUSTANDO_MINUTOS_ACTUALES,
    AJUSTANDO_HORAS_ACTUALES,
    AJUSTANDO_MINUTOS_ALARMA,
    AJUSTANDO_HORAS_ALARMA,
} modo_t;

typedef struct estados_s {
    uint32_t key;
    modo_t modo;
} * estados_s;

/* === Private variable declarations =========================================================== */

static const uint8_t LIMITE_MINUTOS[] = {5, 9};

static const uint8_t LIMITE_HORAS[] = {2, 3};

/* === Private function declarations =========================================================== */

void ActivarAlarma(clock_t reloj);

void CambiarModo(modo_t valor);

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]);

void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]);

void StopByError(board_t board, uint8_t code);

static void ModeTask(void * object);

static void ConfigTask(void * object);

static void KeyTask(void * object);

static void TimeOutTask(void * object);

/* === Public variable definitions ============================================================= */

static board_t board;

static clock_t reloj;

static modo_t modo;

static bool alarma_zumba;

static int contador_timeout = 0;

static uint8_t hora_entrada[4];

static bool timeout_reset = false;

/* === Private variable definitions ============================================================ */

EventGroupHandle_t key_events;

/* === Private function implementation ========================================================= */

void ActivarAlarma(clock_t reloj) {
    DigitalOutputActivate(board->buzzer);
    alarma_zumba = true;
}

void CambiarModo(modo_t valor) {
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

void StopByError(board_t board, uint8_t code) {
    switch (code) {
    case 1:
        DigitalOutputActivate(board->led_1);
        break;
    case 2:
        DigitalOutputActivate(board->led_2);
        break;
    case 3:
        DigitalOutputActivate(board->led_3);
        break;
    case 4:
        DigitalOutputActivate(board->led_1);
        DigitalOutputActivate(board->led_2);
        break;
    case 5:
        DigitalOutputActivate(board->led_1);
        DigitalOutputActivate(board->led_3);
        break;
    case 6:
        DigitalOutputActivate(board->led_2);
        DigitalOutputActivate(board->led_3);
        break;
    case 7:
        DigitalOutputActivate(board->led_1);
        DigitalOutputActivate(board->led_2);
        DigitalOutputActivate(board->led_3);
        break;
    default:
        break;
    }
    while (true) {
    };
}

static void TickTask(void * object) {
    uint8_t hora[6];
    TickType_t last_value = xTaskGetTickCount();
    int ticks;

    while (true) {
        ticks = ClockTick(reloj);
        if (ticks == TICKS_PER_SEC / 2 || ticks == 0) {
            if (modo <= MOSTRANDO_HORA) {
                ClockGetTime(reloj, hora, sizeof(hora));
                DisplayWriteBCD(board->display, hora, sizeof(hora));
                if (modo == MOSTRANDO_HORA) {
                    DisplayToggleDot(board->display, 1);
                }
            }
            if ((ticks == 0) && contador_timeout) {
                contador_timeout--;
            }
        }
        vTaskDelayUntil(&last_value, pdMS_TO_TICKS(1));
    }
}

static void RefreshTask(void * object) {
    while (true) {
        DisplayRefresh(board->display);
        vTaskDelay(pdMS_TO_TICKS(1));
    };
}

static void ModeTask(void * object) {
    estados_s options = object;

    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(3000));
        timeout_reset = true;
        if (DigitalInputGetState(board->set_hora)) {
            CambiarModo(AJUSTANDO_MINUTOS_ACTUALES);
            ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada));
            DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
        }
        if (DigitalInputGetState(board->set_alarma)) {
            CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
            ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada));
            DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
        }
    };
}

static void KeyTask(void * object) {
    board_t board = object;
    uint32_t last_state, current_state, changes, events;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(150));

        current_state = 0;
        if (DigitalInputGetState(board->set_hora)) {
            current_state |= EVENT_SETHORA_ON;
        }
        if (DigitalInputGetState(board->set_alarma)) {
            current_state |= EVENT_SETALARM_ON;
        }
        if (DigitalInputGetState(board->decrementar)) {
            current_state |= EVENT_DEC_ON;
        }
        if (DigitalInputGetState(board->incrementar)) {
            current_state |= EVENT_INC_ON;
        }
        if (DigitalInputGetState(board->cancelar)) {
            current_state |= EVENT_CAN_ON;
        }
        if (DigitalInputGetState(board->aceptar)) {
            current_state |= EVENT_ACP_ON;
        }

        changes = current_state ^ last_state;
        last_state = current_state;
        events = (((changes << 6) & (!current_state << 6)) | (changes & current_state));

        xEventGroupSetBits(key_events, events);
    }
}

static void ConfigTask(void * object) {
    estados_s options = object;

    while (true) {
        xEventGroupWaitBits(key_events, options->key, TRUE, FALSE, portMAX_DELAY);
        timeout_reset = true;
        if (DigitalInputGetState(board->aceptar)) {
            if (!alarma_zumba) {
                if (modo == MOSTRANDO_HORA) {
                    if (!ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))) {
                        ClockAlarmActivate(reloj);
                        DisplayActivateDot(board->display, 3);
                    }
                } else if (modo == AJUSTANDO_MINUTOS_ACTUALES) {
                    CambiarModo(AJUSTANDO_HORAS_ACTUALES);
                } else if (modo == AJUSTANDO_HORAS_ACTUALES) {
                    ClockSetTime(reloj, hora_entrada, sizeof(hora_entrada));
                    CambiarModo(MOSTRANDO_HORA);
                    if (ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))) {
                        DisplayActivateDot(board->display, 3);
                    }
                } else if (modo == AJUSTANDO_MINUTOS_ALARMA) {
                    CambiarModo(AJUSTANDO_HORAS_ALARMA);
                } else if (modo == AJUSTANDO_HORAS_ALARMA) {
                    ClockSetAlarm(reloj, hora_entrada, sizeof(hora_entrada));
                    CambiarModo(MOSTRANDO_HORA);
                    ClockAlarmActivate(reloj);
                    DisplayActivateDot(board->display, 3);
                }
            } else {
                DigitalOutputDeactivate(board->buzzer);
                ClockPostponeAlarm(reloj, TIEMPO_POSPONER);
                alarma_zumba = false;
            }
        }

        if (DigitalInputGetState(board->incrementar)) {
            if ((modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_MINUTOS_ALARMA)) {
                IncrementarBCD(&hora_entrada[2], LIMITE_MINUTOS);
            } else if ((modo == AJUSTANDO_HORAS_ACTUALES) || (modo == AJUSTANDO_HORAS_ALARMA)) {
                IncrementarBCD(hora_entrada, LIMITE_HORAS);
            }

            if ((modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_HORAS_ACTUALES)) {
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            } else if ((modo == AJUSTANDO_HORAS_ALARMA) || (modo == AJUSTANDO_MINUTOS_ALARMA)) {
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            }
        }

        if (DigitalInputGetState(board->decrementar)) {
            if ((modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_MINUTOS_ALARMA)) {
                DecrementarBCD(&hora_entrada[2], LIMITE_MINUTOS);
            } else if ((modo == AJUSTANDO_HORAS_ACTUALES) || (modo == AJUSTANDO_HORAS_ALARMA)) {
                DecrementarBCD(hora_entrada, LIMITE_HORAS);
            }

            if ((modo == AJUSTANDO_MINUTOS_ACTUALES) || (modo == AJUSTANDO_HORAS_ACTUALES)) {
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            } else if ((modo == AJUSTANDO_HORAS_ALARMA) || (modo == AJUSTANDO_MINUTOS_ALARMA)) {
                DisplayWriteBCD(board->display, hora_entrada, sizeof(hora_entrada));
            }
        }
    }
}

static void TimeOutTask(void * object) {
    EventBits_t current_events;
    uint32_t counter;

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        current_events = xEventGroupGetBits(key_events);
        if ((modo <= MOSTRANDO_HORA) || timeout_reset) {
            counter = 0;
            timeout_reset = false;
        } else {
            counter++;
        }
        if (counter == 30) {
            xEventGroupSetBits(key_events, (EVENT_TIMEOUT_FINISHED | current_events));
        }
    }
}

static void CancelKeyTask(void * object) {
    while (true) {
        xEventGroupWaitBits(key_events, EVENT_CAN_ON | EVENT_TIMEOUT_FINISHED, TRUE, FALSE, portMAX_DELAY);
        if (!alarma_zumba) {
            if (modo == MOSTRANDO_HORA) {
                if (ClockGetAlarm(reloj, hora_entrada, sizeof(hora_entrada))) {
                    ClockStopAlarm(reloj);
                    DisplayDeactivateDot(board->display, 3);
                }
            } else {
                if (ClockGetTime(reloj, hora_entrada, sizeof(hora_entrada))) {
                    CambiarModo(MOSTRANDO_HORA);
                } else {
                    CambiarModo(SIN_CONFIGURAR);
                }
            }
        } else {
            ClockStopAlarm(reloj);
            DigitalOutputDeactivate(board->buzzer);
            alarma_zumba = false;
        }
    }
}

/* === Public function implementation ========================================================= */

int main(void) {
    static struct estados_s estados[3];

    reloj = ClockCreate(1000, ActivarAlarma);
    board = BoardCreate();

    SysTick_Init(TICKS_PER_SEC);
    CambiarModo(SIN_CONFIGURAR);

    /* Configuracion de las tareas*/
    estados[0].key = EVENT_SETHORA_ON;
    estados[0].modo = AJUSTANDO_MINUTOS_ACTUALES;
    estados[1].key = EVENT_SETALARM_ON;
    estados[1].modo = AJUSTANDO_MINUTOS_ALARMA;
    estados[2].key = EVENT_DEC_ON | EVENT_INC_ON | EVENT_CAN_ON | EVENT_ACP_ON;
    estados[2].modo = MOSTRANDO_HORA;

    key_events = xEventGroupCreate();
    if (key_events == NULL) {
        StopByError(board, 1);
    }

    /* Tareas */

    if (xTaskCreate(TickTask, "ClockTick", 256, NULL, tskIDLE_PRIORITY + 3, NULL) != pdPASS) {
        StopByError(board, 6);
    }

    if (xTaskCreate(RefreshTask, "RefrescarPantalla", 256, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
        StopByError(board, 2);
    }

    if (xTaskCreate(KeyTask, "Teclas", 256, (void *)board, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(board, 3);
    }

    if (xTaskCreate(ModeTask, "CambiarHora", 256, &estados[0], tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(board, 4);
    }

    if (xTaskCreate(ModeTask, "CambiarAlarma", 256, &estados[1], tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(board, 4);
    }

    if (xTaskCreate(ConfigTask, "Configurar", 256, &estados[2], tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(board, 5);
    }

    if (xTaskCreate(TimeOutTask, "Timer", 256, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(board, 7);
    }

    if (xTaskCreate(CancelKeyTask, "Cancelar", 256, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        StopByError(board, 5);
    }

    /* Arranga S.O*/
    vTaskStartScheduler();

    while (true) {
    };
    return 0;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
