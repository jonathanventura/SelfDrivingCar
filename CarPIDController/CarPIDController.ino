#define USE_ESP8266

#ifdef USE_ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const char *ssid = "ClydesRide";

#include <aREST.h>


#endif

#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <TinyGPS++.h>

#include "GPSManager.h"
#include "ServoPIDController.h"
#include "ESCController.h"

#ifndef USE_ESP8266
#include "Button.h"
#endif

#ifdef USE_ESP8266
#define SERVO_PIN 5
#define ESC_PIN 16
#define GPS_RX_PIN 7
#define GPS_TX_PIN 8
#else
#define BUTTON_PIN 7
#define SERVO_PIN 9
#define ESC_PIN 10
#define GPS_RX_PIN 5
#define GPS_TX_PIN 255
#endif

#define SERVO_PERIOD 100
#define SERVO_Kp 0.1

#ifndef USE_ESP8266
Button button( BUTTON_PIN );
#endif
GPSManager gps_manager( GPS_RX_PIN, GPS_TX_PIN );
ServoPIDController servo_controller( SERVO_PIN, SERVO_Kp );
ESCController esc_controller( ESC_PIN );

double waypoints[32];
int current_waypoint = 0;

void setup() {
  Wire.begin();
  Serial.begin( 9600 );

#ifdef USE_ESP8266
  WiFi.softAP(ssid);
#endif

#ifndef USE_ESP8266
//  button.setup();
#endif
//  esc_controller.setup();
//  servo_controller.setup();
//  gps_manager.setup();

}

void loop() {
#ifndef USE_ESP8266
  button.loop();
#endif

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
//  servo_controller.loop();
  
//  esc_controller.loop();
}


