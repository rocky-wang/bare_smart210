#ifndef CPU_BASE
#define CPU_BASE

#define S5PV210_WATCHDOG_BASE   0xE2700000
#define S5PV210_GPIO_BASE 		0xE0200000
#define S5PV210_GPH2_BASE		0xE0200C40
#define S5PV210_CLOCK_BASE	    0xE0100000

#define	__REG(x)		(*(volatile unsigned int *)(x))

#define readb(a)		(*(volatile unsigned char *)(a))
#define readw(a)		(*(volatile unsigned short *)(a))
#define readl(a)		(*(volatile unsigned int *)(a))

#define writeb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define writew(v,a)		(*(volatile unsigned short *)(a) = (v))
#define writel(v,a)		(*(volatile unsigned int *)(a) = (v))


#endif

