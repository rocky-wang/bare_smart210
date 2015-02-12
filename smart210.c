#include "cpu_base.h"
#include "led_leaf.h"
#include "uart.h"
#include "debug.h"
#include "clock.h"
#include "s5p_nand.h"

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

void nand_operation_test()
{
    char buf[1024];
    int ret;
    
    erase_flash(0x700000);

    nand_write("1234567890",0x700000,10);

    ret = nand_read(buf,0x600000,8);
    if(ret < 0){
        return ;
    }
    buf[ret] = 0;
    INFO("the nand read buf is %s\n",buf);
}

int main_loop()
{
    led_init();
    s5p_uart_init();
    nand_lowlevel_init();
    INFO("the armclock is %d\n",get_arm_clk());
    nand_init_chip();

    //nand_operation_test();
    
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

