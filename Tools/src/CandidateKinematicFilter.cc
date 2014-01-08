#include "Tools/include/CandidateKinematicFilter.hh"

using namespace vecbos;

CandidateKinematicFilter::CandidateKinematicFilter() 
  : CollectionSelector() 
{
  ptrange_ = std::make_pair<float,float>(0,7000);
  etarange_ = std::make_pair<float,float>(-2.5,2.5);
  massrange_ = std::make_pair<float,float>(0,14000);
  mtrange_ = std::make_pair<float,float>(0,14000);
} 

CandidateKinematicFilter::CandidateKinematicFilter(collection input) 
  : CollectionSelector(input) 
{
  ptrange_ = std::make_pair<float,float>(0,7000);
  etarange_ = std::make_pair<float,float>(-2.5,2.5);
  massrange_ = std::make_pair<float,float>(0,14000);
  mtrange_ = std::make_pair<float,float>(0,14000);
} 


CandidateKinematicFilter::collection CandidateKinematicFilter::output() {
  
  collection output;
  for(collection::iterator it=input_.begin(); it != input_.end(); ++it) {
    if( (*it)->eta() < etarange_.first || (*it)->eta() > etarange_.second ) continue;
    if( (*it)->pt() < ptrange_.first || (*it)->eta() > ptrange_.second ) continue;
    if( (*it)->mass() < massrange_.first || (*it)->mass() > massrange_.second ) continue;
    if( (*it)->mt() < mtrange_.first || (*it)->mt() > mtrange_.second ) continue;
    output.push_back(*it);
  }
  return output;
}
