#include <string.h>
#include <iostream>

#include "EgammaTools/include/ElectronIDAlgo.hh"
#include "EgammaTools/include/ElectronIDSelector.hh"

using namespace vecbos;
using namespace std;

ElectronIDSelector::ElectronIDSelector() : rho_(0) {
  vertices_.clear();
  input_.clear();
  output_.clear();
}

ElectronIDSelector::ElectronIDSelector(ElectronCollection input, float rho, VertexCollection vertices) : 
  rho_(rho), vertices_(vertices) 
{ 
  input_ = input;
  output_.clear(); 
}

ElectronCollection ElectronIDSelector::output() {
  if(algo_.compare("hlt")==0) output_hlt();

  else if(algo_.compare("cuts_id")==0)      return output_cuts_id();
  else if(algo_.compare("cuts_iso")==0)     return output_cuts_iso();
  else if(algo_.compare("cuts_convrej")==0) return output_cuts_convrej();
  else if(algo_.compare("cuts_ip")==0)      return output_cuts_ip();
  else if(algo_.compare("cuts")==0)         return output_cuts();

  else if(algo_.compare("mva_id")==0)      return output_mva_id();
  else if(algo_.compare("mva_iso")==0)     return output_mva_iso();
  else if(algo_.compare("mva_convrej")==0) return output_mva_convrej();
  else if(algo_.compare("mva_ip")==0)      return output_mva_ip();
  else if(algo_.compare("mva")==0)         return output_mva();

  else cout << "ElectronIDSelector: algo " << algo_ << " is not foreseen, returning a null collection..." << endl;
  return output_;
}

ElectronCollection ElectronIDSelector::output_hlt() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_hlt()) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_cuts_id() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_cuts_id(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_cuts_iso() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_cuts_iso(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_cuts_convrej() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_cuts_convrej(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_cuts_ip() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_cuts_ip(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_cuts() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_cuts_id(wp_) && algo.pass_cuts_iso(wp_) && 
       algo.pass_cuts_convrej(wp_) && algo.pass_cuts_ip(wp_)
       ) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_mva_id() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_mva_id(algo_,wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_mva_iso() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_mva_iso(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_mva_convrej() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_mva_convrej(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_mva_ip() {
  output_.clear();
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_mva_ip(wp_)) output_.push_back(*ele);
  }
  return output_;
}

ElectronCollection ElectronIDSelector::output_mva() {
  output_.clear();
  int iele=0;
  for(electron_iterator ele=input_.begin(); ele!=input_.end(); ++ele) {
    ElectronIDAlgo algo(rho_,vertices_);
    algo.setElectron(*ele);
    if(algo.pass_mva_id(algo_,wp_) && algo.pass_mva_iso(wp_) && 
       algo.pass_mva_convrej(wp_) && algo.pass_mva_ip(wp_)
       ) output_.push_back(*ele);
    ++iele;
  }
  return output_;
}
