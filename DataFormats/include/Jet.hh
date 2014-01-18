#ifndef VECBOS_JET_HH
#define VECBOS_JET_HH

#include "DataFormats/include/Candidate.hh"
#include <string>

namespace vecbos {
  class Jet : public Candidate {
  public:

    /// Default constructor
    Jet () : JetArea_ (0), Candidate() { }
    /// Constructor from basic values
    Jet (const LorentzVector& fP4, const Point& fVertex);
    /// Constructor from basic values
    Jet (const LorentzVector& fP4, const LorentzVector& fP4Raw, const Point& fVertex);
    /// Destructor
    virtual ~Jet () { }

    /// get the raw jet momentum
    LorentzVector rawP4() { return p4raw_; }
    float rawpt() { return p4raw_.Pt(); }
    float rawenergy() { return p4raw_.Energy(); }

    /// btagging variables of the jet
    struct BTagsJet {
      float combinedSecondaryVertex;
      float combinedSecondaryVertexMVA;
      float jetBProbability;
      float jetProbability;
      float simpleSecondaryVertexHighEff;
      float simpleSecondaryVertexHighPur;
      float trackCountingHighPur;
      float trackCountingHighEff;
      float trackCountingVeryHighEff;
      BTagsJet() 
	: combinedSecondaryVertex(std::numeric_limits<float>::infinity()),
	  combinedSecondaryVertexMVA(std::numeric_limits<float>::infinity()),
	  jetBProbability(std::numeric_limits<float>::infinity()),
	  jetProbability(std::numeric_limits<float>::infinity()),
	  simpleSecondaryVertexHighEff(std::numeric_limits<float>::infinity()),
	  simpleSecondaryVertexHighPur(std::numeric_limits<float>::infinity()),
	  trackCountingHighPur(std::numeric_limits<float>::infinity()),
	trackCountingHighEff(std::numeric_limits<float>::infinity()),
	trackCountingVeryHighEff(std::numeric_limits<float>::infinity())
      {}
    } ;
    
    /// accessors
    float combinedSecondaryVertex() { return BTagsJet_.combinedSecondaryVertex; }
    float combinedSecondaryVertexMVA() { return BTagsJet_.combinedSecondaryVertexMVA; }
    float jetBProbability() { return BTagsJet_.jetBProbability; }
    float jetProbability() { return BTagsJet_.jetProbability; }
    float simpleSecondaryVertexHighEff() { return BTagsJet_.simpleSecondaryVertexHighEff; }
    float simpleSecondaryVertexHighPur() { return BTagsJet_.simpleSecondaryVertexHighPur; }
    float trackCountingHighPur() { return BTagsJet_.trackCountingHighPur; }
    float trackCountingHighEff() { return BTagsJet_.trackCountingHighEff; }
    float trackCountingVeryHighEff() {return BTagsJet_.trackCountingVeryHighEff; }

    /// setter
    void setBTagsJet(BTagsJet tags) { BTagsJet_ = tags; }
    
  private:
    BTagsJet BTagsJet_;
    
  public:
    
    /// scale energy of the jet
    virtual void scaleEnergy (double fScale);
    
    /// set jet area
    virtual void setArea (float fArea) { JetArea_ = fArea;}
    /// get jet area
    virtual float area () const {return JetArea_;}

    /// Print object
    virtual std::string print ();

  private:
    /// the jet area
    float JetArea_;
    /// the raw jet momentum
    LorentzVector p4raw_;
  };
}
#endif
