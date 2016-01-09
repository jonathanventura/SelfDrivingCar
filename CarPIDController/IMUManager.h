
class RingBuffer
{
public:
  RingBuffer()
  : index_(0)
  {
  }

  int get_index() { return index_; }
  
  void insert( double val )
  {
    buf_[index_] = val;
    index_ = (index_+1)%10;
  }

  double get_average()
  {
    double sum = 0;
    for ( int i = 0; i < 10; i++ ) sum += buf_[i];
    return sum/10;
  }
private:
  double buf_[10];
  int index_;
};

class IMUManager
{
public:
  IMUManager()
  : last_sample_time_(0)
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

    LEDPIN_ON
    
    // fill buffers
    while ( gyro_bias_buf_.get_index() != 9 )
    {
      read_from_IMU();
      delay(10);
    }

    LEDPIN_OFF
  }

  void loop()
  {
    if ( millis() - last_sample_time_ > 33 )
    {
      last_sample_time_ = millis();
       
      read_from_IMU();
    }
  }

  double get_gyro()
  {
    return gyro_bias_buf_.get_average() - gyro_buf_.get_average();
  }
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
    if ( fabs(angle) < 5.0 ) gyro_bias_buf_.insert(angle);
    gyro_buf_.insert(angle);
  }

  long int last_sample_time_;

  RingBuffer gyro_bias_buf_;
  RingBuffer gyro_buf_;
};


