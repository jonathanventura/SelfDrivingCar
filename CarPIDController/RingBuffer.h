
class RingBuffer
{
public:
  RingBuffer() : next_(0) { }

  inline void insert( double val )
  {
    buf_[next_] = val;
    next_ = (next_+1)%10;
  }

  inline double get_average() const
  {
    return ( buf_[0] + buf_[1] + buf_[2] + buf_[3] + buf_[4] + buf_[5] + buf_[6] + buf_[7] + buf_[8] + buf_[9] ) / 10.;
  }

  inline int get_index() const { return next_; }
private:
  int next_;
  double buf_[10];  
};

