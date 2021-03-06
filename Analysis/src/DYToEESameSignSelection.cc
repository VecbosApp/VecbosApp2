#include <iostream>

#include "DataFormats/include/SuperCluster.hh"
#include "DataFormats/include/SuperClusterFwd.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "EgammaTools/include/ElectronIDAlgo.hh"
#include "Tools/include/CandidateCombiner.hh"
#include "Tools/include/CandidateSorter.hh"
#include "Tools/include/GenParticleCandidateMatch.hh"
#include "Tools/include/CandidateKinematicFilter.hh"

#include "Analysis/include/DYToEESameSignSelection.hh"


using namespace std;
using namespace vecbos;

DYToEESameSignSelection::DYToEESameSignSelection(TChain *chain) :
  AnalysisBase(chain) { }

void DYToEESameSignSelection::BeginJob(JobConfiguration *conf) {
  AnalysisBase::BeginJob(conf);

  /// electron ID selector
  elid_mva_tight.configure("EgammaTools/data/electrons_mva_tight.cfg");

  /// HLT selector
  doubleele_filter_8TeV = new HLTFilter(fChain,ismc_);
  if(ismc_) doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_mc_2012.txt");
  else doubleele_filter_8TeV->configure("Analysis/data/hlt/double_electron_data_tagandprobe_2012.txt");

  /// output tree
  output = new ElectronChargeMisIDTree((outputFileName_+".root").c_str());
  
}

void DYToEESameSignSelection::EndJob() {
  AnalysisBase::EndJob();
  output->save();
}

void DYToEESameSignSelection::Loop() {

  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntries();
  
  Long64_t nbytes = 0, nb = 0;
  Int_t cachesize = 30000000; //30 MBytes
  fChain->SetCacheSize(cachesize);
  fChain->SetCacheLearnEntries(5);
  std::cout << "The chain contains " << nentries << " entries " << endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = loadTree(jentry);
     if (ientry < 0) break;

     EventHeader header = Event.eventHeader();
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

     output->setVerticesAndRho(PrimaryVertices,rhoFastjet);

     /// keep only the Z->ee with SS electrons (with any charge estimation)
     for(CompositeCandidateCollection::iterator Zee=ZeeSorted.begin(); Zee<ZeeSorted.end(); ++Zee) {

       output->fillRunInfos(Zee->mass(), header.run(), header.lumi(), header.event(),
                            nPU,PrimaryVertices.size());

       Electron *ele1 = dynamic_cast<Electron*>(Zee->daughter(0));
       Electron *ele2 = dynamic_cast<Electron*>(Zee->daughter(1));

       ElectronIDAlgo eleID(rhoFastjet,PrimaryVertices);
       eleID.setElectron(*ele1);	 
       bool ele1ID = eleID.pass_mva("mva","loose");
       eleID.setElectron(*ele2);	 
       bool ele2ID = eleID.pass_mva("mva","loose");
       if( ele1ID && ele2ID ) {
         ElectronCollectionPtr ele_pair;
         ele_pair.push_back(ele1);
         ele_pair.push_back(ele2);
         output->fillElectronInfos(ele_pair);
         
         GenParticleCandidateMatch mcMatch(GenParticles);
         mcMatch.setDeltaR(0.4);
         mcMatch.checkStatus(1);
         GenParticle *matchedEle1 = mcMatch.overlap(ele1,11);
         GenParticle *matchedEle2 = mcMatch.overlap(ele2,11);
         
         float deltaR1(100.), deltaR2(100.);
         if(matchedEle1) deltaR1 = matchedEle1->momentum().DeltaR( ele1->momentum() );
         if(matchedEle2) deltaR2 = matchedEle2->momentum().DeltaR( ele2->momentum() );
         output->fillGenMatch(deltaR1,deltaR2);
         
         output->store();
       }
     }
  } // loop on events
}
