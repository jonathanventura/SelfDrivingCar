
class ESCController
{
public:
  ESCController( int pin )
  : pin_(pin)
  {
    
  }
  
  void setup()
  {
//    min_ = EEPROM.read(3);
//    zero_ = EEPROM.read(4);
//    max_ = EEPROM.read(5);
    min_ = 80;
    zero_ = 100;
    max_ = 120;

    pinMode( pin_, OUTPUT );
    analogWrite( pin_, 0 );
    
    value_ = zero_;

    esc_.attach( pin_ );
    esc_.write( value_ );
  }

  void loop()
  {
    esc_.write( value_ );
  }

  void on()
  {
    value_ = max_;
  }

  void off()
  {
    value_ = zero_;
  }
private:
  int pin_;
  Servo esc_;
  
  double value_;
  
  byte min_;
  byte zero_;
  byte max_;
};



