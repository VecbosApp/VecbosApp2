#include "DataFormats/include/Jet.hh"
#include <sstream> 
#include <string>

using namespace vecbos;

Jet::Jet(const LorentzVector& fP4, const Point& fVertex) 
  : Candidate(0, fP4, fVertex), JetArea_(0),p4raw_(LorentzVector(0, 0, 0, 0))  { }
  
Jet::Jet(const LorentzVector& fP4, const LorentzVector& fP4Raw, const Point& fVertex)
  : Candidate(0, fP4, fVertex), JetArea_(0), p4raw_(fP4Raw) { }

void Jet::scaleEnergy (double fScale) {
  setP4 (p4() * fScale);
}

std::string Jet::print () {
  std::ostringstream out;
  out << "Jet p/px/py/pz/pt: " << p() << '/' << px () << '/' << py() << '/' << pz() << '/' << pt() << std::endl
      << "    eta/phi: " << eta () << '/' << phi () << std::endl;
  return out.str ();
}
