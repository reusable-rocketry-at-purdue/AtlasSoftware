#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    //state rocket is in for state control
    int state; 
    //variables updated by accelTask
    int accel;
    //variables updated by barameterTask
    int deltaT; 
    int temp;
    int off;
    int sense;
    int pressure;

}rocket_t;
typedef struct{
  //variables updated by imu
    int accelX;
    int accelY;
    int accelZ;
    int gyroX;
    int gyroY;
    int gyroZ;

}imu_t;

extern rocket_t rocket;
extern imu_t imu;

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
    int maxPressure = 100; //placeholder for maximum height rocket has to reach before dropping
    if( rocket.pressure >= maxPressure ){
        rocket.state += 1;

    }
} 

//Fall Statez
int checkFall();
int checkIgnition(double, double);
void engineDeflectCalc();

int mainFall(void) {
  int droneAttached = 1; // Logical Value, is rocket attached to the drone still?
  int motorIgnited = 0; // Logical Value, has the motor been ignited?
  int burnComplete = 0; // Logical Value, has the motor run out of propellant?
  int runLoop = 1; // Logical Value, should I run the control loop? (setting this to 0 terminates this program)

  double burnTime = 2.3; // Predetermined value for how long the engine will burn for in seconds - 2.3 is a placeholder value
  double avgAccel = 6.4; // Predetermined vlaue for the average acceleration the rocket will undergo during the burn phase - 6.4 is a placeholder value

  double timeTillCutoff = burnTime; // How much time there is left for the engine to burn, starts to count down when the engine is ignited

  while (runLoop == 1) {
    if (droneAttached == 1)
      droneAttached = checkFall();
    
    if (droneAttached == 0 && motorIgnited == 0)
      motorIgnited = checkIgnition(burnTime, avgAccel);

    if (droneAttached == 0 && motorIgnited == 1 && burnComplete == 0) {
      engineDeflectCalc();
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Code to reduce timeTillCutoff - I presume the board will have a clock of some sort, so whatever the time is, minus the the time during the last loop, subtract from timeTillCutoff
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (timeTillCutoff < 0)
        burnComplete = 1;
    }
    
    if (droneAttached == 0 && motorIgnited == 1 && burnComplete == 1)
      runLoop = 0;
  }
  return(0);
}
int checkFall() {
  if (rocket.accel < -9.5) { // If the acceleration is downwards at more than 9.5 m/s^2 (negative indicating downwards), return integer 0, meaning we are no long attached to the drone
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // BUZZER or LED Code - rocket needs to give off some indication that it now thinks it is falling, so that if it accidentally goes into this mode on the ground, we know it is armed
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return(0);
  }
  else // Else, return 1, saying we are attached to the drone
    return(1);
}
int checkIgnition(int burnTime, int avgAccel) {
  double currentAlt = 23.4; // Current altitude above the ground in m, from barometer measurements, 23.4 is placeholder
  double fallDist; // How far the rocket will fall if the engine were to be ignited right now

  fallDist = (rocket.accel * burnTime) + (0.5 * avgAccel * burnTime ^ 2.0); // Calculates the value fo fallDist
  if (fallDist > currentAlt) { // If the current altitude is a shorter distance than how far the rocket will fall during its burn, return 1, indicating that the engine should be started
    ///////////////////////
    // Engine Ignition Code
    ///////////////////////
    return(1);
  }
  else
    return(0);
}
void engineDeflectCalc(); {
  double propGain = 1; // Gain factor for proportion measurements, 1 is placeholder
  double derivGain = 1; // Gain factor for derivative measurements, 1 is placeholder

  double pitchBeta; // Desired gimbal angle in the pitch axis
  double yawBeta; // Desired gimbal angle in the yaw axis
  
  pitchBeta = (propGain * imu.gyroX) + (derivGain * imu.accelX); // Calculated desired gimbal pitch angle
  yawBeta = (propGain * imu.gyroY) + (derivGain * imu.accelY); // Calculated desired gimbal yaw angle

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
enum rocketState {INIT, AWAITING, ASCEND, FALL, LAND, ERROR}

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
