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

#include "../Tools/src/RooHZZStyle.C"
#include "../Tools/include/LumiReweightingStandAlone.hh"

struct fitbinning {
  enum ptBins {kPtLow = 0,
               kPtHigh = 1}; 
  
  enum etaBins {kEB = 0,
                kEE};
};

using namespace reweight;

void makeFriendTree(bool ismc, bool isrundep=false) {

  std::string file;
  if(ismc) file=std::string("/Users/emanuele/Work/data/egamma/jan22/hzzTree_zee.root");
  else file=std::string("/Users/emanuele/Work/data/egamma/jan22/hzzTree_dataZee_2012_Jan22Rereco.root");
  cout << "===> creating the friend tree for the file " << file << endl;
  
  // 2012 Summer12 MC
  LumiReWeighting LumiWeights( "../Tools/data/pileup/puProfile_Summer12_53X.root",
			       "../Tools/data/pileup/puProfile_Data_8TeV.root",
			       "pileup","pileup");

  // 2012 run-dependent MC
  LumiReWeighting LumiWeights_Runs190456_To_196531( "../Tools/data/pileup/194533-194533-Summer12_DD3_runDep.true.root",
                                                    "../Tools/data/pileup/190456-196531-22Jan_v1.69300.true.root",
                                                    "pileup","pileup");
  
  LumiReWeighting LumiWeights_Runs198022_To_203742( "../Tools/data/pileup/200519-200519-Summer12_DD3_runDep.true.root",
                                                    "../Tools/data/pileup/198022-203742-22Jan_v1.69300.true.root",
                                                    "pileup","pileup");
  
  LumiReWeighting LumiWeights_Runs203777_To_208686( "../Tools/data/pileup/206859-206859-Summer12_DD3_runDep.true.root",
                                                    "../Tools/data/pileup/203777-208686-22Jan_v1.69300.true.root",
                                                    "pileup","pileup");

  // LumiReWeighting ptWeightsZMC( "ptdistr_zee_mc.root", "ptdistr_jpsiee.root", "pt", "pt");
  // LumiReWeighting ptWeightsZDATA( "ptdistr_zee_data.root", "ptdistr_jpsiee.root", "pt", "pt");

  TFile *pF = TFile::Open(file.c_str());
  TTree *pT = (TTree*)pF->Get("zeetree/probe_tree");
  
  TString nF(file);
  nF.ReplaceAll(".root","_friend.root");
  TFile *fF = TFile::Open(nF,"recreate");

  UInt_t run;
  Float_t npu,l1pt,l2pt;
  pT->SetBranchAddress("run", &run);
  pT->SetBranchAddress("l1pt",&l1pt);
  pT->SetBranchAddress("l2pt",&l2pt);
  pT->SetBranchAddress("numTrueInteractions", &npu);

  fF->mkdir("zeetree");
  TTree *fT = new TTree("probe_tree","tree with additional variables");
  Float_t puW;
  Float_t l1ptW, l2ptW;
  fT->Branch("puW", &puW, "puW/F");
  fT->Branch("l1ptW", &l1ptW, "l1ptW/F");
  fT->Branch("l2ptW", &l2ptW, "l2ptW/F");

  for(int i=0; i<pT->GetEntries(); i++) {
    if (i%100000 == 0) std::cout << ">>> Analyzing event # " << i << " / " << pT->GetEntries() << " entries" << std::endl;
     pT->GetEntry(i);

     // this is only needed in MC (rundep MC has 3 runs for 3 periods)
     if(ismc) {
       if(isrundep) {
	 if(run == 194533) puW = LumiWeights_Runs190456_To_196531.weight(npu);
	 else if(run == 200519) puW = LumiWeights_Runs198022_To_203742.weight(npu);
	 else if(run == 206859) puW = LumiWeights_Runs203777_To_208686.weight(npu);
	 else cout << "ERROR! This shoould not happen. Not a run of the run-dependent MC" << endl;
       } else {
	 puW = LumiWeights.weight(npu);
       }
       // remove some unphysical tails
       if(puW > 2) puW = 2.0;
       //       l1ptW = ptWeightsZMC.weight(l1pt);
       //       l2ptW = ptWeightsZMC.weight(l2pt);
     } else {
       puW = 1.;
       //       l1ptW = ptWeightsZDATA.weight(l1pt);
       //       l2ptW = ptWeightsZDATA.weight(l2pt);
     }

     fT->Fill();
  }

  fF->cd("zeetree");
  fT->Write();
  fF->Close();

  cout << "DONE. Friend tree is in file: " << nF.Data() << endl;
}

void drawSPlot(int ptbin, int etabin, const char *varname, 
               const char *axistitle, const char *extracut, int nbins, float min, float max, int logy=0) {

  // Load results file...
  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

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
  TFile *fileMC = TFile::Open("zee_mc_rd1.root");
  TTree *treeMC = (TTree*)fileMC->Get("zeetree/probe_tree");
  treeMC->AddFriend("ptweights=zeetree/probe_tree","zee_mc_rd1_friend.root");

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
  tree1->AddFriend("ptweights=zeetree/probe_tree","sPlots/z_ptbin0_etabin0_friend.root");

  char filesweight2[200];
  sprintf(filesweight2,"sPlots/sweights_jpsi_etabin%d.root",etabin);
  tree2->AddFriend("sweights=dataset",filesweight2);

  char varname1[200], varname2[200];
  sprintf(varname1,"l1%s",varname);
  sprintf(varname2,"l2%s",varname);

  TH1F *signalMC1 = new TH1F(varname1, "", nbins, min, max);
  TH1F *signalMC2 = new TH1F(varname2, "", nbins, min, max);

  char finalcutweightzmc1[600], finalcutweightzmc2[600];
  sprintf(finalcutweightzmc1,"(%s)*puW*l1ptW",finalcut1);   sprintf(finalcutweightzmc2,"(%s)*puW*l1ptW",finalcut2);
  treeMC->Project(varname1,varname1,finalcutweightzmc1);
  treeMC->Project(varname2,varname2,finalcutweightzmc2);
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

  signalsPlotJ1->SetBinErrorOption(TH1::kPoisson);
  // signalsPlotZ1->Sumw2(); signalsPlotZ2->Sumw2();  
  // signalsPlotJ1->Sumw2(); signalsPlotJ2->Sumw2();  
  
  char finalcutweightz1[600], finalcutweightz2[600];
  char finalcutweightj1[600], finalcutweightj2[600];
  sprintf(finalcutweightz1,"(%s)*N_sig_sw*puW*l1ptW",finalcut1);   sprintf(finalcutweightz2,"(%s)*N_sig_sw*puW*l1ptW",finalcut2);
  sprintf(finalcutweightj1,"(%s)*N_sig_sw",finalcut1);   sprintf(finalcutweightj2,"(%s)*N_sig_sw",finalcut2);
  cout << "finalcutweightz1,z2 = " << finalcutweightz1 << "\t" << finalcutweightz2 << endl;
  cout << "finalcutweightj1,j2 = " << finalcutweightj1 << "\t" << finalcutweightj2 << endl;
  tree1->Project(splotnamez1,varname1,finalcutweightz1);   tree1->Project(splotnamez2,varname2,finalcutweightz2);
  tree2->Project(splotnamej1,varname1,finalcutweightj1);   tree2->Project(splotnamej2,varname2,finalcutweightj2);

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
  leg->AddEntry(signalMC1,"simulation","fl");
  //  leg->AddEntry(signalsPlotZ1,"Z(ee) sPlot data","pl");
  leg->AddEntry(signalsPlotJ1,"J/#Psi sPlot data","pl");

  TCanvas c1;
  if(logy) c1.SetLogy(1);
  signalMC1->SetLineColor(kOrange+7);
  signalMC1->SetFillColor(kOrange+7);
  signalMC1->SetLineWidth(2);
  signalMC1->SetFillStyle(3005);

  signalsPlotZ1->SetMarkerStyle(kOpenCircle);
  signalsPlotZ1->SetMarkerSize(1);
  signalsPlotZ1->GetXaxis()->SetTitle(axistitle);
  signalsPlotZ1->GetYaxis()->SetTitle("weighted events");

  signalsPlotJ1->SetMarkerStyle(kFullCircle);
  signalsPlotJ1->SetMarkerSize(1);
  signalsPlotJ1->GetXaxis()->SetTitle(axistitle);
  signalsPlotJ1->GetYaxis()->SetTitle("weighted events");

  signalsPlotJ1->SetMaximum(maxY+sqrt(maxY));

  signalsPlotJ1->Draw("pe1");
  //  signalsPlotZ1->Draw("pe1 same");
  signalMC1->Draw("same hist");
  signalsPlotJ1->Draw("pe1 same");
  leg->Draw();
  text->Draw();

  char figfilename[500];
  sprintf(figfilename,"%s_ptbin%d_etabin%d_sPlot.png",varname,ptbin,etabin);
  c1.SaveAs(figfilename);
  sprintf(figfilename,"%s_ptbin%d_etabin%d_sPlot.pdf",varname,ptbin,etabin);
  c1.SaveAs(figfilename);
  sprintf(figfilename,"%s_ptbin%d_etabin%d_sPlot.C",varname,ptbin,etabin);
  c1.SaveAs(figfilename);

}
  

void makeSPlots() {

  gSystem->Load("libRooFit");
  
  // barrel
  drawSPlot(0,0,"sietaieta","#sigma_{i#eta i#eta}","1",25,0.0,0.03);
  drawSPlot(0,0,"r9","R_{9}","1",8,0.0,1);
  drawSPlot(0,0,"pt","p_{T} (GeV)","1",10,7.0,15.);
  drawSPlot(0,0,"etawidth","#eta width","1",20,0.0,0.02);
  drawSPlot(0,0,"phiwidth","#phi width","1",10,0.0,0.3);
  drawSPlot(0,0,"hOverE","H/E","1",10,0.0,2);
  drawSPlot(0,0,"sip","S.I.P.","1",10,0.0,3);
  drawSPlot(0,0,"bdtID","identification BDT","1",16,-1,1);

  drawSPlot(1,0,"sietaieta","#sigma_{i#eta i#eta}","1",20,0.0,0.02);
  drawSPlot(1,0,"r9","R_{9}","1",10,0.0,1);
  drawSPlot(1,0,"pt","p_{T} (GeV)","1",10,15.,20.);
  drawSPlot(1,0,"etawidth","#eta width","1",20,0.0,0.02);
  drawSPlot(1,0,"phiwidth","#phi width","1",10,0.0,0.3);
  drawSPlot(1,0,"hOverE","H/E","1",10,0.0,2);
  drawSPlot(1,0,"sip","S.I.P.","1",10,0.0,3);

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

