#include <fstream>
#include "JSON/include/reader.h"
#include "JSON/include/elements.h"

#include "JSON/include/JsonFilter.hh"

using namespace vecbos;
using namespace std;

JsonFilter::JsonFilter() : jsonFile_(std::string("")) {
  lastRun_ = -1;
  lastLumi_ = -1;
}

JsonFilter::JsonFilter(std::string jsonFilePath) : jsonFile_(jsonFilePath) { 
  lastRun_ = -1;
  lastLumi_ = -1;
}

void JsonFilter::fillRunLSMap()
{
  
  if (jsonFile_ == "")
    {
      std::cout << "Cannot fill RunLSMap. json file not configured" << std::endl;
      return;
    }

  std::ifstream jsonFileStream;
  jsonFileStream.open(jsonFile_.c_str());
  if (!jsonFileStream.is_open())
    {
      std::cout << "Unable to open file " << jsonFile_ << std::endl;
      return;
    }

  json::Object elemRootFile;
  json::Reader::Read(elemRootFile, jsonFileStream);

  for (json::Object::const_iterator itRun=elemRootFile.Begin();itRun!=elemRootFile.End();++itRun)
    {
      const json::Array& lsSegment = (*itRun).element;
      LSSegments thisRunSegments; 
      for (json::Array::const_iterator lsIterator=lsSegment.Begin();lsIterator!=lsSegment.End();++lsIterator)
        {
          json::Array lsSegment=(*lsIterator);
          json::Number lsStart=lsSegment[0];           
          json::Number lsEnd=lsSegment[1];
          aLSSegment thisSegment;
          thisSegment.first=lsStart.Value();
          thisSegment.second=lsEnd.Value();
          thisRunSegments.push_back(thisSegment);
          //           std::pair<int, int> lsSegment=std::pair<int, int>(atoi(,lsIterator[1]); 
        }
      goodRunLS.insert(aRunsLSSegmentsMapElement(atoi((*itRun).name.c_str()),thisRunSegments));
    }


  std::cout << "[GoodRunLSMap]::Good Run LS map filled with " << goodRunLS.size() << " runs" << std::endl;
  for (runsLSSegmentsMap::const_iterator itR=goodRunLS.begin(); itR!=goodRunLS.end(); ++itR)
    {
      std::cout << "[GoodRunLSMap]::Run " << (*itR).first <<  " LS ranges are: ";
      for (LSSegments::const_iterator iSeg=(*itR).second.begin();iSeg!=(*itR).second.end();++iSeg)
        std::cout << "[" << (*iSeg).first << "," << (*iSeg).second << "] "; 
      std::cout << std::endl;
    }
  std::cout << "And this is all the data I will consider, nothing more. " << std::endl;
}

bool JsonFilter::isGoodRunLS(EventHeader header)
{

  if (jsonFile_ == "") return true;

  int run=header.run();
  int lumi=header.lumi();

  runsLSSegmentsMap::const_iterator thisRun=goodRunLS.find(run);
  if (thisRun == goodRunLS.end()) {
    std::cout << "[GoodRunLS]::Run " << run << " is rejected" << std::endl;
    return false;
  }
  for (LSSegments::const_iterator iSeg=goodRunLS[run].begin();iSeg!=goodRunLS[run].end();++iSeg)
    {
      if ( lumi >= (*iSeg).first && lumi <= (*iSeg).second) {
        if(lastRun_ != run || lastLumi_ != lumi) {
          lastRun_ = run;  
          lastLumi_ = lumi;
          std::cout << "[GoodRunLS]::Run " << lastRun_ << " LS " << lastLumi_ << " is OK" << std::endl;
        }
        return true;
      }
    }
  if(lastRun_ != run || lastLumi_ != lumi) {
    lastRun_ = run;  
    lastLumi_ = lumi;
  }
  std::cout << "[GoodRunLS]::Run " << lastRun_ << " LS " << lastLumi_ << " is rejected" << std::endl;
  return false;
}

