#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rocket.h"

/*
    Init State -> Initializing the rocket, setting parameters, self testing sensors, calibrating servo position
*/

int mainInit(void){
    double servoPos;
    double sensorStart = 0;  //value that a particular sensor to have initially
    ////////////////////////////////////////////////////
    //////////Starting rocket, don't know what goes here
    ////////////////////////////////////////////////////
    servoPos = getServoPos(); //calls function that returns current servo 
    ///////////////////////////////////////////////////////////////////////////////
    // CODE to insruct servos to rotate to rotate to straight downwards orientation
    ///////////////////////////////////////////////////////////////////////////////
    sensorCalibrate(sensorStart); //function that calibrates sensor reading to what it should be

    return(0);
}
/*
    Awaiting State -> Checks values while drone is lifting rocket to final drop height
*/

int mainAwait(void){
    return(0);
}

/*
    Ascend State -> Checks values while drone is lifting rocket to final drop height
*/

int mainAscend(){
    double height;  //height the rocket is above the ground
    double maxHeight; //user input, hieght at which the rocket will be let go at

    while(height < maxHeight){
        height = //read height value from sensors
    }
    releaseRocket() //function that releases rocket
} 

/*
    Fall State
*/

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
      // Code to reduce timeTillCutoff - I presume the board will have a clock of somesort, so whatever the time is, minus the the time during the last loop, subtract from timeTillCutoff
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
  double zAcceleration = -9.5; // zAcceleration is whatever the sensors report the current acceleration towards the ground is - I don't know how the sensors drivers work so I don't know how to code this, ask Dawson - -9.5 is a placeholder

  if (zAcceleration < -9.5) { // If the acceleration is downwards at more than 9.5 m/s^2 (negative indicating downwards), return integer 0, meaning we are no long attached to the drone
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // BUZZER or LED Code - rocket needs to give off some indication that it now thinks it is falling, so that if it accidentally goes into this mode on the ground, we know it is armed
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return(0);
  }
  else // Else, return 1, saying we are attached to the drone
    return(1);
}
int checkIgnition(int burnTime, int avgAccel) {
  double zVelocity = -7; // Current downwards velocity in m/s, again from the sensors, again ask Dawson how to get that information piped over to here, -7 is a placeholder
  double currentAlt = 23.4; // Current altitude above the ground in m, from barometer measurements, 23.4 is placeholder
  double fallDist; // How far the rocket will fall if the engine were to be ignited right now

  fallDist = (zVelocity * burnTime) + (0.5 * avgAccel * burnTime ^ 2.0); // Calculates the value fo fallDist
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

  double pitchAngle = 0.2; // Gyroscope measurement of current pitch angle in radians, 0.2 is placeholder
  double pitchRate = -0.04; // Gyroscope measruement of current pitch angle rate of change in rad/s, -0.04 is placeholder

  double yawAngle = 0.2; // Gyroscope measurement of current yaw angle in radians, 0.2 is placeholder
  double yawRate = -0.04; // Gyroscope measruement of current yaw angle rate of change in rad/s, -0.04 is placeholder

  double pitchBeta; // Desired gimbal angle in the pitch axis
  double yawBeta; // Desired gimbal angle in the yaw axis

  pitchBeta = (propGain * pitchAngle) + (derivGain * pitchRate); // Calculated desired gimbal pitch angle
  yawBeta = (propGain * yawAngle) + (derivGain * yawRate); // Calculated desired gimbal yaw angle

  ///////////////////////////////////////////////////////////////////////////////
  // CODE to insruct servos to rotate to the determined angle as fast as possible
  ///////////////////////////////////////////////////////////////////////////////

  return();
}

/*
    Land State
*/

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
    enum rocketCurrentState = 0;
    while(1 == 1)
    {
        switch(rocket.state)
        {
            case INIT:
                mainInit();
                rocketCurrentState++;

                break;
            case AWAITING:
                mainAwait();
                rocketCurrentState++;

                break;
            case ASCEND:
                mainAscend();
                rocketCurrentState++;

                break;
            case FALL:
                mainFall();
                rocketCurrentState++;

                break;
            case LAND:
                mainLand();
                rocketCurrentState++;

                break;
            case ERROR:

                break;
        }
    }
}
