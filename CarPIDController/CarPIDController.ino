
#include "ESCController.h"
#include "ServoController.h"

#include "GPSManager.h"
#include "IMUManager.h"
#include "WaypointManager.h"

#define LEDPIN_PINMODE             DDRD |= (1<<4);            //D4 to output
#define LEDPIN_TOGGLE              PIND |= (1<<5)|(1<<4);     //switch LEDPIN state (Port D5) & pin D4
#define LEDPIN_OFF                 PORTD |= (1<<5); PORTD &= ~(1<<4);
#define LEDPIN_ON                  PORTD &= ~(1<<5); PORTD |= (1<<4);  

void setup() {
  LEDPIN_PINMODE

  Serial.begin(115200);
  
  gps_setup();
  waypoint_setup();
  mpu6050_setup();
  motor_setup();
  servo_setup();
}

void loop() {
  gps_loop();
  waypoint_loop();
  mpu6050_loop();
  motor_loop();
  servo_loop();
}
