#include "DataFormats/include/Track.hh"

using namespace vecbos;

Track::Track() :
  chi2_(0), vertex_(0,0,0), momentum_(0,0,0), momentumMode_(-999,-999,-999), charge_(0) { 
  isValid_ = false;
}


Track::Track( double chi2,  const Point & vertex, const Vector & momentum, int charge) :
  chi2_( chi2 ), vertex_( vertex ), 
  momentum_( momentum ), momentumMode_(-999,-999,-999), charge_( charge ) { 
  isValid_ = true;
}
