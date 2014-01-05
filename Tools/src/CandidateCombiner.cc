#include <iostream>
#include "Tools/include/CandidateCombiner.hh"
#include "Tools/include/CandidateSorter.hh"

using namespace vecbos;
using namespace std;

CompositeCandidateCollection CandidateCombiner::output() {

  CompositeCandidateCollection output;
  if(inputs_.size()==0) 
    cout << "Warning: requested to combine 0 inputs. Returning an empty collection..." << endl;
  else if(inputs_.size()==1) {
    CandidateCollection cands = inputs_.front();
    for(int i1 = 0; i1 < cands.size(); ++i1) {
      for(int i2 = i1 + 1; i2 < cands.size(); ++i2) {
	CompositeCandidate comp;
	comp.addDaughter(cands[i1]);
	comp.addDaughter(cands[i2]);
	output.push_back(comp);
      }
    }
  } else if(inputs_.size()==2) {
    output = combineTwoCollections(inputs_.front(),inputs_.back());
  } else {
    output = combineTwoCollections(inputs_[0],inputs_[1]);
    for(int input = 2; input < inputs_.size(); ++input) 
      output = combineCompositeCollectionAndLeafCollection(output,inputs_[input]);
  }

  
  CandidateSorter sorter(output,"pt");
  return sorter.output();
}


CompositeCandidateCollection CandidateCombiner::combineTwoCollections(CandidateCollection first, CandidateCollection second) {
  CompositeCandidateCollection output;
  for(int i1 = 0; i1 < first.size(); ++i1) {
    for(int i2 = 0; i2 < second.size(); ++i2) {
      if(first[i1].overlap(second[i2])) continue;
      CompositeCandidate comp;
      comp.addDaughter(first[i1]);
      comp.addDaughter(second[i2]);
      output.push_back(comp);
    }
  }
  return output;
}

CompositeCandidateCollection CandidateCombiner::combineCompositeCollectionAndLeafCollection(CompositeCandidateCollection first, CandidateCollection second) {
  CompositeCandidateCollection output;
  for(int i1 = 0; i1 < first.size(); ++i1) {
    for(int i2 = 0; i2 < second.size(); ++i2) {
      if(first[i1].overlap(second[i2])) continue;
      bool overlapdau=false;
      for(int ndau1=0; ndau1<first[i1].numberOfDaughters(); ++ndau1)
	if(first[i1].daughter(ndau1).overlap(second[i2])) overlapdau=true;
      if(overlapdau) continue;
      CompositeCandidate comp;
      comp.addDaughter(first[i1]);
      comp.addDaughter(second[i2]);
      output.push_back(comp);
    }
  }
  return output;
}
