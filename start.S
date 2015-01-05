/*
 *@Author	: Rocky
 *@Date		: 2014-12-29
 *@Descri	: The Initial for smart210 after reset
*/
#include <cpu_base.h>

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
asm_clock_init:
    ldr r0, =0xE010C000         @ S5PC110_PWR_CFG
    
    /* Set OSC_FREQ value */
    ldr r1, =0xf
    str r1, [r0, #0x100]        @ S5PC110_OSC_FREQ

    /* Set MTC_STABLE value */
    ldr r1, =0xffffffff
    str r1, [r0, #0x110]        @ S5PC110_MTC_STABLE

    /* Set CLAMP_STABLE value */
    ldr r1, =0x3ff03ff
    str r1, [r0, #0x114]        @ S5PC110_CLAMP_STABLE

    ldr r0, =0xE0100000         @ S5PC110_CLOCK_BASE

    /* Set Clock divider */
    ldr r1, =0x14131330         @ 1:1:4:4, 1:4:5
    str r1, [r0, #0x300]
    ldr r1, =0x11110111         @ UART[3210]: MMC[3210]
    str r1, [r0, #0x310]

    /* Set Lock Time */
    ldr r1, =0x2cf          @ Locktime : 30us
    str r1, [r0, #0x000]        @ S5PC110_APLL_LOCK
    ldr r1, =0xe10          @ Locktime : 0xe10 = 3600
    str r1, [r0, #0x008]        @ S5PC110_MPLL_LOCK
    str r1, [r0, #0x010]        @ S5PC110_EPLL_LOCK
    str r1, [r0, #0x020]        @ S5PC110_VPLL_LOCK

    /* S5PC110_APLL_CON */
    ldr r1, =0x80C80601         @ 800MHz
    str r1, [r0, #0x100]
    /* S5PC110_MPLL_CON */
    ldr r1, =0x829B0C01         @ 667MHz
    str r1, [r0, #0x108]
    /* S5PC110_EPLL_CON */
    ldr r1, =0x80600602         @  96MHz VSEL 0 P 6 M 96 S 2
    str r1, [r0, #0x110]
    /* S5PC110_VPLL_CON */
    ldr r1, =0x806C0603         @  54MHz
    str r1, [r0, #0x120]

    /* Set Source Clock */
    ldr r1, =0x10001111         @ A, M, E, VPLL Muxing
    str r1, [r0, #0x200]        @ S5PC1XX_CLK_SRC0

    /* OneDRAM(DMC0) clock setting */
    ldr r1, =0x01000000         @ ONEDRAM_SEL[25:24] 1 SCLKMPLL
    str r1, [r0, #0x218]        @ S5PC110_CLK_SRC6
    ldr r1, =0x30000000         @ ONEDRAM_RATIO[31:28] 3 + 1
    str r1, [r0, #0x318]        @ S5PC110_CLK_DIV6

    /* XCLKOUT = XUSBXTI 24MHz */
    add r2, r0, #0xE000         @ S5PC110_OTHERS
    ldr     r1, [r2]
    orr r1, r1, #(0x3 << 8)     @ CLKOUT[9:8] 3 XUSBXTI
    str r1, [r2]

    /* CLK_IP0 */
    ldr r1, =0x8fefeeb          @ DMC[1:0] PDMA0[3] IMEM[5]
    str r1, [r0, #0x460]        @ S5PC110_CLK_IP0

    /* CLK_IP1 */
    ldr r1, =0xe9fdf0f9         @ FIMD[0] USBOTG[16]
                        @ NANDXL[24]
    str r1, [r0, #0x464]        @ S5PC110_CLK_IP1

    /* CLK_IP2 */
    ldr r1, =0xf75f7fc          @ CORESIGHT[8] MODEM[9]
                        @ HOSTIF[10] HSMMC0[16]
                        @ HSMMC2[18] VIC[27:24]
    str r1, [r0, #0x468]        @ S5PC110_CLK_IP2

    /* CLK_IP3 */
    ldr r1, =0x8eff038c         @ I2C[8:6]
                        @ SYSTIMER[16] UART0[17]
                        @ UART1[18] UART2[19]
                        @ UART3[20] WDT[22]
                        @ PWM[23] GPIO[26] SYSCON[27]
    str r1, [r0, #0x46c]        @ S5PC110_CLK_IP3

    /* CLK_IP4 */
    ldr r1, =0xfffffff1         @ CHIP_ID[0] TZPC[8:5]
    str r1, [r0, #0x470]        @ S5PC110_CLK_IP3

200:
    /* wait at least 200us to stablize all clock */
    mov r2, #0x10000
1:  subs    r2, r2, #1
    bne 1b

    mov pc, lr

.end

