#include <stdint.h>
#include "clock.h"
struct rcc {
	volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR,
			 APB1ENR, BDCR, CSR;
};
#define RCC ((struct rcc*) (0x40021000))
#define RCC_GPIO_B_BIT 3

struct gpio{
	volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
};
#define GPIO(letter) ((struct gpio*) (0x40010800+0x400*((letter)-'A')))

struct exti{
	volatile uint32_t IMR, EMR, RTSR, FTSR, SWIER, PR;
};
#define EXTI ((struct exti*) (0x40010400))

struct afio{
	volatile uint32_t EVCR, MAPR, EXTICR1, EXTICR2, EXTICR3, EXTICR4, MAPR2;
};
#define AFIO ((struct afio*) (0x40010000))

#define NVIC_ISER(x) ((volatile uint32_t*) (0xE000E100+(4*(x))))
/*Данный модуль позволяет не отключать отладку во сне
 * ранее возникала ошибка когда из за строки __asm__("wfi")
 * программа засыпала и не отзывалась на действия
 * */
struct dbgmcu {
    volatile uint32_t IDCODE, CR;
};
#define DBGMCU ((struct dbgmcu *)0xE0042000)

#define MODE_INPUT_FLOATING 0x4
#define MODE_INPUT_PULL_UP 0x8
#define BUTTON_PIN 6

#define RCC_AFIO_BIT 0
#define AFIO_EXTICR2_PORT_B 0b0001
#define EXTI_LINE_SIX 6
#define EXTI9_5_INTERRUPT 23

#define MODE_OUTPUT_2MHZ 0x2
#define LED_PIN 14

#define BIT(x) ((1UL << (x)))


int main(void){
	SystemClock_Config();
	/*включил тактирование для gpio_b*/
	RCC->APB2ENR |= BIT(RCC_GPIO_B_BIT);
    RCC->APB2ENR |= BIT(4);  // IOPCEN для порта C

	struct gpio *gpio_b = GPIO('B');
	/*обнуление нужных битов*/
	gpio_b->CRL &= ~(0xF << (BUTTON_PIN*4));
	/*запись режима в биты*/
	gpio_b->CRL |= (MODE_INPUT_PULL_UP << (BUTTON_PIN*4));
	/*НАстройка на подтяжку вверх*/	
	gpio_b->ODR |= (1u << BUTTON_PIN); //PULL UP
	
	gpio_b->CRH &= ~(0xF << ((LED_PIN-8)*4));
	gpio_b->CRH |= (MODE_OUTPUT_2MHZ << ((LED_PIN-8)*4));

	struct gpio *gpio_c = GPIO('C');
	gpio_c->CRH &= ~(0xF << ((13-8)*4));
    gpio_c->CRH |= (MODE_OUTPUT_2MHZ << ((13-8)*4));
    gpio_c->BSRR = BIT(13+16);  // Выключить (на Blue Pill активен низким)
	/*
	 * Кнопка на 6 пине gpio_b -> нам нужен EXTI6[3:0] bits in AFIO_EXTICR2
	 *
	 * Включить тактирование AFIO !
	 * Поключить порт B к 6 линии в EXTICR регистре !
	 * 
	 * подключить к 6 линии EXTI наш 6 пин
	 * настроить на падение напряжения
	 * 
	 * разрешить прерывания в NVIC_ISER[0]
	 *  
	 *
	 * */

	RCC->APB2ENR |= BIT(RCC_AFIO_BIT);
	AFIO->EXTICR2 |= (AFIO_EXTICR2_PORT_B << 8);
	
	EXTI->IMR |= BIT(EXTI_LINE_SIX); // прерывание на 6 линии
	EXTI->FTSR |=BIT(EXTI_LINE_SIX); // На падение напряжения
	
	*NVIC_ISER(0) |= BIT(EXTI9_5_INTERRUPT);
    gpio_b->BSRR = BIT(LED_PIN + 16);  // Сбросить в 0
	DBGMCU->CR |= BIT(0) | BIT(1) | BIT(2);
	/*чтобы во всех режимах сна была доступна отладка*/
	while(1){
		__asm__("wfi");  // no-operation, один такт процессора
	}
}

static volatile uint32_t led_state = 0;
void EXTI9_5_IRQHandler(void) {
    // Диагностика: переключаем встроенный светодиод на PC13

    if (EXTI->PR & BIT(EXTI_LINE_SIX)){
        EXTI->PR = BIT(EXTI_LINE_SIX);

        // Антидребезг
        for (volatile uint32_t i = 0; i < 4500000; i++) {
            __asm__("nop");
        }

		struct gpio *gpio_c = GPIO('C');
        struct gpio *gpio_b = GPIO('B');
        led_state = !led_state;

        if (led_state) {
            gpio_b->BSRR = BIT(LED_PIN);
			gpio_c->ODR ^= BIT(13);  // Toggle PC13

        } else {
            gpio_b->BSRR = BIT(LED_PIN + 16);
			gpio_c->ODR ^= BIT(13);  // Toggle PC13
        }
    }
}
