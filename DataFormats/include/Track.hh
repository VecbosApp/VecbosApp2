#ifndef VECBOS_TRACK_h
#define VECBOS_TRACK_h

#include "TVector3.h"
#include "TMath.h"

namespace vecbos {

  class Track {
  public:
    /// spatial vector
    typedef TVector3 Vector;
    /// point in the space
    typedef TVector3 Point;

    /// default constructor
    Track();
    /// virtual destructor   
    virtual ~Track() { }
    /// constructor from fit parameters and error matrix  
    Track( double chi2, const Point & referencePoint,
	   const Vector & momentum, int charge);
    /// chi-squared divided by n.d.o.f. (or chi-squared * 1e6 if n.d.o.f. is zero)
    double normalizedChi2() const { return chi2_; }
    /// track electric charge
    int charge() const { return charge_; }
    /// q/p 
    double qoverp() const { return charge() / p(); }
    /// polar angle  
    double theta() const { return momentum_.Theta(); }
    /// Lambda angle
    double lambda() const { return TMath::Pi()/2 - momentum_.Theta(); }
    /// dxy parameter. (This is the transverse impact parameter w.r.t. to (0,0,0) ONLY if refPoint is close to (0,0,0): see parametrization definition above for details). See also function dxy(myBeamSpot) below.
    double dxy() const { return ( - vx() * py() + vy() * px() ) / pt(); }
    /// dxy parameter in perigee convention (d0 = - dxy)
    double d0() const { return - dxy(); }
    /// dsz parameter (THIS IS NOT the SZ impact parameter to (0,0,0) if refPoint is far from (0,0,0): see parametrization definition above for details)
    double dsz() const { return vz()*pt()/p() - (vx()*px()+vy()*py())/pt() * pz()/p(); }
    /// dz parameter (= dsz/cos(lambda)). This is the track z0 w.r.t (0,0,0) only if the refPoint is close to (0,0,0). See also function dz(myBeamSpot) below.
    double dz() const { return vz() - (vx()*px()+vy()*py())/pt() * (pz()/pt()); }
    /// momentum vector magnitude
    double p() const { return momentum_.Mag(); }
    /// track transverse momentum
    double pt() const { return sqrt( momentum_.Perp2() ); }
    /// x coordinate of momentum vector
    double px() const { return momentum_.x(); }
    /// y coordinate of momentum vector
    double py() const { return momentum_.y(); }
    /// z coordinate of momentum vector
    double pz() const { return momentum_.z(); }
    /// azimuthal angle of momentum vector
    double phi() const { return momentum_.Phi(); }
    /// pseudorapidity of momentum vector
    double eta() const { return momentum_.Eta(); }
    /// x coordinate of the reference point on track
    double vx() const { return vertex_.x(); }
    /// y coordinate of the reference point on track
    double vy() const { return vertex_.y(); }
    /// z coordinate of the reference point on track
    double vz() const { return vertex_.z(); }

    /// track momentum vector
    const Vector & momentum() const { return momentum_; }
    /// track momentum error
    const float ptError() const { return ptErr_; }

    /// Reference point on the track
    const Point & referencePoint() const { return vertex_; }

    /// reference point on the track. This method is DEPRECATED, please use referencePoint() instead
    const Point & vertex() const { return vertex_; }

    /// dxy parameter with respect to a user-given beamSpot (WARNING: this quantity can only be interpreted as a minimum transverse distance if beamSpot, if the beam spot is reasonably close to the refPoint, since linear approximations are involved). This is a good approximation for Tracker tracks.
    double dxy(const Point& myBeamSpot) const { 
      return ( - (vx()-myBeamSpot.x()) * py() + (vy()-myBeamSpot.y()) * px() ) / pt(); 
    }

    /// dsz parameter with respect to a user-given beamSpot (WARNING: this quantity can only be interpreted as the distance in the S-Z plane to the beamSpot, if the beam spot is reasonably close to the refPoint, since linear approximations are involved). This is a good approximation for Tracker tracks.
    double dsz(const Point& myBeamSpot) const { 
      return (vz()-myBeamSpot.z())*pt()/p() - ((vx()-myBeamSpot.x())*px()+(vy()-myBeamSpot.y())*py())/pt() *pz()/p(); 
    }
    /// dz parameter with respect to a user-given beamSpot (WARNING: this quantity can only be interpreted as the track z0, if the beamSpot is reasonably close to the refPoint, since linear approximations are involved). This is a good approximation for Tracker tracks.
    double dz(const Point& myBeamSpot) const { 
      return (vz()-myBeamSpot.z()) - ((vx()-myBeamSpot.x())*px()+(vy()-myBeamSpot.y())*py())/pt() * pz()/pt(); 
    }

    /// validity of the track
    bool isValid() { return isValid_; }

   
    /// get the track vertex
    Point getVertex() { return vertex_; }
    /// set the track vertex
    void setVertex(Point vertex) { vertex_ = vertex; }

    struct HitPattern {
      int numberOfValidHits, numberOfLostHits;
      int numberOfPixelHits, trackerLayersWithMeasurement, expectedInnerLayers;
      int numberOfValidPixelBarrelHits, numberOfValidPixelEndcapHits,
	numberOfValidStripTIBHits, numberOfValidStripTIDHits, numberOfValidStripTOBHits,
	numberOfValidStripTECHits, numberOfValidMuonHits;
      int qualityMask;
      HitPattern() :
	numberOfValidHits(0), numberOfLostHits(0),
	numberOfPixelHits(0), trackerLayersWithMeasurement(0), expectedInnerLayers(0),
	numberOfValidPixelBarrelHits(0), numberOfValidPixelEndcapHits(0),
	numberOfValidStripTIBHits(0), numberOfValidStripTIDHits(0), numberOfValidStripTOBHits(0),
	numberOfValidStripTECHits(0), numberOfValidMuonHits(0),
	qualityMask(0) 
      { }
    } ;

    /// get the hit pattern
    HitPattern getHitPattern() { return hitPattern_; }
    /// set the hit pattern
    void setHitPattern(HitPattern pattern) { hitPattern_ = pattern; }

    /// number of valid hits found 
    int numberOfValidHits() const { return hitPattern_.numberOfValidHits; }
    /// number of cases where track crossed a layer without getting a hit.
    int numberOfLostHits() const { return hitPattern_.numberOfLostHits; }
    /// Access the hit pattern counting (in the Tracker) the number of expected crossed layers  before the first trajectory's hit
    int trackerExpectedHitsInner() { return hitPattern_.expectedInnerLayers; }
    /// number of hits in the muon chambers
    int numberOfValidMuonHits() { return hitPattern_.numberOfValidMuonHits; }

    struct ImpactParameters {
      float ip3D, ip3Derr;
      float ip2D, ip2Derr;
      float d0, d0err;
      float dz, dzerr;
      ImpactParameters() :
	ip3D(999.), ip3Derr(999.), ip2D(999.), ip2Derr(999.), 
	d0(999.), d0err(999.), dz(999.), dzerr(999.)
      { }
    } ;

    /// get the impact parameters
    ImpactParameters getImpactParameters() { return ips_; }
    /// set the impact parameters
    void setImpactParameters(ImpactParameters ips) { ips_ = ips; }
    /// absolute significance of impact parameter in 3D
    float sip3D() { return fabs(ips_.ip3D/ips_.ip3Derr); }
    /// absolute significance of impact parameter in 2D
    float sip2D() { return fabs(ips_.ip2D/ips_.ip2Derr); }
    
    /// set the pt error
    void setPtError(float error) { ptErr_ = error; }

  private:
    float chi2_;
     /// innermost (reference) point on track
    Point vertex_;
    /// momentum vector at innermost point
    Vector momentum_;
    /// momentum error
    float ptErr_;
    /// electric charge
    char charge_;
    /// Hit Pattern
    HitPattern hitPattern_;
    /// impact parameters
    ImpactParameters ips_;
    /// is a valid track
    bool isValid_;
    
  };

}

#endif
