#include <iostream>

#include "JetTools/include/FactorizedJetCorrector.h"
#include "JetTools/include/CorrectedJetProducer.hh"

using namespace vecbos;
using namespace std;

void CorrectedJetProducer::setDefaultCorrections() {
  if(gtName_.find("START")!=std::string::npos) corr_ = "L1FastJet:L2Relative:L3Absolute";
  else                                         corr_ = "L1FastJet:L2Relative:L3Absolute:L2L3Residual";
}

void CorrectedJetProducer::correctCollection(vecbos::CandidateCollectionPtr jets, float rho) {
  FactorizedJetCorrector corrector(corr_,corr_,gtName_,jetType_);
  for(candidateptr_iterator jetit=jets.begin(); jetit<jets.end(); ++jetit) {
    Jet *jet = dynamic_cast<Jet*>(*jetit);
    
    corrector.setJetEta( jet->eta() );
    corrector.setJetPt ( jet->rawpt() );
    corrector.setJetE  ( jet->rawenergy() );
    corrector.setJetA  ( jet->area() );
    corrector.setRho   ( rho );

    float jec = corrector.getCorrection();
    float oldjec = jet->pt()/jet->rawpt();

    jet->scaleEnergy(jec/oldjec);
    
  }
}

vecbos::JetCollectionPtr CorrectedJetProducer::output(vecbos::CandidateCollectionPtr jets, float rho) {

  JetCollectionPtr newcoll;

  FactorizedJetCorrector corrector(corr_,corr_,gtName_,jetType_);
  for(candidateptr_iterator jetit=jets.begin(); jetit<jets.end(); ++jetit) {
    Jet *jet = dynamic_cast<Jet*>(*jetit);
    
    corrector.setJetEta( jet->eta() );
    corrector.setJetPt ( jet->rawpt() );
    corrector.setJetE  ( jet->rawenergy() );
    corrector.setJetA  ( jet->area() );
    corrector.setRho   ( rho );

    float jec = corrector.getCorrection();
    float oldjec = jet->pt()/jet->rawpt();

    Jet *scaledJet = new Jet(*jet);
    scaledJet->scaleEnergy(jec/oldjec);

    newcoll.push_back(scaledJet);
  }
  return newcoll;
}
