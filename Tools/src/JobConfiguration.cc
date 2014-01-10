#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "Tools/include/JobConfiguration.hh"

using namespace vecbos;
using namespace std;

JobConfiguration::JobConfiguration(const std::string cfgfile) :
  cfg_(cfgfile), messageFreq_(1000), maxMC_(20), 
  jsonFilePath_(std::string("")), outputFileBase_("vecbosapp_tree") 
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
      if(parameter.compare("MessageFrequency")==0) messageFreq_ = atoi(val.c_str());
      if(parameter.compare("MaxMCTruth")==0) maxMC_ = atoi(val.c_str());
      if(parameter.compare("MaxEvent")==0) maxEvent_ = atoi(val.c_str());
      if(parameter.compare("IsMC")==0) isMC_ = atoi(val.c_str());
      if(parameter.compare("JSON")==0) jsonFilePath_ = val;
      if(parameter.compare("OutputBase")==0) outputFileBase_ = val;
    }
    cfgfile.close();    
  }
  
  pars_int.insert( intentry("MessageFrequency",messageFreq_) );
  pars_int.insert( intentry("MaxMCTruth",maxMC_) );
  pars_int.insert( intentry("MaxEvent",maxEvent_) );
  pars_int.insert( intentry("IsMC",isMC_) );
  pars_string.insert( stringentry("JSON",jsonFilePath_) );
  pars_string.insert( stringentry("OutputBase",outputFileBase_) );

}

