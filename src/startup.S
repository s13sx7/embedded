    .syntax unified
    .cpu cortex-m3
    .thumb

    .global Reset_Handler
    .global Default_Handler

    /* =========================================
     * 1. ВЕКТОРНАЯ ТАБЛИЦА (isr_vector)
     * ========================================= */
    .section .isr_vector, "a", %progbits
    .type g_pfnVectors, %object

g_pfnVectors:
    .word _estack             /* 0x00: Начальный указатель стека (берём из линкер-скрипта) */
    .word Reset_Handler       /* 0x04: Точка входа после сброса */
    .word NMI_Handler         /* 0x08: Немаскируемое прерывание */
    .word HardFault_Handler   /* 0x0C: Аппаратная ошибка */
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0                   /* Зарезервировано */
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler
	/* Внешние прерывания (External Interrupts) */
	.word WWDG_IRQHandler       /* 0x40: IRQ 0  - Сторожевой таймер */
	.word PVD_IRQHandler        /* 0x44: IRQ 1  - Детектор падения напряжения */
	.word TAMPER_IRQHandler     /* 0x48: IRQ 2  - Вторжение (Tamper) */
	.word RTC_IRQHandler        /* 0x4C: IRQ 3  - Часы реального времени */
	.word FLASH_IRQHandler      /* 0x50: IRQ 4  - Прерывание Flash-памяти */
	.word RCC_IRQHandler        /* 0x54: IRQ 5  - Тактирование и сброс */
	.word EXTI0_IRQHandler      /* 0x58: IRQ 6  - Внешняя линия 0 */
	.word EXTI1_IRQHandler      /* 0x5C: IRQ 7  - Внешняя линия 1 */
	.word EXTI2_IRQHandler      /* 0x60: IRQ 8  - Внешняя линия 2 */
	.word EXTI3_IRQHandler      /* 0x64: IRQ 9  - Внешняя линия 3 */
	.word EXTI4_IRQHandler      /* 0x68: IRQ 10 - Внешняя линия 4 */
	.word DMA1_Channel1_IRQHandler /* 0x6C: IRQ 11 - DMA1 Канал 1 */
	.word DMA1_Channel2_IRQHandler /* 0x70: IRQ 12 - DMA1 Канал 2 */
	.word DMA1_Channel3_IRQHandler /* 0x74: IRQ 13 - DMA1 Канал 3 */
	.word DMA1_Channel4_IRQHandler /* 0x78: IRQ 14 - DMA1 Канал 4 */
	.word DMA1_Channel5_IRQHandler /* 0x7C: IRQ 15 - DMA1 Канал 5 */
	.word DMA1_Channel6_IRQHandler /* 0x80: IRQ 16 - DMA1 Канал 6 */
	.word DMA1_Channel7_IRQHandler /* 0x84: IRQ 17 - DMA1 Канал 7 */
	.word ADC1_2_IRQHandler     /* 0x88: IRQ 18 - АЦП1 и АЦП2 */
	.word USB_HP_CAN1_TX_IRQHandler /* 0x8C: IRQ 19 - USB HP / CAN1 TX */
	.word USB_LP_CAN1_RX0_IRQHandler /* 0x90: IRQ 20 - USB LP / CAN1 RX0 */
	.word CAN1_RX1_IRQHandler   /* 0x94: IRQ 21 - CAN1 RX1 */
	.word CAN1_SCE_IRQHandler   /* 0x98: IRQ 22 - CAN1 Ошибка статуса */
	.word EXTI9_5_IRQHandler    /* 0x9C: IRQ 23 - Внешние линии [9:5] */
	.size g_pfnVectors, .-g_pfnVectors

    /* =========================================
     * 2. КОД ЗАПУСКА (Reset_Handler)
     * ========================================= */
    .section .text.Reset_Handler
    .thumb_func
    .type Reset_Handler, %function
Reset_Handler:
    /* --- Шаг А: Копируем секцию .data из Flash в RAM --- */
    ldr r0, =_sdata       /* r0 = адрес начала .data в RAM */
    ldr r1, =_edata       /* r1 = адрес конца .data в RAM */
    ldr r2, =_sidata      /* r2 = адрес, где лежат начальные значения во Flash */

copy_data_loop:
    cmp r0, r1            /* Сравниваем текущий адрес (r0) с концом (r1) */
    bge copy_data_done    /* Если r0 >= r1, значит всё скопировали, выходим */
    ldr r3, [r2], #4      /* Читаем 4 байта из Flash в r3, и увеличиваем r2 на 4 */
    str r3, [r0], #4      /* Записываем эти 4 байта в RAM, и увеличиваем r0 на 4 */
    b copy_data_loop      /* Прыгаем на начало цикла */

copy_data_done:

    /* --- Шаг Б: Обнуляем секцию .bss в RAM --- */
    ldr r0, =_sbss        /* r0 = адрес начала .bss */
    ldr r1, =_ebss        /* r1 = адрес конца .bss */
    movs r2, #0           /* r2 = 0 (чем будем заполнять) */

zero_bss_loop:
    cmp r0, r1
    bge zero_bss_done
    str r2, [r0], #4      /* Записываем 0 в RAM, увеличиваем r0 на 4 */
    b zero_bss_loop

zero_bss_done:

    /* --- Шаг В: Вызываем main --- */
    bl main               /* Branch with Link - вызов функции main на C */
    b .                   /* Если main() вдруг завершится, зависаем здесь навсегда */


    /* =========================================
     * 3. ЗАГЛУШКИ ДЛЯ ПРЕРЫВАНИЙ
     * ========================================= */
    .section .text.Default_Handler
    .thumb_func
    .type Default_Handler, %function
Default_Handler:
    b .                   /* Бесконечный цикл, чтобы не улететь в никуда */

    /* Слабые ссылки: если мы не напишем свой обработчик в C, 
       линкер подставит сюда Default_Handler */
    .weak NMI_Handler
    .thumb_set NMI_Handler, Default_Handler

    .weak HardFault_Handler
    .thumb_set HardFault_Handler, Default_Handler

    .weak MemManage_Handler
    .thumb_set MemManage_Handler, Default_Handler

    .weak BusFault_Handler
    .thumb_set BusFault_Handler, Default_Handler

    .weak UsageFault_Handler
    .thumb_set UsageFault_Handler, Default_Handler

    .weak SVC_Handler
    .thumb_set SVC_Handler, Default_Handler

    .weak DebugMon_Handler
    .thumb_set DebugMon_Handler, Default_Handler

    .weak PendSV_Handler
    .thumb_set PendSV_Handler, Default_Handler

    .weak SysTick_Handler
    .thumb_set SysTick_Handler, Default_Handler

	.weak WWDG_IRQHandler
    .thumb_set WWDG_IRQHandler, Default_Handler

    .weak PVD_IRQHandler
    .thumb_set PVD_IRQHandler, Default_Handler

    .weak TAMPER_IRQHandler
    .thumb_set TAMPER_IRQHandler, Default_Handler

    .weak RTC_IRQHandler
    .thumb_set RTC_IRQHandler, Default_Handler

    .weak FLASH_IRQHandler
    .thumb_set FLASH_IRQHandler, Default_Handler

    .weak RCC_IRQHandler
    .thumb_set RCC_IRQHandler, Default_Handler

    .weak EXTI0_IRQHandler
    .thumb_set EXTI0_IRQHandler, Default_Handler

    .weak EXTI1_IRQHandler
    .thumb_set EXTI1_IRQHandler, Default_Handler

    .weak EXTI2_IRQHandler
    .thumb_set EXTI2_IRQHandler, Default_Handler

    .weak EXTI3_IRQHandler
    .thumb_set EXTI3_IRQHandler, Default_Handler

    .weak EXTI4_IRQHandler
    .thumb_set EXTI4_IRQHandler, Default_Handler

    .weak DMA1_Channel1_IRQHandler
    .thumb_set DMA1_Channel1_IRQHandler, Default_Handler

    .weak DMA1_Channel2_IRQHandler
    .thumb_set DMA1_Channel2_IRQHandler, Default_Handler

    .weak DMA1_Channel3_IRQHandler
    .thumb_set DMA1_Channel3_IRQHandler, Default_Handler

    .weak DMA1_Channel4_IRQHandler
    .thumb_set DMA1_Channel4_IRQHandler, Default_Handler

    .weak DMA1_Channel5_IRQHandler
    .thumb_set DMA1_Channel5_IRQHandler, Default_Handler

    .weak DMA1_Channel6_IRQHandler
    .thumb_set DMA1_Channel6_IRQHandler, Default_Handler

    .weak DMA1_Channel7_IRQHandler
    .thumb_set DMA1_Channel7_IRQHandler, Default_Handler

    .weak ADC1_2_IRQHandler
    .thumb_set ADC1_2_IRQHandler, Default_Handler

    .weak USB_HP_CAN1_TX_IRQHandler
    .thumb_set USB_HP_CAN1_TX_IRQHandler, Default_Handler

    .weak USB_LP_CAN1_RX0_IRQHandler
    .thumb_set USB_LP_CAN1_RX0_IRQHandler, Default_Handler

    .weak CAN1_RX1_IRQHandler
    .thumb_set CAN1_RX1_IRQHandler, Default_Handler

    .weak CAN1_SCE_IRQHandler
    .thumb_set CAN1_SCE_IRQHandler, Default_Handler

    .weak EXTI9_5_IRQHandler
    .thumb_set EXTI9_5_IRQHandler, Default_Handler
