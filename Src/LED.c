#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"
#include "defines.h"
#include "fm_stm32f4_led.h"
#include "fm_stm32f4_delay.h"

/*
    dependencies:
    stm32f4xx.h
    stm32f4xx_gpio.h
    stm32f4xx_rcc.h
    fm_stm32f4_led.h
    fm_stm32f4_delay.h
*/
int LED_init(){
    
    GPIO_InitTypeDef LED_struct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    LED_struct.GPIO_Pin   = GPIO_Pin_12; //any gpio pin on electronics board, 12 is a placeholder
    LED_struct.GPIO_Mode  = GPIO_Mode_OUT; //sets gpio pin as an output
    LED_struct.GPIO_OType = GPIO_OType_PP; //push-pull used bc this is an LED, which is on/off
    LED_struct.GPIO_PuPd  = GPIO_PuPd_down;  //pull down is used bc the LED should be set to 0 when not called, not sure if this is correct
    LED_struct.GPIO_Speed = GPIO_Speed_50MHz; //fequency up to Dawson

    GPIO_Init(PORTD, &LED_struct);

    return 0;
}