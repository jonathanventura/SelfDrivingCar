
#include <EEPROM.h>
#include <Servo.h>

Servo motor;

#define MOTOR_PIN 10

byte motor_value = 90;
byte motor_min;
byte motor_zero;
byte motor_max;

void motor_setup()
{
  motor_min = EEPROM.read(3);
  motor_zero = EEPROM.read(4);
  motor_max = EEPROM.read(5);
  
  motor.attach(MOTOR_PIN);
  
  motor_value = motor_zero;
  motor.write(motor_value);
}

void motor_on()
{
  motor_value = motor_max;
}

void motor_off()
{
  motor_value = motor_zero;
}

void motor_loop()
{
  motor.write(motor_value);
}

