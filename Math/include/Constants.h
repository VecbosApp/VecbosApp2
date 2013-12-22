#include <TMath.h>

namespace math {

  struct PhysConstants {
    float PI;
    float electron_mass;
    float muon_mass;
    float tau_mass;
  PhysConstants() :
    PI(TMath::Pi()),
      electron_mass(0.510998928E-03), // GeV
      muon_mass(105.6583715E-03), // GeV
      tau_mass(1776.82E-03) // GeV
    { }
  };

}
