#ifndef VECBOS_ELECTRONIDALGO_HH
#define VECBOS_ELECTRONIDALGO_HH

#include <string.h>
#include "DataFormats/include/Muon.hh"
#include "DataFormats/include/MuonFwd.hh"
#include "DataFormats/include/VertexFwd.hh"

namespace vecbos {

  class MuonIDAlgo {

  public:
    /// constructor
    MuonIDAlgo(float rho, vecbos::VertexCollection vertices);
    /// destructor
    ~MuonIDAlgo() { }
    
    /// set the muon on which apply the ID
    void setMuon(Muon muon) { muon_ = muon; }
    
    /// pass the cut based selection for the specified woking point - id only
    bool pass_cuts_id(std::string wp);
    /// pass the cut based selection for the specified working point - iso only
    bool pass_cuts_iso(std::string wp);
    /// pass the cut based selection for the specified working point - IP cuts
    bool pass_cuts_ip(std::string wp);

    /// pass the full cut based id+iso+conv rej+IP
    bool pass_cuts(std::string wp) { return pass_cuts_id(wp) && pass_cuts_iso(wp) && pass_cuts_ip(wp); }
    
  private:
    /// the best good primary vertex
    vecbos::Vertex bestVertex_;
    /// the muon to be selected
    vecbos::Muon muon_;
    /// the event rho used to correct the isolation
    float rho_;
    enum cuts_wp {veto=0, loose=1, medium=2, tight=3};
    int getIntCutsWP(std::string wp);
    
  };

}

#endif
