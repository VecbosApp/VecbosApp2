#ifndef VECBOS_HLT_FILTER_HH
#define VECBOS_HLT_FILTER_HH

#include <TTree.h>
#include <TBranch.h>
#include <string>
#include <vector>

namespace vecbos {

  class HLTFilter {

  public:
    typedef std::vector<std::string> path_list;
    typedef std::vector<int> position_list;
    
    /// constructor
    HLTFilter(path_list *nameHLT,  Int_t indexHLT[5000], Int_t firedTrg[5000]);
    /// destructor
    ~HLTFilter() { }

    /// configure from config file
    void configure(std::string cfg);

    /// response of the filter
    bool pass(int run);
  
private:
    
    //! init the tree branches
    //    void init();
    //! get the vector of the positions of the requested / vetoed paths
    position_list triggerMask(int run, path_list paths);
    //! parse the trigger path out of the string minrun-maxrun:path
    std::string getHLTPathForRun(int runN, std::string fullname);
    //! extract the values of all the bits requested and return true if at least one is 1
    bool getTriggersOR(position_list bits);

    //! variables of the tree holding the trigger string paths and positions
    path_list  *nameHLT_;
    Int_t indexHLT_[5000];
    //! variables holding the packed trigger bits values
    Int_t firedTrg_[5000];   //[nTrg]

    //! vector of the required / vetoed trigger paths
    path_list required_paths_;
    path_list vetoed_paths_;
    
    //! vector of the required / vetoed bits
    position_list m_requiredTriggers;
    position_list m_vetoedTriggers;

    //! cache if it is configured
    bool configured_;

  };
    
}

#endif
