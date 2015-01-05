#include "cpu_base.h"
#include "led_leaf.h"
#include "uart.h"
#include "debug.h"
#include "clock.h"

/*
 *Delay fixed count
*/
void mini_delay()
{
    //volatile unsigned int loop = 0xfffff;
    unsigned int loop = 0x7f0000;
    
    __asm__ __volatile__(
    "mov     r2,%[count] \n\
1:   subs    r2,r2,#1    \n\
     bne     1b"
     :
     :[count]"r"(loop)
    );

    //while(loop--);
}

int main_loop()
{
    led_init();
    s5p_uart_init();
    INFO("the armclock is %d\n",get_arm_clk());   
    INFO("the uart pclk is %d\n",get_uart_clk(0));
    show_uart_regs();
    
    while(1){
        led_light(LED1);
        mini_delay();
        led_light(LED2);
        mini_delay();
        led_light(LED3);
        mini_delay();
        led_light(LED4);
        mini_delay();
        led_light(LED1 | LED3);
        mini_delay();
        led_light(0);
        mini_delay();
    }
}

