#include "DataFormats/include/PFJet.hh"
#include <sstream>
#include <string>

using namespace vecbos;

PFJet::PFJet (const LorentzVector& fP4, const Point& fVertex) :
  Jet(fP4,fVertex) 
{ 
  m_specific = Specific();
}

PFJet::PFJet (const LorentzVector& fP4, const LorentzVector& fP4Raw, const Point& fVertex) :
  Jet(fP4,fP4Raw,fVertex) 
{
  m_specific = Specific();
}

std::string PFJet::print () {
  std::ostringstream out;
  out << Jet::print () // generic jet info
      << "    PFJet specific:" << std::endl
      << "      charged hadron energy/multiplicity: " << chargedHadronEnergy () << '/' << chargedHadronMultiplicity () << std::endl
      << "      neutral hadron energy/multiplicity: " << neutralHadronEnergy () << '/' << neutralHadronMultiplicity () << std::endl
      << "      photon energy/multiplicity: " << photonEnergy () << '/' << photonMultiplicity () << std::endl
      << "      electron energy/multiplicity: " << electronEnergy () << '/' << electronMultiplicity () << std::endl
      << "      muon energy/multiplicity: " << muonEnergy () << '/' << muonMultiplicity () << std::endl
      << "      HF Hadron energy/multiplicity: " << HFHadronEnergy () << '/' << HFHadronMultiplicity () << std::endl
      << "      HF EM particle energy/multiplicity: " << HFEMEnergy () << '/' << HFEMMultiplicity () << std::endl
      << "      charged/neutral hadrons energy: " << chargedHadronEnergy () << '/' << neutralHadronEnergy () << std::endl
      << "      charged/neutral em energy: " << chargedEmEnergy () << '/' << neutralEmEnergy () << std::endl
      << "      charged muon energy: " << chargedMuEnergy () << '/' << std::endl
      << "      charged/neutral multiplicity: " << chargedMultiplicity () << '/' << neutralMultiplicity () << std::endl;
  return out.str ();
}

