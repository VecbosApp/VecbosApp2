#include <iostream>

#include "DataFormats/include/SuperCluster.hh"
#include "DataFormats/include/SuperClusterFwd.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "EgammaTools/include/ElectronIDAlgo.hh"
#include "Tools/include/CandidateCombiner.hh"
#include "Tools/include/CandidateSorter.hh"
#include "Tools/include/GenParticleCandidateMatch.hh"
#include "Tools/include/CandidateKinematicFilter.hh"

#include "Analysis/include/DYToEESelection.hh"


using namespace std;
using namespace vecbos;

DYToEESelection::DYToEESelection(TChain *chain) :
  AnalysisBase(chain) { }

void DYToEESelection::BeginJob(bool isMC) {
  AnalysisBase::BeginJob(isMC);

  /// electron ID selector
  elid_mva_tight.configure("EgammaTools/data/electrons_mva_tight.cfg");

  /// HLT selector
  doubleele_filter_8TeV = new HLTFilter(fChain,isMC);
  if(ismc_) doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_mc_2012.txt");
  else doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_data_2012.txt");

  /// output tree
  output = new ElectronIDTree((outputFileName_+".root").c_str());
  output->addRunInfos();
  output->addAttributesSignal();
  output->addMomenta();
  output->addMVAs();
  output->addElectronIdBits();
  output->addDenominatorFakeBits();
  output->addIsolations();
  
}

void DYToEESelection::EndJob() {
  AnalysisBase::EndJob();
  output->save();
}

void DYToEESelection::Loop() {

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  
  Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = loadTree(jentry);
     if (ientry < 0) break;

     output->setVertices(PrimaryVertices);

     EventHeader header = Event.eventHeader();
     output->fillRunInfos(header.run(), header.lumi(), header.event(),
			 nPU,PrimaryVertices.size(), rhoFastjet, 1);

     bool passhlt = doubleele_filter_8TeV->pass(jentry,header.run());
     if(!passhlt) continue;
     // if(passhlt) cout << "\t===>This event passes HLT " << endl;

     CandidateKinematicFilter eleFilter;
     eleFilter.source(Electrons);
     eleFilter.setPtRange(7,1000); 
     eleFilter.setEtaRange(-2.5,2.5); 
     CandidateCollectionPtr AcceptanceElectrons = eleFilter.output();

     CandidateCombiner zeecombiner;
     zeecombiner.addDaughterCollection(AcceptanceElectrons);
     CompositeCandidateCollection zeeUnsorted = zeecombiner.output();

     /// sort by the scalar sum of the daughters
     CandidateSorter sorter(zeeUnsorted,"sumpt");
     CompositeCandidateCollection ZeeSorted = sorter.output();

     if(ZeeSorted.size()>0) {
       CompositeCandidate Zee = ZeeSorted.front();
       Electron *ele1 = dynamic_cast<Electron*>(Zee.daughter(0));
       Electron *ele2 = dynamic_cast<Electron*>(Zee.daughter(1));

       ElectronIDAlgo eleID(rhoFastjet,PrimaryVertices);
       eleID.setElectron(*ele1);	 
       if( eleID.pass_mva("mva","loose") ) fillProbe(Zee.mass(), ele2);
       eleID.setElectron(*ele2);	 
       if( eleID.pass_mva("mva","loose") ) fillProbe(Zee.mass(), ele1);       
       output->store();
     }
   }
}

void DYToEESelection::fillProbe(float zeemass, Electron *electron) {

  output->fillElectronInfos(*electron);

  GenParticleCandidateMatch mcMatch(GenParticles);
  mcMatch.setDeltaR(0.4);
  mcMatch.checkStatus(1);
  GenParticle *matchedEle = mcMatch.overlap(electron,11);

  if(matchedEle) output->fillAttributesSignal(zeemass,
					      1,
					      matchedEle->energy(),
					      matchedEle->theta(),
					      matchedEle->phi());

  else output->fillAttributesSignal(zeemass,0,-1,-999,-999);	 
}
