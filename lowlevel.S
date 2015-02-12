/*
 *Configure Device low level inital
**/
#include "cpu_base.h"

.global asm_uart_init

asm_uart_init:
    /* set GPIO to enable UART0-UART4 */
    ldr	r8, =S5PV210_GPIO_BASE
    ldr	r1, =0x22222222
    str	r1, [r8, #0x0]			@ S5PC100_GPIO_A0_OFFSET	
    ldr	r1, =0x00002222
    str	r1, [r8, #0x20]			@ S5PC100_GPIO_A1_OFFSET

    bx  lr
    

