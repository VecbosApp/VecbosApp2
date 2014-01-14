#ifndef VECBOS_JET_PILEUP_ID_ALGO_HH
#define VECBOS_JET_PILEUP_ID_ALGO_HH

#include "DataFormats/include/PFJet.hh"

namespace vecbos {

  class JetPileupIdentification {
  public:
    
    enum version_t { mva_53x=0, mva_42x, cutbased };
    enum wp_t { loose=0, medium, tight };

    /// constructor
    JetPileupIdentification(version_t version, wp_t wp) : version_(version), wp_(wp) { };
    /// destructor
    ~JetPileupIdentification() { };

    /// set the jet
    void setPFJet(PFJet *jet) { jet_ = jet; }

    /// output of MVA ID
    bool output(wp_t wp);

  private:
    version_t version_;
    wp_t wp_;
    PFJet *jet_;

  };
}

#endif
