#include <cpu_base.h>
#include <led_leaf.h>

void led_init()
{
    volatile unsigned int *gpj2con = (unsigned int *)ADDR_GPJ2CON;
    unsigned int var;

    var = gpj2con[0];
    var &= ~(0xffff<<0);
    var |= (0x1111<<0);
    gpj2con[0] = var;
}

/*
 * Use or Operation to set LED
 * Eg: LED1 | LED2 : led1 and led2 on,led3 and led4 off
 *      LED2 | LED4 : led2 and led4 on,led1 and led3 off
*/
void led_light(unsigned char status)
{
    volatile unsigned char *gpj2dat = (unsigned char *)ADDR_GPJ2DAT;
    unsigned int var;

    /*1. All LED off */
    var = gpj2dat[0];
    var |= (0xf<<0);
    gpj2dat[0] = var;

    /*2. According to STATUS set LED */
    status &= (0xf<<0);
    gpj2dat[0] &= ~status;
}



