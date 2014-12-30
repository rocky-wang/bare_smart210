#include <cpu_base.h>
#include <led_leaf.h>

/*
 *Delay fixed count
*/
void mini_delay()
{
    volatile unsigned int loop = 0xfffff;

    while(loop--);
}

int main_loop()
{
    led_init();

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
    }
}

