#ifndef LED_LEAF_H
#define LED_LEAF_H

#define ADDR_GPJ2CON    (0xE0200280)
#define ADDR_GPJ2DAT    (0xE0200284)

/* SMART210 LED set low level to light */
enum LED_STATE{LED1 = 0x1<<0,LED2 = 0x1<<1,
                LED3 = 0x1<<2,LED4 = 0x1<<3};

extern void led_init();
extern void led_light(unsigned char status);


#endif
