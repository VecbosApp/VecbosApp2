#ifndef VECBOS_PFJET_ID_ALGO_HH
#define VECBOS_PFJET_ID_ALGO_HH

#include "DataFormats/include/PFJet.hh"

namespace vecbos {

  class PFJetIdentification {
  public:

    /// working points available for both mva and cutbased
    enum wp_t { loose=0, medium, tight };

    /// constructor
    PFJetIdentification(wp_t wp) : wp_(wp) { };
    /// destructor
    ~PFJetIdentification() { };

    /// output of the ID
    bool output(PFJet *jet);

  private:
    wp_t wp_;
  };
}

#endif
