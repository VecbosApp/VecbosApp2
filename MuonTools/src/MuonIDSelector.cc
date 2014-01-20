#include <string.h>
#include <iostream>

#include "MuonTools/include/MuonIDAlgo.hh"
#include "MuonTools/include/MuonIDSelector.hh"

using namespace vecbos;
using namespace std;

MuonIDSelector::MuonIDSelector() : rho_(0) {
  vertices_.clear();
  input_.clear();
  output_.clear();
}

MuonIDSelector::MuonIDSelector(MuonCollectionPtr input, float rho, VertexCollection vertices) : 
  rho_(rho), vertices_(vertices) 
{ 
  input_ = input;
  output_.clear(); 
}

MuonCollectionPtr MuonIDSelector::output() {
  if(algo_.compare("cuts_id")==0)       return output_cuts_id();
  else if(algo_.compare("cuts_iso")==0) return output_cuts_iso();
  else if(algo_.compare("cuts_ip")==0)  return output_cuts_ip();
  else if(algo_.compare("cuts")==0)     return output_cuts();

  else cout << "MuonIDSelector: algo " << algo_ << " is not foreseen, returning a null collection..." << endl;
  return output_;
}

MuonCollectionPtr MuonIDSelector::output_cuts_id() {
  output_.clear();
  for(muonptr_iterator mu=input_.begin(); mu!=input_.end(); ++mu) {
    MuonIDAlgo algo(rho_,vertices_);
    algo.setMuon(*(*mu));
    if(algo.pass_cuts_id(wp_)) output_.push_back(*mu);
  }
  return output_;
}

MuonCollectionPtr MuonIDSelector::output_cuts_iso() {
  output_.clear();
  for(muonptr_iterator mu=input_.begin(); mu!=input_.end(); ++mu) {
    MuonIDAlgo algo(rho_,vertices_);
    algo.setMuon(*(*mu));
    if(algo.pass_cuts_iso(wp_)) output_.push_back(*mu);
  }
  return output_;
}

MuonCollectionPtr MuonIDSelector::output_cuts_ip() {
  output_.clear();
  for(muonptr_iterator mu=input_.begin(); mu!=input_.end(); ++mu) {
    MuonIDAlgo algo(rho_,vertices_);
    algo.setMuon(*(*mu));
    if(algo.pass_cuts_ip(wp_)) output_.push_back(*mu);
  }
  return output_;
}

MuonCollectionPtr MuonIDSelector::output_cuts() {
  output_.clear();
  for(muonptr_iterator mu=input_.begin(); mu!=input_.end(); ++mu) {
    MuonIDAlgo algo(rho_,vertices_);
    algo.setMuon(*(*mu));
    if(algo.pass_cuts_id(wp_) && algo.pass_cuts_iso(wp_) && algo.pass_cuts_ip(wp_)
       ) output_.push_back(*mu);
  }
  return output_;
}

