#include "ws2812.h"
#include "hal_pinmux_define.h"
#include "hal.h"
#include "type_def.h"
#include "mt7687_cm4_hw_memmap.h"
#include "hal_gpio_7687.h"
#define SIG_PIN HAL_GPIO_26
#define LED_NUM	16
uint32_t leds_color[LED_NUM];
static uint32_t remainder1=SIG_PIN % 32;;

#define SIG_H DRV_WriteReg32(IOT_GPIO_AON_BASE + IOT_GPIO_DOUT1_SET, 1 << remainder1)
#define SIG_L DRV_WriteReg32(IOT_GPIO_AON_BASE + IOT_GPIO_DOUT1_RESET, 1 << remainder1)
static void io_init(void)
{
	hal_gpio_init(SIG_PIN);//sda
	hal_pinmux_set_function(SIG_PIN, HAL_GPIO_26_GPIO26);
	hal_gpio_set_direction(SIG_PIN,HAL_GPIO_DIRECTION_OUTPUT);
	hal_gpio_pull_up(SIG_PIN);
	hal_gpio_set_output(SIG_PIN,HAL_GPIO_DATA_LOW);

}

static void delay_ns(volatile uint32_t ns)
{
	while(ns--);
}
static void send_bit(uint8_t bit)
{
	if(bit==0)
	{
		SIG_H;//one plus 80~120us
		SIG_H;SIG_H;SIG_H;SIG_H;
		//delay_ns(350);
		
		SIG_L;//one 170us
		SIG_L;SIG_L;SIG_L;SIG_L;SIG_L;SIG_L;SIG_L;SIG_L;
		//delay_ns(800);
	}
	else //  1
	{
		SIG_H;SIG_H;SIG_H;SIG_H;SIG_H;SIG_H;SIG_H;SIG_H;SIG_H;SIG_H;
		//delay_ns(700);
		SIG_L;SIG_L;SIG_L;SIG_L;SIG_L;SIG_L;
		//delay_ns(600);

	}

}
static void reset(void)
{
	SIG_L;
	delay_ns(1300);
}
static test()
{
	while(1)
		{
		send_bit(1);
		send_bit(0);
		SIG_H;
		reset();
		SIG_H;
		SIG_L;
		}

}
void ws2812_init(void)
{
	memset(leds_color,0,LED_NUM);
	io_init();
	leds_color[1]=0xff0000;
	leds_color[3]=0x00ff00;
	leds_color[5]=0x0000ff;
	set_leds_color(leds_color,LED_NUM);
}
void set_GRB(uint8_t g,uint8_t r,uint8_t b)
{
	for(uint8_t i=0; i<8; i++)
	{

		send_bit(g&0x80);
		g<<=1;

	}
	for(uint8_t i=0; i<8; i++)
	{

		send_bit(r&0x80);
		r<<=1;

	}
	for(uint8_t i=0; i<8; i++)
	{

		send_bit(b&0x80);
		b<<=1;

	}
}
void set_24bit_color(uint32_t color)
{

	uint8_t g=(uint8_t)(color>>16);
	uint8_t b=(uint8_t)(color>>8);
	uint8_t r=(uint8_t)(color>>0);
	set_GRB( g, b, r);
	
	
}

void set_leds_color(uint32_t *leds_color,uint32_t num)
{
	while(num--)
	{
		set_24bit_color(*leds_color);
		leds_color++;
	}

}
