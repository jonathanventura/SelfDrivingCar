
#define kMaxNumCommands 256

// command format: throttle, steering, duration
// zero duration marks end of list
// throttle and steering are assumed to be Servo parameters: (0 to 180, 90 is zero point)

class CommandController
{
public:
  CommandController( long *commands, int throttle_zero = 90, int steering_zero = 90 )
  : _commands( commands ), 
  _throttle_zero( throttle_zero ), _steering_zero( steering_zero ),
  _current_command( -1 )
  {
    
  }
  
  void setup()
  {
    
  }

  void loop()
  {
    // Check if stopped
    if ( _current_command < 0 ) return;

    // Check if we have reached the end of the command list
    if ( getDuration() == 0 ) {
      _current_command = -1;
      return;
    }

    // Check duration of current command
    long current_time = millis();
    if ( current_time - _last_command_time >= getDuration() )
    {
      // Go to next command
      _current_command++;
      _last_command_time = current_time;

      // Check if we have past the end of the list
      if ( _current_command >= kMaxNumCommands ) _current_command = -1;
    }
  }

  void start()
  {
    _current_command = 0;
    _last_command_time = millis();
  }

  void stop()
  {
    _current_command = -1;
  }

  bool isRunning()
  {
    return ( _current_command >= 0 );
  }
  
  long getThrottle() {
    if ( _current_command < 0 ) return _throttle_zero;
    return _commands[3*_current_command+0];
  }

  long getSteering() {
    if ( _current_command < 0 ) return _steering_zero;
    return _commands[3*_current_command+1];
  }
  
  long getDuration() {
    if ( _current_command < 0 ) return 0;
    return _commands[3*_current_command+2];
  }

private:
  long *_commands;                  // list of command values (throttle, steering, duration)
  int _throttle_zero;               // zero point for throttle (usually 90 but may be different)
  int _steering_zero;               // zero point for steering (usually 90 but may be different)
  int _current_command;             // current command index (-1 if we haven't started)
  unsigned long _last_command_time; // time (in ms) since last command started
};



