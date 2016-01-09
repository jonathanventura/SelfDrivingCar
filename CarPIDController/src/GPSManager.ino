
#include <TinyGPS++.h>

#include <SoftwareSerial.h>

TinyGPSPlus gps;
double lat, lon;

SoftwareSerial swserial(6,255);

void gps_setup()
{
  swserial.begin(4800);
}

void gps_loop()
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

