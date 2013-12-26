#ifndef ELECTRON_FLAGS_HH
#define ELECTRON_FLAGS_HH

namespace vecbosflags {

  enum ElectronFiducialBit { isEERingGap=0, isEEDeeGap=1, isEEGap=2, isEBPhiGap=3, isEBEtaGap=4, isEBGap=5, isEBEEGap=6, isGap=7, isEE=8, isEB=9 };
  enum ElectronRecoBit { isTrackerDriven=0, isEcalDriven=1 };
  enum ElectronEnergyCorrectionBit { isMomentumCorrected=0, isEcalEnergyCorrected=1 };
  enum ElectronClassification { UNKNOWN=-1, GOLDEN=0, BIGBREM=1, BADTRACK=2, SHOWERING=3, GAP=4 } ;
    

}

#endif
