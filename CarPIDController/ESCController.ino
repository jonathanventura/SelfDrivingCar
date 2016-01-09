
class ESCController
{
public:
  ESCController( int pin )
  {
    min_value = EEPROM.read(3);
    zero_value = EEPROM.read(4);
    max_value = EEPROM.read(5);
  
    esc.attach(pin);
  
    current_value = zero_value;
    esc.write(current_value);
  }
  
  void setup()
  {
    
  }
  
  void loop()
  {
    esc.write(current_value);
  }

  void on()
  {
    current_value = max_value;
  }
  
  void off()
  {
    current_value = zero_value;
  }
  }
private:
  Servo esc;

  byte current_value;
  
  byte min_value;
  byte zero_value;
  byte max_value;
};


