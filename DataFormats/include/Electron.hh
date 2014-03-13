#ifndef VECBOS_ELECTRON_H
#define VECBOS_ELECTRON_H

#include "DataFormats/include/RecoCandidate.hh"
#include "DataFormats/include/Track.hh"
#include "DataFormats/include/SuperCluster.hh"
#include "DataFormats/include/ElectronFlags.hh"

namespace vecbos {

  class Electron : public RecoCandidate {
  public:

    /// Lorentz vector
    typedef TLorentzVector LorentzVector;
    /// point in the space
    typedef TVector3 Point;
    /// point in the space
    typedef TVector3 Vector;

    // some nested structures defined later on
    struct ChargeInfo ;
    struct TrackClusterMatching ;
    struct TrackExtrapolations ;
    struct ClosestCtfTrack ;
    struct FiducialFlags ;
    struct ShowerShape ;
    struct IsolationVariables ;
    struct ConversionRejection ;
    struct ClassificationVariables ;

    /// default constructor
    Electron() : RecoCandidate() { }
    /// constructor from constituents
    Electron( int charge, const LorentzVector & p4, const Point & vertexPosition, SuperCluster supercluster, SuperCluster pfsupercluster, Track gsfTrack, Track ctfTrack);
    /// destructor
    ~Electron() { };

  private:
    SuperCluster superCluster_;
    SuperCluster pfSuperCluster_;
    Track gsfTrack_;
    Track ctfTrack_;

  public:
    struct ChargeInfo
    {
      int scPixCharge ;
      bool isGsfCtfScPixConsistent ;
      bool isGsfScPixConsistent ;
      bool isGsfCtfConsistent ;
      ChargeInfo()
        : scPixCharge(0), isGsfCtfScPixConsistent(false),
          isGsfScPixConsistent(false), isGsfCtfConsistent(false)
        {}
     } ;

    /// setters
    void setScPixCharge(int charge);

    /// Charge info accessors
    int scPixCharge() const { return chargeInfo_.scPixCharge ; }
    bool isGsfCtfScPixChargeConsistent() const { return chargeInfo_.isGsfCtfScPixConsistent ; }
    bool isGsfScPixChargeConsistent() const { return chargeInfo_.isGsfScPixConsistent ; }
    bool isGsfCtfChargeConsistent() const { return chargeInfo_.isGsfCtfConsistent ; }
    const ChargeInfo & chargeInfo() const { return chargeInfo_ ; }

  private:
    /// Complementary attributes
    ChargeInfo chargeInfo_ ;

    /// === Core attributes ===
  public:
    SuperCluster superCluster() const;
    Track gsfTrack() const { return gsfTrack_; }
    Track closestTrack() const { return ctfTrack_; }
    Track track() const { return gsfTrack_; }
    bool ecalDrivenSeed() const;
    bool trackerDrivenSeed() const;
    SuperCluster pfSuperCluster() const { return pfSuperCluster_ ; }
    
    // setters
    void setRecoFlags(int flags) { recoflags_ = flags; }

  private:
    int recoflags_;

  //=======================================================
  // Track-Cluster Matching Attributes
  //=======================================================

  public:

    struct TrackClusterMatching
    {
      float eSuperClusterOverP ;        // the supercluster energy / track momentum at the PCA to the beam spot
      float eSeedClusterOverP ;         // the seed cluster energy / track momentum at the PCA to the beam spot
      float eSeedClusterOverPout ;      // the seed cluster energy / track momentum at calo extrapolated from the outermost track state
      float eEleClusterOverPout ;       // the electron cluster energy / track momentum at calo extrapolated from the outermost track state
      float deltaEtaSuperClusterAtVtx ; // the supercluster eta - track eta position at calo extrapolated from innermost track state
      float deltaEtaSeedClusterAtCalo ; // the seed cluster eta - track eta position at calo extrapolated from the outermost track state
      float deltaEtaEleClusterAtCalo ;  // the electron cluster eta - track eta position at calo extrapolated from the outermost state
      float deltaPhiEleClusterAtCalo ;  // the electron cluster phi - track phi position at calo extrapolated from the outermost track state
      float deltaPhiSuperClusterAtVtx ; // the supercluster phi - track phi position at calo extrapolated from the innermost track state
      float deltaPhiSeedClusterAtCalo ; // the seed cluster phi - track phi position at calo extrapolated from the outermost track state
      TrackClusterMatching()
       : eSuperClusterOverP(0.),
         eSeedClusterOverP(0.),
         eSeedClusterOverPout(0.),
         eEleClusterOverPout(0.),
         deltaEtaSuperClusterAtVtx(std::numeric_limits<float>::infinity()),
         deltaEtaSeedClusterAtCalo(std::numeric_limits<float>::infinity()),
         deltaEtaEleClusterAtCalo(std::numeric_limits<float>::infinity()),
         deltaPhiEleClusterAtCalo(std::numeric_limits<float>::infinity()),
         deltaPhiSuperClusterAtVtx(std::numeric_limits<float>::infinity()),
         deltaPhiSeedClusterAtCalo(std::numeric_limits<float>::infinity())
        {}
     } ;

    // accessors
    float eSuperClusterOverP() const { return trackClusterMatching_.eSuperClusterOverP ; }
    float eSeedClusterOverP() const { return trackClusterMatching_.eSeedClusterOverP ; }
    float eSeedClusterOverPout() const { return trackClusterMatching_.eSeedClusterOverPout ; }
    float eEleClusterOverPout() const { return trackClusterMatching_.eEleClusterOverPout ; }
    float deltaEtaSuperClusterTrackAtVtx() const { return trackClusterMatching_.deltaEtaSuperClusterAtVtx ; }
    float deltaEtaSeedClusterTrackAtCalo() const { return trackClusterMatching_.deltaEtaSeedClusterAtCalo ; }
    float deltaEtaEleClusterTrackAtCalo() const { return trackClusterMatching_.deltaEtaEleClusterAtCalo ; }
    float deltaPhiSuperClusterTrackAtVtx() const { return trackClusterMatching_.deltaPhiSuperClusterAtVtx ; }
    float deltaPhiSeedClusterTrackAtCalo() const { return trackClusterMatching_.deltaPhiSeedClusterAtCalo ; }
    float deltaPhiEleClusterTrackAtCalo() const { return trackClusterMatching_.deltaPhiEleClusterAtCalo ; }
    const TrackClusterMatching & trackClusterMatching() const { return trackClusterMatching_ ; }

    // setter
    void setTrackClusterMatching(TrackClusterMatching tkclu) { trackClusterMatching_ = tkclu ; }

  private:

    // attributes
    TrackClusterMatching trackClusterMatching_ ;

  //=======================================================
  // Track extrapolations
  //=======================================================

  public :

    struct TrackExtrapolations
     {
      Point  positionAtVtx ;     // the track PCA to the beam spot
      Vector momentumAtVtx ;     // the track momentum at the PCA to the beam spot
     } ;

    // accessors
    Point trackPositionAtVtx() const { return trackExtrapolations_.positionAtVtx ; }
    Vector trackMomentumAtVtx() const { return trackExtrapolations_.momentumAtVtx ; }
    float trackPAtOuter() const { return gsfTrack_.p()*(1 - fbrem()) ; }
    const TrackExtrapolations & trackExtrapolations() const { return trackExtrapolations_ ; }

  private:

    // attributes
    TrackExtrapolations trackExtrapolations_ ;

  //=======================================================
  // Fiducial Flags
  //=======================================================

  public :

    struct FiducialFlags
     {
      bool isEB ;        // true if particle is in ECAL Barrel
      bool isEE ;        // true if particle is in ECAL Endcaps
      bool isEBEEGap ;   // true if particle is in the crack between EB and EE
      bool isEBEtaGap ;  // true if particle is in EB, and inside the eta gaps between modules
      bool isEBPhiGap ;  // true if particle is in EB, and inside the phi gaps between modules
      bool isEEDeeGap ;  // true if particle is in EE, and inside the gaps between dees
      bool isEERingGap ; // true if particle is in EE, and inside the gaps between rings
      FiducialFlags()
       : isEB(false), isEE(false), isEBEEGap(false),
         isEBEtaGap(false), isEBPhiGap(false),
             isEEDeeGap(false), isEERingGap(false)
           {}
     } ;

    // accessors
    bool isEB() const { return fiducialFlags_.isEB ; }
    bool isEE() const { return fiducialFlags_.isEE ; }
    bool isGap() const { return ((isEBEEGap())||(isEBGap())||(isEEGap())) ; }
    bool isEBEEGap() const { return fiducialFlags_.isEBEEGap ; }
    bool isEBGap() const { return (isEBEtaGap()||isEBPhiGap()) ; }
    bool isEBEtaGap() const { return fiducialFlags_.isEBEtaGap ; }
    bool isEBPhiGap() const { return fiducialFlags_.isEBPhiGap ; }
    bool isEEGap() const { return (isEEDeeGap()||isEERingGap()) ; }
    bool isEEDeeGap() const { return fiducialFlags_.isEEDeeGap ; }
    bool isEERingGap() const { return fiducialFlags_.isEERingGap ; }
    bool isInFiducialRegion();
    const FiducialFlags & fiducialFlags() const { return fiducialFlags_ ; }

    // setter from the multibit word
    void setFiducialFlags(int flags);

  private:

    // attributes
    FiducialFlags fiducialFlags_ ;
    int fiducialflags_;

  //=======================================================
  // Shower Shape Variables
  //=======================================================

  public :
    
    // accessors
    float sigmaIetaIeta() const { return superCluster().sigmaIetaIeta() ; }
    float sigmaIphiIphi() const { return superCluster().sigmaIphiIphi() ; }
    float e1x5() const { return superCluster().e1x5() ; }
    float e2x5Max() const { return superCluster().e2x5Max() ; }
    float e5x5() const { return superCluster().e5x5() ; }
    float r9() const { return superCluster().e3x3() / superCluster().energy(); }
    float hcalOverEcal() const { return hcalOverEcal_ ; }

    // setters 
    void setHcalOverEcal(float hoe) { hcalOverEcal_ = hoe; }

  private:

    // attributes
    float hcalOverEcal_;


  //=======================================================
  // Isolation Variables
  //=======================================================

  public :

    struct DetectorIsolationVariables
    {
      float tkSumPt ;                // track iso deposit with electron footprint removed
      float ecalRecHitSumEt ;        // ecal iso deposit with electron footprint removed
      float hcalTowerSumEt ;         // hcal iso deposit without the DeltaR=0.15 cone of H/E with electron footprint removed
      float hcalTowerFullConeSumEt ; // hcal iso deposit with electron footprint removed
      
      DetectorIsolationVariables()
	: tkSumPt(0.), ecalRecHitSumEt(0.),
	  hcalTowerSumEt(0.), hcalTowerFullConeSumEt(0.)
      {}
    } ;

    struct PFIsolationVariables
    {
      float chargedSumPt ;                  // charged candidates PF-isolation with electron footprint removed from PF-noPU
      float photonSumPt ;                   // photon candidates PF-isolation with electron footprint removed from PF-noPU
      float neutralHadronSumPt ;            // neutral hadron candidates PF-isolation with electron footprint removed from PF-noPU
      float sumPUPt ;                       // charged candidates PF-isolation with electron footprint removed from PF-PU. To be used for deltabeta correction
      float directionalChargedSumPt ;       // directional charged candidates PF-isolation with electron footprint removed from PF-noPU
      float directionalPhotonSumPt ;        // directional photon candidates PF-isolation with electron footprint removed from PF-noPU
      float directionalNeutralHadronSumPt ; // directional neutral hadron candidates PF-isolation with electron footprint removed from PF-noPU
      
      PFIsolationVariables()
	: chargedSumPt(0.), photonSumPt(0.), neutralHadronSumPt(0.),
	  sumPUPt(0.),
	  directionalChargedSumPt(0.), directionalPhotonSumPt(0.), directionalNeutralHadronSumPt(0.)
      {}
    } ;

    // 03 accessors
    float dr03Isolation() const { return dr03_.tkSumPt + dr03_.ecalRecHitSumEt + dr03_.hcalTowerSumEt; }
    float dr03TkSumPt() const { return dr03_.tkSumPt ; }
    float dr03EcalRecHitSumEt() const { return dr03_.ecalRecHitSumEt ; }
    float dr03HcalTowerSumEt() const { return dr03_.hcalTowerSumEt ; }
    float dr03HcalTowerFullConeSumEt() const { return dr03_.hcalTowerSumEt ; }

    float dr03pfIsolation() const { return dr03PF_.chargedSumPt +  dr03PF_.photonSumPt + dr03PF_.neutralHadronSumPt ; }
    float dr03pfIsolationDeltaBetaCorr() const { return  dr03PF_.chargedSumPt + fmax(dr03PF_.photonSumPt + dr03PF_.neutralHadronSumPt - 0.5*dr03PF_.sumPUPt, 0.); }
    float dr03pfChargedSumPt() const { return dr03PF_.chargedSumPt ; }
    float dr03pfPhotonSumEt() const { return dr03PF_.photonSumPt ; }
    float dr03pfNeutralHadronSumEt() const { return dr03PF_.neutralHadronSumPt ; }

    const DetectorIsolationVariables & dr03DetectorIsolationVariables() const { return dr03_ ; }
    const PFIsolationVariables & dr03PFIsolationVariables() const { return dr03PF_ ; }

    // 04 accessors
    float dr04Isolation() const { return dr04_.tkSumPt + dr04_.ecalRecHitSumEt + dr04_.hcalTowerSumEt; }
    float dr04TkSumPt() const { return dr04_.tkSumPt ; }
    float dr04EcalRecHitSumEt() const { return dr04_.ecalRecHitSumEt ; }
    float dr04HcalTowerSumEt() const { return dr04_.hcalTowerSumEt ; }
    float dr04HcalTowerFullConeSumEt() const { return dr04_.hcalTowerSumEt ; }

    float dr04pfIsolation() const { return dr04PF_.chargedSumPt +  dr04PF_.photonSumPt + dr04PF_.neutralHadronSumPt ; }
    float dr04pfIsolationDeltaBetaCorr() const { return  dr04PF_.chargedSumPt + fmax(dr04PF_.photonSumPt + dr04PF_.neutralHadronSumPt - 0.5*dr04PF_.sumPUPt, 0.); }
    float dr04pfChargedSumPt() const { return dr04PF_.chargedSumPt ; }
    float dr04pfPhotonSumEt() const { return dr04PF_.photonSumPt ; }
    float dr04pfNeutralHadronSumEt() const { return dr04PF_.neutralHadronSumPt ; }

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



  //=======================================================
  // Conversion Rejection Information
  //=======================================================

  public :

    struct ConversionRejection
     {
       float dist ; // distance to the conversion partner
       float dcot ; // difference of cot(angle) with the conversion partner track
       float radius ; // signed conversion radius
       bool hasMatchedConv ; // matched with a conversion vertex
      ConversionRejection()
       : dist(std::numeric_limits<float>::infinity()),
         dcot(std::numeric_limits<float>::infinity()),
         radius(std::numeric_limits<float>::infinity()),
	 hasMatchedConv(false)
       {}
     } ;

    // accessors
    float convDist() const { return conversionRejection_.dist ; }
    float convDcot() const { return conversionRejection_.dcot ; }
    float convRadius() const { return conversionRejection_.radius ; }
    bool hasMatchedConv() const { return conversionRejection_.hasMatchedConv ; }
    const ConversionRejection & conversionRejectionVariables() const { return conversionRejection_ ; }

    /// setter
    void setConversionVariables( const ConversionRejection & conv ) { conversionRejection_ = conv ; }

  private:

    // attributes
    ConversionRejection conversionRejection_ ;

  public:
    
    struct IDMvaOutput
    {
      float mvaTriggering ;      // trained for electrons that passed the single/double electron trigger
      float mvaNonTriggering ;   // trained on reconstructed electrons 
      float mvaPFlow ;           // old output of the PFlow electron identification
      IDMvaOutput()
        : mvaTriggering(-999999999.), mvaNonTriggering(-999999999.), mvaPFlow(-999999999.)
       {}
     } ;

    // acessors
    float mvaTriggering() const { return idMvaOutput_.mvaTriggering ; }
    float mvaNonTriggering() const { return idMvaOutput_.mvaNonTriggering ; }
    float mvaPFlow() const { return idMvaOutput_.mvaPFlow ; }

    // setter
    void setIDMVAs( const IDMvaOutput & mvas ) { idMvaOutput_ = mvas; }

  private:
    IDMvaOutput idMvaOutput_;

  public :
    
    // setters
    void setClassification( int myclass ) { class_ = myclass ; }
    void setFbrem( float fbrem ) { trackFbrem_ = fbrem ; }
    void setNbrem( float nbrem ) { numberOfBrems_ = nbrem ; }

    // get classification-related variables
    float trackFbrem() const { return trackFbrem_ ; };
    float superClusterFbrem() const ;
    float pfSuperClusterFbrem() const ;
    float fbrem() const { return trackFbrem() ; }
    int numberOfBrems() const { return numberOfBrems_; }

  private:
    int class_ ; // fbrem and number of clusters based electron classification
    float trackFbrem_;
    int numberOfBrems_;

  public:
    //=======================================================
    // Corrected Momentum Information
    //=======================================================
    LorentzVector calibratedP4();
    float calibratedEnergy() { return calibratedEnergy_ ; }
    float calibratedEnergyError() { return calibratedEnergyError_ ; }
    float trackMomentumError() { return trackMomentumError_ ; }
    
    // setters
    void setCalibratedEnergy(float energy) { calibratedEnergy_ = energy ; }
    void setCalibratedEnergyError(float error) { calibratedEnergyError_ = error ; }
    void setTrackMomentumError(float error) { trackMomentumError_ = error ; }

  private:
    float calibratedEnergy_;
    float calibratedEnergyError_;
    float trackMomentumError_;
    
  };

}

#endif

