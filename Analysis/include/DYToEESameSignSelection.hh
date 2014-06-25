#ifndef DY_TO_EE_SAMESIGN_SELECTION_HH
#define DY_TO_EE_SAMESIGN_SELECTION_HH

#include "Analysis/include/AnalysisBase.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "Tools/include/HLTFilter.hh"
#include "DataFormats/include/Electron.hh"
#include "OutputTrees/include/ElectronChargeMisIDTree.hh"

namespace vecbos {

  class DYToEESameSignSelection : public AnalysisBase {
  public:
    //! constructor
    DYToEESameSignSelection(TChain *chain);
    //! destructor
    virtual ~DYToEESameSignSelection() { };
    //! configure the needed stuff
    void BeginJob(JobConfiguration *conf);
    //! close the needed stuff
    void EndJob();
    //! loop over events
    void Loop();
    
  private:

    ElectronIDSelector elid_mva_tight;
    HLTFilter *doubleele_filter_8TeV;
    ElectronChargeMisIDTree *output;

  };

}

#endif
