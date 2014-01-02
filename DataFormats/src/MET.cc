#include "DataFormats/include/MET.hh"

using namespace vecbos;
using namespace std;

MET::MET()
{
  sumet_ = 0.0;
  significance_ = 0.0;
}

MET::MET( const LorentzVector& p4, const Point& vtx ) : 
  Candidate( 0, p4, vtx )
{
  sumet_ = 0.0;
  significance_ = 0.0;
}

MET::MET( double sumet, double significance, const LorentzVector& p4, const Point& vtx ) : 
  Candidate( 0, p4, vtx ) 
{
  sumet_ = sumet;
  significance_ = significance;
}
