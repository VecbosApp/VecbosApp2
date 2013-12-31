#ifndef VECBOS_ELECTRONIDALGO_HH
#define VECBOS_ELECTRONIDALGO_HH

#include <string.h>
#include "DataFormats/include/Electron.hh"
#include "DataFormats/include/ElectronFwd.hh"

namespace vecbos {

  class ElectronIDAlgo {

  public:
    /// constructor
    ElectronIDAlgo(vecbos::Electron electron, float rho) :
      electron_(electron), rho_(rho) { }
    /// destructor
    ~ElectronIDAlgo() { }
    
    /// pass the offline emulation of the HLT 2012 
    /// includes ID, iso, full conv. rejection as the denominator object for triggering electrons
    bool pass_hlt();
    
    /// pass the cut based selection for the specified working point - id only
    bool pass_cuts_id(std::string wp);
    /// pass the cut based selection for the specified working point - iso only
    bool pass_cuts_iso(std::string wp);
    /// pass the cut based selection for the specified working point - conversion rejection only
    bool pass_cuts_convrej(std::string wp);
    /// pass the cut based selection for the specified working point - IP cuts
    bool pass_cuts_ip(std::string wp);

    /// pass the full cut based id+iso+conv rej+IP
    bool pass_cuts(std::string wp) { return pass_cuts_id(wp) && pass_cuts_iso(wp) 
	&& pass_cuts_convrej(wp) && pass_cuts_ip(wp); }
    
    /// pass the MVA for the specified working point - id only
    bool pass_mva_id(std::string algo, std::string wp);
    /// pass the MVA for the specified working point - iso only
    bool pass_mva_iso(std::string wp);
    /// pass the cut based selection for the specified working point - conversion rejection only
    bool pass_mva_convrej(std::string wp);
    /// pass the MVA for the specified working point - sip only
    bool pass_mva_ip(std::string wp);

    /// pass the full mva based id+iso+conv rej+IP (only the ID part is MVA)
    bool pass_mva(std::string algo, std::string wp) { return pass_mva_id(algo,wp) && pass_mva_iso(wp) 
	&& pass_mva_convrej(wp) && pass_mva_ip(wp); }

  private:
    /// the electron to be selected
    vecbos::Electron electron_;
    /// the event rho used to correct the isolation
    float rho_;
    enum cuts_wp {veto=0, loose=1, medium=2, tight=3};
    int getIntCutsWP(std::string wp);
    
  };

}

#endif
