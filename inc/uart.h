#ifndef UART_H
#define UART_H

union br_rest {	
    unsigned short	slot;		/* udivslot */	
    unsigned char	value;		/* ufracval */
};

struct s5p_uart {
	unsigned int	ulcon;
	unsigned int	ucon;
	unsigned int	ufcon;
	unsigned int	umcon;
	unsigned int	utrstat;
	unsigned int	uerstat;
	unsigned int	ufstat;
	unsigned int	umstat;
	unsigned char	utxh;
	unsigned char	res1[3];
	unsigned char	urxh;
	unsigned char	res2[3];
	unsigned int	ubrdiv;
	union br_rest	rest;
	unsigned char	res3[0x3d0];
};

#define S5PV210_UART_BASE   0xE2900000

extern void s5p_uart_init();
extern void puts(const char *str);

#endif
