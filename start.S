/*
 *@Author	: Rocky
 *@Date		: 2014-12-29
 *@Descri	: The Initial for smart210 after reset
*/
#include <cpu_base.h>

.global _start
.global main_loop
_start:
	b	reset
    ldr pc, _undefined_instruction
    ldr pc, _software_interrupt
    ldr pc, _prefetch_abort
    ldr pc, _data_abort
    ldr pc, _not_used
    ldr pc, _irq
    ldr pc, _fiq
_undefined_instruction: .word _undefined_instruction
_software_interrupt:    .word _software_interrupt
_prefetch_abort:    .word _prefetch_abort
_data_abort:        .word _data_abort
_not_used:      .word _not_used
_irq:           .word _irq
_fiq:           .word _fiq
_pad:           .word 0x12345678 /* now 16*4=64 */

reset:
	/* 
	 * Disable Watchdog 
	*/
	LDR     R1,=S5PV210_WATCHDOG_BASE
	LDR     R0,=0x0
	STR     R0,[R1]

	/*
	 * Setup SYS-Clock
	 * but we use IROM Configuration for test LED --- version 0.1
	*/
	bl      asm_clock_init

	/*
	 * LED Blink Test
	 * not set sp stack value,use IROM default value --- version 0.1
	*/
	bl      main_loop
	
loop:
	b		loop

/*
 *Do Nothing
 */
asm_clock_init:
    bx      lr
.end

