#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

#define LEDPIN_PINMODE             DDRD |= (1<<4);            //D4 to output
#define LEDPIN_TOGGLE              PIND |= (1<<5)|(1<<4);     //switch LEDPIN state (Port D5) & pin D4
#define LEDPIN_OFF                 PORTD |= (1<<5); PORTD &= ~(1<<4);
#define LEDPIN_ON                  PORTD &= ~(1<<5); PORTD |= (1<<4);  

#include <Firmata.h>

//#include <EEPROM.h>

//#include <SoftwareSerial.h>
//#include <TinyGPS++.h>
//#include "GPSManager.h"

#include "IMUManager.h"
#include "ServoVelocityController.h"
#include "ESCController.h"

#include "Button.h"

#define BUTTON_PIN 7
#define SERVO_PIN 9
#define ESC_PIN 10

#define SERVO_Kp 10

Button button( BUTTON_PIN );

IMUManager imu_manager;
//GPSManager gps_manager( GPS_RX_PIN, GPS_TX_PIN );
ServoVelocityController servo_controller( SERVO_PIN, SERVO_Kp );
ESCController esc_controller( ESC_PIN );

void analogWriteCallback(byte pin, int value)
{
//  if ( pin == SERVO_PIN ) servo_controller.set_desired_velocity( value-90 );
  for ( int i = 0; i < value; i++ ) {
    LEDPIN_ON
    delay(500);
    LEDPIN_OFF
    delay(500);
  }
}

void stringCallback(char *msg){
  int value = String(msg).toInt();
  for ( int i = 0; i < value; i++ ) {
    LEDPIN_TOGGLE
    delay(1000);
  }
}

void setup() {
  Wire.begin();

  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.attach(STRING_DATA, stringCallback);
  Firmata.begin(57600);
//  Serial.begin(57600);

  LEDPIN_PINMODE
  
  button.setup();

  imu_manager.setup();
  
//  esc_controller.setup();
  servo_controller.setup();
//  gps_manager.setup();

}

void loop() {
  while (Firmata.available())
    Firmata.processInput();

  button.loop();

//  bool new_gps_data = gps_manager.loop();
  
//  if ( esc ) esc_controller.on();
//  else esc_controller.off();

//  if ( new_gps_data )
//  {
//    // update waypoint 
//    if ( gps_manager.distanceTo( waypoints[0], waypoints[1] ) < 5 )
//    {
////      waypoint++;
//    }
//    
//    if ( gps_manager.getHeading() != 0 )
//    {
//      double course = gps_manager.courseTo( waypoints[0], waypoints[1] );
//      servo_controller.update( gps_manager.getHeading(), course );
//    } else {
//      servo_controller.off();
//    }

    
//  }
//

  if ( imu_manager.loop() ) {
    servo_controller.update( imu_manager.get_gyro() );
  }
  
  servo_controller.loop();
  
//  esc_controller.loop();
}


