#ifndef VECBOS_PFJET_HH
#define VECBOS_PFJET_HH

#include "DataFormats/include/Jet.hh"

namespace vecbos {

  class PFJet : public Jet {

  public:
    /// Default constructor
    PFJet () : Jet() { }
    /// Constructor from basic values
    PFJet (const LorentzVector& fP4, const Point& fVertex);
    /// Constructor from values
    PFJet (const LorentzVector& fP4, const LorentzVector& fP4Raw, const Point& fVertex);
    /// Destructor
    virtual ~PFJet () { }

    struct Specific {
      float mChargedHadronEnergy;
      float mNeutralHadronEnergy;
      float mPhotonEnergy;
      float mElectronEnergy;
      float mMuonEnergy;
      float mHFHadronEnergy;
      float mHFEMEnergy;
      
      int mChargedHadronMultiplicity;
      int mNeutralHadronMultiplicity;
      int mPhotonMultiplicity;
      int mElectronMultiplicity;
      int mMuonMultiplicity;
      int mHFHadronMultiplicity;
      int mHFEMMultiplicity;
      Specific () 
	: mChargedHadronEnergy (0),
	  mNeutralHadronEnergy (0),
	  mPhotonEnergy (0),
	  mElectronEnergy (0),
	  mMuonEnergy (0),
	  mHFHadronEnergy (0),
	  mHFEMEnergy (0),
	  
	  mChargedHadronMultiplicity (0),
	  mNeutralHadronMultiplicity (0),
	  mPhotonMultiplicity (0),
	  mElectronMultiplicity (0),
	  mMuonMultiplicity (0),
	  mHFHadronMultiplicity (0),
	  mHFEMMultiplicity (0)
	  
      { }
    };

    /// chargedHadronEnergy 
    float chargedHadronEnergy () {return m_specific.mChargedHadronEnergy;}
    ///  chargedHadronEnergyFraction
    float  chargedHadronEnergyFraction () {return chargedHadronEnergy () / energy ();}
    /// neutralHadronEnergy
    float neutralHadronEnergy () {return m_specific.mNeutralHadronEnergy;}
    /// neutralHadronEnergyFraction
    float neutralHadronEnergyFraction () {return neutralHadronEnergy () / energy ();}
    /// photonEnergy 
    float photonEnergy () {return m_specific.mPhotonEnergy;}
    /// photonEnergyFraction
    float photonEnergyFraction () {return photonEnergy () / energy ();}
    /// electronEnergy 
    float electronEnergy () {return m_specific.mElectronEnergy;}
    /// electronEnergyFraction
    float electronEnergyFraction () {return electronEnergy () / energy ();}
    /// muonEnergy 
    float muonEnergy () {return m_specific.mMuonEnergy;}
    /// muonEnergyFraction
    float muonEnergyFraction () {return muonEnergy () / energy ();}
    /// HFHadronEnergy 
    float HFHadronEnergy () {return m_specific.mHFHadronEnergy;}
    /// HFHadronEnergyFraction
    float HFHadronEnergyFraction () {return HFHadronEnergy () / energy ();}
    /// HFEMEnergy 
    float HFEMEnergy () {return m_specific.mHFEMEnergy;}
    /// HFEMEnergyFraction
    float HFEMEnergyFraction () {return HFEMEnergy () / energy ();}

    /// chargedHadronMultiplicity
    int chargedHadronMultiplicity () {return m_specific.mChargedHadronMultiplicity;}
    /// neutralHadronMultiplicity
    int neutralHadronMultiplicity () {return m_specific.mNeutralHadronMultiplicity;}
    /// photonMultiplicity
    int photonMultiplicity () {return m_specific.mPhotonMultiplicity;}
    /// electronMultiplicity
    int electronMultiplicity () {return m_specific.mElectronMultiplicity;}
    /// muonMultiplicity
    int muonMultiplicity () {return m_specific.mMuonMultiplicity;}
    /// HFHadronMultiplicity
    int HFHadronMultiplicity () {return m_specific.mHFHadronMultiplicity;}
    /// HFEMMultiplicity
    int HFEMMultiplicity () {return m_specific.mHFEMMultiplicity;}

    /// chargedEmEnergy
    float chargedEmEnergy () {return m_specific.mElectronEnergy;}
    /// chargedEmEnergyFraction
    float chargedEmEnergyFraction () {return chargedEmEnergy () / energy ();}
    /// chargedMuEnergy
    float chargedMuEnergy () {return m_specific.mMuonEnergy;}
    /// chargedMuEnergyFraction
    float chargedMuEnergyFraction () {return chargedMuEnergy () / energy ();}
    /// neutralEmEnergy
    float neutralEmEnergy () {return m_specific.mPhotonEnergy;}
    /// neutralEmEnergyFraction
    float neutralEmEnergyFraction () {return neutralEmEnergy () / energy ();}
  
    /// chargedMultiplicity
    int chargedMultiplicity () {return m_specific.mChargedHadronMultiplicity + m_specific.mElectronMultiplicity + m_specific.mMuonMultiplicity;}
    /// neutralMultiplicity
    int neutralMultiplicity () {return m_specific.mNeutralHadronMultiplicity + m_specific.mPhotonMultiplicity + m_specific.mHFHadronMultiplicity + m_specific.mHFEMMultiplicity;}
    
    /// block getter 
    const Specific getSpecific() const { return m_specific; }
    /// block setter
    void setSpecific(Specific specific) { m_specific = specific; }
    
  private:
    Specific m_specific;

  public:

    /// Print object in details
    virtual std::string print ();
  };
}

#endif

