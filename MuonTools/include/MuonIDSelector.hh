#ifndef VECBOS_MUON_ID_SELECTOR_HH
#define VECBOS_MUON_ID_SELECTOR_HH

#include "Tools/include/CollectionSelector.hh"
#include "DataFormats/include/CandidateFwd.hh"
#include "DataFormats/include/MuonFwd.hh"
#include "DataFormats/include/VertexFwd.hh"

namespace vecbos {

  class MuonIDSelector : public CollectionSelector<vecbos::Candidate*> {
  public:

    /// default constructor
    MuonIDSelector();
    /// constructor from collection
    MuonIDSelector(CandidateCollectionPtr input, float rho, VertexCollection vertices);
    /// destructor
    ~MuonIDSelector() { };

    /// selected objects according the configured algo and wp
    vecbos::CandidateCollectionPtr output();
    
    /// set the rho for isolation correction with FastJet
    void setRho(float rho) { rho_ = rho; }

    /// set the primary vertices collection
    void setPrimaryVertices(VertexCollection vertices) { vertices_ = vertices; }

  private:
    /// rho (to correct isolation with FastJet)
    float rho_;
    /// primary vertices collection
    vecbos::VertexCollection vertices_;

    /// output for cutbased selection - id only
    vecbos::CandidateCollectionPtr output_cuts_id();
    /// output for cutbased selection - iso only
    vecbos::CandidateCollectionPtr output_cuts_iso();
    /// output for cutbased selection - ip only
    vecbos::CandidateCollectionPtr output_cuts_ip();
    /// output for cutbased selection - id + iso + conversion rejection + ip
    vecbos::CandidateCollectionPtr output_cuts();

  };

}

#endif
