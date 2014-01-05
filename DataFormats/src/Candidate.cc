#include "DataFormats/include/Candidate.hh"

using namespace vecbos;

Candidate::~Candidate() {  }

bool Candidate::overlap(Candidate c) {
  return (this->pdgId()==c.pdgId() &&
	  this->charge()==c.charge() &&
	  this->p4()==c.p4() &&
	  this->vertex()==c.vertex());
}
