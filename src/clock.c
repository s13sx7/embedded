#include <stdint.h>
#include "hardware.h"
#include "clock.h"

int SystemClock_Config(void){
	int check = 1;
	/*enable HSE*/
	RCC->CR |= BIT(RCC_CR_HSE_ON);
	
	for (volatile uint32_t i = 0; i < 500000; ++i){
		/*check HSE RDY*/
		if (RCC->CR & BIT(RCC_CR_HSERDY)){
			check = 0;
			break;
		}
	}

	if (check)
		return 1;
	
	check = 1;

	/*switch latency for 72MHz (wait state = 2)*/
	FLASH->ACR |= BITS(FLASH_ACR_LATENCY_72MHZ, FLASH_ACR_LATENCY);
	/*Enable prefetch buffer*/
	FLASH->ACR |= BIT(FLASH_ACR_PRFTBE);

	/*enable prescaler for ours bus*/
	RCC->CFGR &= ~BITS(0xF, RCC_CFGR_HPRE);
	RCC->CFGR &= ~BITS(7, RCC_CFGR_PPRE2);
	RCC->CFGR &= ~BITS(7, RCC_CFGR_PPRE1);
	RCC->CFGR |= BITS(RCC_CFGR_PPRE1_2DIV, RCC_CFGR_PPRE1);

	/*choose PLL source*/
	RCC->CFGR |= BIT(RCC_CFGR_PLLSRC);
	
	/*choose PLL multiplication factor*/
	RCC->CFGR &= ~BITS(0xF, RCC_CFGR_PLLMUL);
	RCC->CFGR |= BITS(RCC_CFGR_PLLMUL_9, RCC_CFGR_PLLMUL);
	
	/*enable PLL*/
	RCC->CR |= BIT(RCC_CR_PLLON);
	for (volatile uint32_t i = 0; i < 500000; ++i){
		/*check PLL RDY*/
		if (RCC->CR & BIT(RCC_CR_PLLRDY)){
			check = 0;
			break;
		}
	}

	if (check)
		return 2;
	
	check = 1;

	RCC->CFGR &= ~BITS(0b11, RCC_CFGR_SW);
	RCC->CFGR |= BITS(RCC_CFGR_SW_PLL, RCC_CFGR_SW);

	for (volatile uint32_t i = 0; i < 500000; ++i){
		/*check SW status*/
		if ((RCC->CFGR & (0b11 << 2)) == (RCC_CFGR_SWS << 2)){
			check = 0;
			break;
		}
	}
	if (check)
		return 3;

	return 0;
}
