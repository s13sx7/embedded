#include "led_sim.h"

/*
 * Используем 13-й бит, чтобы позже было легче связать
 * это с реальным PC13 на Blue Pill.
 *
 * Важно:
 * на реальной плате Blue Pill светодиод часто активен низким уровнем,
 * но в симуляторе мы пока считаем его условно активным высоким.
 */
#define LED_PIN_MASK (1u << 13)

/*
 * volatile здесь стоит для учебной привычки:
 * в реальном коде состояние порта может меняться "извне".
 */
static volatile uint32_t simulated_port;

void led_sim_init(void)
{
    simulated_port = 0u;
}

void led_sim_on(void)
{
    simulated_port |= LED_PIN_MASK;
}

void led_sim_off(void)
{
    simulated_port &= (uint32_t)~LED_PIN_MASK;
}

void led_sim_toggle(void)
{
    simulated_port ^= LED_PIN_MASK;
}

uint32_t led_sim_get_state(void)
{
    return (simulated_port & LED_PIN_MASK) ? 1u : 0u;
}
