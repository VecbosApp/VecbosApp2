#include <iostream>
#include <sstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TPaveText.h"

#include "../../Tools/src/RooHZZStyle.C"

void PlotSpectrum() {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  TFile *file = TFile::Open("/Users/emanuele/Work/data/egamma/jan22/hzzTree_dataZee_2012_Jan22Rereco.root");
  TTree *tree = (TTree*)file->Get("zeetree/probe_tree");
 
  const Int_t nbins = 1000;
  Double_t xmin = 1;
  Double_t xmax = 1e3;
  Double_t logxmin = TMath::Log10(xmin);
  Double_t logxmax = TMath::Log10(xmax);
  Double_t binwidth = (logxmax-logxmin)/nbins;
  Double_t xbins[nbins+1];
  xbins[0] = xmin;
  for (Int_t i=1;i<=nbins;i++) {
    xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
  }

  TFile *fileo = TFile::Open("spectrum.root","recreate");
  TH1F *spectrum = new TH1F("spectrum","",nbins,xbins);
  spectrum->SetLineColor(kBlack);
  

  Float_t zmass;
  tree->SetBranchAddress("zmass",&zmass);
  for(int j=0; j<tree->GetEntries(); ++j) {
    tree->GetEntry(j);
    if(zmass<xbins[1] || zmass>xbins[nbins]) continue;
    if(j%1000000==0) std::cout << "Processing event " << j << std::endl;
    spectrum->Fill(zmass);
  }


  TCanvas *can = new TCanvas("can","",1200,600);
  can->SetLogy();
  can->SetLogx();
  spectrum->Draw();
  can->SaveAs("spectrum_raw.pdf");

  fileo->cd();
  spectrum->Write();
  fileo->Close();
  
}

void refinePlot() {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  TFile *file = TFile::Open("spectrum.root");
  TH1F *spectrum = (TH1F*)file->Get("spectrum");
  spectrum->SetFillColor(kGray);
  spectrum->SetLineColor(kBlack);
  spectrum->SetLineWidth(0);

  spectrum->GetXaxis()->SetTitle("dielectron mass (GeV/c^{2})");
  spectrum->GetXaxis()->SetRangeUser(2.1,1e3);
  spectrum->GetYaxis()->SetTitle("Events");
  spectrum->GetYaxis()->SetRangeUser(1,1e7);

  TPaveText *lumi = new TPaveText(200,1e4,800,1e6);
  //  lumi->AddText("CMS, #sqrt{s} = 8 TeV, #int L dt = 19.6 fb^{-1}");
  lumi->AddText("CMS, #sqrt{s} = 8 TeV");
  lumi->AddText("#int L dt = 19.6 fb^{-1}");
  lumi->SetBorderSize(0);
  lumi->SetFillStyle(0);
  lumi->SetTextAlign(12);
  lumi->SetTextFont(42);
  lumi->SetTextSize(0.04);

  TPaveText *Zlabel = new TPaveText(110,5e5,150,1e6);
  Zlabel->AddText("Z");
  Zlabel->SetBorderSize(0);
  Zlabel->SetFillStyle(0);
  Zlabel->SetTextAlign(12);
  Zlabel->SetTextFont(42);
  Zlabel->SetTextSize(0.04);

  TPaveText *Y1Slabel = new TPaveText(6,2e3,8,1e4);
  Y1Slabel->AddText("#Upsilon(1S)");
  Y1Slabel->SetBorderSize(0);
  Y1Slabel->SetFillStyle(0);
  Y1Slabel->SetTextAlign(12);
  Y1Slabel->SetTextFont(42);
  Y1Slabel->SetTextSize(0.04);

  TPaveText *YnSlabel = new TPaveText(11,7e2,13,1e4);
  YnSlabel->AddText("#Upsilon(2S,3S)");
  YnSlabel->SetBorderSize(0);
  YnSlabel->SetFillStyle(0);
  YnSlabel->SetTextAlign(12);
  YnSlabel->SetTextFont(42);
  YnSlabel->SetTextSize(0.04);

  TPaveText *Psi2Slabel = new TPaveText(3.5,60,5,1e2);
  Psi2Slabel->AddText("#psi(2S)");
  Psi2Slabel->SetBorderSize(0);
  Psi2Slabel->SetFillStyle(0);
  Psi2Slabel->SetTextAlign(12);
  Psi2Slabel->SetTextFont(42);
  Psi2Slabel->SetTextSize(0.04);

  TPaveText *Jlabel = new TPaveText(3,1e2,5,1e3);
  Jlabel->AddText("J/#psi");
  Jlabel->SetBorderSize(0);
  Jlabel->SetFillStyle(0);
  Jlabel->SetTextAlign(12);
  Jlabel->SetTextFont(42);
  Jlabel->SetTextSize(0.04);

  TCanvas *can = new TCanvas("can","",1200,600);
  can->SetLogy();
  can->SetLogx();
  spectrum->Draw();
  lumi->Draw();
  Zlabel->Draw();
  Y1Slabel->Draw();
  YnSlabel->Draw();
  Psi2Slabel->Draw();
  Jlabel->Draw();
  can->SaveAs("spectrum_refined.pdf");
  can->SaveAs("spectrum_refined.png");
  can->SaveAs("spectrum_refined.C");
  can->SaveAs("spectrum_refined.root");

  TFile *fileh = TFile::Open("spectrum_refined.root","update");
  spectrum->Write();
  fileh->Close();

}
