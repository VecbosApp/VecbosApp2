#ifndef VECBOS_JOB_CONFIGURATION_HH
#define VECBOS_JOB_CONFIGURATION_HH

#include <string>
#include <map>

namespace vecbos {

  class JobConfiguration {
  public:
    
    typedef std::pair<std::string,int> intentry;
    typedef std::pair<std::string,std::string> stringentry;
    
    /// constructor from config file
    JobConfiguration(const std::string cfgfile);
    /// destructor
    ~JobConfiguration() { };

    /// read the core framework configurations
    void configure();
    
    /// get a string parameter
    std::string getStringPar(std::string parname) { return pars_string[parname]; }

    /// get an integer parameter
    int getIntPar(std::string parname) { return pars_int[parname]; }

  private:
    /// data members
    std::string cfg_;

    /// the containers of the configuration parameters
    std::map<std::string,int> pars_int;
    std::map<std::string,std::string> pars_string;
  };

}

#endif
