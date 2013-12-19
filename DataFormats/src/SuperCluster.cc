#include "DataFormats/include/SuperCluster.hh"

using namespace vecbos;

SuperCluster::SuperCluster() : 
  energy_(0.) {
  Point zero(0.,0.,0.);
  TVector3 nulldir(0.,0.,0.);
  ClusterShapes shapes;
  position_=zero;
  direction_=nulldir;
  shape_=shapes;
}

SuperCluster::~SuperCluster() { }

