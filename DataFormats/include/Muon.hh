#ifndef VECBOS_MUON_H
#define VECBOS_MUON_H

#include "DataFormats/include/Track.hh"
#include "DataFormats/include/RecoCandidate.hh"

namespace vecbos {

  class Muon : public RecoCandidate {
  public:

    /// Lorentz vector
    typedef TLorentzVector LorentzVector;
    /// point in the space
    typedef TVector3 Point;
    /// point in the space
    typedef TVector3 Vector;

    /// constructor from constituents
    Muon( int charge, const LorentzVector & p4, const Point & vertexPosition,
	  Track innerTrack, Track staTrack, Track gblTrack);
    /// destructor
    ~Muon() { };

  private:
    Track innerTrack_, standaloneTrack_, globalMuonTrack_;
    
  public:

    /// ID
    bool isPFId() { return pfid_; }
    void setPFId(bool id) { pfid_ = id; }

    /// muon type - type of the algorithm that reconstructed this muon
    /// multiple algorithms can reconstruct the same muon
    static const unsigned int GlobalMuon     =  1<<1;
    static const unsigned int TrackerMuon    =  1<<2;
    static const unsigned int StandAloneMuon =  1<<3;
    static const unsigned int CaloMuon =  1<<4;
    static const unsigned int PFMuon =  1<<5;
    static const unsigned int RPCMuon =  1<<6;
    void setType(int type) { type_ = type; }

    int type() const { return type_; }
    bool isMuon() const { return true; }
    bool isGlobalMuon()     const { return type_ & GlobalMuon; }
    bool isTrackerMuon()    const { return type_ & TrackerMuon; }
    bool isStandAloneMuon() const { return type_ & StandAloneMuon; }
    bool isCaloMuon() const { return type_ & CaloMuon; }
    bool isPFMuon() const {return type_ & PFMuon;} //fix me ! Has to go to type
    bool isRPCMuon() const {return type_ & RPCMuon;}

  private:

    bool pfid_;
    int type_;
        
  public:

    //=======================================================
    // Isolation Variables
    //=======================================================
    
    struct DetectorIsolationVariables
    {
      float tkSumPt ;
      float emSumEt ;
      float hcalSumEt ; 
      
      DetectorIsolationVariables()
	: tkSumPt(0.), emSumEt(0.), hcalSumEt(0.)
      {}
    } ;

    struct PFIsolationVariables
    {
      float chargedSumPt ;
      float photonSumPt ;
      float neutralHadronSumPt ;
      float sumPUPt ; 
      
      PFIsolationVariables()
	: chargedSumPt(0.), photonSumPt(0.), neutralHadronSumPt(0.), sumPUPt(0.)
      {}
    } ;

    // DeltaR = 03 accessors
    float dr03Isolation() const { return dr03_.tkSumPt + dr03_.emSumEt + dr03_.hcalSumEt; }
    float dr03TkSumPt() const { return dr03_.tkSumPt; }
    float dr03EmSumPt() const { return dr03_.emSumEt; }
    float dr03HcalSumPt() const { return dr03_.hcalSumEt; }

    float dr03pfIsolation() const { return dr03PF_.chargedSumPt +  dr03PF_.photonSumPt + dr03PF_.neutralHadronSumPt ; }
    float dr03pfIsolationDeltaBetaCorr() const { return  dr03PF_.chargedSumPt + fmax(dr03PF_.photonSumPt + dr03PF_.neutralHadronSumPt - 0.5*dr03PF_.sumPUPt, 0.); }
    float dr03pfChargedSumPt() const { return dr03PF_.chargedSumPt; }
    float dr03pfPhotonSumPt() const { return dr03PF_.photonSumPt; }
    float dr03pfNeutralHadronSumPt() const { return dr03PF_.neutralHadronSumPt; }
    float dr03pfPUPt() const { return dr03PF_.sumPUPt; }
    
    const DetectorIsolationVariables & dr03DetectorIsolationVariables() const { return dr03_ ; }
    const PFIsolationVariables & dr03PFIsolationVariables() const { return dr03PF_ ; }


    // DeltaR = 04 accessors
    float dr04Isolation() const { return dr04_.tkSumPt + dr04_.emSumEt + dr04_.hcalSumEt; }
    float dr04TkSumPt() const { return dr04_.tkSumPt; }
    float dr04EmSumPt() const { return dr04_.emSumEt; }
    float dr04HcalSumPt() const { return dr04_.hcalSumEt; }

    float dr04pfIsolation() const { return dr04PF_.chargedSumPt +  dr04PF_.photonSumPt + dr04PF_.neutralHadronSumPt ; }
    float dr04pfIsolationDeltaBetaCorr() const { return  dr04PF_.chargedSumPt + fmax(dr04PF_.photonSumPt + dr04PF_.neutralHadronSumPt - 0.5*dr04PF_.sumPUPt, 0.); }
    float dr04pfChargedSumPt() const { return dr04PF_.chargedSumPt; }
    float dr04pfPhotonSumPt() const { return dr04PF_.photonSumPt; }
    float dr04pfNeutralHadronSumPt() const { return dr04PF_.neutralHadronSumPt; }
    float dr04pfPUPt() const { return dr04PF_.sumPUPt; }

    const DetectorIsolationVariables & dr04DetectorIsolationVariables() const { return dr04_ ; }
    const PFIsolationVariables & dr04PFIsolationVariables() const { return dr04PF_ ; }
    
    /// setters
    void setDr03DetectorIsolation( const DetectorIsolationVariables & dr03 ) { dr03_ = dr03 ; }
    void setDr04DetectorIsolation( const DetectorIsolationVariables & dr04 ) { dr04_ = dr04 ; }
    void setDr03PFIsolation( const PFIsolationVariables & dr03 ) { dr03PF_ = dr03 ; }
    void setDr04PFIsolation( const PFIsolationVariables & dr04 ) { dr04PF_ = dr04 ; }
    
    
  private:
    
    // attributes
    DetectorIsolationVariables dr03_ ;
    DetectorIsolationVariables dr04_ ;
    PFIsolationVariables dr03PF_ ;
    PFIsolationVariables dr04PF_ ;

  public:
    /// get number of chambers with matched segments
    int numberOfMatches( ) const { return numberOfMatches_; }
    void setNumberOfMatches(int n) { numberOfMatches_ = n; }

  private:
    int numberOfMatches_;


  public:
    //=======================================================
    // Corrected Momentum Information
    //=======================================================
    LorentzVector calibratedP4();
    float calibratedMomentum() { return calibratedMomentum_ ; }
    
    // setters
    void setCalibrateMomentum(float momentum) { calibratedMomentum_ = momentum ; }

  private:
    float calibratedMomentum_;

  };

}

#endif
