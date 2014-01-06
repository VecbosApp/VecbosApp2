#include <iostream>

#include "DataFormats/include/CompositeCandidate.hh"

using namespace std;
using namespace vecbos;

CompositeCandidate::~CompositeCandidate() { }

Candidate CompositeCandidate::daughter(int i) { 
  return (i < numberOfDaughters()) ? dau[ i ] : Candidate(); 
}

int CompositeCandidate::numberOfDaughters() { return dau.size(); }

void CompositeCandidate::addDaughter(Candidate & cand) {
  dau.push_back(cand);
  charge_ += cand.charge();

  this->clearCache();
  LorentzVector myp4 = this->p4();
  myp4 += cand.p4();
  pt_  = myp4.Pt();
  eta_ = myp4.Eta(); 
  phi_ = myp4.Phi();
  mass_ = myp4.M();

  vertex_ = Point(0, 0, 0); /// otherwise, set it by hand
}
