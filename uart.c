#include "uart.h"
#include "cpu_base.h"
#include "clock.h"
#include "debug.h"

static const int udivslot[] = {
	0,
	0x0080,
	0x0808,
	0x0888,
	0x2222,
	0x4924,
	0x4a52,
	0x54aa,
	0x5555,
	0xd555,
	0xd5d5,
	0xddd5,
	0xdddd,
	0xdfdd,
	0xdfdf,
	0xffdf,
};

void serial_setbrg_dev(const unsigned long rate)
{
	struct s5p_uart *const uart = (struct s5p_uart *)S5PV210_UART_BASE;
	unsigned long uclk = get_uart_clk(0);
	unsigned long baudrate = rate;
	unsigned long val;

	val = uclk / baudrate;

	writel(val / 16 - 1, &uart->ubrdiv);

	writew(udivslot[val % 16], &uart->rest.slot);
}

void show_uart_regs()
{
    struct s5p_uart *const uart = (struct s5p_uart *)S5PV210_UART_BASE;

    INFO("The div reg is %x\n",readl(&uart->ubrdiv));
    INFO("The slot reg is %x\n",readw(&uart->rest.slot));
}


void s5p_uart_init()
{
    struct s5p_uart *uart_base = (struct s5p_uart *)S5PV210_UART_BASE;

    writel(0x3,&uart_base->ulcon);
    writel(0x5,&uart_base->ucon);

   serial_setbrg_dev(115200);
}

void putc(unsigned char c)
{
    struct s5p_uart *uart_base = (struct s5p_uart *)S5PV210_UART_BASE;

    if(c == '\n'){
        while( !(__REG(&uart_base->utrstat) & (0x1<<2)));
        writeb('\r',&uart_base->utxh);
    }

    while( !(__REG(&uart_base->utrstat) & (0x1<<2)));
    writeb(c,&uart_base->utxh);
}

void puts(const char *str)
{
    while(*str){
        putc(*str);
        str++;
    }
}

