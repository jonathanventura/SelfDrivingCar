
class ServoPIDController
{
public:
  ServoPIDController( int pin, int period, double Kp )
  : pin_(pin), setpoint_(0), period_(period), last_time_(0), Kp_(Kp)
  {
    
  }
  
  void setup()
  {
    min_ = EEPROM.read(0);
    zero_ = EEPROM.read(1);
    max_ = EEPROM.read(2);

    value_ = zero_;

    servo_.attach( pin_ );
    servo_.write( value_ );
  }

  void set_setpoint( double gyro )
  {
    setpoint_ = gyro;
  }

  void loop( double gyro )
  {
    // PID update
    if ( millis() - last_time_ >= period_ )
    {
      double pid_update = Kp_ * ( gyro - setpoint_ );
      double new_value = value_ + pid_update;
      new_value = (new_value>max_)?max_:new_value;
      new_value = (new_value<min_)?min_:new_value;
  
      value_ = new_value;
      
      last_time_ = millis();
    }

    servo_.write( value_ );
  }

  double get_pwm_value()
  {
    return value_;
  }
private:
  int pin_;
  Servo servo_;
  
  double setpoint_;
  double value_;

  int period_;
  long int last_time_;
  
  double Kp_;
  
  byte min_;
  byte zero_;
  byte max_;
};



