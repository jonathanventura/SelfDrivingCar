
#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <TinyGPS++.h>

#define LEDPIN_PINMODE             DDRD |= (1<<4);            //D4 to output
#define LEDPIN_TOGGLE              PIND |= (1<<5)|(1<<4);     //switch LEDPIN state (Port D5) & pin D4
#define LEDPIN_OFF                 PORTD |= (1<<5); PORTD &= ~(1<<4);
#define LEDPIN_ON                  PORTD &= ~(1<<5); PORTD |= (1<<4);  

#include "GPSManager.h"
#include "IMUManager.h"
#include "ServoPIDController.h"
#include "ESCController.h"
#include "SerialManager.h"

#define GPS_RX_PIN 6
#define SERVO_PIN 9
#define ESC_PIN 10

#define SERVO_PERIOD 100
#define SERVO_Kp 0.1

GPSManager gps_manager( GPS_RX_PIN );
IMUManager imu_manager;
ServoPIDController servo_controller( SERVO_PIN, SERVO_PERIOD, SERVO_Kp );
ESCController esc_controller( ESC_PIN );
SerialManager serial_manager;

// outputs
double gyro = 0;
double pwm = 0;

// inputs
int esc = 0;
int servo = 0;

void setup() {
  LEDPIN_PINMODE

  esc_controller.setup();
  servo_controller.setup();
  
  gps_manager.setup();
  imu_manager.setup();

  serial_manager.setup();
}

void loop() {
  gps_manager.loop();
  imu_manager.loop();
  gyro = imu_manager.get_gyro();
  pwm = servo_controller.get_pwm_value();
  
  serial_manager.loop( gyro, pwm, esc, servo );

  if ( esc ) esc_controller.on();
  else esc_controller.off();
  
  servo_controller.set_setpoint( servo );

  servo_controller.loop( gyro );

  esc_controller.loop();
}


