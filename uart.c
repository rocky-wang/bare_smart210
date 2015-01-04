#include "uart.h"
#include "cpu_base.h"

void s5p_uart_init()
{
    struct s5p_uart *uart_base = (struct s5p_uart *)S5PV210_UART_BASE;

    writel(0x3,&uart_base->ulcon);
    writel(0x5,&uart_base->ucon);

    writel(34,&uart_base->ubrdiv);
    writew(0xdfdd,&uart_base->rest.slot);
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

