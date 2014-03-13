#ifndef ELECTRON_FAKERATE_ESTIMATION_HH
#define ELECTRON_FAKERATE_ESTIMATION_HH

#include <string>
#include "egtree.h"


namespace macros {

  class ElectronFakeRateEstimation : public egtree {
  public:
    /// constructor
    ElectronFakeRateEstimation(TTree *tree=0) : egtree(tree) { }
    /// destructor
    ~ElectronFakeRateEstimation() { };
    
    /// set the output file name
    void SetOutputFileName(std::string outfilename) { outname_ = outfilename; }
    
    /// calculate the fake rate for some 
    void calculateFakeRates();
    
  private:
    std::string outname_;
  };

}

#endif

