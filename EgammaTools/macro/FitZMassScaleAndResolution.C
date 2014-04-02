

//======================================================== //
// For ECAL with CMS Detector at LHC                       //
// Roofit Macro for Unbinned fit to Z peak                 //
//======================================================== //

#ifndef __CINT__
#include<stdio.h>
#include<string>
#include<sstream> 
#include<iostream>
#include<fstream>
#endif

#include "RooGlobalFunc.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooExponential.h"
#include "RooFFTConvPdf.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"

#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"
#include "hzztree.C"
#include "../../Tools/src/RooHZZStyle.C"
 
using namespace RooFit;

void makefit(string inputFilename, string outFilename,  
	     Int_t ptBin, Int_t etaBin, Int_t R9Bin,
	     double minMass, double maxMass, 
	     double mean_bw, double gamma_bw, double cutoff_cb, double power_cb,
	     const char *plotOpt, const int nbins, Int_t isMC);

void makejpsifit(string inputFilename, string outFilename,  
		 Int_t ptBin, Int_t etaBin,
		 double minMass, double maxMass, 
		 double mean_bw, double gamma_bw, double cutoff_cb, double power_cb,
		 const char *plotOpt, const int nbins, Int_t isMC);

void FitZMassScaleAndResolution(string inputFilename, string outFilename, Int_t ptBin, Int_t etaBin, Int_t R9Bin, Int_t isMC) {

  // Define Fit Inputs and Call Fit
  double minMass = 75;
  double maxMass = 105;
  double mean_bw = 91.1876;
  double gamma_bw = 2.4952;
  double cutoff_cb = 1.0;
//double power_cb = 1.40;		// Use to fix some fits
  double power_cb = 2.45;
  const char *plotOpt = "NEU";
  int nbins = 40;
  if(ptBin==0 && etaBin==1 && R9Bin==0) nbins = 25; // unique low stat sample

  // Call the fitting program and output a workspace with a root file
  // of the model and data as well as a pdf of the fit
  makefit(inputFilename, outFilename, ptBin, etaBin, R9Bin, minMass,  maxMass,  mean_bw,  gamma_bw,  cutoff_cb, power_cb, plotOpt, nbins, isMC);

}
//______________________________________________________________


void makefit(string inputFilename, string outFilename, 
	     Int_t ptBin, Int_t etaBin, Int_t R9Bin,
	     double minMass, double maxMass, 
	     double mean_bw, double gamma_bw, double cutoff_cb, double power_cb, 
	     const char* plotOpt, const int nbins, Int_t isMC) {

//   gROOT->ProcessLine(".L tdrstyle.C");
//   setTDRStyle();
//   gStyle->SetPadRightMargin(0.05);

  //Create Data Set
  RooRealVar mass("zmass","m(e^{+}e^{-})",minMass,maxMass,"GeV/c^{2}");
  RooRealVar puw("puW","pileup weight",0.,2.);
  //  mass.setRange(80,100);

  // Reading everything from root tree instead
  TFile *tfile = TFile::Open(inputFilename.c_str());
  TTree *ttree = (TTree*)tfile->Get("zeetree/probe_tree");
  hzztree *zeeTree = new hzztree(ttree);

  TString nF(inputFilename.c_str());
  nF.ReplaceAll(".root","_friend.root");
  ttree->AddFriend("puweights=zeetree/probe_tree",nF.Data());
  float puW;
  ttree->SetBranchAddress("puW",&puW);
  
  RooArgSet zMassArgSet(mass,puw);
  RooDataSet* data = new RooDataSet("data", "ntuple parameters", zMassArgSet, RooFit::WeightVar("puW"));

  for (int i = 0; i < zeeTree->fChain->GetEntries(); i++) {
    if(i%100000==0) cout << "Processing Event " << i << endl;
    zeeTree->fChain->GetEntry(i);

    //*************************************************************************
    //Electron Selection
    //*************************************************************************
    // already passed for this tree

    //*************************************************************************
    //Compute electron four vector;
    //*************************************************************************
    double ele1pt = zeeTree->l1pt;
    double ele2pt = zeeTree->l2pt;

    double ELECTRONMASS = 0.51e-3;
    TLorentzVector ele1FourVector;
    ele1FourVector.SetPtEtaPhiM(zeeTree->l1pt, zeeTree->l1eta, zeeTree->l1phi, ELECTRONMASS);
    TLorentzVector ele2FourVector;
    ele2FourVector.SetPtEtaPhiM(zeeTree->l2pt, zeeTree->l2eta, zeeTree->l2phi, ELECTRONMASS);

    
    //*************************************************************************
    //pt and eta cuts on electron
    //*************************************************************************
    if (! (ele1pt > 7 && ele2pt > 7
           && fabs( zeeTree->l1eta) < 2.5 
           && fabs( zeeTree->l2eta) < 2.5 )) continue;

    //*************************************************************************
    //pt bins and eta bins
    //*************************************************************************
    Int_t Ele1PtBin = -1;
    Int_t Ele1EtaBin = -1;
    Int_t Ele2PtBin = -1;
    Int_t Ele2EtaBin = -1;
    Int_t Ele1R9Bin = -1;
    Int_t Ele2R9Bin = -1;
    if (ele1pt > 20 && ele1pt < 30) Ele1PtBin = 0;
    else if (ele1pt < 40) Ele1PtBin = 1;
    else if (ele1pt < 50) Ele1PtBin = 2;
    else Ele1PtBin = 3;
    if (ele2pt > 20 && ele2pt < 30) Ele2PtBin = 0;
    else if (ele2pt < 40) Ele2PtBin = 1;
    else if (ele2pt < 50) Ele2PtBin = 2;
    else Ele2PtBin = 3;
    if (fabs(zeeTree->l1sceta) < 1.479) Ele1EtaBin = 0;
    else Ele1EtaBin = 1;
    if (fabs(zeeTree->l2sceta) < 1.479) Ele2EtaBin = 0;
    else Ele2EtaBin = 1;
    if (zeeTree->l1r9 > 0.94) Ele1R9Bin = 0;
    else Ele1R9Bin = 1;
    if (zeeTree->l2r9 > 0.94) Ele2R9Bin = 0;
    else Ele2R9Bin = 1;

    if (!(Ele1PtBin == ptBin || Ele2PtBin == ptBin)) continue; 
    if (!(Ele1EtaBin == etaBin && Ele2EtaBin == etaBin)) continue; 
    if (!(Ele1R9Bin == R9Bin && Ele2R9Bin == R9Bin)) continue; 
    
    //*************************************************************************
    // restrict range of mass
    //*************************************************************************
    double zMass = (ele1FourVector+ele2FourVector).M();
    if (zMass < minMass || zMass > maxMass) continue;

    //*************************************************************************
    //set mass variable
    //*************************************************************************
    zMassArgSet.setRealValue("zmass", zMass);    
    
    data->add(zMassArgSet,puW);
  }
  cout << "data->isWeighted() = " << data->isWeighted() << endl;

  // do binned fit to gain time...
  mass.setBins(nbins);
  RooDataHist *bdata = new RooDataHist("data_binned","data_binned", zMassArgSet, *data);
  cout << "bdata->isWeighted() = " << bdata->isWeighted() << endl;

  cout << "dataset size: " << data->numEntries() << endl;

//   // Closing file
//   treeFile->Close();
  //====================== Parameters===========================

  //Crystal Ball parameters
//   RooRealVar cbBias ("#Deltam_{CB}", "CB Bias", -.01, -10, 10, "GeV/c^{2}");
//   RooRealVar cbSigma("sigma_{CB}", "CB Width", 1.7, 0.8, 5.0, "GeV/c^{2}");
//   RooRealVar cbCut  ("a_{CB}","CB Cut", 1.05, 1.0, 3.0);
//   RooRealVar cbPower("n_{CB}","CB Order", 2.45, 0.1, 20.0);
  RooRealVar cbBias ("#Deltam_{CB}", "CB Bias", -.01, -10, 10, "GeV/c^{2}");
  RooRealVar cbSigma("#sigma_{CB}", "CB Width", 1.5, 0.8, 5.0, "GeV/c^{2}");
  RooRealVar cbCut  ("a_{CB}","CB Cut", 1.0, 1.0, 3.0);
  RooRealVar cbPower("n_{CB}","CB Order", 2.5, 0.1, 20.0);
  cbCut.setVal(cutoff_cb);
  cbPower.setVal(power_cb);
  //  if(!isMC && !(ptBin==0 && etaBin==1 && R9Bin==0)) {
  //    cbCut.setConstant(kTRUE);
  //    cbPower.setConstant(kTRUE);
  //  }

  // Just checking
  //cbCut.Print();
  //cbPower.Print();

  //Breit_Wigner parameters
  RooRealVar bwMean("m_{Z}","BW Mean", 91.1876, "GeV/c^{2}");
  bwMean.setVal(mean_bw);
  RooRealVar bwWidth("#Gamma_{Z}", "BW Width", 2.4952, "GeV/c^{2}");
  bwWidth.setVal(gamma_bw);

  // Fix the Breit-Wigner parameters to PDG values
  bwMean.setConstant(kTRUE);
  bwWidth.setConstant(kTRUE);

  // Exponential Background parameters
  RooRealVar expRate("#lambda_{exp}", "Exponential Rate", -0.064, -1, 1);
  RooRealVar c0("c_{0}", "c0", 1., 0., 50.);

  //Number of Signal and Background events
  RooRealVar nsig("N_{S}", "# signal events", 524, 0.1, 10000000000.);
  RooRealVar nbkg("N_{B}", "# background events", 43, 1., 10000000.);

  //============================ P.D.F.s=============================

  // Mass signal for two decay electrons p.d.f.
  RooBreitWigner bw("bw", "bw", mass, bwMean, bwWidth);
  RooCBShape  cball("cball", "Crystal Ball", mass, cbBias, cbSigma, cbCut, cbPower);
  RooFFTConvPdf BWxCB("BWxCB", "bw X crystal ball", mass, bw, cball);

  // Mass background p.d.f.
  RooExponential bg("bg", "exp. background", mass, expRate);

  // Mass model for signal electrons p.d.f.
  RooAddPdf model("model", "signal", RooArgList(BWxCB), RooArgList(nsig));

  TStopwatch t ;
  t.Start() ;
  RooFitResult *fitres = model.fitTo(*bdata,Hesse(1),Minos(1),Timer(1),Save(1));
  fitres->SetName("fitres");
  t.Print() ;

  TCanvas* c = new TCanvas("c","Unbinned Invariant Mass Fit", 0,0,800,600);

  //========================== Plotting  ============================
  //Create a frame
  RooPlot* plot = mass.frame(Range(minMass,maxMass),Bins(nbins));
  // Add data and model to canvas
  int col = (isMC ? kAzure+4 : kGreen+1);
  data->plotOn(plot);
  model.plotOn(plot,LineColor(col));
  data->plotOn(plot);
  model.paramOn(plot, Format(plotOpt, AutoPrecision(1)), Parameters(RooArgSet(cbBias, cbSigma, cbCut, cbPower, bwMean, bwWidth, expRate, nsig, nbkg)), Layout(0.15,0.45,0.80));
  plot->getAttText()->SetTextSize(.03);
  plot->SetTitle("");
  plot->Draw();

  // Print Fit Values
  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(.1);
  tex->SetTextFont(132);
  //  tex->Draw();
  tex->SetTextSize(0.057);
  if(isMC) tex->DrawLatex(0.65, 0.75, "Z #rightarrow e^{+}e^{-} MC");
  else tex->DrawLatex(0.65, 0.75, "Z #rightarrow e^{+}e^{-} data");
  tex->SetTextSize(0.030);
  tex->DrawLatex(0.645, 0.65, Form("BW Mean = %.2f GeV/c^{2}", bwMean.getVal()));
  tex->DrawLatex(0.645, 0.60, Form("BW #sigma = %.2f GeV/c^{2}", bwWidth.getVal()));
  c->Update();
  c->SaveAs((outFilename + ".pdf").c_str());
  c->SaveAs((outFilename + ".png").c_str());

  // tablefile << Form(Outfile + "& $ %f $ & $ %f $ & $ %f $\\ \hline",cbBias.getVal(), cbSigma.getVal(), cbCut.getVal());
  // Output workspace with model and data

  RooWorkspace *w = new RooWorkspace("ZeeMassScaleAndResolutionFit");
  w->import(model);
  w->import(*bdata);
  w->writeToFile((outFilename + ".root").c_str());  

  TFile *tfileo = TFile::Open((outFilename + ".root").c_str(),"update");
  fitres->Write();
  tfileo->Close();

}



void FitJPsiMassScaleAndResolution(string inputFilename, string outFilename, Int_t ptBin, Int_t etaBin, Int_t isMC) {

  // Define Fit Inputs and Call Fit
  double minMass = 2.6;
  double maxMass = 3.6;
  double mean_bw = 3.096916;
  double gamma_bw = 92.9e-6;
  double cutoff_cb = 1.8;
  double power_cb = 2.0;
  const char *plotOpt = "NEU";
  int nbins = 50;
  if(ptBin==2) nbins = 60;
  // very low stat in data
  if(ptBin==3) nbins = (isMC) ? 25 : 22;

  // Call the fitting program and output a workspace with a root file
  // of the model and data as well as a pdf of the fit
  makejpsifit(inputFilename, outFilename, ptBin, etaBin, minMass,  maxMass,  mean_bw,  gamma_bw,  cutoff_cb, power_cb, plotOpt, nbins, isMC);

}
//______________________________________________________________


void makejpsifit(string inputFilename, string outFilename, 
		 Int_t ptBin, Int_t etaBin,
		 double minMass, double maxMass, 
		 double mean_bw, double gamma_bw, double cutoff_cb, double power_cb, 
		 const char* plotOpt, const int nbins, Int_t isMC) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  //Create Data Set
  RooRealVar mass("zmass","m(e^{+}e^{-})",minMass,maxMass,"GeV/c^{2}");

  // Reading everything from root tree instead
  TFile *tfile = TFile::Open(inputFilename.c_str());
  TTree *ttree = (TTree*)tfile->Get("zeetree/probe_tree");
  hzztree *zeeTree = new hzztree(ttree);

  RooArgSet zMassArgSet(mass);
  RooDataSet* data = new RooDataSet("data", "ntuple parameters", zMassArgSet);

  for (int i = 0; i < zeeTree->fChain->GetEntries(); i++) {
    if(i%100000==0) cout << "Processing Event " << i << endl;
    zeeTree->fChain->GetEntry(i);

    //*************************************************************************
    //Electron Selection
    //*************************************************************************
    // already passed for this tree

    //*************************************************************************
    //Compute electron four vector;
    //*************************************************************************
    double ele1pt = zeeTree->l1pt;
    double ele2pt = zeeTree->l2pt;

    double ELECTRONMASS = 0.51e-3;
    TLorentzVector ele1FourVector;
    ele1FourVector.SetPtEtaPhiM(zeeTree->l1pt, zeeTree->l1eta, zeeTree->l1phi, ELECTRONMASS);
    TLorentzVector ele2FourVector;
    ele2FourVector.SetPtEtaPhiM(zeeTree->l2pt, zeeTree->l2eta, zeeTree->l2phi, ELECTRONMASS);

    
    //*************************************************************************
    //pt and eta cuts on electron
    //*************************************************************************
    if (! (ele1pt > 7 && ele2pt > 7
           && fabs( zeeTree->l1eta) < 2.5 
           && fabs( zeeTree->l2eta) < 2.5 )) continue;

    //*************************************************************************
    //pt bins and eta bins
    //*************************************************************************
    Int_t Ele1PtBin = -1;
    Int_t Ele1EtaBin = -1;
    Int_t Ele2PtBin = -1;
    Int_t Ele2EtaBin = -1;
    if (ele1pt > 7 && ele1pt < 10) Ele1PtBin = 0;
    else if (ele1pt < 20) Ele1PtBin = 1;
    else Ele1PtBin = 2;
    if (ele2pt > 7 && ele2pt < 10) Ele2PtBin = 0;
    else if (ele2pt < 20) Ele2PtBin = 1;
    else Ele2PtBin = 2;
    if (fabs(zeeTree->l1sceta) < 1.479) Ele1EtaBin = 0;
    else Ele1EtaBin = 1;
    if (fabs(zeeTree->l2sceta) < 1.479) Ele2EtaBin = 0;
    else Ele2EtaBin = 1;

    if (!(Ele1PtBin == ptBin || Ele2PtBin == ptBin)) continue; 
    if (!(Ele1EtaBin == etaBin && Ele2EtaBin == etaBin)) continue; 
    
    //*************************************************************************
    // restrict range of mass
    //*************************************************************************
    double zMass = (ele1FourVector+ele2FourVector).M();
    if (zMass < minMass || zMass > maxMass) continue;

    //*************************************************************************
    //set mass variable
    //*************************************************************************
    zMassArgSet.setRealValue("zmass", zMass);    

    data->add(zMassArgSet);
  }

  // do binned fit to gain time...
  mass.setBins(nbins);
  RooDataHist *bdata = new RooDataHist("data_binned","data_binned", zMassArgSet, *data);

  cout << "dataset size: " << data->numEntries() << endl;

//   // Closing file
//   treeFile->Close();
  //====================== Parameters===========================

  //Crystal Ball parameters
//   RooRealVar cbBias ("#Deltam_{CB}", "CB Bias", -.01, -10, 10, "GeV/c^{2}");
//   RooRealVar cbSigma("sigma_{CB}", "CB Width", 1.7, 0.8, 5.0, "GeV/c^{2}");
//   RooRealVar cbCut  ("a_{CB}","CB Cut", 1.05, 1.0, 3.0);
//   RooRealVar cbPower("n_{CB}","CB Order", 2.45, 0.1, 20.0);
  RooRealVar cbBias ("#Deltam_{CB}", "CB Bias", -.01, -10, 10, "GeV/c^{2}");
  RooRealVar cbSigma("#sigma_{CB}", "CB Width", 1.5, 0.01, 5.0, "GeV/c^{2}");
  RooRealVar cbCut  ("a_{CB}","CB Cut", 1.0, 1.0, 3.0);
  RooRealVar cbPower("n_{CB}","CB Order", 2.5, 0.1, 20.0);
  cbCut.setVal(cutoff_cb);
  cbPower.setVal(power_cb);

  // Just checking
  //cbCut.Print();
  //cbPower.Print();

  //Breit_Wigner parameters
  RooRealVar bwMean("m_{JPsi}","BW Mean", 3.096916, "GeV/c^{2}");
  bwMean.setVal(mean_bw);
  RooRealVar bwWidth("#Gamma_{JPsi}", "BW Width", 92.9e-6, "GeV/c^{2}");
  bwWidth.setVal(gamma_bw);

  // Fix the Breit-Wigner parameters to PDG values
  bwMean.setConstant(kTRUE);
  bwWidth.setConstant(kTRUE);

  // Exponential Background parameters
  RooRealVar expRate("#lambda_{exp}", "Exponential Rate", -0.064, -1, 1);
  RooRealVar c0("c_{0}", "c0", 1., 0., 50.);

  //Number of Signal and Background events
  RooRealVar nsig("N_{S}", "# signal events", 524, 0.1, 10000000000.);
  RooRealVar nbkg("N_{B}", "# background events", 43, 1., 10000000.);

  //============================ P.D.F.s=============================

  // Mass signal for two decay electrons p.d.f.
  RooBreitWigner bw("bw", "bw", mass, bwMean, bwWidth);
  RooCBShape  cball("cball", "Crystal Ball", mass, cbBias, cbSigma, cbCut, cbPower);
  RooFFTConvPdf BWxCB("BWxCB", "bw X crystal ball", mass, bw, cball);

  // Mass background p.d.f.
  RooExponential bg("bg", "exp. background", mass, expRate);

  // Mass model for signal electrons p.d.f.
  RooAddPdf model("model", "signal", RooArgList(BWxCB), RooArgList(nsig));

  TStopwatch t ;
  t.Start() ;
  double fitmin, fitmax;
  if(isMC) {
    fitmin = (etaBin==0) ? 3.00 : 2.7;
    fitmax = (etaBin==0) ? 3.20 : 3.4;
  } else {
    fitmin = (etaBin==0) ? ( (ptBin>=2) ? 3.01 : 3.02 ) : 2.7;
    fitmax = (etaBin==0) ? ( (ptBin==3) ? 3.23 : 3.22 ) : 3.4;
  }
  RooFitResult *fitres = model.fitTo(*bdata,Range(fitmin,fitmax),Hesse(1),Minos(1),Timer(1),Save(1));
  fitres->SetName("fitres");
  t.Print() ;

  TCanvas* c = new TCanvas("c","Unbinned Invariant Mass Fit", 0,0,800,600);

  //========================== Plotting  ============================
  //Create a frame
  RooPlot* plot = mass.frame(Range(minMass,maxMass),Bins(nbins));
  // Add data and model to canvas
  int col = (isMC ? kAzure+4 : kGreen+1);
  data->plotOn(plot);
  model.plotOn(plot,LineColor(col));
  data->plotOn(plot);
  model.paramOn(plot, Format(plotOpt, AutoPrecision(1)), Parameters(RooArgSet(cbBias, cbSigma, cbCut, cbPower, bwMean, bwWidth, expRate, nsig, nbkg)), Layout(0.15,0.45,0.80));
  plot->getAttText()->SetTextSize(.03);
  plot->SetTitle("");
  plot->Draw();

  // Print Fit Values
  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(.1);
  tex->SetTextFont(132);
  //  tex->Draw();
  tex->SetTextSize(0.057);
  if(isMC) tex->DrawLatex(0.65, 0.75, "J/#psi #rightarrow e^{+}e^{-} MC");
  else tex->DrawLatex(0.65, 0.75, "J/#psi #rightarrow e^{+}e^{-} data");
  tex->SetTextSize(0.030);
  tex->DrawLatex(0.645, 0.65, Form("BW Mean = %.2f GeV/c^{2}", bwMean.getVal()));
  tex->DrawLatex(0.645, 0.60, Form("BW #sigma = %.2f GeV/c^{2}", bwWidth.getVal()));
  c->Update();
  c->SaveAs((outFilename + ".pdf").c_str());
  c->SaveAs((outFilename + ".png").c_str());

  // tablefile << Form(Outfile + "& $ %f $ & $ %f $ & $ %f $\\ \hline",cbBias.getVal(), cbSigma.getVal(), cbCut.getVal());
  // Output workspace with model and data

  RooWorkspace *w = new RooWorkspace("JPsieeMassScaleAndResolutionFit");
  w->import(model);
  w->import(*bdata);
  w->writeToFile((outFilename + ".root").c_str());  

  TFile *tfileo = TFile::Open((outFilename + ".root").c_str(),"update");
  fitres->Write();
  tfileo->Close();

}


void plotResolution() {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  double binedgesZ[5] = {20,30,40,50,70};
  double binedgesJPsi[4] = {7,10,20,40};
  TGraphAsymmErrors gScaleZ[2][2];
  TGraphAsymmErrors gResoZ[2][2];
  TGraphAsymmErrors gScaleJPsi[2];
  TGraphAsymmErrors gResoJPsi[2];

  // Z->ee
  for(int ipt=0; ipt<4; ++ipt) {
    for(int ieta=0; ieta<2; ++ieta) {
      for(int ir9=0; ir9<2; ++ir9) {

	cout << "Analyzing pt bin = " << ipt << ", eta bin: " << ieta << "  and r9 bin: " << ir9 << endl;

	stringstream mcfile, datafile;
	mcfile << "mcZ2012_PtBin" << ipt << "_EtaBin" << ieta << "_R9Bin" << ir9 << ".root";
	datafile << "dataZ2012_PtBin" << ipt << "_EtaBin" << ieta << "_R9Bin" << ir9 << ".root";
    
	TFile *tmcfile = TFile::Open(mcfile.str().c_str());
	RooFitResult *mcfr = (RooFitResult*)tmcfile->Get("fitres");
	float mcDM = ((RooRealVar*)(mcfr->floatParsFinal().find("#Deltam_{CB}")))->getVal();
	float mcDM_err = ((RooRealVar*)(mcfr->floatParsFinal().find("#Deltam_{CB}")))->getError();
	float mcS = ((RooRealVar*)(mcfr->floatParsFinal().find("#sigma_{CB}")))->getVal();
	float mcS_err = ((RooRealVar*)(mcfr->floatParsFinal().find("#sigma_{CB}")))->getError();

	TFile *tdatafile = TFile::Open(datafile.str().c_str());
	RooFitResult *datafr = (RooFitResult*)tdatafile->Get("fitres");
	float dataDM = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getVal();
	float dataDM_err = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getError();
	float dataS = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getVal();
	float dataS_err = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getError();
	
	float rM = (dataDM-mcDM)/(dataDM + 91.19);
	float rM_err = rM * sqrt(dataDM_err*dataDM_err + mcDM_err*mcDM_err);
	float delta = dataS-mcS;
	float delta_err = sqrt(dataS_err*dataS_err+mcS_err*mcS_err);
	float rS = delta/(mcS);
	cout << "rS = " << rS << endl;
	float rS_err = fabs(rS) * sqrt(pow(delta_err/delta,2) + pow(mcS_err/mcS,2));
	
	float bincenter=(binedgesZ[ipt+1]+binedgesZ[ipt])/2.;
	// add some offset not to overlap points
	bincenter += (ieta==0) ? 2. : -2.;
	bincenter += (ir9==0) ? 1. : -1.;
	
	float binerrup=binedgesZ[ipt+1]-bincenter;
	float binerrdn=bincenter-binedgesZ[ipt];

	gScaleZ[ieta][ir9].SetPoint(ipt,bincenter,rM);
	gScaleZ[ieta][ir9].SetPointError(ipt,binerrdn,binerrup,rM_err,rM_err);
	gResoZ[ieta][ir9].SetPoint(ipt,bincenter,rS);
	gResoZ[ieta][ir9].SetPointError(ipt,binerrdn,binerrup,rS_err,rS_err);
      }
    }
  }

  // JPsi->ee
  for(int ipt=0; ipt<3; ++ipt) {
    for(int ieta=0; ieta<1; ++ieta) {

	cout << "Analyzing pt bin = " << ipt << ", eta bin: " << ieta << endl;

	stringstream mcfile, datafile;
	mcfile << "mcJPsi2012_PtBin" << ipt << "_EtaBin" << ieta << ".root";
	datafile << "dataJPsi2012_PtBin" << ipt << "_EtaBin" << ieta << ".root";
    
	TFile *tmcfile = TFile::Open(mcfile.str().c_str());
	RooFitResult *mcfr = (RooFitResult*)tmcfile->Get("fitres");
	float mcDM = ((RooRealVar*)(mcfr->floatParsFinal().find("#Deltam_{CB}")))->getVal();
	float mcDM_err = ((RooRealVar*)(mcfr->floatParsFinal().find("#Deltam_{CB}")))->getError();
	float mcS = ((RooRealVar*)(mcfr->floatParsFinal().find("#sigma_{CB}")))->getVal();
	float mcS_err = ((RooRealVar*)(mcfr->floatParsFinal().find("#sigma_{CB}")))->getError();

	TFile *tdatafile = TFile::Open(datafile.str().c_str());
	RooFitResult *datafr = (RooFitResult*)tdatafile->Get("fitres");
	float dataDM = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getVal();
	float dataDM_err = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getError();
	float dataS = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getVal();
	float dataS_err = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getError();
	
	float rM = (dataDM-mcDM)/(dataDM + 3.096);
	float rM_err = rM * sqrt(dataDM_err*dataDM_err + mcDM_err*mcDM_err);
	float delta = dataS-mcS;
	float delta_err = sqrt(dataS_err*dataS_err+mcS_err*mcS_err);
	float rS = delta/(mcS);
	cout << "rS = " << rS << endl;
	float rS_err = fabs(rS) * sqrt(pow(delta_err/delta,2) + pow(mcS_err/mcS,2));
	
	float bincenter=(binedgesJPsi[ipt+1]+binedgesJPsi[ipt])/2.;
	// add some offset not to overlap points
	bincenter += (ieta==0) ? 0.5 : -0.5;
	
	std::cout << "bincenter = " << bincenter << std::endl;

	float binerrup=binedgesJPsi[ipt+1]-bincenter;
	float binerrdn=bincenter-binedgesJPsi[ipt];

	gScaleJPsi[ieta].SetPoint(ipt,bincenter,rM);
	gScaleJPsi[ieta].SetPointError(ipt,binerrdn,binerrup,rM_err,rM_err);
	gResoJPsi[ieta].SetPoint(ipt,bincenter,rS);
	gResoJPsi[ieta].SetPointError(ipt,binerrdn,binerrup,rS_err,rS_err);
      }
  }
  
  
  TCanvas *c1 = new TCanvas("c1","",600,600);
  
  gScaleZ[0][0].GetXaxis()->SetTitle("electron p_{T} (GeV)");
  gScaleZ[0][0].GetYaxis()->SetTitle("#Delta m/m (data - sim.)");
  gScaleZ[0][0].Draw("AP");
  for(int ieta=0; ieta<2; ++ieta) {
    for(int ir9=0; ir9<2; ++ir9) {
      if(ieta==0 && ir9==0) gScaleZ[0][0].Draw("AP");
      else gScaleZ[ieta][ir9].Draw("P");
    }
  }
  c1->SaveAs("delta-scale.pdf");
  c1->SaveAs("delta-scale.png");

  TCanvas *c2 = new TCanvas("c2","",600,600);
  c2->SetLeftMargin(0.20);
  c2->SetBottomMargin(0.20); 
  c2->SetTopMargin(0.10); 
  gResoZ[0][0].GetXaxis()->SetTitle("electron p_{T} (GeV)");
  gResoZ[0][0].GetYaxis()->SetTitle("(#sigma^{data}_{eff}-#sigma^{MC}_{eff})/#sigma^{MC}_{eff}");
  gResoZ[0][0].GetYaxis()->SetTitleOffset(1.8);
  gResoZ[0][0].GetXaxis()->SetTitleOffset(1.5);
  
  for(int ieta=0; ieta<2; ++ieta) {
    for(int ir9=0; ir9<2; ++ir9) {
      gResoZ[ieta][ir9].SetMarkerSize(1.);
      gResoZ[ieta][ir9].GetYaxis()->SetRangeUser(-0.30,0.30);
      gResoZ[ieta][ir9].GetXaxis()->SetLimits(0,80);

      if(ieta==0) { 
	gResoZ[ieta][ir9].SetMarkerColor(kAzure-3);
	gResoZ[ieta][ir9].SetLineColor(kAzure-3);
      }
      if(ieta==1) {
	gResoZ[ieta][ir9].SetMarkerColor(kGreen-3);
	gResoZ[ieta][ir9].SetLineColor(kGreen-3);
      }
      if(ir9==0) gResoZ[ieta][ir9].SetMarkerStyle(kFullCircle);
      if(ir9==1) gResoZ[ieta][ir9].SetMarkerStyle(kOpenTriangleUp);

      if(ieta==0 && ir9==0) gResoZ[0][0].Draw("AP");
      else gResoZ[ieta][ir9].Draw("P");
    }
  }

  gResoJPsi[0].SetMarkerColor(kRed-3);
  gResoJPsi[0].SetLineColor(kRed-3);
  gResoJPsi[0].SetMarkerSize(1.);
  gResoJPsi[0].SetMarkerStyle(kOpenSquare);
  gResoJPsi[0].GetYaxis()->SetRangeUser(-0.30,0.30);
  gResoJPsi[0].GetXaxis()->SetLimits(0.,80);
  gResoJPsi[0].Draw("P");


  TLegend* leg = new TLegend(0.50,0.20,0.90,0.50);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetTextFont(42);
  leg->SetFillColor(0);
  leg->AddEntry(&gResoZ[0][0],"Z,    |#eta|<1.5, golden","pl");
  leg->AddEntry(&gResoZ[0][1],"Z,    |#eta|<1.5, showering","pl");
  leg->AddEntry(&gResoZ[1][0],"Z,    |#eta|>1.5, golden","pl");
  leg->AddEntry(&gResoZ[1][1],"Z,    |#eta|>1.5, showering","pl");
  leg->AddEntry(&gResoJPsi[0],"J/#psi , |#eta|<1.5","pl");
  leg->Draw();

  TPaveText *pt = new TPaveText(-3,0.32,70,0.35,"br");
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextAlign(12);
   pt->SetTextFont(42);
   pt->SetTextSize(0.04);
   pt->AddText("CMS, #sqrt{s} = 8 TeV,              #intL dt = 19.6 fb^{-1}");
   pt->Draw();

   TLine *zero = new TLine(0,0,80,0);
   zero->SetLineColor(kGray+2);
   zero->SetLineWidth(1);
   zero->Draw();

   c2->SaveAs("delta-resolution.pdf");
   c2->SaveAs("delta-resolution.png");
   c2->SaveAs("delta-resolution.C");

}
