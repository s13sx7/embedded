#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include "led_sim.h"

/*
 * Простая задержка для учебной симуляции.
 * Позже на реальном МК мы заменим такие вещи на таймеры/SysTick.
 */
static void delay_ms(unsigned int ms)
{
    usleep(ms * 1000u);
}

int main(void)
{
    led_sim_init();

    printf("Simulated Blue Pill LED (PC13-like)\n");

    for (int i = 0; i < 10; ++i)
    {
        led_sim_toggle();

        printf("LED is %s\n",
               led_sim_get_state() ? "ON" : "OFF");

        delay_ms(100);
    }

    led_sim_off();

    return 0;
}
