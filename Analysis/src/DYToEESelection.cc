#include <iostream>

#include "DataFormats/include/SuperCluster.hh"
#include "DataFormats/include/SuperClusterFwd.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "EgammaTools/include/ElectronIDAlgo.hh"
#include "Tools/include/CandidateCombiner.hh"
#include "Tools/include/CandidateSorter.hh"

#include "Analysis/include/DYToEESelection.hh"


using namespace std;
using namespace vecbos;

DYToEESelection::DYToEESelection(TChain *chain) :
  AnalysisBase(chain) { }

void DYToEESelection::BeginJob(bool isMC) {
  ismc_=isMC;

  /// electron ID selector
  elid_mva_tight.configure("EgammaTools/data/electrons_mva_tight.cfg");

  /// HLT selector
  doubleele_filter_8TeV = new HLTFilter(fChain);
  if(ismc_) doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_mc_2012.txt");
  else doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_data_2012.txt");

  /// output tree
  output = new ElectronIDTree("eleid.root");
  output->addRunInfos();
  output->addAttributesSignal();
  output->addMomenta();
  output->addMVAs();
  output->addElectronIdBits();
  output->addDenominatorFakeBits();
  output->addIsolations();
  
}

void DYToEESelection::EndJob() {
  output->save();
}

void DYToEESelection::Loop() {

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  
  Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = loadTree(jentry);
     if (ientry < 0) break;
     cout << "Processing Event " << jentry << endl; 
     
     EventHeader header = Event.eventHeader();
     cout << "Event header: run = " << header.run() << "\tlumi = " << header.lumi() 
      	  << "\t evt = " << header.event() << endl;
     
     output->setVertices(PrimaryVertices);
     output->fillRunInfos(header.run(), header.lumi(), header.event(),
			 nPU,PrimaryVertices.size(), rhoFastjet, 1);

     // bool passhlt = doubleele_filter_8TeV->pass(jentry,header.run());
     // if(passhlt) cout << "\t===>This event passes HLT " << endl;

     CandidateCombiner zeecombiner;
     zeecombiner.addDaughterCollection(Electrons);
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
       if( eleID.pass_mva("mva","loose") ) {
	 output->fillElectronInfos(*ele2);
	 output->fillAttributesSignal(Zee.mass(),1,0,0,0);
       }
       eleID.setElectron(*ele2);
       if( eleID.pass_mva("mva","loose") ) {
	 output->fillElectronInfos(*ele1);
	 output->fillAttributesSignal(Zee.mass(),1,0,0,0);
       }

       output->store();
     }
   }
}
