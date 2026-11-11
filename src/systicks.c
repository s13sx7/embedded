#include "hardware.h"
#include "systicks.h"

void Systick_Init(void){
	STK->CTRL = 0;
	STK->LOAD = 71999;
	STK->VAL = 0;
	STK->CTRL |= 0b111;
}
