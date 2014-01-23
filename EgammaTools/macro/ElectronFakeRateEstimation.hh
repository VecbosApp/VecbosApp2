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

    /// attach the friend tree
    void addFriend(const char *filename) { _friendtree = std::string(filename); }

    /// calculate the fake rate for some 
    void calculate();

  private:
    std::string _friendtree;
 };

}

#endif

