#ifndef VECBOS_GENPARTICLE_FWD_H
#define VECBOS_GENPARTICLE_FWD_H

#include "DataFormats/include/GenParticle.hh"

namespace vecbos {

  typedef std::vector<vecbos::GenParticle> GenParticleCollection;
  typedef std::vector<vecbos::GenParticle*> GenParticleCollectionPtr;
  typedef std::vector<vecbos::GenParticle*>::iterator genparticleptr_iterator;

}


#endif

