
#include <EEPROM.h>
#include <Servo.h>

Servo servo;

#define SERVO_PIN 9

#define SERVO_PERIOD 100

const double servo_Kp = 0.1;

// PID setpoint in degrees/sec (desired angular velocity)
double servo_setpoint = 0;

double servo_value = 90;
byte servo_min;
byte servo_zero;
byte servo_max;

void servo_setup()
{
  servo_min = EEPROM.read(0);
  servo_zero = EEPROM.read(1);
  servo_max = EEPROM.read(2);
  
  servo_value = servo_zero;
  
  servo.attach(SERVO_PIN);
  servo.write(servo_value);
}

void servo_set( double setpoint )
{
  servo_setpoint = setpoint;
}

void servo_loop()
{
  static long int last_time = 0;

  // PID update
  if ( millis() - last_time >= SERVO_PERIOD )
  {
    // request ang. vel. from gyroscope
    double angle = mpu6050_get_gyroZ();

    double pid_update = servo_Kp * ( angle - servo_setpoint );
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
    
  servo.write(servo_value);
}

