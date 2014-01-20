#ifndef VECBOS_ELECTRONIDSELECTOR_HH
#define VECBOS_ELECTRONIDSELECTOR_HH

#include "Tools/include/CollectionSelector.hh"
#include "DataFormats/include/CandidateFwd.hh"
#include "DataFormats/include/ElectronFwd.hh"
#include "DataFormats/include/VertexFwd.hh"

namespace vecbos {

  class ElectronIDSelector : public CollectionSelector<vecbos::Candidate*> {
  public:

    /// default constructor
    ElectronIDSelector();
    /// constructor from collection
    ElectronIDSelector(CandidateCollectionPtr input, float rho, VertexCollection vertices);
    /// destructor
    ~ElectronIDSelector() { };

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

    /// output for offline trigger emulation
    vecbos::CandidateCollectionPtr output_hlt();

    /// output for cutbased selection - id only
    vecbos::CandidateCollectionPtr output_cuts_id();
    /// output for cutbased selection - iso only
    vecbos::CandidateCollectionPtr output_cuts_iso();
    /// output for cutbased selection - conversion rejection only
    vecbos::CandidateCollectionPtr output_cuts_convrej();
    /// output for cutbased selection - ip only
    vecbos::CandidateCollectionPtr output_cuts_ip();
    /// output for cutbased selection - id + iso + conversion rejection + ip
    vecbos::CandidateCollectionPtr output_cuts();

    /// output for MVA selection - id only
    vecbos::CandidateCollectionPtr output_mva_id();
    /// output for MVA selection - iso only
    vecbos::CandidateCollectionPtr output_mva_iso();
    /// output for MVA selection - conversion rejection only
    vecbos::CandidateCollectionPtr output_mva_convrej();
    /// output for MVA selection - ip only
    vecbos::CandidateCollectionPtr output_mva_ip();
    /// output for MVA selection - id + iso + conversion rejection + ip
    vecbos::CandidateCollectionPtr output_mva();

  };

}

#endif
