#include "Tools/include/HLTFilter.hh"

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace vecbos;
using namespace std;


HLTFilter::HLTFilter(TTree *tree) : tree_(tree), configured_(false) {
  init();
}

void HLTFilter::init() {
  nameHLT = 0;
  tree_->SetBranchAddress("firedTrg", firedTrg, &b_firedTrg);
  tree_->SetBranchAddress("nameHLT", &nameHLT, &b_nameHLT);
  tree_->SetBranchAddress("indexHLT", indexHLT, &b_indexHLT);
}

void HLTFilter::configure(std::string cfg) {
  std::ifstream cfgfile(cfg.c_str());
  if(!cfgfile.good()) {
    std::cout << "Error!   Unable to open the cfg file " << cfg << std::endl;
    configured_ = false;
    return;
  } else {
    int requirebool;
    std::string val;
    while (cfgfile >> requirebool >> val) {
      if(requirebool==1) required_paths_.push_back(val);
      else if(requirebool==0) vetoed_paths_.push_back(val);
      else cout << "Warning! Strange character found as first field in the trigger string: " << requirebool 
		<< " in file " << cfg << ", expecting 0/1" << endl;
    }
    cfgfile.close();
    configured_=true;
  }
}

bool HLTFilter::pass(int entry, int run) {
  if(!configured_) {
    cout << "Warning! HLTFilter not configured, returning false... " << endl;
    return false;
  }
  m_requiredTriggers.clear();
  m_vetoedTriggers.clear();

  tree_->GetEntry(entry);
  
  m_requiredTriggers = triggerMask(run, required_paths_);
  m_vetoedTriggers = triggerMask(run, vetoed_paths_);

  bool req = getTriggersOR(m_requiredTriggers);
  bool veto = getTriggersOR(m_vetoedTriggers);
  
  return (req && !veto);
}

HLTFilter::position_list HLTFilter::triggerMask(int run, path_list paths) {

  position_list mask;

  for (path_list::const_iterator fIter=paths.begin(); fIter!=paths.end(); ++fIter)
    {   
      std::string pathName = getHLTPathForRun(run,*fIter);
      for(unsigned int i=0; i<nameHLT->size(); i++)
        {
          if(nameHLT->at(i).find(pathName) != string::npos)
            {
              mask.push_back( indexHLT[i] ) ;
              break;
            }
        }
    }
  
  return mask;
}

std::string HLTFilter::getHLTPathForRun(int runN, std::string fullname) {
  TString fullName = TString(fullname.c_str());
  TObjArray* selectionTokens = fullName.Tokenize(":");
  if (selectionTokens->GetEntries()!=2) {
    std::cout << "Wrong trigger strings " << selectionTokens->GetEntries() << std::endl;
    return std::string("NOPATH");
  }
  TString RunRange =((TObjString*)(*selectionTokens)[0])->GetString();
  TString HLTPathName =((TObjString*)(*selectionTokens)[1])->GetString();
  
  TObjArray* runs = RunRange.Tokenize("-");
  if (runs->GetEntries()!=2) {
    std::cout << "Wrong trigger run range strings " << runs->GetEntries() << std::endl;
    return std::string("NOPATH");    
  }
  
  const char *minStr = (((TObjString*)(*runs)[0])->GetString()).Data();
  const char *maxStr = (((TObjString*)(*runs)[1])->GetString()).Data();

  int min = atoi(minStr);
  int max = atoi(maxStr);

  if(runN>=min && runN<=max) return std::string(HLTPathName.Data());
  else return std::string("NOPATH");
}

bool HLTFilter::getTriggersOR(position_list bits) {

  // unpack the trigger words
  for( int i=0; i<bits.size(); i++ ) {

    int block =  bits[i]/30;
    int pos = bits[i]%30;
    int word = firedTrg[block];
    
    if ( (word >> pos)%2 ) return true;
  }

  return false;

}
