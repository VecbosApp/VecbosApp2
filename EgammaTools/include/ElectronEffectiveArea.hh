#ifndef ElectronEffectiveArea_H
#define ElectronEffectiveArea_H

#include "DataFormats/include/Electron.hh"

namespace vecbos {

  class ElectronEffectiveArea{
  public:

    enum ElectronEffectiveAreaType {
      kEleTrkIso03, 
      kEleEcalIso03, 
      kEleHcalIso03, 
      kEleTrkIso04, 
      kEleEcalIso04, 
      kEleHcalIso04, 
      kEleChargedIso03, 
      kEleGammaIso03, 
      kEleNeutralHadronIso03, 
      kEleGammaAndNeutralHadronIso03,
      kEleChargedIso04, 
      kEleGammaIso04, 
      kEleNeutralHadronIso04, 
      kEleGammaAndNeutralHadronIso04,
      kEleGammaIsoDR0p0To0p1,
      kEleGammaIsoDR0p1To0p2,
      kEleGammaIsoDR0p2To0p3,
      kEleGammaIsoDR0p3To0p4,
      kEleGammaIsoDR0p4To0p5,
      kEleNeutralHadronIsoDR0p0To0p1,
      kEleNeutralHadronIsoDR0p1To0p2,
      kEleNeutralHadronIsoDR0p2To0p3,
      kEleNeutralHadronIsoDR0p3To0p4,
      kEleNeutralHadronIsoDR0p4To0p5
    };
      
    enum ElectronEffectiveAreaTarget {
      kEleEANoCorr,
      kEleEAData2011,
      kEleEASummer11MC,
      kEleEAFall11MC,
      kEleEAData2012
    };

    /// empty constructor (used to use just the standalone function)
    ElectronEffectiveArea(ElectronEffectiveAreaType type = kEleGammaAndNeutralHadronIso03, ElectronEffectiveAreaTarget EffectiveAreaTarget = kEleEAData2012);
    ~ElectronEffectiveArea() { };
  
    /// get the EA from eta
    double GetElectronEffectiveArea(double SCEta);
    /// get the EA from the electron
    double GetElectronEffectiveArea(Electron electron) { return GetElectronEffectiveArea(electron.superCluster().eta()); }

    /// get the rho corrected isolation for electron
    double getRhoCorrectedIsolation03(Electron electron, float rho);
    double getRhoCorrectedIsolation04(Electron electron, float rho);

  private:
    ElectronEffectiveAreaType type_;
    ElectronEffectiveAreaTarget target_;
    

  };

}

#endif
