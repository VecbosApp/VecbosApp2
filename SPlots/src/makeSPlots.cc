#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TFile.h>
#include <RooDataSet.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <TPaveText.h>

struct fitbinning {
  enum ptBins {kPtLow = 0,
               kPtHigh = 1}; 
  
  enum etaBins {kEB = 0,
                kEE};
};

void drawSPlot(int ptbin, int etabin, const char *varname, 
               const char *axistitle, const char *extracut, int nbins, float min, float max, int logy=0) {

  // Load results file...
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0) ;
  gStyle->SetOptTitle(0) ;

  char cut1[200], cut2[200];
  if(ptbin==fitbinning::kPtLow) {
    sprintf(cut1,"l1pt<15");
    sprintf(cut2,"l2pt<15");
  } else {
    sprintf(cut1,"l1pt>15 && l1pt<20");
    sprintf(cut2,"l2pt>15 && l2pt<20");
  }
  if(etabin==fitbinning::kEB) {
    sprintf(cut1,"%s && abs(l1eta)<1.479",cut1);
    sprintf(cut2,"%s && abs(l2eta)<1.479",cut2);
  } else {
    sprintf(cut1,"%s && abs(l1eta)>1.479",cut1);
    sprintf(cut2,"%s && abs(l2eta)>1.479",cut2);
  }

  char finalcut1[500], finalcut2[200];
  sprintf(finalcut1,"%s && %s",cut1,extracut);
  sprintf(finalcut2,"%s && %s",cut2,extracut);

  // MC
  TFile *fileMC = TFile::Open("zeeMC2012LowPt.root");
  TTree *treeMC = (TTree*)fileMC->Get("zeetree/probe_tree");

  // data with sWeights
  char filesmall1[200];
  sprintf(filesmall1,"sPlots/z_ptbin0_etabin%d.root",etabin);
  TFile *file1 = TFile::Open(filesmall1);
  TTree *tree1 = (TTree*)file1->Get("zeetree/probe_tree");

  char filesmall2[200];
  sprintf(filesmall2,"sPlots/jpsi_etabin%d.root",etabin);
  TFile *file2 = TFile::Open(filesmall2);
  TTree *tree2 = (TTree*)file2->Get("zeetree/probe_tree");

  char filesweight1[200];
  sprintf(filesweight1,"sPlots/sweights_z_ptbin0_etabin%d.root",etabin);
  tree1->AddFriend("sweights=dataset",filesweight1);

  char filesweight2[200];
  sprintf(filesweight2,"sPlots/sweights_jpsi_etabin%d.root",etabin);
  tree2->AddFriend("sweights=dataset",filesweight2);

  char varname1[200], varname2[200];
  sprintf(varname1,"l1%s",varname);
  sprintf(varname2,"l2%s",varname);

  TH1F *signalMC1 = new TH1F(varname1, "", nbins, min, max);
  TH1F *signalMC2 = new TH1F(varname2, "", nbins, min, max);
  treeMC->Project(varname1,varname1,finalcut1);
  treeMC->Project(varname2,varname2,finalcut2);
  signalMC1->Add(signalMC2);

  char splotnamez1[200], splotnamez2[200], splotnamej1[200], splotnamej2[200];
  sprintf(splotnamez1,"%s_sPlotSigZ",varname1);
  sprintf(splotnamez2,"%s_sPlotSigZ",varname2);
  TH1F *signalsPlotZ1 = new TH1F(splotnamez1,"", nbins, min, max);
  TH1F *signalsPlotZ2 = new TH1F(splotnamez2,"", nbins, min, max);

  sprintf(splotnamej1,"%s_sPlotSigJ",varname1);
  sprintf(splotnamez2,"%s_sPlotSigJ",varname2);
  TH1F *signalsPlotJ1 = new TH1F(splotnamej1,"", nbins, min, max);
  TH1F *signalsPlotJ2 = new TH1F(splotnamej2,"", nbins, min, max);

  signalsPlotZ1->Sumw2(); signalsPlotZ2->Sumw2();  
  signalsPlotJ1->Sumw2(); signalsPlotJ2->Sumw2();  
  
  char finalcutweight1[600], finalcutweight2[600];
  sprintf(finalcutweight1,"(%s)*N_sig_sw",finalcut1);   sprintf(finalcutweight2,"(%s)*N_sig_sw",finalcut2);
  cout << "finalcutweight1 = " << finalcutweight1 << endl;
  cout << "finalcutweight2 = " << finalcutweight2 << endl;
  tree1->Project(splotnamez1,varname1,finalcutweight1);   tree1->Project(splotnamez2,varname2,finalcutweight2);
  tree2->Project(splotnamej1,varname1,finalcutweight1);   tree2->Project(splotnamej2,varname2,finalcutweight2);

  signalsPlotZ1->Add(signalsPlotZ2);
  signalsPlotJ1->Add(signalsPlotJ2);

  // sPlot has the correct normalization. Normalize the MC to this area
  float integral1 = signalsPlotZ1->Integral();
  float integral2 = signalsPlotJ1->Integral();
  float integralMC = signalMC1->Integral();
  cout << integralMC << "  " << integral1 << "   " << integral2 << endl;
  signalMC1->Scale(integral2/integralMC);
  signalsPlotZ1->Scale(integral2/integral1);

  float maxY = TMath::Max(signalsPlotZ1->GetMaximum(),signalsPlotJ1->GetMaximum());
  maxY = TMath::Max(maxY,float(signalMC1->GetMaximum()));
  maxY += maxY*0.15;

  TPaveText *text = new TPaveText(0.60,0.50,0.90,0.70,"brNDC");
  if(ptbin==0) text->AddText("p_{T}<15 GeV");
  else text->AddText("15<p_{T}<20 GeV");
  if(etabin==0) text->AddText("|#eta|<1.479");
  else text->AddText("1.48<|#eta|<2.5");
  text->SetBorderSize(0);
  text->SetFillStyle(0);
  text->SetTextAlign(12);
  text->SetTextFont(132);
  text->SetTextSize(0.04);

  TLegend* leg = new TLegend(0.60,0.70,0.90,0.80);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(signalMC1,"Z(ee) simulation","fl");
  leg->AddEntry(signalsPlotZ1,"Z(ee) sPlot data","pl");
  leg->AddEntry(signalsPlotJ1,"J/#Psi sPlot data","pl");

  TCanvas c1;
  if(logy) c1.SetLogy(1);
  signalMC1->SetLineColor(kOrange+7);
  signalMC1->SetFillColor(kOrange+7);
  signalMC1->SetLineWidth(1.5);
  signalMC1->SetFillStyle(3005);

  signalsPlotZ1->SetMarkerStyle(8);
  signalsPlotZ1->SetMarkerSize(1);
  signalsPlotZ1->GetXaxis()->SetTitle(axistitle);
  signalsPlotZ1->GetYaxis()->SetTitle("weighted events");

  signalsPlotJ1->SetMarkerStyle(kOpenCircle);
  signalsPlotJ1->SetMarkerSize(1);
  signalsPlotJ1->GetXaxis()->SetTitle(axistitle);
  signalsPlotJ1->GetYaxis()->SetTitle("weighted events");

  signalsPlotJ1->SetMaximum(maxY+sqrt(maxY));

  signalsPlotJ1->Draw("pe1");
  signalsPlotZ1->Draw("pe1 same");
  signalMC1->Draw("same hist");
  leg->Draw();
  text->Draw();

  char figfilename[500];
  sprintf(figfilename,"%s_ptbin%d_etabin%d_sPlot.png",varname,ptbin,etabin);
  c1.SaveAs(figfilename);
  sprintf(figfilename,"%s_ptbin%d_etabin%d_sPlot.pdf",varname,ptbin,etabin);
  c1.SaveAs(figfilename);

}
  

void makeSPlots() {

  gSystem->Load("libRooFit");
  
  // barrel
  drawSPlot(0,0,"sietaieta","#sigma_{i#eta i#eta}","1",25,0.0,0.03);
  drawSPlot(0,0,"r9","R_{9}","1",8,0.0,1);
  drawSPlot(0,0,"pt","p_{T} (GeV)","1",10,7.0,10.);
  drawSPlot(0,0,"etawidth","#eta width","1",20,0.0,0.02);
  drawSPlot(0,0,"phiwidth","#phi width","1",10,0.0,0.3);
  drawSPlot(0,0,"hOverE","H/E","1",10,0.0,2);
  drawSPlot(0,0,"sip","S.I.P.","1",10,0.0,3);
  drawSPlot(0,0,"bdtID","identification BDT","1",8,0.0,1);

  drawSPlot(1,0,"sietaieta","#sigma_{i#eta i#eta}","1",20,0.0,0.02);
  drawSPlot(1,0,"r9","R_{9}","1",10,0.0,1);
  drawSPlot(1,0,"pt","p_{T} (GeV)","1",10,7.0,10.);
  drawSPlot(1,0,"etawidth","#eta width","1",20,0.0,0.02);
  drawSPlot(1,0,"phiwidth","#phi width","1",10,0.0,0.3);
  drawSPlot(1,0,"hOverE","H/E","1",10,0.0,2);
  drawSPlot(1,0,"sip","S.I.P.","1",10,0.0,3);
  drawSPlot(1,0,"bdtID","identification BDT","1",15,0.0,1);

//     drawSPlot(1,ieta,"EoP","E/P_{in}","vertices<10",50,0.0,3.0);
//     drawSPlot(1,ieta,"HoE","H/E","vertices<10",50,0.0,0.1,1);
//     drawSPlot(1,ieta,"deta","#Delta #eta_{in}","vertices<10",50,-0.02,0.02);
//     drawSPlot(1,ieta,"dphi","#Delta #phi_{in}","vertices<10",50,-0.1,0.1);
//     drawSPlot(1,ieta,"see","#sigma_{i#eta i#eta}","vertices<10",50,0.0,0.02);
//     //     drawSPlot(1,ieta,"sep","#sigma_{i#eta i#phi}","vertices<10",50,0.0,0.05);
//     //     drawSPlot(1,ieta,"spp","#sigma_{i#phi i#phi}","vertices<10",50,0.0,0.05);
//     drawSPlot(1,ieta,"etawidth","#eta width","vertices<10",50,0.0,0.02);
//     drawSPlot(1,ieta,"phiwidth","#phi width","vertices<10",50,0.0,0.2);
//     drawSPlot(1,ieta,"missHits","miss pix hits","vertices<10",50,0.0,10.0);
//     drawSPlot(1,ieta,"fbrem","f_{brem}","vertices<10",50,-0.2,1.0);
//     drawSPlot(1,ieta,"R9","R9","vertices<10",50,0.0,1.2); 

//     drawSPlot(0,ieta,"EoP","E/P_{in}","vertices<10",20,0.0,3.0);
//     drawSPlot(0,ieta,"HoE","H/E","vertices<10",30,0.0,0.1,1);
//     drawSPlot(0,ieta,"deta","#Delta #eta_{in}","vertices<10",30,-0.02,0.02);
//     drawSPlot(0,ieta,"dphi","#Delta #phi_{in}","vertices<10",15,-0.1,0.1);
//     drawSPlot(0,ieta,"see","#sigma_{i#eta i#eta}","vertices<10",30,0.0,0.05);
//     //     drawSPlot(0,ieta,"sep","#sigma_{i#eta i#phi}","vertices<10",30,0.0,0.05);
//     //     drawSPlot(0,ieta,"spp","#sigma_{i#phi i#phi}","vertices<10",30,0.0,0.05);
//     drawSPlot(0,ieta,"etawidth","#eta width","vertices<10",30,0.0,0.06);
//     drawSPlot(0,ieta,"phiwidth","#phi width","vertices<10",30,0.0,0.2);
//     drawSPlot(0,ieta,"missHits","miss pix hits","vertices<10",10,0.0,10.0);
//     drawSPlot(0,ieta,"fbrem","f_{brem}","vertices<10",20,-0.2,1.0);
//     drawSPlot(0,ieta,"R9","R9","vertices<10",10,0.0,1.2); 

  // endcap
  //  drawSPlot(0,1,"sietaieta","#sigma_{i#eta i#eta}","1",50,0.0,0.05);
  //  drawSPlot(0,1,"pt","p_{T} (GeV)","1",10,7.0,10.);

}

