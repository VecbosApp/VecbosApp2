#ifndef METRECO_PFMET_HH
#define METRECO_PFMET_HH

#include "DataFormats/include/MET.hh"

namespace vecbos {

  class PFMET : public MET {
  public:
    /// default constructor
    PFMET();
    /// constructor from values
    PFMET( const LorentzVector& p4, const Point& vtx );
    /// constructor from values
    PFMET( double sumet, double significance, const LorentzVector& p4, const Point& vtx );
    /// destructor
    virtual ~PFMET() { }

    /// MET corrections
    struct PFMETCorr 
    {
      LorentzVector Type0Met; 
      LorentzVector Type1Met; 
      LorentzVector Type1p2Met; 
      PFMETCorr()
	: Type0Met( LorentzVector(0, 0, 0, 0) ),
	  Type1Met( LorentzVector(0, 0, 0, 0) ),
	  Type1p2Met( LorentzVector(0, 0, 0, 0) )
      {}
    } ;

    /// accessors of MET corrections
    LorentzVector type0() const { return PFMETCorrs_.Type0Met; }
    LorentzVector type1() const { return PFMETCorrs_.Type1Met; }
    LorentzVector type1p2() const { return PFMETCorrs_.Type1p2Met; }
    const PFMETCorr & METCorrections() const { return PFMETCorrs_; }

    /// accessor of the ET of the corrected MET
    float etType0() const { return PFMETCorrs_.Type0Met.Pt(); }
    float etType1() const { return PFMETCorrs_.Type1Met.Pt(); }
    float etType1p2() const { return PFMETCorrs_.Type1p2Met.Pt(); }

    /// setter of PFMET corrections
    void setCorrections(PFMETCorr corrs) { PFMETCorrs_ = corrs; }

  private:
    /// PF MET corrections
    PFMETCorr PFMETCorrs_;

  };

}

#endif
