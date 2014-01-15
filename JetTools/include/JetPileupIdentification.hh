#ifndef VECBOS_JET_PILEUP_ID_ALGO_HH
#define VECBOS_JET_PILEUP_ID_ALGO_HH

#include "DataFormats/include/PFJet.hh"

namespace vecbos {

  class JetPileupIdentification {
  public:
    
    /// different verison of the ID:
    /// mva_53x = training for 2012 data
    /// mva_42x = training for 2011 data
    /// cutbased approach for both 2011 and 2012 data
    enum version_t { mva_53x=0, mva_42x, cutbased };
    /// working points available for both mva and cutbased
    enum wp_t { loose=0, medium, tight };

    /// constructor
    JetPileupIdentification(version_t version, wp_t wp) : version_(version), wp_(wp) { };
    /// destructor
    ~JetPileupIdentification() { };

    /// output of the ID
    bool output(PFJet *jet);

  private:
    version_t version_;
    wp_t wp_;
  };
}

#endif
