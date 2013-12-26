#ifndef VECBOS_MUON_FWD_HH
#define VECBOS_MUON_FWD_HH

#include <vector>
#include "DataFormats/include/Muon.hh"

namespace vecbos {

  typedef std::vector<Muon> MuonCollection;
  typedef std::vector<Muon>::iterator muon_iterator;

}

#endif
