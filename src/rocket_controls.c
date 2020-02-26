#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//Init State -> Initializing the rocket, setting parameters, self testing sensors, calibrating servo position
int mainInit(void){
    initAccel();
    barometerInit();
    gpsInit();
    imuInit();

    return(0);
}

//Awaiting State -> Time after initialization prior to "launch"
int mainAwait(void){
    return(0);

}


//Ascend State -> Checks values while drone is lifting rocket to final drop height
int mainAscend(){
    int maxAlt = 100; //placeholder for maximum altitude rocket has to reach before dropping
    if( rocket.alt >= maxAlt ){
        rocket.state += 1;

    }
} 

//Fall Statez

void engineDeflectCalc();

int main(void){
    double burnTime = 2.3; // Predetermined value for how long the engine will burn for in seconds - 2.3 is a placeholder value
    double avgAccel = 6.4; // Predetermined vlaue for the average acceleration the rocket will undergo during the burn phase - 6.4 is a placeholder value

    double fallDist; // How far the rocket will fall if the engine were to be ignited right now

    double timeTillCutOff = burnTime; // How much time there is left for the engine to burn, starts to count down when the engine is ignited

    while(rocket.accelZ < -9.5){
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // BUZZER or LED Code - rocket needs to give off some indication that it now thinks it is falling, so that if it accidentally goes into this mode on the ground, we know it is armed
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
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

  pitchBeta = (propGain * rocket.pitchAngle) + (derivGain * rocket.pitchVelo); // Calculated desired gimbal pitch angle
  yawBeta = (propGain * rocket.yawAngle) + (derivGain * rocket.yawVelo); // Calculated desired gimbal yaw angle

  ///////////////////////////////////////////////////////////////////////////////
  // CODE to insruct servos to rotate to the determined angle as fast as possible
  ///////////////////////////////////////////////////////////////////////////////

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

void rocketTask()
{
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
