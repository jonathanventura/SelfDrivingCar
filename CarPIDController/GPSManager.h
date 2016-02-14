
class GPSManager
{
public:
  GPSManager( int rx_pin )
  : rx_pin_(rx_pin),
  swserial_(rx_pin,255),
  lat_(0),lon_(0),
  speed_(0),
  heading_(0)
  {
    
  }
  
  void setup()
  {
    pinMode( rx_pin_, INPUT );
    swserial_.begin(4800);
  }

  void loop()
  {
    while ( swserial_.available() )
    {
      if ( gps_.encode( swserial_.read() ) )
      {
        lat_ = gps_.location.lat();
        lon_ = gps_.location.lng();
        speed_ = gps_.speed.kmph();
        heading_ = gps_.course.deg();
      }
    }
  }
private:
  int rx_pin_;
  SoftwareSerial swserial_;
  TinyGPSPlus gps_;
  double lat_, lon_;
  double speed_;
  double heading_;
};



