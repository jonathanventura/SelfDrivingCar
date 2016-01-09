
#include <Wire.h>

// I2C address of the MPU-6050
#define MPU_I2C_ADDR 0x68

double gyro_bias_thresh = 5.;

#define GYRO_BIAS_BUF_LEN 10
int gyro_bias_buf_index = 0;
double gyro_bias_buf[GYRO_BIAS_BUF_LEN];

#define GYRO_BUF_LEN 10
int gyro_buf_index = 0;
double gyro_buf[GYRO_BUF_LEN];

double mpu6050_read_from_IMU()
{
    // send request to device
    // starting with register 0x47 (GYRO_ZOUT_H)
    // request a total of 2 registers
    Wire.beginTransmission( MPU_I2C_ADDR );
    Wire.write( 0x47 );  
    Wire.endTransmission( false );
    Wire.requestFrom( MPU_I2C_ADDR, 2, true );
  
    // read 16-bit value 
    // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    int16_t gyroZ = Wire.read() << 8 | Wire.read();  
  
    // convert to degrees
    double angle = gyroZ/65.5;

    // write to ring buffers
    if ( fabs(angle) < gyro_bias_thresh )
    {
      gyro_bias_buf[gyro_bias_buf_index] = angle;
      gyro_bias_buf_index = (gyro_bias_buf_index+1)%GYRO_BIAS_BUF_LEN;
    }

    gyro_buf[gyro_buf_index] = angle;
    gyro_buf_index = (gyro_buf_index+1)%GYRO_BUF_LEN;
    
    return angle;
}

void mpu6050_setup()
{
  // wake up the MPU 6050
  // set PWR_MGMT_1 register (0x6B) to zero
  Wire.begin();
  Wire.beginTransmission( MPU_I2C_ADDR );
  Wire.write( 0x6B );  
  Wire.write( 0 );     
  Wire.endTransmission( true );

  LEDPIN_ON

  // fill buffers
  while ( gyro_bias_buf_index < GYRO_BIAS_BUF_LEN - 1 )
  {
    mpu6050_read_from_IMU();
    delay(10);
  }

  LEDPIN_OFF
}

void mpu6050_loop()
{
  static long int last_sample_time = 0;
  
  if ( millis() - last_sample_time > 33 )
  {
    last_sample_time = millis();
     
    mpu6050_read_from_IMU();
  }
}

double mpu6050_get_gyroZ()
{
  double bias_sum = 0;  
  for ( int i = 0; i < GYRO_BIAS_BUF_LEN; i++ ) bias_sum += gyro_bias_buf[i];
  
  double angle_sum = 0;
  for ( int i = 0; i < GYRO_BUF_LEN; i++ ) angle_sum += gyro_buf[i];

  return angle_sum / GYRO_BUF_LEN - bias_sum / GYRO_BIAS_BUF_LEN;
}

