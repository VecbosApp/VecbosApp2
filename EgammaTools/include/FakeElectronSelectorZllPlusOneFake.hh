///---------------------------------------
// Description:
// Class to fill a tree of fake electron candidates to study ele ID
//---------------------------------------

#ifndef FAKE_ELECTRON_SELECTOR_ZLL_PLUS_ONE_FAKE_H
#define FAKE_ELECTRON_SELECTOR_ZLL_PLUS_ONE_FAKE_H

#include "Analysis/include/AnalysisBase.hh"
#include "Tools/include/HLTFilter.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "MuonTools/include/MuonIDSelector.hh"
#include "OutputTrees/include/ElectronIDTree.hh"

namespace vecbos {
  
  class FakeElectronSelectorZllPlusOneFake : public AnalysisBase {

  public:
    //! constructor
    FakeElectronSelectorZllPlusOneFake(TChain *tree=0);
    //! destructor
    virtual ~FakeElectronSelectorZllPlusOneFake() { }
    //! configure the needed stuff
    void BeginJob(JobConfiguration *conf);
    //! close the needed stuff
    void EndJob();
    //! loop over events
    void Loop();
    
  private:
    
    /// data members
    ElectronIDSelector elid_mva_loose;
    MuonIDSelector muonid_loose;
    HLTFilter *doublelep_filter_8TeV;
    ElectronIDTree *output;
    
    // fill the tree for the selected Z(ll)+1 electron
    void fillProbe(Electron *electron, float zmass, int zdecay);

  };
}

#endif
