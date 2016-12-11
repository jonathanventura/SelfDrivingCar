
class RobotSimulator
{
public:
  RobotSimulator( long time_scale = 100, float angular_velocity_noise = 30.f*M_PI/180.f )
  : _time_scale( time_scale ),
  _angular_velocity_noise( 0, angular_velocity_noise*angular_velocity_noise ),
  _last_update_time( 0 ),
  _x( 0 ),
  _y( 0 ),
  _heading( 0 ),
  _velocity( 0 ),
  _angular_velocity( 0 )
  {
    
  }
  
  void setup()
  {
    
  }

  bool loop()
  {
    if ( _last_update_time == 0 ) _last_update_time = millis();

    if ( millis() - _last_update_time >= 200/_time_scale )
    {
      const float delta_t = (millis() - _last_update_time)*_time_scale/1000.f;
      
      // Integrate motion
      _x += cosf(_heading) * _velocity * delta_t;
      _y += sinf(_heading) * _velocity * delta_t;
    
      _heading += ( _angular_velocity + _angular_velocity_noise.random() ) * delta_t;
      while ( _heading > 2*M_PI ) _heading -= 2*M_PI;
      while ( _heading < -2*M_PI ) _heading += 2*M_PI;

      _last_update_time = millis();
      
      return true;
    }

    return false;
  }

  void setDesiredVelocity( float desired_velocity )
  {
    _velocity = desired_velocity;
  }

  void setDesiredAngularVelocity( float desired_angular_velocity )
  {
    _angular_velocity = desired_angular_velocity;
  }
  
  float getX() {
    return _x;
  }

  float getY() {
    return _y;
  }
  
  float getHeading() {
    return _heading;
  }
  
private:
  long _time_scale;
  Gaussian _angular_velocity_noise;
  
  long _last_update_time;
  float _x;
  float _y;
  float _heading;
  float _velocity;
  float _angular_velocity;
};



