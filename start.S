/*
 *@Author	: Rocky
 *@Date		: 2014-12-29
 *@Descri	: The Initial for smart210 after reset
*/
#include "cpu_base.h"

.global _start
.global main_loop
.global asm_uart_init

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
#ifdef CONFIG_WATCHDOG_DISABLE
	LDR     R1,=S5PV210_WATCHDOG_BASE
	LDR     R0,=0x0
	STR     R0,[R1]
#endif
    /*
        * Disable L1 Cache  --- V0.2
       */
#ifdef CONFIG_SYS_ICACHE_OFF
    mrc	    p15, 0, r0, c1, c0, 0
    bic 	r0, r0, #0x00001000
    mcr	    p15, 0, r0, c1, c0, 0
#endif
    
	/*
	 * Setup SYS-Clock
	 * but we use the max of Configuration for test LED --- version 0.5
	*/
#ifdef CONFIG_SYS_PLL_ON
	bl      asm_clock_init
#endif
    /*
        * Uart Asm init,inital the GPIO setup
        */
    bl      asm_uart_init
	/*
	 * LED Blink Test
	 * not set sp stack value,use IROM default value --- version 0.1
	*/
	bl      main_loop
	
loop:
	b		loop

/*
 *Setup max freq.
 */
#ifdef CONFIG_SYS_PLL_ON
asm_clock_init:
    ldr r0, =S5PV210_CLOCK_BASE

    /* Set Lock Time */
    ldr r1, =0x2cf              @ Locktime : 30us
    str r1, [r0, #0x000]        @ S5PV210_APLL_LOCK
    ldr r1, =0xe10              @ Locktime : 0xe10 = 3600
    str r1, [r0, #0x008]        @ S5PV210_MPLL_LOCK
    str r1, [r0, #0x010]        @ S5PV210_EPLL_LOCK
    str r1, [r0, #0x020]        @ S5PV210_VPLL_LOCK

    /* Set Clock divider */
    ldr r1, =CLK_DIV0_VAL       
    str r1, [r0, #0x300]        @ CLK_DIV0
    
    /* S5PV210_APLL_CON */
    ldr r1, =APLL_VALUE         @ 1000MHz
    str r1, [r0, #0x100]
    /* S5PV210_MPLL_CON */
    ldr r1, =MPLL_VALUE         @ 667MHz
    str r1, [r0, #0x108]
    /* S5PV210_EPLL_CON */
    ldr r1, =EPLL_VALUE         @  96MHz VSEL 0 P 6 M 96 S 2
    str r1, [r0, #0x110]
    /* S5PV210_VPLL_CON */
    ldr r1, =VPLL_VALUE         @  54MHz
    str r1, [r0, #0x120]

    /* Set Source Clock */
    ldr r1, =CLK_SRC0_VAR       @ A, M, E, VPLL Muxing
    str r1, [r0, #0x200]        @ S5PV210_CLK_SRC0

    /* CLK_IP0 */
    ldr r1, =0x23       @ DMC0-1[0:1] MDMA PDMA IMEM[5] G3D G2D MFC FIMC0-2 ROTATOR CSIS
    str r1, [r0, #0x460]        @ S5PV210_CLK_IP0
#if 0
    /* CLK_IP1 */
    ldr r1, =0x0ffffffff        @ FIMDD DSIM VP MIXER TVENC HDMI USBOTG/HOST ONENAND-XL  
                        @ CFCON SROM NFCON
    str r1, [r0, #0x464]        @ S5PV210_CLK_IP1
#endif

    /* CLK_IP2 */
    ldr r1, =0x0F000000  @ SESS SDM CORESIGHT MODEM JTAG HSMMC0-3
                         @ TSI VIC0-3[24:27] TZIC0-3
    str r1, [r0, #0x468]        @ S5PV210_CLK_IP2

    /* CLK_IP3 */
    ldr r1, =0x0c1F0000  @ SPDIF AC97 I2S0-2 I2C0-2 HDMI SPI0-1
                         @ RTC SYSTIME[16] UART0-UART3[17:20] KEYIF 
                         @ WDT PWM TSADC GPIO[26] SYSCON[27] PCM0-3 
    str r1, [r0, #0x46c]        @ S5PV210_CLK_IP3

    /* CLK_IP4 */
    ldr r1, =0xfffffff1         @ CHIP_ID[0] TZPC[8:5]
    str r1, [r0, #0x470]        @ S5PV210_CLK_IP3

200:
    /* wait at least 200us to stablize all clock */
    mov r2, #0x10000
1:  subs    r2, r2, #1
    bne 1b

    mov pc, lr

#endif

.end

