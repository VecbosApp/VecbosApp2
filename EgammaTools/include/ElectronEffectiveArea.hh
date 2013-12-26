#ifndef ElectronEffectiveArea_H
#define ElectronEffectiveArea_H

namespace vecbos {

  class ElectronEffectiveArea{
  public:
    ElectronEffectiveArea() { };
    ~ElectronEffectiveArea() { };
  
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

    double GetElectronEffectiveArea(ElectronEffectiveAreaType type, double SCEta, ElectronEffectiveAreaTarget EffectiveAreaTarget = kEleEAData2011);

  };

}

#endif
