#include <iostream>
#include <vector>

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TString.h>
#include <TLegend.h>
#include <TPaveText.h>

#include "../../Tools/src/RooHZZStyle.C"

using namespace std;

void drawOneComparison(vector<TH1F*> histos, vector<TString> descr, TString xaxislabel, const char *filename, TPaveText *t=0) {

  if(histos.size()>4) {
    cout << "more than 4 histos not implemented." << endl;
    return;
  }

  if(histos.size()!=descr.size()) {
    cout << "description not complete!" << endl;
    return;
  }

  vector<int> colors;
  colors.push_back(kRed+1);
  colors.push_back(kAzure-6);
  colors.push_back(kTeal+3);
  colors.push_back(kOrange+3);

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);

  c1->SetGridx();
  c1->SetGridy();  
  TLegend* legend = new TLegend(0.20, 0.70, 0.43, 0.90);
  legend->SetBorderSize(   0);
  legend->SetFillColor (   0);
  legend->SetTextAlign (  12);
  legend->SetTextFont  (  42);
  legend->SetTextSize  (0.05);

  for(int i=0;i<(int)histos.size();++i) {
    
    histos[i]->SetMinimum(0);
    histos[i]->SetMaximum(0.2);
    histos[i]->SetMarkerSize(2);
    histos[i]->SetMarkerStyle(20);
    histos[i]->SetMarkerColor(colors[i]);
    histos[i]->SetLineColor(colors[i]);
    histos[i]->SetTitle("");
    if(TString(histos[i]->GetName()).Contains("PU")) {
      histos[i]->SetMaximum(0.6);
      histos[i]->Fit("pol1","","same",1,35);
      histos[i]->GetFunction("pol1")->SetLineColor(colors[i]);
    }
    histos[i]->GetXaxis()->SetTitle(xaxislabel);
    histos[i]->GetYaxis()->SetTitle("Reco #rightarrow ID efficiency");
    histos[i]->GetYaxis()->SetTitleOffset(1.5);

    legend->AddEntry(histos[i],descr[i]);

    if(i==0) { 
      histos[i]->Draw("pe1");
      for(int bin=1;bin<=histos[i]->GetNbinsX();bin++) {
	cout << "fakeRate[" << bin << "] = " << histos[i]->GetBinContent(bin) << ";" << endl;
      }
      for(int bin=1;bin<=histos[i]->GetNbinsX();bin++) {
	cout << "fakeRate_err[" << bin << "] = " << histos[i]->GetBinError(bin) << ";" << endl;
      }
    }
    else histos[i]->Draw("same pe1");
  }
  legend->Draw();
  if(t) t->Draw();

  TPaveText *pt = new TPaveText(0.15,0.96,0.89,0.99,"brNDC");
  pt->SetBorderSize(0);
  pt->SetFillStyle(0);
  pt->SetTextAlign(12);
  pt->SetTextSize(0.03);
  pt->SetTextFont(42);
  TText *text = pt->AddText(0.01,0.3,"CMS");
  text = pt->AddText(0.7,0.3,"#sqrt{s} = 8 TeV, L = 19.7 fb^{-1}");
  pt->Draw();
  
  TString basename(filename);
  basename.ReplaceAll("Eff","FR");
  c1->SaveAs(basename+TString(".png"));
  c1->SaveAs(basename+TString(".pdf"));

}

void drawIDCutWPs() {

  TFile *file = TFile::Open("fakerates_nontrg.root");

  /// === pt ===
  /// barrel 
  TH1F *CutsVetoPtBarrel = (TH1F*)file->Get("wp_cuts_veto_PtBarrel_Eff");
  TH1F *CutsLoosePtBarrel = (TH1F*)file->Get("wp_cuts_loose_PtBarrel_Eff");
  TH1F *CutsMediumPtBarrel = (TH1F*)file->Get("wp_cuts_medium_PtBarrel_Eff");
  TH1F *CutsTightPtBarrel = (TH1F*)file->Get("wp_cuts_tight_PtBarrel_Eff");

  /// endcap 
  TH1F *CutsVetoPtEndcap = (TH1F*)file->Get("wp_cuts_veto_PtEndcap_Eff");
  TH1F *CutsLoosePtEndcap = (TH1F*)file->Get("wp_cuts_loose_PtEndcap_Eff");
  TH1F *CutsMediumPtEndcap = (TH1F*)file->Get("wp_cuts_medium_PtEndcap_Eff");
  TH1F *CutsTightPtEndcap = (TH1F*)file->Get("wp_cuts_tight_PtEndcap_Eff");

  vector<TString> labels;
  labels.push_back("cuts veto");
  labels.push_back("cuts loose");
  labels.push_back("cuts medium");
  labels.push_back("cuts tight");


  vector<TH1F*> barrel, endcap;
  barrel.push_back(CutsVetoPtBarrel);
  barrel.push_back(CutsLoosePtBarrel);
  barrel.push_back(CutsMediumPtBarrel);
  barrel.push_back(CutsTightPtBarrel);

  endcap.push_back(CutsVetoPtEndcap);
  endcap.push_back(CutsLoosePtEndcap);
  endcap.push_back(CutsMediumPtEndcap);
  endcap.push_back(CutsTightPtEndcap);

  drawOneComparison(barrel,labels,"p_{T} [GeV]","fr_cuts_barrel");
  drawOneComparison(endcap,labels,"p_{T} [GeV]","fr_cuts_endcap");
  

}


void drawIDmvaWPs() {

  TFile *file = TFile::Open("fakerates_nontrg.root");

  /// === pt ===
  /// barrel 
  TH1F *MvaLoosePtBarrel = (TH1F*)file->Get("wp_mva_loose_PtBarrel_Eff");
  TH1F *MvaTightPtBarrel = (TH1F*)file->Get("wp_mva_tight_PtBarrel_Eff");

  /// endcap 
  TH1F *MvaLoosePtEndcap = (TH1F*)file->Get("wp_mva_loose_PtEndcap_Eff");
  TH1F *MvaTightPtEndcap = (TH1F*)file->Get("wp_mva_tight_PtEndcap_Eff");

  vector<TString> labels;
  labels.push_back("mva loose");
  labels.push_back("mva tight");


  vector<TH1F*> barrel, endcap;
  barrel.push_back(MvaLoosePtBarrel);
  barrel.push_back(MvaTightPtBarrel);

  endcap.push_back(MvaLoosePtEndcap);
  endcap.push_back(MvaTightPtEndcap);

  drawOneComparison(barrel,labels,"p_{T} [GeV]","fr_mva_barrel");
  drawOneComparison(endcap,labels,"p_{T} [GeV]","fr_mva_endcap");
  

}


