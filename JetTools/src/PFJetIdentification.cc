#include <iostream>
#include "JetTools/include/PFJetIdentification.hh"

using namespace std;
using namespace vecbos;

bool PFJetIdentification::output(PFJet *jet) {

  int nConstituents = jet->chargedHadronMultiplicity() + jet->neutralHadronMultiplicity() +
    jet->photonMultiplicity() + jet->electronMultiplicity() + jet->muonMultiplicity() + 
    jet->HFHadronMultiplicity() + jet->HFEMMultiplicity();
  int chargedMultiplicity =  jet->chargedHadronMultiplicity() + jet->electronMultiplicity() + jet->muonMultiplicity();
  float eta = jet->eta();

  switch(wp_) {
  case loose:
    if(jet->neutralHadronEnergyFraction()>=0.99 || jet->photonEnergyFraction()>=0.99 || nConstituents<=1) return false;
    if(fabs(eta)<2.4 && (jet->chargedHadronEnergyFraction()==0 || chargedMultiplicity==0 || jet->chargedEmEnergyFraction()>=0.99) ) return false;
    break;
  case medium:
    if(jet->neutralHadronEnergyFraction()>=0.95 || jet->photonEnergyFraction()>=0.95 || nConstituents<=1) return false;
    if(fabs(eta)<2.4 && (jet->chargedHadronEnergyFraction()==0 || chargedMultiplicity==0 || jet->chargedEmEnergyFraction()>=0.99) ) return false;
    break;
  case tight:
    if(jet->neutralHadronEnergyFraction()>=0.90 || jet->photonEnergyFraction()>=0.90 || nConstituents<=1) return false;
    if(fabs(eta)<2.4 && (jet->chargedHadronEnergyFraction()==0 || chargedMultiplicity==0 || jet->chargedEmEnergyFraction()>=0.99) ) return false;
    break;
  default:
    std::cout << "Jet::isPFJetID(nt WP). Requested wrong Working point. Available are loose, medium, tight." << std::endl;
    return false;
  }
  return true;

}
