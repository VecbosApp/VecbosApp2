#include <iostream>

#include "DataFormats/include/EventHeader.hh"
#include "Analysis/include/AnalysisBase.hh"

#include "DataFormats/include/Vertex.hh"
#include "DataFormats/include/SuperCluster.hh"
#include "DataFormats/include/Track.hh"

#include "Tools/include/VertexSelector.hh"
#include "Math/include/Constants.h"

using namespace std;
using namespace vecbos;

AnalysisBase::AnalysisBase(TTree *tree) :
  VecbosEventContent(tree)
{
  if(tree !=0) init(tree);
  else return;
}


void AnalysisBase::init(TTree* tree) {
  if(!tree) return;
  else Init(tree);
}

int AnalysisBase::loadTree(Long64_t entry) {

  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }

  // load the Event Header (run, lumi,...)
  loadEventHeader();

  // load the collection of *good* primary vertices
  loadPrimaryVertices();

  // load the Super Cluster collections
  loadSuperClusters();
  loadPFSuperClusters();

  // load the tracks collections
  loadGeneralTracks();
  loadGsfTracks();
  loadGlobalMuonTracks();
  loadStandaloneMuonTracks();

  // load the GSF Electron collection
  loadElectronCollection();
  
  return centry;
}

void AnalysisBase::loadEventHeader() {
  EventHeader eventHeader(runNumber,eventNumber,lumiBlock);
  Event.setEventHeader(eventHeader);
}


void AnalysisBase::loadPrimaryVertices() {

  PrimaryVertices.clear();

  VertexCollection tmpVertices;
  tmpVertices.clear();  
  for(int i=0; i<nPV; ++i) {

    Vertex::Point position(PVxPV[i],PVyPV[i],PVzPV[i]);
    Vertex::Error error(PVErrxPV[i],PVErryPV[i],PVErrzPV[i]);

    Vertex vtx(position,error,chi2PV[i],ndofPV[i]);
    vtx.setValidity(isValidPV[i]);
    vtx.setFake(isFakePV[i]);
    vtx.setSumPt(SumPtPV[i]);
    vtx.setTracksSize(trackSizePV[i]);

    tmpVertices.push_back(vtx);
  }

  VertexSelector sel(tmpVertices);
  PrimaryVertices = sel.goodCollection();

}

void AnalysisBase::loadSuperClusters() {

  SuperClusters.clear();

  for(int i=0; i<nSC; ++i) {
    
    TVector3 position(xPosSC[i],yPosSC[i],zPosSC[i]);
    TVector3 direction;
    float pt = energySC[i]*fabs(sin(thetaSC[i]));
    direction.SetPtEtaPhi(pt,etaSC[i],phiSC[i]);
    
    SuperCluster::ClusterShapes shapes;
    shapes.nCrystals = nCrystalsSC[i];
    shapes.phiWidth = phiWidthSC[i];
    shapes.etaWidth = etaWidthSC[i];
    shapes.e3x3 = e3x3SC[i];
    shapes.e3x1 = e3x1SC[i];
    shapes.e1x3 = e1x3SC[i];
    shapes.e4x4 = e4x4SC[i];
    shapes.e5x5 = e5x5SC[i];
    shapes.e2x2 = e2x2SC[i];
    shapes.e1x5 = e1x5SC[i];
    shapes.e2x5Max = e2x5MaxSC[i];
    shapes.e2x5Left = e2x5LeftSC[i];
    shapes.e2x5Right = e2x5RightSC[i];
    shapes.e2x5Top = e2x5TopSC[i];
    shapes.e2x5Bottom = e2x5BottomSC[i];
    shapes.eLeft = eLeftSC[i];
    shapes.eRight = eRightSC[i];
    shapes.eTop = eTopSC[i];
    shapes.eBottom = eBottomSC[i];
    shapes.e4SwissCross = e4SwissCrossSC[i];
    shapes.eMax = eMaxSC[i];
    shapes.e2nd = e2ndSC[i];
    shapes.sigmaIetaIeta = sqrt(covIEtaIEtaSC[i]);
    shapes.sigmaIphiIphi = sqrt(covIPhiIPhiSC[i]);
    shapes.sigmaIetaIphi = sqrt(covIEtaIPhiSC[i]);

    SuperCluster supercl(energySC[i],position,direction,shapes);
    supercl.setRawEnergy(rawEnergySC[i]);
    supercl.setPreshowerEnergy(esEnergySC[i]);
    supercl.setPreshowerEnergyPlane1(esL1EnergySC[i]);
    supercl.setPreshowerEnergyPlane2(esL2EnergySC[i]);
    supercl.setClustersSize(nBCSC[i]);

    SuperClusters.push_back(supercl);

  }

}


void AnalysisBase::loadPFSuperClusters() {

  PFSuperClusters.clear();

  for(int i=0; i<nPFSC; ++i) {
    
    TVector3 position(xPosPFSC[i],yPosPFSC[i],zPosPFSC[i]);
    TVector3 direction;
    float pt = energyPFSC[i]*fabs(sin(thetaPFSC[i]));
    direction.SetPtEtaPhi(pt,etaPFSC[i],phiPFSC[i]);
    
    SuperCluster::ClusterShapes shapes;
    shapes.nCrystals = nCrystalsPFSC[i];
    shapes.phiWidth = phiWidthPFSC[i];
    shapes.etaWidth = etaWidthPFSC[i];
    shapes.e3x3 = e3x3PFSC[i];
    shapes.e3x1 = e3x1PFSC[i];
    shapes.e1x3 = e1x3PFSC[i];
    shapes.e4x4 = e4x4PFSC[i];
    shapes.e5x5 = e5x5PFSC[i];
    shapes.e2x2 = e2x2PFSC[i];
    shapes.e1x5 = e1x5PFSC[i];
    shapes.e2x5Max = e2x5MaxPFSC[i];
    shapes.e2x5Left = e2x5LeftPFSC[i];
    shapes.e2x5Right = e2x5RightPFSC[i];
    shapes.e2x5Top = e2x5TopPFSC[i];
    shapes.e2x5Bottom = e2x5BottomPFSC[i];
    shapes.eLeft = eLeftPFSC[i];
    shapes.eRight = eRightPFSC[i];
    shapes.eTop = eTopPFSC[i];
    shapes.eBottom = eBottomPFSC[i];
    shapes.e4SwissCross = e4SwissCrossPFSC[i];
    shapes.eMax = eMaxPFSC[i];
    shapes.e2nd = e2ndPFSC[i];
    shapes.sigmaIetaIeta = sqrt(covIEtaIEtaPFSC[i]);
    shapes.sigmaIphiIphi = sqrt(covIPhiIPhiPFSC[i]);
    shapes.sigmaIetaIphi = sqrt(covIEtaIPhiPFSC[i]);

    SuperCluster supercl(energyPFSC[i],position,direction,shapes);
    supercl.setRawEnergy(rawEnergyPFSC[i]);
    supercl.setPreshowerEnergy(esEnergyPFSC[i]);
    supercl.setPreshowerEnergyPlane1(esL1EnergyPFSC[i]);
    supercl.setPreshowerEnergyPlane2(esL2EnergyPFSC[i]);
    supercl.setClustersSize(nBCPFSC[i]);

    PFSuperClusters.push_back(supercl);

  }

}


void AnalysisBase::loadGeneralTracks() {

  GeneralTracks.clear();

  Track::Point firstGoodPV(0.,0.,0.);
  if(PrimaryVertices.size()>0) firstGoodPV.SetXYZ(PrimaryVertices.front().x(),
						  PrimaryVertices.front().y(),
						  PrimaryVertices.front().z() );

  for(int i=0; i<nTrack; ++i) {

    Track::Vector momentum(pxTrack[i],pyTrack[i],pzTrack[i]);

    Track track(trackNormalizedChi2Track[i], firstGoodPV, momentum, chargeTrack[i]);

    Track::Point trackVxt(trackVxTrack[i],trackVyTrack[i],trackVzTrack[i]);
    track.setVertex(trackVxt);
  
    Track::HitPattern pattern;
    pattern.numberOfValidHits = trackValidHitsTrack[i];
    pattern.numberOfLostHits = trackLostHitsTrack[i];
    pattern.numberOfPixelHits = pixelHitsTrack[i];
    pattern.trackerLayersWithMeasurement = trackerLayersWithMeasurementTrack[i];
    pattern.expectedInnerLayers = expInnerLayersTrack[i];
    pattern.numberOfValidPixelBarrelHits = numberOfValidPixelBarrelHitsTrack[i];
    pattern.numberOfValidPixelEndcapHits = numberOfValidPixelEndcapHitsTrack[i];
    pattern.numberOfValidStripTIBHits = numberOfValidStripTIBHitsTrack[i];
    pattern.numberOfValidStripTIDHits = numberOfValidStripTIDHitsTrack[i];
    pattern.numberOfValidStripTOBHits = numberOfValidStripTOBHitsTrack[i];
    pattern.numberOfValidStripTECHits = numberOfValidStripTECHitsTrack[i];
    pattern.numberOfValidMuonHits = numberOfValidMuonHitsTrack[i];
    pattern.qualityMask = qualityMaskTrack[i];
    track.setHitPattern(pattern);
  
    Track::ImpactParameters ips;
    ips.ip3D = impactPar3DTrack[i];
    ips.ip3Derr = impactPar3DErrorTrack[i];
    ips.ip2D = transvImpactParTrack[i];
    ips.ip2Derr = transvImpactParErrorTrack[i];
    ips.d0 = d0Track[i];
    ips.d0err = d0ErrorTrack[i];
    ips.dz = dzTrack[i];
    ips.dzerr = dzErrorTrack[i];
    track.setImpactParameters(ips);

    track.setPtError(ptErrorTrack[i]);

    GeneralTracks.push_back(track);

  }

}

void AnalysisBase::loadGsfTracks() {

  GsfTracks.clear();

  Track::Point firstGoodPV(0.,0.,0.);
  if(PrimaryVertices.size()>0) firstGoodPV.SetXYZ(PrimaryVertices.front().x(),
						    PrimaryVertices.front().y(),
						    PrimaryVertices.front().z() );
  for(int i=0; i<nGsfTrack; ++i) {

    Track::Vector momentum(pxGsfTrack[i],pyGsfTrack[i],pzGsfTrack[i]);

    Track track(trackNormalizedChi2GsfTrack[i], firstGoodPV, momentum, chargeGsfTrack[i]);

    Track::Point trackVxt(trackVxGsfTrack[i],trackVyGsfTrack[i],trackVzGsfTrack[i]);
    track.setVertex(trackVxt);
    
    Track::HitPattern pattern;
    pattern.numberOfValidHits = trackValidHitsGsfTrack[i];
    pattern.numberOfLostHits = trackLostHitsGsfTrack[i];
    pattern.numberOfPixelHits = pixelHitsGsfTrack[i];
    pattern.trackerLayersWithMeasurement = trackerLayersWithMeasurementGsfTrack[i];
    pattern.expectedInnerLayers = expInnerLayersGsfTrack[i];
    pattern.numberOfValidPixelBarrelHits = numberOfValidPixelBarrelHitsGsfTrack[i];
    pattern.numberOfValidPixelEndcapHits = numberOfValidPixelEndcapHitsGsfTrack[i];
    pattern.numberOfValidStripTIBHits = numberOfValidStripTIBHitsGsfTrack[i];
    pattern.numberOfValidStripTIDHits = numberOfValidStripTIDHitsGsfTrack[i];
    pattern.numberOfValidStripTOBHits = numberOfValidStripTOBHitsGsfTrack[i];
    pattern.numberOfValidStripTECHits = numberOfValidStripTECHitsGsfTrack[i];
    pattern.numberOfValidMuonHits = numberOfValidMuonHitsGsfTrack[i];
    pattern.qualityMask = qualityMaskGsfTrack[i];

    track.setHitPattern(pattern);
    
    Track::ImpactParameters ips;
    ips.ip3D = impactPar3DGsfTrack[i];
    ips.ip3Derr = impactPar3DErrorGsfTrack[i];
    ips.ip2D = transvImpactParGsfTrack[i];
    ips.ip2Derr = transvImpactParErrorGsfTrack[i];
    ips.d0 = d0GsfTrack[i];
    ips.d0err = d0ErrorGsfTrack[i];
    ips.dz = dzGsfTrack[i];
    ips.dzerr = dzErrorGsfTrack[i];
    
    track.setImpactParameters(ips);

    track.setPtError(ptErrorGsfTrack[i]);

    GsfTracks.push_back(track);

    }
}

void AnalysisBase::loadGlobalMuonTracks() {

  GlobalMuonTracks.clear();

  Track::Point firstGoodPV(0.,0.,0.);
  if(PrimaryVertices.size()>0) firstGoodPV.SetXYZ(PrimaryVertices.front().x(),
						    PrimaryVertices.front().y(),
						    PrimaryVertices.front().z() );

  for(int i=0; i<nGlobalMuonTrack; ++i) {

    Track::Vector momentum(pxGlobalMuonTrack[i],pyGlobalMuonTrack[i],pzGlobalMuonTrack[i]);

    Track track(trackNormalizedChi2GlobalMuonTrack[i], firstGoodPV, momentum, chargeGlobalMuonTrack[i]);

    Track::Point trackVxt(trackVxGlobalMuonTrack[i],trackVyGlobalMuonTrack[i],trackVzGlobalMuonTrack[i]);
    track.setVertex(trackVxt);
    
    Track::HitPattern pattern;
    pattern.numberOfValidHits = trackValidHitsGlobalMuonTrack[i];
    pattern.numberOfLostHits = trackLostHitsGlobalMuonTrack[i];
    pattern.numberOfPixelHits = pixelHitsGlobalMuonTrack[i];
    pattern.trackerLayersWithMeasurement = trackerLayersWithMeasurementGlobalMuonTrack[i];
    pattern.expectedInnerLayers = expInnerLayersGlobalMuonTrack[i];
    pattern.numberOfValidPixelBarrelHits = numberOfValidPixelBarrelHitsGlobalMuonTrack[i];
    pattern.numberOfValidPixelEndcapHits = numberOfValidPixelEndcapHitsGlobalMuonTrack[i];
    pattern.numberOfValidStripTIBHits = numberOfValidStripTIBHitsGlobalMuonTrack[i];
    pattern.numberOfValidStripTIDHits = numberOfValidStripTIDHitsGlobalMuonTrack[i];
    pattern.numberOfValidStripTOBHits = numberOfValidStripTOBHitsGlobalMuonTrack[i];
    pattern.numberOfValidStripTECHits = numberOfValidStripTECHitsGlobalMuonTrack[i];
    pattern.numberOfValidMuonHits = numberOfValidMuonHitsGlobalMuonTrack[i];
    pattern.qualityMask = qualityMaskGlobalMuonTrack[i];

    track.setHitPattern(pattern);
    
    Track::ImpactParameters ips;
    ips.ip3D = impactPar3DGlobalMuonTrack[i];
    ips.ip3Derr = impactPar3DErrorGlobalMuonTrack[i];
    ips.ip2D = transvImpactParGlobalMuonTrack[i];
    ips.ip2Derr = transvImpactParErrorGlobalMuonTrack[i];
    ips.d0 = d0GlobalMuonTrack[i];
    ips.d0err = d0ErrorGlobalMuonTrack[i];
    ips.dz = dzGlobalMuonTrack[i];
    ips.dzerr = dzErrorGlobalMuonTrack[i];
    
    track.setImpactParameters(ips);

    track.setPtError(ptErrorGlobalMuonTrack[i]);

    GlobalMuonTracks.push_back(track);

    }
}

void AnalysisBase::loadStandaloneMuonTracks() {

  StandaloneMuonTracks.clear();

  Track::Point firstGoodPV(0.,0.,0.);
  if(PrimaryVertices.size()>0) firstGoodPV.SetXYZ(PrimaryVertices.front().x(),
						    PrimaryVertices.front().y(),
						    PrimaryVertices.front().z() );

  for(int i=0; i<nSTAMuonTrack; ++i) {

    Track::Vector momentum(pxSTAMuonTrack[i],pySTAMuonTrack[i],pzSTAMuonTrack[i]);

    Track track(trackNormalizedChi2STAMuonTrack[i], firstGoodPV, momentum, chargeSTAMuonTrack[i]);

    Track::Point trackVxt(trackVxSTAMuonTrack[i],trackVySTAMuonTrack[i],trackVzSTAMuonTrack[i]);
    track.setVertex(trackVxt);
    
    Track::HitPattern pattern;
    pattern.numberOfValidHits = trackValidHitsSTAMuonTrack[i];
    pattern.numberOfLostHits = trackLostHitsSTAMuonTrack[i];
    pattern.numberOfPixelHits = pixelHitsSTAMuonTrack[i];
    pattern.trackerLayersWithMeasurement = trackerLayersWithMeasurementSTAMuonTrack[i];
    pattern.expectedInnerLayers = expInnerLayersSTAMuonTrack[i];
    pattern.numberOfValidPixelBarrelHits = numberOfValidPixelBarrelHitsSTAMuonTrack[i];
    pattern.numberOfValidPixelEndcapHits = numberOfValidPixelEndcapHitsSTAMuonTrack[i];
    pattern.numberOfValidStripTIBHits = numberOfValidStripTIBHitsSTAMuonTrack[i];
    pattern.numberOfValidStripTIDHits = numberOfValidStripTIDHitsSTAMuonTrack[i];
    pattern.numberOfValidStripTOBHits = numberOfValidStripTOBHitsSTAMuonTrack[i];
    pattern.numberOfValidStripTECHits = numberOfValidStripTECHitsSTAMuonTrack[i];
    pattern.numberOfValidMuonHits = numberOfValidMuonHitsSTAMuonTrack[i];
    pattern.qualityMask = qualityMaskSTAMuonTrack[i];

    track.setHitPattern(pattern);
    
    Track::ImpactParameters ips;
    ips.d0 = d0STAMuonTrack[i];
    ips.d0err = d0ErrorSTAMuonTrack[i];
    ips.dz = dzSTAMuonTrack[i];
    ips.dzerr = dzErrorSTAMuonTrack[i];
    
    track.setImpactParameters(ips);

    track.setPtError(ptErrorSTAMuonTrack[i]);

    StandaloneMuonTracks.push_back(track);

    }
}

void AnalysisBase::loadElectronCollection() {

  ElectronCollection.clear();

  math::PhysConstants constants;

  for(int i=0; i<nEle; ++i) {
    int charge=chargeEle[i];

    TVector3 momentum3D(pxEle[i],pyEle[i],pzEle[i]);
    TLorentzVector p4Ele;
    p4Ele.SetVectM(momentum3D,constants.electron_mass);

    Vertex::Point vtx(vertexXEle[i],vertexYEle[i],vertexZEle[i]);

    int indexSC = superClusterIndexEle[i];
    SuperCluster sclu = (indexSC>=0) ? SuperClusters[indexSC] : SuperCluster();
    int indexPFSC = PFsuperClusterIndexEle[i];
    SuperCluster pfsclu = (indexPFSC>=0) ? PFSuperClusters[indexSC] : SuperCluster();

    int indexGsfTrack = gsfTrackIndexEle[i];
    Track gsfTrack = GsfTracks[indexGsfTrack]; // there is always a GsfTrack associated to an electron
    int indexCtfTrack = trackIndexEle[i];
    Track ctfTrack = (indexCtfTrack>=0) ? GeneralTracks[indexCtfTrack] : Track();

    Electron electron(charge,p4Ele,vtx,sclu,pfsclu,gsfTrack,ctfTrack);
    
    electron.setRecoFlags(recoFlagsEle[i]);
    electron.setFiducialFlags(fiducialFlagsEle[i]);
    electron.setScPixCharge(scPixChargeEle[i]);

    electron.setFbrem(fbremEle[i]);
    electron.setNbrem(nbremsEle[i]);
    electron.setClassification(classificationEle[i]);

    Electron::TrackClusterMatching trkclu;
    trkclu.eSuperClusterOverP   = eSuperClusterOverPEle[i];
    trkclu.eSeedClusterOverPout = eSeedOverPoutEle[i];
    trkclu.eSeedClusterOverP    = trkclu.eSeedClusterOverPout * electron.trackPAtOuter() / gsfTrack.p();
    trkclu.eEleClusterOverPout  = eEleClusterOverPoutEle[i];
    trkclu.deltaEtaSuperClusterAtVtx = deltaEtaAtVtxEle[i];
    trkclu.deltaPhiSuperClusterAtVtx = deltaPhiAtVtxEle[i];
    trkclu.deltaEtaSeedClusterAtCalo = deltaEtaAtCaloEle[i];
    trkclu.deltaPhiSeedClusterAtCalo = deltaPhiAtCaloEle[i];
    trkclu.deltaEtaEleClusterAtCalo = deltaEtaEleClusterTrackAtCaloEle[i];
    trkclu.deltaPhiEleClusterAtCalo = deltaPhiEleClusterTrackAtCaloEle[i];
    electron.setTrackClusterMatching(trkclu);

    electron.setHcalOverEcal(hOverEEle[i]);

    Electron::DetectorIsolationVariables detectorIso03;
    detectorIso03.tkSumPt = dr03TkSumPtEle[i];
    detectorIso03.ecalRecHitSumEt = dr03EcalRecHitSumEtEle[i];
    detectorIso03.hcalTowerSumEt = dr03HcalTowerSumEtEle[i];
    detectorIso03.hcalTowerFullConeSumEt = dr03HcalTowerSumEtFullConeEle[i];

    Electron::DetectorIsolationVariables detectorIso04;
    detectorIso04.tkSumPt = dr04TkSumPtEle[i];
    detectorIso04.ecalRecHitSumEt = dr04EcalRecHitSumEtEle[i];
    detectorIso04.hcalTowerSumEt = dr04HcalTowerSumEtEle[i];
    detectorIso04.hcalTowerFullConeSumEt = dr04HcalTowerSumEtFullConeEle[i];
    
    Electron::PFIsolationVariables pfIso03;
    pfIso03.chargedSumPt = pfCandChargedIso03Ele[i];
    pfIso03.photonSumPt = pfCandPhotonIso03Ele[i];
    pfIso03.neutralHadronSumPt = pfCandNeutralIso03Ele[i];
    pfIso03.sumPUPt = pfCandChargedPUIso03Ele[i];
    pfIso03.directionalChargedSumPt = -999.;
    pfIso03.directionalPhotonSumPt = -999.;
    pfIso03.directionalNeutralHadronSumPt = -999.;

    Electron::PFIsolationVariables pfIso04;
    pfIso04.chargedSumPt = pfCandChargedIso04Ele[i];
    pfIso04.photonSumPt = pfCandPhotonIso04Ele[i];
    pfIso04.neutralHadronSumPt = pfCandNeutralIso04Ele[i];
    pfIso04.sumPUPt = pfCandChargedPUIso04Ele[i];
    pfIso04.directionalChargedSumPt = pfCandChargedDirIso04Ele[i];
    pfIso04.directionalPhotonSumPt = pfCandPhotonDirIso04Ele[i];
    pfIso04.directionalNeutralHadronSumPt = pfCandNeutralDirIso04Ele[i];

    electron.setDr03DetectorIsolation(detectorIso03);
    electron.setDr04DetectorIsolation(detectorIso04);
    electron.setDr03PFIsolation(pfIso03);
    electron.setDr04PFIsolation(pfIso04);

    Electron::ConversionRejection cr;
    cr.dist = convDistEle[i];
    cr.dcot = convDcotEle[i];
    cr.radius = convRadiusEle[i];
    cr.hasMatchedConv = hasMatchedConversionEle[i];

    electron.setConversionVariables(cr);

    Electron::IDMvaOutput mvas;
    mvas.mvaTriggering = mvaidtrigEle[i];
    mvas.mvaNonTriggering = mvaidnontrigEle[i];

    electron.setIDMVAs(mvas);

    electron.setCalibratedEnergy(calibEnergyEle[i]);
    electron.setCalibratedEnergyError(calibEnergyErrorEle[i]);
    electron.setTrackMomentumError(trackMomentumErrorEle[i]);

  }

}

void AnalysisBase::loadMuonCollection() {

  MuonCollection.clear();

  math::PhysConstants constants;

  for(int i=0; i<nMuon; ++i) {
    int charge=chargeMuon[i];

    TVector3 momentum3D(pxMuon[i],pyMuon[i],pzMuon[i]);
    TLorentzVector p4Muon;
    p4Muon.SetVectM(momentum3D,constants.muon_mass);

    Vertex::Point vtx(vertexXMuon[i],vertexYMuon[i],vertexZMuon[i]);

    int indexInnerTrack = trackIndexMuon[i];
    Track innerTrack = GeneralTracks[indexInnerTrack];

    int indexStandaloneMuonTrack = standAloneTrackIndexMuon[i];
    Track staTrack = StandaloneMuonTracks[indexStandaloneMuonTrack];

    int indexCombinedTrack = combinedTrackIndexMuon[i];
    Track combTrack = GlobalMuonTracks[indexCombinedTrack];

    Muon muon(charge,p4Muon,vtx,innerTrack,staTrack,combTrack);
  }

}
