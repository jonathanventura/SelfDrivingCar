//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
//#include "Defines.h"
//char auth[] = BLYNKAUTH;

#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <TinyGPS++.h>

#include "GPSManager.h"
#include "CompassManager.h"
#include "ServoPIDController.h"
#include "ESCController.h"

#define SERVO_PIN 9
#define ESC_PIN 10
#define GPS_RX_PIN 7

#define SERVO_PERIOD 100
#define SERVO_Kp 0.1

GPSManager gps_manager( GPS_RX_PIN );
CompassManager compass_manager;
ServoPIDController servo_controller( SERVO_PIN, SERVO_PERIOD, SERVO_Kp );
ESCController esc_controller( ESC_PIN );

// outputs
double gyro = 0;
double pwm = 0;

// inputs
int esc = 0;
int servo = 10;

void setup() {
//  Blynk.begin(auth, WIFINAME, WIFIPASS, BLYNKHOST );
  Wire.begin();
  Serial.begin( 9600 );

  esc_controller.setup();
  servo_controller.setup();
  compass_manager.setup();
  gps_manager.setup();
}

void loop() {
  gps_manager.loop();
  compass_manager.loop();

  pwm = servo_controller.get_pwm_value();
  
  if ( esc ) esc_controller.on();
  else esc_controller.off();
  
  servo_controller.set_setpoint( servo );

  servo_controller.loop( gyro );

  esc_controller.loop();

  double heading = compass_manager.get_heading();
  Serial.println(heading);
}


