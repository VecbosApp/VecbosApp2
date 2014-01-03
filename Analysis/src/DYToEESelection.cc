#include <iostream>

#include "DataFormats/include/SuperCluster.hh"
#include "DataFormats/include/SuperClusterFwd.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"

#include "Analysis/include/DYToEESelection.hh"


using namespace std;
using namespace vecbos;

DYToEESelection::DYToEESelection(TChain *chain) :
  AnalysisBase(chain) { }

void DYToEESelection::BeginJob(bool isMC) {
  ismc_=isMC;
  elid_mva_tight.configure("EgammaTools/data/electrons_mva_tight.cfg");
  doubleele_filter_8TeV = new HLTFilter(nameHLT,indexHLT,firedTrg);
  if(ismc_) doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_mc_2012.txt");
  else doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_data_2012.txt");
}

void DYToEESelection::Loop() {

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  
  Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = loadTree(jentry);
     if (ientry < 0) break;
     cout << "Processing Event " << jentry << endl; 
     
     // EventHeader header = Event.eventHeader();
     // cout << "Event header: run = " << header.run() << "\tlumi = " << header.lumi() 
     // 	  << "\t evt = " << header.event() << endl;
     
     //    cout << "bella pise  " <<  nameHLT->size() << endl;

     /*
     bool passhlt = doubleele_filter_8TeV->pass(header.run());
     //     if(passhlt) cout << "\t===>This event passes HLT " << endl;

     elid_mva_tight.source(Electrons);
     elid_mva_tight.setRho(rhoFastjet);
     elid_mva_tight.setPrimaryVertices(PrimaryVertices);

     ElectronCollection tight_electrons = elid_mva_tight.output();
     cout << "reco electrons size = " << Electrons.size() << "   loose electrons = " << tight_electrons.size() << endl;
     */

   }

}
