#ifndef VECBOS_CORRECTED_JET_PRODUCER_HH
#define VECBOS_CORRECTED_JET_PRODUCER_HH

#include <string>
#include "DataFormats/include/JetFwd.hh"

namespace vecbos {

  class CorrectedJetProducer {

  public:
    /// constructor from global tag and jet type
    CorrectedJetProducer(const std::string& globalTag, const std::string& jetType="AK5PF") :
      gtName_(globalTag), jetType_(jetType), corr_("none") { };
    /// destructor
    ~CorrectedJetProducer() { };

    /// set the corrections to be applied (in the LX:LY:...LZ format)
    void setCorrections(const std::string& corrections) { corr_ = corrections; }

    /// set the default corrections
    void setDefaultCorrections();

    /// update an existing jet collection with new energy
    void correctCollection(vecbos::CandidateCollectionPtr jets, float rho);
    /// get a new jet collection with corrections applied
    vecbos::JetCollectionPtr output(vecbos::CandidateCollectionPtr jets, float rho);

  private:
    
    /// the global tag and jet type
    std::string gtName_; 
    std::string jetType_;

    std::string corr_;

  };

}

#endif
