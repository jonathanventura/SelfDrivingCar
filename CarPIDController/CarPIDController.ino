
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <EEPROM.h>
#include <Wire.h>

#define LEDPIN_PINMODE             DDRD |= (1<<4);            //D4 to output
#define LEDPIN_TOGGLE              PIND |= (1<<5)|(1<<4);     //switch LEDPIN state (Port D5) & pin D4
#define LEDPIN_OFF                 PORTD |= (1<<5); PORTD &= ~(1<<4);
#define LEDPIN_ON                  PORTD &= ~(1<<5); PORTD |= (1<<4);  

#include "GPSManager.h"
#include "IMUManager.h"
#include "ServoPIDController.h"
#include "ESCController.h"

#define GPS_RX_PIN 6
#define SERVO_PIN 9
#define ESC_PIN 10

#define SERVO_PERIOD 100
#define SERVO_Kp 0.1

GPSManager gps_manager( GPS_RX_PIN );
IMUManager imu_manager;
ServoPIDController servo_controller( SERVO_PIN, SERVO_PERIOD, SERVO_Kp );
ESCController esc_controller( ESC_PIN );

void setup() {
  LEDPIN_PINMODE

  Serial.begin(115200);
  
  esc_controller.setup();
  servo_controller.setup();
  
  gps_manager.setup();
  imu_manager.setup();
}

void loop() {
  gps_manager.loop();
  imu_manager.loop();

  double gyro = imu_manager.get_gyro();
  servo_controller.loop( gyro );

  esc_controller.loop();

  static long int last_report_time = 0;
  if ( last_report_time - millis() > 100 )
  {
    Serial.println( gyro );
    last_report_time = millis();
  }
}


