#include "DataFormats/include/PFMET.hh"

using namespace vecbos;

PFMET::PFMET() : MET() { 
  PFMETCorrs_ = PFMETCorr();
} 

PFMET::PFMET( const LorentzVector& p4, const Point& vtx ) :
  MET(p4,vtx) {
  PFMETCorrs_ = PFMETCorr();
}

PFMET::PFMET( double sumet, double significance, const LorentzVector& p4, const Point& vtx ) :
  MET(sumet,significance,p4,vtx) {
  PFMETCorrs_ = PFMETCorr();
}
