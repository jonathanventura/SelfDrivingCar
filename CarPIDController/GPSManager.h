
class GPSManager
{
public:
  GPSManager( int rx_pin )
  : swserial(rx_pin,255)
  {
    
  }
  
  void setup()
  {
    swserial.begin(4800);
  }

  void loop()
  {
    while ( swserial.available() )
    {
      if ( gps.encode( swserial.read() ) )
      {
        lat = gps.location.lat();
        lon = gps.location.lng();
      }
    }
  }
private:
  SoftwareSerial swserial;
  TinyGPSPlus gps;
  double lat, lon;
};



