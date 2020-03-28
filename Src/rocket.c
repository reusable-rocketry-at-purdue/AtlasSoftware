/*
 *  rocket.c -> Created by: Dawson Moore
 *                      on: 2/11/2020
 */

#include "rocket.h"

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
void rocketTask()
{
  while (RRAP == GREAT)
  {
    switch (rocket.state)
    {
      case (INIT):
      
        break;
        
      case (AWAITING):
      
        break;
      
      case (ASCEND):
      
        break;
        
      case (FALL):
       
        break;
        
      case (LAND):
      
        break;
        
      case (ERROR):
      
        break;
    }
  }
}
