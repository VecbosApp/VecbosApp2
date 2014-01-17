#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "Tools/include/JobConfiguration.hh"

using namespace vecbos;
using namespace std;

JobConfiguration::JobConfiguration(const std::string cfgfile) :
  cfg_(cfgfile)
{ }

void JobConfiguration::configure() {
  std::ifstream cfgfile(cfg_.c_str());
  if(!cfgfile.good()) {
    std::cout << "Error!   Unable to open the cfg file " << cfg_ << std::endl;
    return;
  } else {
    std::string parameter;
    std::string val; 
    while (!cfgfile.eof()) {
      cfgfile >> parameter >> val;
      if(parameter.compare("MessageFrequency")==0) pars_int.insert( intentry(parameter, atoi(val.c_str())) );
      if(parameter.compare("MaxMCTruth")==0) pars_int.insert( intentry(parameter, atoi(val.c_str())) );
      if(parameter.compare("MaxEvent")==0) pars_int.insert( intentry(parameter, atoi(val.c_str())) );
      if(parameter.compare("IsMC")==0) pars_int.insert( intentry(parameter, atoi(val.c_str())) );
      if(parameter.compare("JSON")==0) pars_string.insert( stringentry(parameter, val) );
      if(parameter.compare("OutputBase")==0) pars_string.insert( stringentry(parameter, val) );
      if(parameter.compare("GlobalTagMC")==0) pars_string.insert( stringentry(parameter, val) );
      if(parameter.compare("GlobalTagDATA")==0) pars_string.insert( stringentry(parameter, val) );
    }
    cfgfile.close();    
  }
  
}

