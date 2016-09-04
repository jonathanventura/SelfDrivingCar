//#define USE_ESP8266

#ifdef USE_ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const char *ssid = "ClydesRide";
#endif

#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>

#include "Button.h"
#include "CommandController.h"

#ifdef USE_ESP8266
#define BUTTON_PIN 0
#define STEERING_PIN D5
#define THROTTLE_PIN D6
#else
#define BUTTON_PIN 7
#define STEERING_PIN 9
#define THROTTLE_PIN 10

#define THROTTLE_ZERO 100
#define STEERING_ZERO 100
#endif

// RC10T steering range: min 70 zero 100 max 130

// throttle, servo, duration
// last command should have duration = 0
long gCommands[] = {
  100,  70, 1000,
  100, 130, 1000,
  100, 100,    0
};

Button gButton( BUTTON_PIN );
CommandController gController( gCommands, THROTTLE_ZERO, STEERING_ZERO );

Servo gThrottle;
Servo gSteering;

void setup() {
  Wire.begin();
  Serial.begin( 9600 );

#ifdef USE_ESP8266
  WiFi.softAP(ssid);
#endif

//  pinMode( THROTTLE_PIN, OUTPUT );
//  gThrottle.attach( THROTTLE_PIN );
//  gThrottle.write( 90 );

  pinMode( STEERING_PIN, OUTPUT );
  gSteering.attach( STEERING_PIN );
  gSteering.write( 90 );

  gButton.setup();
}

void loop() {
  gButton.loop();
  
  if ( gButton.didPress() ) {
    delay(1000);
    gController.start();
  }

  gController.loop();

  if ( gController.isRunning() )
  {
    Serial.print( gController.getThrottle() );
    Serial.print( "\t" );
    Serial.print( gController.getSteering() );
    Serial.print( "\n" );
  }

//  gThrottle.write( gController.getThrottle() );
  gSteering.write( gController.getSteering() );
}


