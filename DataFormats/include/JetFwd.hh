#ifndef VECBOS_JET_FWD_HH
#define VECBOS_JET_FWD_HH

#include "DataFormats/include/Jet.hh"

namespace vecbos {

  typedef std::vector<vecbos::Jet> JetCollection;
  typedef std::vector<vecbos::Jet>::iterator jet_iterator;

  typedef std::vector<vecbos::Jet*> JetCollectionPtr;
  typedef std::vector<vecbos::Jet*>::iterator jetptr_iterator;

}

#endif
