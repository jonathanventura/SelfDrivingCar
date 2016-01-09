
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <EEPROM.h>
#include <Wire.h>

#include "GPSManager.h"
#include "IMUManager.h"

#define LEDPIN_PINMODE             DDRD |= (1<<4);            //D4 to output
#define LEDPIN_TOGGLE              PIND |= (1<<5)|(1<<4);     //switch LEDPIN state (Port D5) & pin D4
#define LEDPIN_OFF                 PORTD |= (1<<5); PORTD &= ~(1<<4);
#define LEDPIN_ON                  PORTD &= ~(1<<5); PORTD |= (1<<4);  

#define GPS_RX_PIN 6
#define SERVO_PIN 9
#define ESC_PIN 10

#define SERVO_PERIOD 100
#define SERVO_Kp 0.1

GPSManager gps_manager( GPS_RX_PIN );
IMUManager imu_manager;

Servo servo;
double servo_setpoint;
double servo_value;
byte servo_min;
byte servo_zero;
byte servo_max;

Servo esc;
byte esc_value;
byte esc_min;
byte esc_zero;
byte esc_max;

void setup() {
  LEDPIN_PINMODE

  Serial.begin(115200);
  
  gps_manager.setup();
  imu_manager.setup();

  // servo setup
  servo_min = EEPROM.read(0);
  servo_zero = EEPROM.read(1);
  servo_max = EEPROM.read(2);
  esc_min = EEPROM.read(3);
  esc_zero = EEPROM.read(4);
  esc_max = EEPROM.read(5);

  servo_value = servo_zero;
  esc_value = esc_zero;
  
  servo.attach( SERVO_PIN );
  servo.write( servo_value );

  esc.attach( ESC_PIN );
  esc.write( esc_value );
}

void loop() {
  gps_manager.loop();
  imu_manager.loop();

  servo_pid_loop();
}

void servo_pid_loop()
{
    static long int last_time = 0;
  
    // PID update
    if ( millis() - last_time >= SERVO_PERIOD )
    {
      // request ang. vel. from gyroscope
      double gyro = imu_manager.get_gyro();
  
      double pid_update = SERVO_Kp * ( gyro - servo_setpoint );
      double new_value = (double)servo_value + pid_update;
      new_value = (new_value>servo_max)?servo_max:new_value;
      new_value = (new_value<servo_min)?servo_min:new_value;
  
      servo_value = new_value;
  
  //    Serial.println(angle);
  //    Serial.print( (angle - servo_setpoint) );
  //    Serial.print(" ");
  //    Serial.print( pid_update );
  //    Serial.print(" ");
  //    Serial.println(servo_value-servo_zero);
      
      last_time = millis();
    }
  
    static long int last_setpoint_time = 0;
    static double setpoint_increment = -1;
    if ( millis() - last_setpoint_time >= SERVO_PERIOD*10 )
    {
       servo_setpoint += setpoint_increment;
       if ( servo_setpoint > 10 || servo_setpoint < -10 ) setpoint_increment = -setpoint_increment;
       last_setpoint_time = millis();
    }
}


