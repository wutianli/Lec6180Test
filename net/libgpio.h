
#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

enum GPIO_PIN{
        GPIO_SET_RUN = 0,
        GPIO_SET_BYPASS,
        GPIO_SET_COM1,
        GPIO_SET_COM2,
        GPIO_SET_COM3,
        GPIO_SET_COM4,
        GPIO_SET_COM5,
        GPIO_SET_COM6,
        GPIO_SET_COM7,
        GPIO_SET_COM8,
        GPIO_SET_PIO1,
        GPIO_SET_PIO2,
        GPIO_SET_PIO3,
        GPIO_SET_PIO4,
};

int lec_gpio_init(int block_mode);
int lec_set_beep(unsigned int flag);
int lec_set_gpio_output(unsigned pin, int value);
int lec_gpio_release(void);

#endif

