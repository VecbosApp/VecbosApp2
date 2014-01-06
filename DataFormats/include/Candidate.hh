#ifndef VECBOS_CANDIDATE_H
#define VECBOS_CANDIDATE_H

#include "TLorentzVector.h"
#include "TVector3.h"

namespace vecbos {
  
  class Candidate {
  public:

    /// electric charge type
    typedef int Charge;
    /// Lorentz vector
    typedef TLorentzVector LorentzVector;
    /// point in the space
    typedef TVector3 Point;
    /// point in the space
    typedef TVector3 Vector;

    /// default constructor
    Candidate() :
            charge_(0), pt_(0), eta_(0), phi_(0), mass_(0), 
	    vertex_(0, 0, 0), pdgId_(0), status_(0), cacheCartesianFixed_( false ) { };
    /// constructor from values  
    Candidate( int charge, const LorentzVector & p4, const Point & vtx = Point( 0, 0, 0 ),
		   int pdgId = 0, int status = 0) :
      charge_( charge ), pt_( p4.Pt() ), eta_( p4.Eta() ), phi_( p4.Phi() ), mass_( p4.M() ),
      vertex_( vtx ), pdgId_( pdgId ), status_( status ), cacheCartesianFixed_( false ) { };
    /// destructor
    virtual ~Candidate();
    /// electric charge
    virtual int charge() const { return charge_; }
    /// set electric charge
    virtual void setCharge( int q ) { charge_ = q; }
    /// four-momentum Lorentz vector
    virtual const LorentzVector & p4() { cacheP4(); return p4_;  }
    /// spatial momentum vector
    virtual Vector momentum() { cacheP4(); return p4_.Vect(); }
    /// magnitude of momentum vector
    virtual double p() { cacheP4(); return p4_.P(); }
    /// energy
    virtual double energy() { cacheP4(); return p4_.E(); }
    /// transverse energy 
    virtual double et() { cacheP4(); return p4_.Et(); }
    /// mass
    virtual float mass() { cacheP4(); return p4_.M(); }
    /// mass squared
    virtual float massSqr() { cacheP4(); return p4_.M2(); }
    /// transverse mass
    virtual double mt() { cacheP4(); return p4_.Mt(); }
    /// transverse mass squared
    virtual double mtSqr() { cacheP4(); return p4_.Mt2(); }
    /// x coordinate of momentum vector
    virtual double px() { cacheP4(); return p4_.Px(); }
    /// y coordinate of momentum vector
    virtual double py() { cacheP4(); return p4_.Py(); }
    /// z coordinate of momentum vector
    virtual double pz() { cacheP4(); return p4_.Pz(); }
    /// transverse momentum
    virtual float pt() { return pt_; }
    /// momentum azimuthal angle
    virtual float phi() { return phi_; }
    /// momentum polar angle
    virtual double theta() { cacheP4(); return p4_.Theta(); }
    /// momentum pseudorapidity
    virtual float eta() { return eta_; }
    /// rapidity
    virtual double rapidity() { cacheP4(); return p4_.Rapidity(); }
    /// rapidity
    virtual double y() { cacheP4(); return p4_.Rapidity(); }
    /// set 4-momentum
    virtual void setP4( const LorentzVector & p4 ) {
      p4_ = p4;
      pt_ = p4_.Pt();
      eta_ = p4_.Eta();
      phi_ = p4_.Phi();
      mass_ = p4_.M();
      cacheCartesianFixed_ = true;
    }
    /// set particle mass
    virtual void setMass( double m )  {
      mass_ = m;
      clearCache();
    }
    virtual void setPz( double pz )  {
      cacheP4();
      p4_.SetPz(pz);
      pt_ = p4_.Pt();
      eta_ = p4_.Eta();
      phi_ = p4_.Phi();
      mass_ = p4_.M();
    }
    /// vertex position
    virtual const Point & vertex() const { return vertex_; }
   /// x coordinate of vertex position                                                   
    virtual double vx() const  { return vertex_.X(); }
    /// y coordinate of vertex position                                                   
    virtual double vy() const  { return vertex_.Y(); }
    /// z coordinate of vertex position                                                   
    virtual double vz() const  { return vertex_.Z(); }
    /// set vertex                                                                        
    virtual void setVertex( const Point & vertex )   { vertex_ = vertex; }
    
    /// PDG identifier                                                                    
    virtual int pdgId() const { return pdgId_; }
    // set PDG identifier                                                                 
    virtual void setPdgId( int pdgId )  { pdgId_ = pdgId; }
    /// status word                                                                       
    virtual int status() const { return status_; }
    /// set status word                                                                   
    virtual void setStatus( int status ) { status_ = status; }    
    /// basic overlap: check pdgId, charge, p4, vertex position
    bool overlap(Candidate c);

  protected:
    /// electric charge                                                                   
    int charge_;
    /// four-momentum Lorentz vector                                                      
    float pt_, eta_, phi_, mass_;
    /// vertex position                                                                   
    Point vertex_;
    /// PDG identifier                                                                    
    int pdgId_;
    /// status word                                                                       
    int status_;
    /// internal cache for p4                                                             
    LorentzVector p4_;
    /// has cache been set?                                                               
    mutable  bool cacheCartesianFixed_;
    /// fill the p4 LorentzVector
    inline void cacheP4() { 
      //      if(cacheCartesianFixed_) return;
      p4_.SetPtEtaPhiM(pt_,eta_,phi_,mass_);  
      cacheCartesianFixed_=true;
    }
    /// clear internal cache                                                              
    inline void clearCache() const {
      cacheCartesianFixed_ = false;
    }

  };

}

#endif


