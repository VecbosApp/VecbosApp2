//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jan 21 17:50:37 2014 by ROOT version 5.34/11
// from TTree T1/eleID tree
// found on file: data/fakes-zll1e.root
//////////////////////////////////////////////////////////

#ifndef egtree_h
#define egtree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class egtree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         Charge;
   Bool_t          IsEB;
   Bool_t          IsEE;
   Float_t         EEleoPout;
   Float_t         ESeedoPout;
   Float_t         EoP;
   Float_t         EoPin;
   Float_t         IoEmIoP;
   Float_t         HoE;
   Float_t         eledeta;
   Float_t         deta;
   Float_t         dphi;
   Float_t         detacalo;
   Float_t         dphicalo;
   Float_t         fbrem;
   Float_t         nbrems;
   Float_t         missHits;
   Float_t         dist;
   Float_t         dcot;
   Float_t         d0;
   Float_t         dz;
   Float_t         ip3d;
   Float_t         ip3ds;
   Float_t         kfhits;
   Float_t         kflayers;
   Float_t         kfchi2;
   Float_t         gsfchi2;
   Float_t         s9s25;
   Float_t         phiwidth;
   Float_t         etawidth;
   Float_t         see;
   Float_t         sep;
   Float_t         spp;
   Float_t         NClusters;
   Float_t         e1x5e5x5;
   Float_t         scEt;
   Float_t         scEta;
   Float_t         scPhi;
   Float_t         SCRawEnergy;
   Float_t         ecalenergy;
   Float_t         esenergy;
   Float_t         PreShowerOverRaw;
   Float_t         EtaSeed;
   Float_t         PhiSeed;
   Float_t         ESeed;
   Float_t         IEtaSeed;
   Float_t         IPhiSeed;
   Float_t         EtaCrySeed;
   Float_t         PhiCrySeed;
   Float_t         IEtaCrySeed;
   Float_t         IPhiCrySeed;
   Float_t         EMaxSeed;
   Float_t         ETopSeed;
   Float_t         EBottomSeed;
   Float_t         ELeftSeed;
   Float_t         ERightSeed;
   Float_t         E2ndSeed;
   Float_t         E2x5RightSeed;
   Float_t         E2x5LeftSeed;
   Float_t         E2x5TopSeed;
   Float_t         E2x5BottomSeed;
   Float_t         E2x5MaxSeed;
   Float_t         E1x5Seed;
   Float_t         E2x2Seed;
   Float_t         E3x3Seed;
   Float_t         E5x5Seed;
   Float_t         OneMinusSeedE1x5OverE5x5;
   Float_t         R9;
   Bool_t          matchConv;
   Bool_t          ecaldriven;
   UInt_t          run;
   UInt_t          lumi;
   ULong64_t       event;
   Bool_t          EventNumberParity;
   Float_t         vertices;
   Float_t         rho;
   Float_t         npu[3];
   Bool_t          mcmatch;
   Float_t         mass;
   Float_t         zdec;
   Float_t         etrue;
   Float_t         thetatrue;
   Float_t         phitrue;
   Float_t         pt[4];
   Float_t         eta[4];
   Float_t         phi[4];
   Float_t         bdttrg;
   Float_t         bdtnontrg;
   Int_t           wp_cuts[4];
   Int_t           wp_cuts_id[4];
   Int_t           wp_cuts_iso[4];
   Int_t           wp_cuts_conv[4];
   Int_t           wp_mva[4];
   Int_t           wp_mva_id[4];
   Int_t           wp_mva_iso[4];
   Int_t           wp_mva_conv[4];
   Bool_t          DenomFake;
   Float_t         leadJetPt;
   Float_t         trkIso04;
   Float_t         ecalIso04;
   Float_t         hcalIso04;
   Float_t         combPFIsoDBeta;
   Float_t         chaPFIso;
   Float_t         neuPFIso;
   Float_t         phoPFIso;

   // List of branches
   TBranch        *b_Charge;   //!
   TBranch        *b_IsEB;   //!
   TBranch        *b_IsEE;   //!
   TBranch        *b_EEleoPout;   //!
   TBranch        *b_ESeedoPout;   //!
   TBranch        *b_EoP;   //!
   TBranch        *b_EoPin;   //!
   TBranch        *b_IoEmIoP;   //!
   TBranch        *b_HoE;   //!
   TBranch        *b_eledeta;   //!
   TBranch        *b_deta;   //!
   TBranch        *b_dphi;   //!
   TBranch        *b_detacalo;   //!
   TBranch        *b_dphicalo;   //!
   TBranch        *b_fbrem;   //!
   TBranch        *b_nbrems;   //!
   TBranch        *b_missHits;   //!
   TBranch        *b_dist;   //!
   TBranch        *b_dcot;   //!
   TBranch        *b_d0;   //!
   TBranch        *b_dz;   //!
   TBranch        *b_ip3d;   //!
   TBranch        *b_ip3ds;   //!
   TBranch        *b_kfhits;   //!
   TBranch        *b_kflayers;   //!
   TBranch        *b_kfchi2;   //!
   TBranch        *b_gsfchi2;   //!
   TBranch        *b_s9s25;   //!
   TBranch        *b_phiwidth;   //!
   TBranch        *b_etawidth;   //!
   TBranch        *b_see;   //!
   TBranch        *b_sep;   //!
   TBranch        *b_spp;   //!
   TBranch        *b_NClusters;   //!
   TBranch        *b_e1x5e5x5;   //!
   TBranch        *b_scEt;   //!
   TBranch        *b_scEta;   //!
   TBranch        *b_scPhi;   //!
   TBranch        *b_SCRawEnergy;   //!
   TBranch        *b_ecalenergy;   //!
   TBranch        *b_esenergy;   //!
   TBranch        *b_PreShowerOverRaw;   //!
   TBranch        *b_EtaSeed;   //!
   TBranch        *b_PhiSeed;   //!
   TBranch        *b_ESeed;   //!
   TBranch        *b_IEtaSeed;   //!
   TBranch        *b_IPhiSeed;   //!
   TBranch        *b_EtaCrySeed;   //!
   TBranch        *b_PhiCrySeed;   //!
   TBranch        *b_IEtaCrySeed;   //!
   TBranch        *b_IPhiCrySeed;   //!
   TBranch        *b_EMaxSeed;   //!
   TBranch        *b_ETopSeed;   //!
   TBranch        *b_EBottomSeed;   //!
   TBranch        *b_ELeftSeed;   //!
   TBranch        *b_ERightSeed;   //!
   TBranch        *b_E2ndSeed;   //!
   TBranch        *b_E2x5RightSeed;   //!
   TBranch        *b_E2x5LeftSeed;   //!
   TBranch        *b_E2x5TopSeed;   //!
   TBranch        *b_E2x5BottomSeed;   //!
   TBranch        *b_E2x5MaxSeed;   //!
   TBranch        *b_E1x5Seed;   //!
   TBranch        *b_E2x2Seed;   //!
   TBranch        *b_E3x3Seed;   //!
   TBranch        *b_E5x5Seed;   //!
   TBranch        *b_OneMinusSeedE1x5OverE5x5;   //!
   TBranch        *b_R9;   //!
   TBranch        *b_matchConv;   //!
   TBranch        *b_ecaldriven;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!
   TBranch        *b_EventNumberParity;   //!
   TBranch        *b_vertices;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_npu;   //!
   TBranch        *b_mcmatch;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_zdec;   //!
   TBranch        *b_etrue;   //!
   TBranch        *b_thetatrue;   //!
   TBranch        *b_phitrue;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_bdthww;   //!
   TBranch        *b_bdtnontrg;   //!
   TBranch        *b_wp_cuts;   //!
   TBranch        *b_wp_cuts_id;   //!
   TBranch        *b_wp_cuts_iso;   //!
   TBranch        *b_wp_cuts_conv;   //!
   TBranch        *b_wp_mva;   //!
   TBranch        *b_wp_mva_id;   //!
   TBranch        *b_wp_mva_iso;   //!
   TBranch        *b_wp_mva_conv;   //!
   TBranch        *b_DenomFake;   //!
   TBranch        *b_leadJetPt;   //!
   TBranch        *b_trkIso04;   //!
   TBranch        *b_ecalIso04;   //!
   TBranch        *b_hcalIso04;   //!
   TBranch        *b_combPFIsoDBeta;   //!
   TBranch        *b_chPFIso;   //!
   TBranch        *b_neuPFIso;   //!
   TBranch        *b_phoPFIso;   //!

   egtree(TTree *tree=0);
   virtual ~egtree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef egtree_cxx
egtree::egtree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data/fakes-zll1e.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data/fakes-zll1e.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("data/fakes-zll1e.root:/electrons");
      dir->GetObject("T1",tree);

   }
   Init(tree);
}

egtree::~egtree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t egtree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t egtree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void egtree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Charge", &Charge, &b_Charge);
   fChain->SetBranchAddress("IsEB", &IsEB, &b_IsEB);
   fChain->SetBranchAddress("IsEE", &IsEE, &b_IsEE);
   fChain->SetBranchAddress("EEleoPout", &EEleoPout, &b_EEleoPout);
   fChain->SetBranchAddress("ESeedoPout", &ESeedoPout, &b_ESeedoPout);
   fChain->SetBranchAddress("EoP", &EoP, &b_EoP);
   fChain->SetBranchAddress("EoPin", &EoPin, &b_EoPin);
   fChain->SetBranchAddress("IoEmIoP", &IoEmIoP, &b_IoEmIoP);
   fChain->SetBranchAddress("HoE", &HoE, &b_HoE);
   fChain->SetBranchAddress("eledeta", &eledeta, &b_eledeta);
   fChain->SetBranchAddress("deta", &deta, &b_deta);
   fChain->SetBranchAddress("dphi", &dphi, &b_dphi);
   fChain->SetBranchAddress("detacalo", &detacalo, &b_detacalo);
   fChain->SetBranchAddress("dphicalo", &dphicalo, &b_dphicalo);
   fChain->SetBranchAddress("fbrem", &fbrem, &b_fbrem);
   fChain->SetBranchAddress("nbrems", &nbrems, &b_nbrems);
   fChain->SetBranchAddress("missHits", &missHits, &b_missHits);
   fChain->SetBranchAddress("dist", &dist, &b_dist);
   fChain->SetBranchAddress("dcot", &dcot, &b_dcot);
   fChain->SetBranchAddress("d0", &d0, &b_d0);
   fChain->SetBranchAddress("dz", &dz, &b_dz);
   fChain->SetBranchAddress("ip3d", &ip3d, &b_ip3d);
   fChain->SetBranchAddress("ip3ds", &ip3ds, &b_ip3ds);
   fChain->SetBranchAddress("kfhits", &kfhits, &b_kfhits);
   fChain->SetBranchAddress("kflayers", &kflayers, &b_kflayers);
   fChain->SetBranchAddress("kfchi2", &kfchi2, &b_kfchi2);
   fChain->SetBranchAddress("gsfchi2", &gsfchi2, &b_gsfchi2);
   fChain->SetBranchAddress("s9s25", &s9s25, &b_s9s25);
   fChain->SetBranchAddress("phiwidth", &phiwidth, &b_phiwidth);
   fChain->SetBranchAddress("etawidth", &etawidth, &b_etawidth);
   fChain->SetBranchAddress("see", &see, &b_see);
   fChain->SetBranchAddress("sep", &sep, &b_sep);
   fChain->SetBranchAddress("spp", &spp, &b_spp);
   fChain->SetBranchAddress("NClusters", &NClusters, &b_NClusters);
   fChain->SetBranchAddress("e1x5e5x5", &e1x5e5x5, &b_e1x5e5x5);
   fChain->SetBranchAddress("scEt", &scEt, &b_scEt);
   fChain->SetBranchAddress("scEta", &scEta, &b_scEta);
   fChain->SetBranchAddress("scPhi", &scPhi, &b_scPhi);
   fChain->SetBranchAddress("SCRawEnergy", &SCRawEnergy, &b_SCRawEnergy);
   fChain->SetBranchAddress("ecalenergy", &ecalenergy, &b_ecalenergy);
   fChain->SetBranchAddress("esenergy", &esenergy, &b_esenergy);
   fChain->SetBranchAddress("PreShowerOverRaw", &PreShowerOverRaw, &b_PreShowerOverRaw);
   fChain->SetBranchAddress("EtaSeed", &EtaSeed, &b_EtaSeed);
   fChain->SetBranchAddress("PhiSeed", &PhiSeed, &b_PhiSeed);
   fChain->SetBranchAddress("ESeed", &ESeed, &b_ESeed);
   fChain->SetBranchAddress("IEtaSeed", &IEtaSeed, &b_IEtaSeed);
   fChain->SetBranchAddress("IPhiSeed", &IPhiSeed, &b_IPhiSeed);
   fChain->SetBranchAddress("EtaCrySeed", &EtaCrySeed, &b_EtaCrySeed);
   fChain->SetBranchAddress("PhiCrySeed", &PhiCrySeed, &b_PhiCrySeed);
   fChain->SetBranchAddress("IEtaCrySeed", &IEtaCrySeed, &b_IEtaCrySeed);
   fChain->SetBranchAddress("IPhiCrySeed", &IPhiCrySeed, &b_IPhiCrySeed);
   fChain->SetBranchAddress("EMaxSeed", &EMaxSeed, &b_EMaxSeed);
   fChain->SetBranchAddress("ETopSeed", &ETopSeed, &b_ETopSeed);
   fChain->SetBranchAddress("EBottomSeed", &EBottomSeed, &b_EBottomSeed);
   fChain->SetBranchAddress("ELeftSeed", &ELeftSeed, &b_ELeftSeed);
   fChain->SetBranchAddress("ERightSeed", &ERightSeed, &b_ERightSeed);
   fChain->SetBranchAddress("E2ndSeed", &E2ndSeed, &b_E2ndSeed);
   fChain->SetBranchAddress("E2x5RightSeed", &E2x5RightSeed, &b_E2x5RightSeed);
   fChain->SetBranchAddress("E2x5LeftSeed", &E2x5LeftSeed, &b_E2x5LeftSeed);
   fChain->SetBranchAddress("E2x5TopSeed", &E2x5TopSeed, &b_E2x5TopSeed);
   fChain->SetBranchAddress("E2x5BottomSeed", &E2x5BottomSeed, &b_E2x5BottomSeed);
   fChain->SetBranchAddress("E2x5MaxSeed", &E2x5MaxSeed, &b_E2x5MaxSeed);
   fChain->SetBranchAddress("E1x5Seed", &E1x5Seed, &b_E1x5Seed);
   fChain->SetBranchAddress("E2x2Seed", &E2x2Seed, &b_E2x2Seed);
   fChain->SetBranchAddress("E3x3Seed", &E3x3Seed, &b_E3x3Seed);
   fChain->SetBranchAddress("E5x5Seed", &E5x5Seed, &b_E5x5Seed);
   fChain->SetBranchAddress("OneMinusSeedE1x5OverE5x5", &OneMinusSeedE1x5OverE5x5, &b_OneMinusSeedE1x5OverE5x5);
   fChain->SetBranchAddress("R9", &R9, &b_R9);
   fChain->SetBranchAddress("matchConv", &matchConv, &b_matchConv);
   fChain->SetBranchAddress("ecaldriven", &ecaldriven, &b_ecaldriven);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("EventNumberParity", &EventNumberParity, &b_EventNumberParity);
   fChain->SetBranchAddress("vertices", &vertices, &b_vertices);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("npu", npu, &b_npu);
   fChain->SetBranchAddress("mcmatch", &mcmatch, &b_mcmatch);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("zdec", &zdec, &b_zdec);
   fChain->SetBranchAddress("etrue", &etrue, &b_etrue);
   fChain->SetBranchAddress("thetatrue", &thetatrue, &b_thetatrue);
   fChain->SetBranchAddress("phitrue", &phitrue, &b_phitrue);
   fChain->SetBranchAddress("pt", pt, &b_pt);
   fChain->SetBranchAddress("eta", eta, &b_eta);
   fChain->SetBranchAddress("phi", phi, &b_phi);
   fChain->SetBranchAddress("bdttrg", &bdttrg, &b_bdthww);
   fChain->SetBranchAddress("bdtnontrg", &bdtnontrg, &b_bdtnontrg);
   fChain->SetBranchAddress("wp_cuts", wp_cuts, &b_wp_cuts);
   fChain->SetBranchAddress("wp_cuts_id", wp_cuts_id, &b_wp_cuts_id);
   fChain->SetBranchAddress("wp_cuts_iso", wp_cuts_iso, &b_wp_cuts_iso);
   fChain->SetBranchAddress("wp_cuts_conv", wp_cuts_conv, &b_wp_cuts_conv);
   fChain->SetBranchAddress("wp_mva", wp_mva, &b_wp_mva);
   fChain->SetBranchAddress("wp_mva_id", wp_mva_id, &b_wp_mva_id);
   fChain->SetBranchAddress("wp_mva_iso", wp_mva_iso, &b_wp_mva_iso);
   fChain->SetBranchAddress("wp_mva_conv", wp_mva_conv, &b_wp_mva_conv);
   fChain->SetBranchAddress("DenomFake", &DenomFake, &b_DenomFake);
   fChain->SetBranchAddress("leadJetPt", &leadJetPt, &b_leadJetPt);
   fChain->SetBranchAddress("trkIso04", &trkIso04, &b_trkIso04);
   fChain->SetBranchAddress("ecalIso04", &ecalIso04, &b_ecalIso04);
   fChain->SetBranchAddress("hcalIso04", &hcalIso04, &b_hcalIso04);
   fChain->SetBranchAddress("combPFIsoDBeta", &combPFIsoDBeta, &b_combPFIsoDBeta);
   fChain->SetBranchAddress("chaPFIso", &chaPFIso, &b_chPFIso);
   fChain->SetBranchAddress("neuPFIso", &neuPFIso, &b_neuPFIso);
   fChain->SetBranchAddress("phoPFIso", &phoPFIso, &b_phoPFIso);
   Notify();
}

Bool_t egtree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void egtree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t egtree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef egtree_cxx
