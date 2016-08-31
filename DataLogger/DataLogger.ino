
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>


#include <TinyGPS++.h>

#define GPS_RX_PIN D5
#define GPS_TX_PIN D6

SoftwareSerial gGPSSerial( GPS_RX_PIN, GPS_TX_PIN );
TinyGPSPlus gGPS;

#include "ConfigureGPS.h"

uint32_t gLastGPSTime = 0;

void start_gps()
{
  pinMode( GPS_RX_PIN, INPUT );
  if ( GPS_TX_PIN != 255 ) pinMode( GPS_TX_PIN, OUTPUT );
  gGPSSerial.begin( 9600 );

  if ( GPS_TX_PIN != 255 ) configureGPS();
}

bool read_gps()
{
  bool new_data = false;
  while ( gGPSSerial.available() ) {
    if ( gGPS.encode( gGPSSerial.read() ) ) new_data = true;
//    Serial.write( gGPSSerial.read() );
  }
  return new_data;
}

void start_imu()
{
  Wire.begin();
  
  // wake up the MPU 6050
  // set PWR_MGMT_1 register (0x6B) to zero
  Wire.beginTransmission( 0x68 );
  Wire.write( 0x6B );  
  Wire.write( 0 );     
  Wire.endTransmission( true );
}

void read_imu()
{
  // send request to device
  // starting with register 0x47 (GYRO_ZOUT_H)
  // request a total of 2 registers
  Wire.beginTransmission( 0x68 );
  Wire.write( 0x47 );  
  Wire.endTransmission( false );
  Wire.requestFrom( 0x68, 2, true );

  // read 16-bit value 
  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  int16_t gyroZ = Wire.read() << 8 | Wire.read();  

  // convert to degrees
  double angle = gyroZ/65.5;
}

void setup() {
  Serial.begin( 9600 );
  
  start_gps();
  //start_imu();
}

void loop() {
  bool new_gps_data = read_gps();
  //read_imu();

  if ( new_gps_data )
  {
    if ( !gGPS.location.isValid() ) { return; }
    if ( gLastGPSTime == gGPS.time.value() ) { return; }

    Serial.print(gGPS.time.value());
    Serial.print(",");
    Serial.print(gGPS.location.lat(),15);
    Serial.print(",");
    Serial.print(gGPS.location.lng(),15);
    Serial.print(",");
    Serial.print(gGPS.speed.mps(),15);
    Serial.print(",");
    Serial.print(gGPS.course.deg(),15);
    Serial.println("");
    

    gLastGPSTime = gGPS.time.value();
  }
}


