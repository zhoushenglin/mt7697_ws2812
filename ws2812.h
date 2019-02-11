#ifndef __WS2812_H__
#define __WS2812_H__
#include <stdint.h>



void ws2812_init(void);

void set_leds_color(uint32_t *leds_color,uint32_t num);

#endif
