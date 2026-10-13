#ifndef LED_SIM_H
#define LED_SIM_H

#include <stdint.h>

/*
 * Простой "симулятор" порта микроконтроллера.
 * Мы будем управлять одним условным светодиодом.
 */

void led_sim_init(void);
void led_sim_on(void);
void led_sim_off(void);
void led_sim_toggle(void);
uint32_t led_sim_get_state(void);
void led_sim_set(uint32_t on);
#endif /* LED_SIM_H */
