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
    
    /// constructor from tree
    HLTFilter(TTree *tree, bool isMC);
    /// destructor
    ~HLTFilter() { }

    /// configure from config file
    void configure(std::string cfg);

    /// require the bits also on Mc
    void requireHLTOnMC() { requireHLTOnMC_=true; }

    /// response of the filter
    bool pass(int entry, int run);
  
private:
    
    //! init the tree branches
    void init();
    //! get the vector of the positions of the requested / vetoed paths
    position_list triggerMask(int run, path_list paths);
    //! parse the trigger path out of the string minrun-maxrun:path
    std::string getHLTPathForRun(int runN, std::string fullname);
    //! extract the values of all the bits requested and return true if at least one is 1
    bool getTriggersOR(position_list bits);

    //! the tree holding the trigger results
    TTree * tree_;
    TBranch        *b_firedTrg;
    TBranch        *b_indexHLT;
    TBranch        *b_nameHLT;
    
    //! variables of the tree holding the trigger string paths and positions
    path_list  *nameHLT;
    Int_t indexHLT[5000];
    Int_t firedTrg[5000];

    //! vector of the required / vetoed trigger paths
    path_list required_paths_;
    path_list vetoed_paths_;
    
    //! vector of the required / vetoed bits
    position_list m_requiredTriggers;
    position_list m_vetoedTriggers;

    //! cache if it is configured
    bool configured_;

    //! require the HLT on the MC
    bool ismc_;
    bool requireHLTOnMC_;

  };
    
}

#endif
