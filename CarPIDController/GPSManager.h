
class GPSManager
{
public:
  GPSManager( int rx_pin )
  : swserial_(rx_pin,255)
  {
    
  }
  
  void setup()
  {
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
      }
    }
  }
private:
  SoftwareSerial swserial_;
  TinyGPSPlus gps_;
  double lat_, lon_;
};



