#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_servo.h"

#define servo_MAX_pos 2000
#define servo_MIN_pos 1000

typedef struct{
    //state rocket is in for state control
    int state; 
    //
    int accelZ;
    int veloZ;
    //variables updated by barameterTask
    int deltaT; 
    int temp;
    int off;
    int sense;
    int pressure;
    //
    int alt;
    //
    int pitchAngle;
    int yawAngle;
    int pitchVelo;
    int yawVelo;

}rocket_t;

extern rocket_t rocket;

void initRTOSObjects()
{
    QueueHandle_t tx_queue = xQueueCreate(40, sizeof(CanTxMsgTypeDef)); //Change 40 to defined constant
    QueueHandle_t rx_queue = xQueueCreate(40, sizeof(CanTxMsgTypeDef));
    
    xTaskCreate(accelTask, "Accelerometer Driver", 256, NULL, 1, NULL); //Change 256 and 1 to defines and determine proper values
    xTaskCreate(barometerTask, "Barometer Driver", 256, NULL, 1, NULL);
    xTaskCreate(gpsTask, "GPS Driver", 256, NULL, 1, NULL);
    xTaskCreate(imuTask, "Imu Driver", 256, NULL, 1, NULL);
    xTaskCreate(rocketTask, "Main State Machine", 256, NULL, 1, NULL);
}
int mainInit(void){
    initAccel();
    barometerInit();
    gpsInit();
    imuInit();

    return(0);
}
int mainAwait(void){
    return(0);

}
int mainAscend(){
    int maxAlt = 100; //placeholder for maximum altitude rocket has to reach before dropping
    if( rocket.alt >= maxAlt ){
        rocket.state += 1;

    }
} 
void engineDeflectCalc();

int mainDrop(void){
    double burnTime = 2.3; // Predetermined value for how long the engine will burn for in seconds - 2.3 is a placeholder value
    double avgAccel = 6.4; // Predetermined vlaue for the average acceleration the rocket will undergo during the burn phase - 6.4 is a placeholder value

    double fallDist; // How far the rocket will fall if the engine were to be ignited right now

    double timeTillCutOff = burnTime; // How much time there is left for the engine to burn, starts to count down when the engine is ignited

    while(rocket.accelZ < -9.5){
        FM_LED_Toggle(0);   //should light up LED

    }
    do{
        fallDist = (rocket.veloZ * burnTime) + (0.5 * avgAccel * burnTime ^ 2.0); // Calculates the value fo fallDist
  
    }while( fallDist < rocket.alt);
    ///////////////////////
    // Engine Ignition Code
    ///////////////////////

    while(timeTillCutOff > 0){
       engineDeflectCalc(); 

    }

    return(0);
}

void engineDeflectCalc() {
  double propGain = 1; // Gain factor for proportion measurements, 1 is placeholder
  double derivGain = 1; // Gain factor for derivative measurements, 1 is placeholder

  double pitchBeta; // Desired gimbal angle in the pitch axis
  double yawBeta; // Desired gimbal angle in the yaw axis
  
  int pitchPosition; //servo position for servo that controls pitch direction
  int yawPosition; //servo position for servo that controls yaw direction

  pitchBeta = (propGain * rocket.pitchAngle) + (derivGain * rocket.pitchVelo); // Calculated desired gimbal pitch angle
  yawBeta = (propGain * rocket.yawAngle) + (derivGain * rocket.yawVelo); // Calculated desired gimbal yaw angle

  pitchPosition = 1500 + pitchBeta * (500/90);
  yawPosition   = 1500 + yawBeta * (500/90);

  TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, pitchPosition);
  TM_PWM_SetChannelMicros(&TIM4_Data, TM_PWM_Channel_2, yawPosition);

  return();
}

//Land State
int landMain(){
    return(0);
}
/*
 * rocketTask()
 *
 * Finite state machine that computes and adjusts all sensor values and gimbal positioning.
 *
 * States:
 * INIT -> Initializing the rocket, setting parameters, self testing sensors, calibrating servo position
 * AWAITING -> Time after initialization prior to "launch"
 * ASCEND -> Checks values while drone is lifting rocket to final drop height
 * FALL -> Calculates the proper angle to gimbal the motor
 * LAND -> Powers down sensors and enters low power mode
 * ERROR -> Handles cases when parameter values exceed proper limits
 */
enum rocketState {INIT, AWAITING, ASCEND, FALL, LAND, ERROR};

void main()
{
    TM_SERVO_t pitchServo, yawServo;

    TM_DELAY_Init();
    TM_SERVO_Init(&pitchServo, TIM2, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
    TM_SERVO_Init(&yawServo, TIM2, TM_PWM_Channel_2, TM_PWM_PinsPack_2);
 
    rocket.state = 0;

    while(1 == 1)
    {
        switch((rocket.state))
        {
            case INIT:
                mainInit();
                rocket.state++;

                break;
            case AWAITING:
                mainAwait();
                rocket.state++;

                break;
            case ASCEND:
                mainAscend();
                //rocket.state is incremented in mainAscend function when altitude exceeds target starting altitude

                break;
            case FALL:
                mainFall();
                rocket.state++;

                break;
            case LAND:
                mainLand();
                rocket.state++;

                break;
            case ERROR:

                break;
        }
    }
}
