#ifndef DY_TO_EE_SELECTION_HH
#define DY_TO_EE_SELECTION_HH

#include "Analysis/include/AnalysisBase.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "Tools/include/HLTFilter.hh"

namespace vecbos {

  class DYToEESelection : public AnalysisBase {
  public:
    //! constructor
    DYToEESelection(TChain *chain);
    //! destructor
    virtual ~DYToEESelection() { };
    //! configure the needed stuff
    void BeginJob(bool isMC);
    //! loop over events
    void Loop();
    
  private:

    ElectronIDSelector elid_mva_tight;
    HLTFilter *doubleele_filter_8TeV;
    bool ismc_;

  };

}

#endif
