#include <stdint.h>
struct rcc {
	volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR,
			 APB1ENR, BDCR, CSR;
};
#define RCC ((struct rcc*) (0x40021000))
#define RCC_GPIO_C_BIT 4

struct gpio{
	volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
};
#define GPIO(letter) ((struct gpio*) (0x40010800+0x400*((letter)-'A')))

#define MODE_INPUT_FLOATING 0x4
#define MODE_INPUT_PULL_UP 0x8
#define BUTTON_PIN 6

#define MODE_OUTPUT_2MHZ 0x2
#define LED_PIN 14

#define BIT(x) ((1UL << (x)))


int main(void){
	RCC->APB2ENR |= BIT(RCC_GPIO_B_BIT);
	struct gpio *gpio_b = GPIO('B');
	gpio_b->CRL &= ~(0xF << (BUTTON_PIN*4));
	gpio_b->CRL |= (MODE_INPUT_PULL_UP << (BUTTON_PIN*4));
	//gpio_b->ODR &= ~(1u << BUTTON_PIN_ODR); не нужно все равно 1
	gpio_b->ODR |= (1u << BUTTON_PIN); //PULL UP
	gpio_b->CRH &= ~(0xF << (LED_PIN*4));
	gpio_b->CRH |= (MODE


	while(1){
		gpio_c->BSRR = BIT(13);
		for (volatile uint32_t i = 0; i < 500000; i++) {
		   __asm__("nop");  // no-operation, один такт процессора
		}
		gpio_c->BSRR = BIT(13+16);
		for (volatile uint32_t i = 0; i < 500000; i++) {
			__asm__("nop");  // no-operation, один такт процессора
		}
	}
}
