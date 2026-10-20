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
	gpio_b->CRH &= ~(0xF << ((LED_PIN-8)*4));
	gpio_b->CRH |= (MODE_OUTPUT_2MHZ << ((LED_PIN-8)*4));
	uint32_t last_state = 1;
	while(1){
		uint32_t current_state = (gpio_b->IDR & BIT(BUTTON_PIN))?1:0;
		if (current_state != last_state){
			for (volatile uint32_t i = 0; i < 1000000; ++i){
				__asm__("nop");
			}
		}
		
		uint32_t current_state = (gpio_b->IDR & BIT(BUTTON_PIN))?1:0;
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
