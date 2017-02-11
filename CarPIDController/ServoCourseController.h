
class ServoPIDController
{
public:
  ServoPIDController( int pin, double Kp )
  : pin_(pin), Kp_(Kp)
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

  void off()
  {
    value_ = zero_;
  }

  void update( int heading, int course )
  {
    int diff = getAngularDifference( course, heading );
    double new_value = Kp_ * diff;
    new_value = (new_value>max_)?max_:new_value;
    new_value = (new_value<min_)?min_:new_value;
    
    value_ = new_value;
  }

  void loop( )
  {
    servo_.write( value_ );
  }

  double get_pwm_value()
  {
    return value_;
  }
private:
  int pin_;
  Servo servo_;
  
  double value_;

  double Kp_;
  
  byte min_;
  byte zero_;
  byte max_;

  int getAngularDifference( int a, int b )
  {
      // http://stackoverflow.com/questions/7570808/how-do-i-calculate-the-difference-of-two-angle-measures
      int d = abs(a - b) % 360; 
      int r = d > 180 ? 360 - d : d;

      //calculate sign 
      int sign = (a - b >= 0 && a - b <= 180) || (a - b <=-180 && a- b>= -360) ? 1 : -1; 
      r *= sign;
  }
};



