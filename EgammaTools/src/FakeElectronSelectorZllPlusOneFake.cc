#include "Tools/include/CandidateCombiner.hh"
#include "Tools/include/CandidateSorter.hh"
#include "Tools/include/CandidateKinematicFilter.hh"
#include "Math/include/Constants.h"

#include "EgammaTools/include/FakeElectronSelectorZllPlusOneFake.hh"

using namespace std;
using namespace vecbos;

FakeElectronSelectorZllPlusOneFake::FakeElectronSelectorZllPlusOneFake(TChain *chain) :
  AnalysisBase(chain) { }

void FakeElectronSelectorZllPlusOneFake::BeginJob(JobConfiguration *conf) {
  AnalysisBase::BeginJob(conf);
  
  /// muon ID selector
  muonid_loose.configure("MuonTools/data/muons_cuts_loose.cfg");
  /// electron ID selector
  elid_mva_loose.configure("EgammaTools/data/electrons_mva_loose.cfg");

  /// HLT selector
  doublelep_filter_8TeV = new HLTFilter(fChain,ismc_);
  if(ismc_) {
    doublelep_filter_8TeV->configure("Analysis/data/hlt/double_electron_mc_2012.txt");
    doublelep_filter_8TeV->configure("Analysis/data/hlt/double_muon_data_2012.txt"); // to be checked if it is really the same as in data
  }
  else {
    doublelep_filter_8TeV->configure("Analysis/data/hlt/double_electron_data_2012.txt");
    doublelep_filter_8TeV->configure("Analysis/data/hlt/double_muon_data_2012.txt");
  }

  /// output tree
  output = new ElectronIDTree((outputFileName_+".root").c_str());
  output->addRunInfos();
  output->addAttributesSignal();
  output->addMomenta();
  output->addMVAs();
  output->addElectronIdBits();
  output->addDenominatorFakeBits();
  output->addIsolations();
  output->addEventInfos();
  
}

void FakeElectronSelectorZllPlusOneFake::EndJob() {
  AnalysisBase::EndJob();
  output->save();
}

void FakeElectronSelectorZllPlusOneFake::Loop() {

  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;

  math::PhysConstants physconst;

  Int_t cachesize = 30000000; //30 MBytes
  fChain->SetCacheSize(cachesize);
  fChain->SetCacheLearnEntries(5);
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = loadTree(jentry);
    if (ientry < 0) break;
    
    output->setVertices(PrimaryVertices);
    
    EventHeader header = Event.eventHeader();
    output->fillRunInfos(header.run(), header.lumi(), header.event(),
			 nPU,PrimaryVertices.size(), rhoFastjet, 1);

    if( !doublelep_filter_8TeV->pass(jentry,header.run()) ) continue;

    CandidateKinematicFilter eleFilter;
    eleFilter.source(Electrons);
    eleFilter.setPtRange(7,1000); 
    eleFilter.setEtaRange(-2.5,2.5); 
    CandidateCollectionPtr AcceptanceElectrons = eleFilter.output();

    Electron *electron = 0;
    float zmass;

    /// ========================================================
    /// Make the Z => mumu + 1 electron first
    /// ========================================================
    CandidateKinematicFilter muonFilter;
    muonFilter.source(Muons);
    muonFilter.setPtRange(5,1000); 
    muonFilter.setEtaRange(-2.4,2.4); 
    CandidateCollectionPtr AcceptanceMuons = muonFilter.output();

    muonid_loose.source(AcceptanceMuons);
    muonid_loose.setPrimaryVertices(PrimaryVertices);
    CandidateCollectionPtr LooseMuons = muonid_loose.output();

    CandidateCombiner zmmcombiner;
    zmmcombiner.addDaughterCollection(LooseMuons);
    zmmcombiner.setMassRange(physconst.Z0_mass-15.0, physconst.Z0_mass+15.0);
    CompositeCandidateCollection zmmUnsorted = zmmcombiner.output();

    /// sort by the scalar sum of the daughters
    CandidateSorter zmmsorter(zmmUnsorted,"sumpt");
    CompositeCandidateCollection ZmmSorted = zmmsorter.output();
    
    if(ZmmSorted.size() > 0 && AcceptanceElectrons.size() > 0) {
      zmass = ZmmSorted.front().mass();
      electron = dynamic_cast<Electron*>(AcceptanceElectrons.front());
      fillProbe(electron, zmass, 13);
      continue;
    }
    
    /// if not found a Z(mumu) + 1 electron...
    /// ========================================================
    /// Make the Z => ee + 1 electron
    /// ========================================================
    
    elid_mva_loose.source(AcceptanceElectrons);
    elid_mva_loose.setRho(rhoFastjet);
    elid_mva_loose.setPrimaryVertices(PrimaryVertices);
    CandidateCollectionPtr LooseElectrons = elid_mva_loose.output();
    
    CandidateCombiner zeecombiner;
    zeecombiner.addDaughterCollection(LooseElectrons);
    zeecombiner.setMassRange(physconst.Z0_mass-15.0, physconst.Z0_mass+15.0);
    CompositeCandidateCollection zeeUnsorted = zeecombiner.output();
    
    /// sort by the scalar sum of the daughters
    CandidateSorter zeesorter(zeeUnsorted,"sumpt");
    CompositeCandidateCollection ZeeSorted = zeesorter.output();
    
    if(ZeeSorted.size()==0) continue;
    
    CompositeCandidate Zee = ZeeSorted.front();
    zmass = Zee.mass();
    Electron *ele1 = dynamic_cast<Electron*>(Zee.daughter(0));
    Electron *ele2 = dynamic_cast<Electron*>(Zee.daughter(1));
    
    for(candidateptr_iterator ele = AcceptanceElectrons.begin(); ele < AcceptanceElectrons.end(); ++ele) {
      // skip the Z->ee daughters
      if( (*ele)->overlap(*ele1) || (*ele)->overlap(*ele2) ) continue; 
      // skip the possible brem candidates in a cone 0.3 from Z leptons
      if( (*ele)->angularMatch(*ele1) || (*ele)->angularMatch(*ele2) ) continue;
      electron = dynamic_cast<Electron*>(*ele);
      break;
    }

    if(electron > 0) fillProbe(electron, zmass, 11);

  } // loop over the events
}

void FakeElectronSelectorZllPlusOneFake::fillProbe(Electron *electron, float zmass, int zdecay) {
  output->fillElectronInfos(*electron);
  output->fillAttributesSignal(zmass, zdecay, -1, -1, -1);
  output->fillEventInfos(PfMet.p());
  output->store();
}
