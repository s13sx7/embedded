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

void pattern_fast_blink(int count){
	for(int i = 0; i < count; ++i){
        led_sim_set((uint32_t)i%2);
        printf("LED is %s\n",
               led_sim_get_state() ? "ON" : "OFF");

        delay_ms(100);
	}
}

void pattern_slow_blink(int count){
	for(int i = 0; i < count; ++i){
        led_sim_set((uint32_t)i%2);
        printf("LED is %s\n",
               led_sim_get_state() ? "ON" : "OFF");

        delay_ms(500);
	}
}

int main(void)
{
    led_sim_init();

    printf("Simulated Blue Pill LED (PC13-like)\n");
	printf("fast blink\n");
	pattern_fast_blink(10);
	printf("slow blink\n");
	pattern_slow_blink(10);

    led_sim_off();

    return 0;
}
