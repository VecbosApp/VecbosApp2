#ifndef METRECO_MET_HH
#define METRECO_MET_HH

#include "DataFormats/include/Candidate.hh"

namespace vecbos
{
  class MET : public Candidate {
  public:
    /// default constructor
    MET();
    /// constructor from values
    MET( const LorentzVector& p4, const Point& vtx );
    /// constructor from values
    MET( double sumet, double significance, const LorentzVector& p4, const Point& vtx );
    /// destructor
    virtual ~MET() { }
    
    /// scalar sum of transverse energy over all objects
    double sumEt() const { return sumet_; }       
    
    /// MET Significance = MET / std::sqrt(SumET)
    double mEtSig() { return ( sumet_ ? (this->et() / std::sqrt(sumet_)) : (0.0) ); }
    /// real MET significance
    double significance() const { return significance_; }

  private:
    double sumet_;
    double significance_;
  };
}

#endif
