#include <Arduino.h>

#include "Button.h"
#include "CheckpointMonitor.h"
#include "Gaussian.h"
#include "RobotSimulator.h"

#define BUTTON_PIN 17

// x, y, radius
// last checkpoint should have radius = 0
float gCheckpoints[] = {
  // straightaway
  148,  0,  10,
  // first turn
  148, 55,  10,
  // second turn
   80, 55,  10,
   // third turn
   80, 118, 10,
   // fourth turn
    0, 118, 10,
    // hairpins
    0,  65, 10,
    0,  55, 1,
   10,  55, 1,
   10,  25, 1,
    0,  25, 1,
    0,  0,  1,
      0, 0, 0
};

const float gMaxVelocity = 10.f;
const float gMaxAngularVelocity = 90.*M_PI/180.f;

Button gButton( BUTTON_PIN );
CheckpointMonitor gCheckpointMonitor( gCheckpoints );
RobotSimulator gRobotInterface;
//CarInterface gRobotInterface;
//DiffDriveInterface gRobotInterface;

float gX = 0;
float gY = 0;
float gHeading = 0;

void setup() {
  Serial.begin( 9600 );

  gButton.setup();
}

void loop() {
  gButton.loop();
  
  if ( gButton.didPress() ) {
    delay(1000);
    gCheckpointMonitor.start();
  }

  if ( gCheckpointMonitor.isRunning() )
  {
    // check for position update
    if ( gRobotInterface.loop() )
    {
      // Update checkpoint and current distances
      gCheckpointMonitor.updateCheckpoint( gRobotInterface.getX(), gRobotInterface.getY(), gRobotInterface.getHeading() );

      // Calculate desired velocity
      float desired_velocity = gCheckpointMonitor.getRadius();
      
      // Calculate desired angular velocity
      // PID controller with kP = 1, kI = 0, kD = 0.
      float desired_angular_velocity = -gCheckpointMonitor.getHeadingError();
      if ( desired_angular_velocity > gMaxAngularVelocity ) desired_angular_velocity = gMaxAngularVelocity;
      if ( desired_angular_velocity < -gMaxAngularVelocity ) desired_angular_velocity = -gMaxAngularVelocity;

      // Set desired velocities
      gRobotInterface.setDesiredVelocity( desired_velocity );
      gRobotInterface.setDesiredAngularVelocity( desired_angular_velocity );
  
//    Serial.print( gCheckpointMonitor.getX() );
//    Serial.print( "\t" );
//    Serial.print( gCheckpointMonitor.getY() );
//    Serial.print( "\t" );
//    Serial.print( gCheckpointMonitor.getDistance() );
//    Serial.print( "\t" );
//    Serial.print( gCheckpointMonitor.getHeadingError() );
//    Serial.print( "\t" );
      Serial.print( gRobotInterface.getX() );
      Serial.print( "\t" );
      Serial.print( gRobotInterface.getY() );
//    Serial.print( "\t" );
//    Serial.print( gRobotInterface.getHeading() );
      Serial.print( "\n" );
    }
  }
}


