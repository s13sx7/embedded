#include <stdint.h>
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
	//включил тактирование для gpio_b
	RCC->APB2ENR |= BIT(RCC_GPIO_B_BIT);
	struct gpio *gpio_b = GPIO('B');
	//обнулил
	gpio_b->CRL &= ~(0xF << (BUTTON_PIN*4));
	//записал
	gpio_b->CRL |= (MODE_INPUT_PULL_UP << (BUTTON_PIN*4));
	//gpio_b->ODR &= ~(1u << BUTTON_PIN_ODR); не нужно все равно 1
	gpio_b->ODR |= (1u << BUTTON_PIN); //PULL UP
	gpio_b->CRH &= ~(0xF << ((LED_PIN-8)*4));
	gpio_b->CRH |= (MODE_OUTPUT_2MHZ << ((LED_PIN-8)*4));
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

	uint32_t last_state = 1;
	while(1){
#if 1
		uint32_t current_state = (gpio_b->IDR & BIT(BUTTON_PIN))?1:0;
		if (current_state != last_state){
			for (volatile uint32_t i = 0; i < 60000; ++i){
				__asm__("nop");
			}
		}
		
		current_state = (gpio_b->IDR & BIT(BUTTON_PIN))?1:0;
		if (current_state != last_state){
			last_state = current_state;
			if (current_state == 0){
				gpio_b->BSRR = BIT(LED_PIN);
			}else {
				gpio_b->BSRR = BIT(LED_PIN+16);   // Выключить
			}
		}
	}
}
#endif
#if 0
		uint32_t current_state = (gpio_b->IDR & BIT(BUTTON_PIN))?1:0;
		if (current_state == 0){
				gpio_b->BSRR = BIT(LED_PIN);
			}else {
				gpio_b->BSRR = BIT(LED_PIN+16);   // Выключить
			}
	}
}
#endif
