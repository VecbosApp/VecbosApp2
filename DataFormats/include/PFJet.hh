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
    struct JetId {
      float weightedDz1;
      float weightedDz2;
      float betastar;
      float rms;
      float mva;
      float dR2Mean;
      float dRMean;
      JetId()
	: weightedDz1(0.), weightedDz2(0.), 
	  betastar(0.), rms(0.), mva(0.),
	  dR2Mean(0.), dRMean(0.)
      { }  
    };

    /// accessors
    float mva() const { return jetid_.mva; }
    float weightedDz1() const { return jetid_.weightedDz1; }
    float weightedDz2() const { return jetid_.weightedDz2; }
    float betastar() const { return jetid_.betastar; }
    float rms() const { return jetid_.rms; }
    float dR2Mean() const { return jetid_.dR2Mean; }
    float dRMean() const { return jetid_.dRMean; }
    
    /// block setter
    JetId getJetId() const { return jetid_; }
    /// block getter
    void setJetId(JetId id) { jetid_ = id; }

  private:
    JetId jetid_;

  public:
    struct QGLikelihoodVars {
      
      float ptD;
      float rmsCand;
      float axis1;
      float axis2;
      float pull;
      float tana;
      
      float ptD_QC;
      float rmsCand_QC;
      float axis1_QC;
      float axis2_QC;
      float pull_QC;
      float tana_QC;
      
      int nChg_ptCut;
      int nChg_QC;
      int nChg_ptCut_QC;
      int nNeutral_ptCut;
      
      float Rchg;
      float Rneutral;
      float R;
      float Rchg_QC;
      
      float pTMax;
      float pTMaxChg;
      float pTMaxNeutral;
      float pTMaxChg_QC;

      QGLikelihoodVars()
	: ptD(0.), rmsCand(0.), axis1(0.), axis2(0.), pull(0.), tana(0.),
	  ptD_QC(0.), rmsCand_QC(0.), axis1_QC(0.), axis2_QC(0.), pull_QC(0.), tana_QC(0.), 
	  nChg_ptCut(0.), nChg_QC(0.), nChg_ptCut_QC(0.), nNeutral_ptCut(0.), 
	  Rchg(0.), Rneutral(0.), R(0.), Rchg_QC(0.),       
	  pTMax(0.), pTMaxChg(0.), pTMaxNeutral(0.), pTMaxChg_QC(0.)
      { }
    };

    /// accessors
    float ptD() const { return qglikelihood_.ptD ; }
    float rmsCand() const { return qglikelihood_.rmsCand ; }
    float axis1() const { return qglikelihood_.axis1 ; }
    float axis2() const { return qglikelihood_.axis2 ; }
    float pull() const { return qglikelihood_.pull ; }
    float tana() const { return qglikelihood_.tana ; }
    
    float ptD_QC() const { return qglikelihood_.ptD_QC ; }
    float rmsCand_QC() const { return qglikelihood_.rmsCand_QC ; }
    float axis1_QC() const { return qglikelihood_.axis1_QC ; }
    float axis2_QC() const { return qglikelihood_.axis2_QC ; }
    float pull_QC() const { return qglikelihood_.pull_QC ; }
    float tana_QC() const { return qglikelihood_.tana_QC ; }
    
    int nChg_ptCut() const { return qglikelihood_.nChg_ptCut ; }
    int nChg_QC() const { return qglikelihood_. nChg_QC; }
    int nChg_ptCut_QC() const { return qglikelihood_.nChg_ptCut_QC ; }
    int nNeutral_ptCut() const { return qglikelihood_.nNeutral_ptCut ; }
    
    float Rchg() const { return qglikelihood_.Rchg ; }
    float Rneutral() const { return qglikelihood_.Rneutral ; }
    float R() const { return qglikelihood_.R ; }
    float Rchg_QC() const { return qglikelihood_.Rchg_QC ; }
    
    float pTMax() const { return qglikelihood_.pTMax ; }
    float pTMaxChg() const { return qglikelihood_.pTMaxChg ; }
    float pTMaxNeutral() const { return qglikelihood_.pTMaxNeutral ; }
    float pTMaxChg_QC() const { return qglikelihood_.pTMaxChg ; }

    /// block setter
    QGLikelihoodVars getQGVars() const { return qglikelihood_; }
    /// block getter
    void setQGVars(QGLikelihoodVars vars) { qglikelihood_ = vars; }

  private:
    QGLikelihoodVars qglikelihood_;    

  public:

    /// Print object in details
    virtual std::string print ();
  };
}

#endif

