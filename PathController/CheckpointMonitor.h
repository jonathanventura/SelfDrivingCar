
#define kMaxNumCheckpoints 256

// checkpoint format: x, y, radius
// zero radius marks end of list

class CheckpointMonitor
{
public:
  CheckpointMonitor( float *checkpoints )
  : _checkpoints( checkpoints ), 
  _current_checkpoint( -1 ),
  _current_distance( 0 ),
  _current_heading_error( 0 )
  {
    
  }
  
  // x, y: current estimate of robot position
  // heading: current estimate of robot heading (in radians)
  void updateCheckpoint( float x, float y, float heading )
  {
    // Check if stopped
    if ( _current_checkpoint < 0 ) return;

    // Check if we have reached the end of the checkpoint list
    if ( getRadius() == 0 ) {
      _current_checkpoint = -1;
      return;
    }

    // Get distance to current checkpoint
    const float current_difference_x = getX() - x;
    const float current_difference_y = getY() - y;
    const float current_distance_sq = current_difference_x*current_difference_x + current_difference_y*current_difference_y;
    _current_distance = sqrtf(current_distance_sq);

    // Check radius
    const float current_radius = getRadius();
    if ( _current_distance <= current_radius )
    {
      // Go to next checkpoint
      _current_checkpoint++;
      
      // Check if we have past the end of the list
      if ( _current_checkpoint >= kMaxNumCheckpoints ) _current_checkpoint = -1;
    }

    // Get heading error
    const float current_difference_heading = atan2f(current_difference_y,current_difference_x);
    _current_heading_error = atan2f(sinf(heading - current_difference_heading), cosf(heading - current_difference_heading));
  }

  void start()
  {
    _current_checkpoint = 0;
  }

  void stop()
  {
    _current_checkpoint = -1;
  }

  bool isRunning()
  {
    return ( _current_checkpoint >= 0 );
  }
  
  float getX() {
    if ( _current_checkpoint < 0 ) return 0;
    return _checkpoints[3*_current_checkpoint+0];
  }

  float getY() {
    if ( _current_checkpoint < 0 ) return 0;
    return _checkpoints[3*_current_checkpoint+1];
  }
  
  float getRadius() {
    if ( _current_checkpoint < 0 ) return 0;
    return _checkpoints[3*_current_checkpoint+2];
  }

  float getDistance() {
    return _current_distance;
  }
  
  float getHeadingError() {
    return _current_heading_error;
  }

private:
  float *_checkpoints;                  // list of checkpoint values (x, y, radius)
  int _current_checkpoint;             // current checkpoint index (-1 if we haven't started)
  float _current_distance;
  float _current_heading_error;       // current heading error
};



