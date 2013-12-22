#ifndef VECBOS_ELECTRON_FWD_HH
#define VECBOS_ELECTRON_FWD_HH

#include <vector>
#include "DataFormats/include/Electron.hh"

namespace vecbos {

  typedef std::vector<Electron> ElectronCollection;
  typedef std::vector<Electron>::iterator electron_iterator;

}

#endif
