#ifndef VECBOS_JSON_FILTER_HH
#define VECBOS_JSON_FILTER_HH

#include <string>
#include "DataFormats/include/EventHeader.hh"

namespace vecbos {

  class JsonFilter {
  public:

    typedef std::pair<unsigned int,unsigned int> aLSSegment;
    typedef std::vector< std::pair<unsigned int,unsigned int> > LSSegments;
    typedef unsigned int aRun;
    typedef std::map< aRun, LSSegments > runsLSSegmentsMap;
    typedef std::pair < aRun, LSSegments > aRunsLSSegmentsMapElement;

    /// default constructor
    JsonFilter();
    /// constructor from JSON input file
    JsonFilter(std::string jsonFilePath);
    /// destructor
    ~JsonFilter() { };

    /// Fill RunLSMap according to json file
    void fillRunLSMap();
    /// Set Good Run LS
    void setJsonGoodRunList(std::string jsonFilePath) { jsonFile_ = jsonFilePath; }
    /// check if Run/LS is a good one
    bool isGoodRunLS(vecbos::EventHeader header);

  private:
    runsLSSegmentsMap goodRunLS; 
    std::string jsonFile_;
    int lastRun_, lastLumi_;

  };

}

#endif

