
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <EEPROM.h>

#include "ESCController.h"
#include "GPSManager.h"

#define LEDPIN_PINMODE             DDRD |= (1<<4);            //D4 to output
#define LEDPIN_TOGGLE              PIND |= (1<<5)|(1<<4);     //switch LEDPIN state (Port D5) & pin D4
#define LEDPIN_OFF                 PORTD |= (1<<5); PORTD &= ~(1<<4);
#define LEDPIN_ON                  PORTD &= ~(1<<5); PORTD |= (1<<4);  

#define SERVO_PIN 9
#define ESC_PIN 10
#define GPS_RX_PIN 6

ESCController esc_controller( ESC_PIN );
GPSManager gps_manager( GPS_RX_PIN );

void setup() {
  LEDPIN_PINMODE

  Serial.begin(115200);
  
  esc_controller.setup();
  gps_manager.setup();
  
//  waypoint_setup();
//  mpu6050_setup();
//  servo_setup();
}

void loop() {
  gps_manager.loop();
  
  esc_controller.loop();
  
//  waypoint_loop();
//  mpu6050_loop();
//  servo_loop();
}
