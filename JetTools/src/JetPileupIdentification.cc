#include <iostream>
#include "JetTools/include/JetPileupIdentification.hh"

using namespace vecbos;
using namespace std;

bool JetPileupIdentification::output(wp_t wp) {

  bool isOk = true;

  float pt = jet_->pt(); 
  float eta = jet_->eta();
  int ieta;
  if ( fabs(eta)<=2.5 ) ieta = 0;
  else if ( fabs(eta)<=2.75 ) ieta = 1;
  else if ( fabs(eta)<=3.0 ) ieta = 2;
  else if ( fabs(eta)<=5.0 ) ieta = 3;

  if(version_ < cutbased) {

    float id = jet_->mva();
    
    // Tight Id
    float Pt010_Tight[2][4]     = {{-0.83,-0.81,-0.74,-0.81},
				   {-0.50,-0.20,-0.83,-0.70}};
    float Pt1020_Tight[2][4]    = {{-0.83,-0.81,-0.74,-0.81},
				   {-0.50,-0.20,-0.83,-0.70}};
    float Pt2030_Tight[2][4]    = {{ 0.73, 0.05,-0.26,-0.42},
				   {-0.2,  0.00, 0.00, 0.00}};
    float Pt3050_Tight[2][4]    = {{ 0.73, 0.05,-0.26,-0.42},
				   {-0.20, 0.00, 0.00, 0.00}};
    
    // Medium Id
    float Pt010_Medium[2][4]    = {{-0.83,-0.92,-0.90,-0.92},
				   {-0.73,-0.89,-0.89,-0.83}};
    float Pt1020_Medium[2][4]   = {{-0.83,-0.92,-0.90,-0.92},
				   {-0.73,-0.89,-0.89,-0.83}};
    float Pt2030_Medium[2][4]   = {{ 0.10,-0.36,-0.54,-0.54},
				   { 0.10,-0.40,-0.40,-0.45}};
    float Pt3050_Medium[2][4]   = {{ 0.10,-0.36,-0.54,-0.54},
				   {0.1,  -0.4, -0.4, -0.45}};
    
    // Loose Id
    float Pt010_Loose[2][4]     = {{-0.95,-0.96,-0.94,-0.95},
				   {-0.90,-0.90,-0.90,-0.90}};
    float Pt1020_Loose[2][4]    = {{-0.95,-0.96,-0.94,-0.95},
				   {-0.90,-0.90,-0.90,-0.90}};
    float Pt2030_Loose[2][4]    = {{-0.63,-0.60,-0.55,-0.45},
				   {-0.4,-0.85,-0.7,-0.6}};
    float Pt3050_Loose[2][4]    = {{-0.63,-0.60,-0.55,-0.45},
				   {-0.4,-0.85,-0.7,-0.6}};
      

    switch(wp) {
    case JetPileupIdentification::tight:
      if (pt<10           && id<Pt010_Tight[version_][ieta] ) isOk = false;
      if (pt<20 && pt>=10 && id<Pt1020_Tight[version_][ieta]) isOk = false;
      if (pt<30 && pt>=20 && id<Pt2030_Tight[version_][ieta]) isOk = false;
      if (pt<50 && pt>=30 && id<Pt3050_Tight[version_][ieta]) isOk = false;
    case JetPileupIdentification::medium:
      if (pt<10           && id<Pt010_Medium[version_][ieta] ) isOk = false;
      if (pt<20 && pt>=10 && id<Pt1020_Medium[version_][ieta]) isOk = false;
      if (pt<30 && pt>=20 && id<Pt2030_Medium[version_][ieta]) isOk = false;
      if (pt<50 && pt>=30 && id<Pt3050_Medium[version_][ieta]) isOk = false;
    case JetPileupIdentification::loose:
      if (pt<10           && id<Pt010_Loose[version_][ieta] ) isOk = false;
      if (pt<20 && pt>=10 && id<Pt1020_Loose[version_][ieta]) isOk = false;
      if (pt<30 && pt>=20 && id<Pt2030_Loose[version_][ieta]) isOk = false;
      if (pt<50 && pt>=30 && id<Pt3050_Loose[version_][ieta]) isOk = false;
    default:
      cout << "WARNING: JET PU ID working point = " << wp << " not foreseen" << endl;
    }
  } else {
    // 4 Eta Categories  0-2.5 2.5-2.75 2.75-3.0 3.0-5.0
    // Tight Id
    float Pt010_BetaStarTight[4]    = { 0.15, 0.15, 999., 999. };
    float Pt1020_BetaStarTight[4]   = { 0.15, 0.15, 999., 999. };
    float Pt2030_BetaStarTight[4]   = { 0.15, 0.15, 999., 999. };
    float Pt3050_BetaStarTight[4]   = { 0.15, 0.15, 999., 999. };
    
    // Medium Id => Daniele
    float Pt010_BetaStarMedium[4]   = { 0.2, 0.3, 999., 999. };
    float Pt1020_BetaStarMedium[4]  = { 0.2, 0.3, 999., 999. };
    float Pt2030_BetaStarMedium[4]  = { 0.2, 0.3, 999., 999. };
    float Pt3050_BetaStarMedium[4]  = { 0.2, 0.3, 999., 999. };
    
    // Loose Id
    float Pt010_BetaStarLoose[4]    = { 0.2, 0.3, 999., 999. };
    float Pt1020_BetaStarLoose[4]   = { 0.2, 0.3, 999., 999. };
    float Pt2030_BetaStarLoose[4]   = { 0.2, 0.3, 999., 999. };
    float Pt3050_BetaStarLoose[4]   = { 0.2, 0.3, 999., 999. };

    /// RMS variable
    // Tight Id
    float Pt010_RMSTight[4]         = { 0.06, 0.07, 0.04, 0.05 };
    float Pt1020_RMSTight[4]        = { 0.06, 0.07, 0.04, 0.05 };
    float Pt2030_RMSTight[4]        = { 0.05, 0.07, 0.03, 0.045 };
    float Pt3050_RMSTight[4]        = { 0.05, 0.06, 0.03, 0.04 };
    
    /// Medium Id => Daniele
    float Pt010_RMSMedium[4]        = { 0.06, 0.03, 0.03, 0.04 };
    float Pt1020_RMSMedium[4]       = { 0.06, 0.03, 0.03, 0.04 };
    float Pt2030_RMSMedium[4]       = { 0.06, 0.03, 0.03, 0.04 };
    float Pt3050_RMSMedium[4]       = { 0.06, 0.03, 0.03, 0.04 };
    
    /// Loose[4] Id
    float Pt010_RMSLoose[4]         = { 0.06, 0.05, 0.05, 0.070 };
    float Pt1020_RMSLoose[4]        = { 0.06, 0.05, 0.05, 0.070 };
    float Pt2030_RMSLoose[4]        = { 0.06, 0.05, 0.05, 0.055 };
    float Pt3050_RMSLoose[4]        = { 0.06, 0.05, 0.05, 0.055 };

    float betastar = jet_->betastar();
    float rms = jet_->rms();

    switch(wp) {
    case JetPileupIdentification::tight:
      if (pt<10           && betastar < Pt010_BetaStarTight[ieta]  && rms < Pt010_RMSTight[ieta]) isOk = false;
      if (pt<20 && pt>=10 && betastar < Pt1020_BetaStarTight[ieta] && rms < Pt1020_RMSTight[ieta]) isOk = false;
      if (pt<30 && pt>=20 && betastar < Pt2030_BetaStarTight[ieta] && rms < Pt2030_RMSTight[ieta]) isOk = false;
      if (pt<50 && pt>=30 && betastar < Pt3050_BetaStarTight[ieta] && rms < Pt3050_RMSTight[ieta]) isOk = false;
    case JetPileupIdentification::medium:
      if (pt<10           && betastar < Pt010_BetaStarMedium[ieta]  && rms < Pt010_RMSMedium[ieta]) isOk = false;
      if (pt<20 && pt>=10 && betastar < Pt1020_BetaStarMedium[ieta] && rms < Pt1020_RMSMedium[ieta]) isOk = false;
      if (pt<30 && pt>=20 && betastar < Pt2030_BetaStarMedium[ieta] && rms < Pt2030_RMSMedium[ieta]) isOk = false;
      if (pt<50 && pt>=30 && betastar < Pt3050_BetaStarMedium[ieta] && rms < Pt3050_RMSMedium[ieta]) isOk = false;
    case JetPileupIdentification::loose:
      if (pt<10           && betastar < Pt010_BetaStarLoose[ieta]  && rms < Pt010_RMSLoose[ieta]) isOk = false;
      if (pt<20 && pt>=10 && betastar < Pt1020_BetaStarLoose[ieta] && rms < Pt1020_RMSLoose[ieta]) isOk = false;
      if (pt<30 && pt>=20 && betastar < Pt2030_BetaStarLoose[ieta] && rms < Pt2030_RMSLoose[ieta]) isOk = false;
      if (pt<50 && pt>=30 && betastar < Pt3050_BetaStarLoose[ieta] && rms < Pt3050_RMSLoose[ieta]) isOk = false;
    default:
      cout << "WARNING: JET PU ID working point = " << wp << " not foreseen" << endl;
    }
  }

  return isOk;

}
