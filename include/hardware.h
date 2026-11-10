#ifndef HARDWARE_H
#define HARDWARE_H

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

struct flash{
	volatile uint32_t ACR;
};
#define FLASH ((struct flash*) 0x40022000)

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
#define BITS(X,Y) ((X) << (Y))

#define RCC_CR_HSE_ON 16
#define RCC_CR_HSERDY 17

#define FLASH_ACR_LATENCY 0
#define FLASH_ACR_LATENCY_72MHZ 0b010

#define FLASH_ACR_PRFTBE 4

#define RCC_CFGR_HPRE 4

#define RCC_CFGR_PPRE1 8
#define RCC_CFGR_PPRE1_2DIV 0b100

#define RCC_CFGR_PPRE2 11

#define RCC_CFGR_PLLSRC 16

#define RCC_CFGR_PLLMUL 18
#define RCC_CFGR_PLLMUL_9 0b0111

#define RCC_CFGR_SW 0
#define RCC_CFGR_SW_PLL 0b10

#define RCC_CFGR_SWS 0b10
#define RCC_CR_PLLON 24
#define RCC_CR_PLLRDY 25

#endif //HARDWARE_H
