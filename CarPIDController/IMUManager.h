
class RingBuffer
{
public:
  RingBuffer()
  : index(0)
  {
  }

  int get_index() { return index; }
  
  void insert(double val)
  {
    buf[index] = val;
    index = (index+1)%10;
  }

  double get_average()
  {
    double sum = 0;
    for ( int i = 0; i < 10; i++ ) sum += buf[i];
    return sum/10;
  }
private:
  double buf[10];
  int index;
};

class IMUManager
{
public:
  IMUManager()
  {
    
  }
  
  void setup()
  {
    // wake up the MPU 6050
    // set PWR_MGMT_1 register (0x6B) to zero
    Wire.begin();
    Wire.beginTransmission( 0x68 );
    Wire.write( 0x6B );  
    Wire.write( 0 );     
    Wire.endTransmission( true );
  
    // fill buffers
    while ( gyro_bias_buf.get_index() != 9 )
    {
      read_from_IMU();
      delay(10);
    }
  }

  void loop()
  {
    static long int last_sample_time = 0;
    
    if ( millis() - last_sample_time > 33 )
    {
      last_sample_time = millis();
       
      read_from_IMU();
    }
  }

  double get_gyro()
  {
    return gyro_bias_buf.get_average() - gyro_buf.get_average();
  }

//  double get_accel()
//  {
//    
//  }
private:
  void read_from_IMU()
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

    // write to ring buffers
    if ( fabs(angle) < 5.0 ) gyro_bias_buf.insert(angle);
    gyro_buf.insert(angle);
  }

  RingBuffer gyro_bias_buf;
  RingBuffer gyro_buf;
  
//  RingBuffer accel_bias_buf;
//  RingBuffer accel_buf;
};


