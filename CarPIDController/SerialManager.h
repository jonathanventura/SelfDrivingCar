
class SerialManager
{
public:
  SerialManager()
  : last_send_time_(0)
  {
    
  }
  
  void setup()
  {
    Serial.begin(115200);
  }

  void loop( int gyro, int pwm, int &esc, int &servo )
  {
    if ( Serial.available() )
    {
      // format:
      // esc (int16)
      // servo (int16)
      Serial.readBytes((byte*)serial_buf_,4);
      esc = serial_buf_[0];
      servo = serial_buf_[1];
    }

    if ( millis() - last_send_time_ > 1000 )
    {
      // format:
      // gyro (int16)
      // pwm (int16)
      serial_buf_[0] = gyro;
      serial_buf_[1] = pwm;
      Serial.write((byte*)serial_buf_,4);
      
      last_send_time_ = millis();
    }
  }
private:
  long int last_send_time_;
  int serial_buf_[2];
};



