#ifndef VECBOS_SUPERCLUSTER_HH
#define VECBOS_SUPERCLUSTER_HH

#include <TVector3.h>

namespace vecbos {

  class SuperCluster {
  public:
    
    /// point in the space
    typedef TVector3 Point;
    
    /// container of the cluster shapes
    struct ClusterShapes;
      
    /// default constructor (empty)
    SuperCluster();
    /// constructor from the energy and position
    SuperCluster( float energy, const Point& position, const TVector3& direction, ClusterShapes shapes ) :
      energy_(energy), position_(position), direction_(direction), shape_(shapes) { isValid_=true; };
    /// destructor
    ~SuperCluster();
    
    /// energy with basic calibrations
    float energy() const { return energy_; }

    /// raw uncorrected energy (sum of energies of component BasicClusters)
    float rawEnergy() const { return rawEnergy_; }
    
    Point position() const { return position_; }

    TVector3 direction() const { return direction_; }

    /// energy of the Basic Cluster seed of the Super Cluster
    float seedClusterEnergy() const { return seedClusterEnergy_; }

    /// energy deposited in preshower 
    float preshowerEnergy() const { return preshowerEnergy_; }
    float preshowerEnergyPlane1() const {return preshowerEnergy1_; }
    float preshowerEnergyPlane2() const {return preshowerEnergy2_; }

    /// number of BasicCluster constituents
    int clustersSize() const { return numberOfSubClusters_; }

    /// obtain the cluster shapes of the seed of the Super Cluster
    struct ClusterShapes {
      int nCrystals;
      float phiWidth, etaWidth;
      float e3x3, e3x1, e1x3, e4x4, e5x5, e2x2, e1x5;
      float e2x5Max, e2x5Left, e2x5Right, e2x5Top, e2x5Bottom, eLeft, eRight, eTop, eBottom;
      float e4SwissCross;
      float eMax, e2nd;
      float sigmaIetaIeta, sigmaIphiIphi, sigmaIetaIphi;
      ClusterShapes()
	: nCrystals(0), 
	  phiWidth(0.), etaWidth(0.),
	  e3x3(0.), e3x1(0.), e1x3(0.), e4x4(0.), e5x5(0.), e2x2(0.), e1x5(0.),
	  e2x5Max(0.), e2x5Left(0.), e2x5Right(0.), e2x5Top(0.), e2x5Bottom(0.), eLeft(0.), eRight(0.), eTop(0.), eBottom(0.),
	  e4SwissCross(0.),
	  eMax(0.), e2nd(0.),
	  sigmaIetaIeta(0.), sigmaIphiIphi(0.), sigmaIetaIphi(0.)
      {}
    } ;

    int nCrystals() const { return shape_.nCrystals; }
    float phiWidth() const { return shape_.phiWidth; }
    float etaWidth() const { return shape_.etaWidth; }
    float e3x3() const { return shape_.e3x3; }
    float e3x1() const { return shape_.e3x1; }
    float e1x3() const { return shape_.e1x3; }
    float e4x4() const { return shape_.e4x4; }
    float e5x5() const { return shape_.e5x5; }
    float e2x2() const { return shape_.e2x2; }
    float e1x5() const { return shape_.e1x5; }
    float e2x5Max() const { return shape_.e2x5Max; }
    float e2x5Left() const { return shape_.e2x5Left; }
    float e2x5Right() const { return shape_.e2x5Right; }
    float e2x5Top() const { return shape_.e2x5Top; }
    float e2x5Bottom() const { return shape_.e2x5Bottom; }
    float eLeft() const { return shape_.eLeft; }
    float eRight() const { return shape_.eRight; }
    float eTop() const { return shape_.eTop; }
    float eBottom() const { return shape_.eBottom; }
    float e4SwissCross() const { return shape_.e4SwissCross; }
    float eMax() const { return shape_.eMax; }
    float e2nd() const { return shape_.e2nd; }
    float sigmaIetaIeta() const { return shape_.sigmaIetaIeta; }
    float sigmaIphiIphi() const { return shape_.sigmaIphiIphi; }
    float sigmaIetaIphi() const { return shape_.sigmaIetaIphi; }


    //Assign new variables to supercluster
    void setRawEnergy( float rawEnergy ) { rawEnergy_ = rawEnergy; }
    void setPreshowerEnergy( float preshowerEnergy ) { preshowerEnergy_ = preshowerEnergy; };
    void setPreshowerEnergyPlane1( float preshowerEnergy1 ) { preshowerEnergy1_ = preshowerEnergy1; }; 
    void setPreshowerEnergyPlane2( float preshowerEnergy2 ) { preshowerEnergy2_ = preshowerEnergy2; }; 

    void setClusterShapes( ClusterShapes shapes ) { shape_ = shapes; }

    void setClustersSize(int numberOfSubClusters) { numberOfSubClusters_ = numberOfSubClusters; }

    /// validity of the super cluster
    bool isValid() { return isValid_; }

  private:

    Point position_;
    TVector3 direction_;
    float energy_;
    float preshowerEnergy_;
    float rawEnergy_;
    float seedClusterEnergy_;
    
    ClusterShapes shape_;
    float numberOfSubClusters_;

    float preshowerEnergy1_;
    float preshowerEnergy2_;

    bool isValid_;
  };

}

#endif

