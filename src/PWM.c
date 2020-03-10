#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_pwm.h"

// https://stm32f4-discovery.net/2014/09/library-33-pwm-stm32f4xx/

typedef enum {
    TM_PWM_Result_Ok = 0,
    TM_PWM_Result_FrequencyTooHigh,
    TM_PWM_Result_FrequencyTooLow,
    TM_PWM_Result_PulseTooHigh,
    TM_PWM_Result_TimerNotValid,
    TM_PWM_Result_ChannelNotValid,
    TM_PWM_Result_PinNotValid
} TM_PWM_Result_t;
 
typedef struct {
    TIM_TypeDef* TIM;
    uint32_t Period;
    uint32_t Prescaler;
    uint32_t Frequency;
    uint32_t Micros;
} TM_PWM_TIM_t;
 
/**
 * Channel selected for PWM on specific timer
 *
 */
typedef enum {
    TM_PWM_Channel_1 = 0,
    TM_PWM_Channel_2,
    TM_PWM_Channel_3,
    TM_PWM_Channel_4
} TM_PWM_Channel_t;
 
/**
 * Pin selected for corresponding channel on specific channel
 *
 */
typedef enum {
    TM_PWM_PinsPack_1 = 0,
    TM_PWM_PinsPack_2,
    TM_PWM_PinsPack_3
} TM_PWM_PinsPack_t;
 
extern TM_PWM_Result_t TM_PWM_InitTimer(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, double PWMFrequency);
 
extern TM_PWM_Result_t TM_PWM_InitChannel(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
 
extern TM_PWM_Result_t TM_PWM_SetChannel(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t Pulse);
 
extern TM_PWM_Result_t TM_PWM_SetChannelPercent(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, float percent);
 
extern TM_PWM_Result_t TM_PWM_SetChannelMicros(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t micros);

void initPWM(){
    //initializes two timers for PWMs for both servos, placeholders
    TM_PWM_TIM_t TIM4_Data, TIM2_Data; 

    TM_DELAY_Init();
    // Set PWM to 50Hz frequency on timer TIM2 , both timer and frequency are placeholders
    TM_PWM_InitTime(TIM2, &TIM2_data, 50); 
    TM_PWM_InitTime(TIM4, &TIM4_data, 50); 
    // Initialize PWM on TIM2, Channel 1 and PinsPack 2 = PA5, once again, placeholders
    TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
    TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_2);
    // Set channel 1 value, 1500us = servo at center position */
    TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, 1500); 
    TM_PWM_SetChannelMicros(&TIM4_Data, TM_PWM_Channel_2, 1500); 
    
}