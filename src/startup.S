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
    /* Для STM32F103 здесь должно быть ещё много векторов периферии, 
       но для базового blink нам хватит этого минимума Cortex-M3 */

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
