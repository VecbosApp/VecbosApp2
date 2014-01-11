#ifndef DY_TO_EE_SELECTION_HH
#define DY_TO_EE_SELECTION_HH

#include "Analysis/include/AnalysisBase.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"
#include "Tools/include/HLTFilter.hh"
#include "DataFormats/include/Electron.hh"
#include "OutputTrees/include/ElectronIDTree.hh"

namespace vecbos {

  class DYToEESelection : public AnalysisBase {
  public:
    //! constructor
    DYToEESelection(TChain *chain);
    //! destructor
    virtual ~DYToEESelection() { };
    //! configure the needed stuff
    void BeginJob(JobConfiguration *conf);
    //! close the needed stuff
    void EndJob();
    //! loop over events
    void Loop();
    //! set the output file name
    void setOutputFile(std::string file) { outputFileName_ = file; }
    
  private:

    //! fill the electron tree with the given probe
    void fillProbe(float zeemass, Electron *electron);

    ElectronIDSelector elid_mva_tight;
    HLTFilter *doubleele_filter_8TeV;
    ElectronIDTree *output;

  };

}

#endif
