// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

// ROOT includes
#include <TROOT.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TCanvas.h>
#include "TObjArray.h"
#include "TObjString.h"

namespace macros {

  class PlotIDVariables {
  public:
    /// default constructor
    PlotIDVariables() : outputPath_("./"), localData_(false), plotAllVariables_(false) { };
    /// constructor from parameters
    PlotIDVariables(std::string outputPath) : outputPath_(outputPath), localData_(false), plotAllVariables_(false) { };
    
    /// destructor
    ~PlotIDVariables() { };

    /// use the data in the local path, not on EOS (default is EOS)
    void setLocalData() { localData_ = true; }

    /// call this if you want to plot all the set of variables
    void doAllVariables() { plotAllVariables_ = true; }

    /// plot signal vs bkg vaiables for non triggering electrons
    void plotSignalVsBackgroundNonTriggering() { compareDistributions(true,false); }

    /// plot signal vs bkg vaiables for non triggering electrons
    void plotSignalVsBackgroundTriggering() { compareDistributions(true,true); }

    /// plot signal vs data for non triggering electrons
    void plotSignalVsDataNonTriggering() { compareDistributions(false,false); }

    /// plot signal vs data for triggering electrons
    void plotSignalVsDataTriggering() { compareDistributions(false,true); }

  private:
    
    /// the folder where to put th output
    std::string outputPath_;
    
    void makeInputVarDistributions(TString var, TString title, pair<float,float> range, TTree *treeSig, TTree* treeBkg, TString cutSig, TString cutBkg, TString namefile, bool sigVsBkg,bool applyOfflineHLT);
    void compareDistributions(bool sigVsBkg, bool applyOfflineHLT=false);
    
    bool localData_;
    bool plotAllVariables_;

  };

}


using namespace std;
using namespace macros;

void PlotIDVariables::makeInputVarDistributions(TString var, TString title, pair<float,float> range, TTree *treeSig, TTree* treeBkg, TString cutSig, TString cutBkg, TString namefile, bool sigVsBkg,bool applyOfflineHLT) {

  std::cout << "Plotting variable " << title.Data() << " with cuts: " << std::endl;
  std::cout << "\tOn signal Z->ee: " << cutSig.Data() << std::endl;
  if(sigVsBkg) cout << "\tOn bkg: " << cutBkg.Data() << std::endl;
  else cout << "\tOn data Z->ee: " << cutBkg.Data() << std::endl;

  TH1F *var_sig = new TH1F("var_sig","",30,range.first,range.second);
  TH1F *var_bkg = new TH1F("var_bkg","",30,range.first,range.second);

  var_sig->Sumw2();
  var_bkg->Sumw2();
  
  treeSig->Project("var_sig",var,cutSig);
  treeBkg->Project("var_bkg",var,cutBkg);

  float bkgnorm = var_bkg->Integral();
  float signorm = var_sig->Integral();
  var_sig->Scale(bkgnorm/signorm);
  var_sig->Scale(bkgnorm/signorm);

  var_sig->Scale(signorm/bkgnorm);
  float max = TMath::Max(var_sig->GetMaximum(),var_bkg->GetMaximum());
  max = max + 0.2 * max;
  var_sig->SetMaximum(max);

  var_sig->SetLineColor(kAzure+3);
  var_sig->SetLineWidth(2);
  var_sig->SetFillColor(kAzure+5);
  //  var_sig->SetMarkerColor(kAzure-7);
  var_sig->SetFillStyle(1001);
  if(!sigVsBkg) var_sig->SetMarkerStyle(kFullDotLarge);

  var_sig->GetXaxis()->SetLabelFont(42);
  var_sig->GetXaxis()->SetLabelSize(0.035);
  var_sig->GetXaxis()->SetTitleSize(0.08);
  var_sig->GetXaxis()->SetTitleFont(42);
  var_sig->GetYaxis()->SetLabelFont(42);
  var_sig->GetYaxis()->SetLabelSize(0.035);
  var_sig->GetYaxis()->SetTitleSize(0.08);
  var_sig->GetYaxis()->SetTitleOffset(0.2);
  var_sig->GetYaxis()->SetTitleFont(42);
  var_sig->GetZaxis()->SetLabelFont(42);
  var_sig->GetZaxis()->SetLabelSize(0.035);
  var_sig->GetZaxis()->SetTitleSize(0.035);
  var_sig->GetZaxis()->SetTitleFont(42);


  var_bkg->SetLineColor(kOrange+2);
  var_bkg->SetLineWidth(2);
  //  var_bkg->SetMarkerColor(kRed);
  var_bkg->SetFillColor(kOrange+1);
  var_bkg->SetFillStyle(3002);

  if(!sigVsBkg) var_bkg->SetMarkerStyle(8);

  // draw the legend
  TLegend* legend = new TLegend(0.55, 0.75, 0.80, 0.85);
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.031);
  legend->AddEntry(var_sig, " Z(ee) MC", "f");
  if(sigVsBkg) 
    if(applyOfflineHLT) legend->AddEntry(var_bkg, "QCD data", "f");
    else legend->AddEntry(var_bkg, "Z(ll)+1 electron data", "f");
  else legend->AddEntry(var_sig, "Z(ee) data", "pe");

  // cosmetics
  TPaveText *text = new TPaveText(0.15,0.90,0.77,0.98,"brNDC");
  text->AddText("CMS Preliminary           #sqrt{s} = 8 TeV,  L = 19.6 fb^{-1}");
  text->SetBorderSize(0);
  text->SetFillStyle(0);
  text->SetTextAlign(12);
  text->SetTextFont(132);
  text->SetTextSize(0.04);

  TCanvas c1("c1","",600,600);
  c1.Range(-1.146789,-2319.078,5.688073,12419.95);
  c1.SetFillColor(0);
  c1.SetBorderMode(0);
  c1.SetBorderSize(2);
  c1.SetLeftMargin(0.1677852);
  c1.SetFrameBorderMode(0);
  c1.SetFrameBorderMode(0);
  c1.cd();

  if(var.Contains("HoE")||var.Contains("Iso")||var.Contains("bdt")) {
    var_sig->SetMinimum(1);
    c1.SetLogy();
  }

  var_sig->GetXaxis()->SetTitle(title);
  var_sig->GetYaxis()->SetTitle("events");
  var_sig->GetYaxis()->SetTitleOffset(1.8);
  var_sig->GetXaxis()->SetTitleOffset(1.0);
  var_sig->GetXaxis()->SetTitleSize(0.04);
  var_sig->GetYaxis()->SetTitleSize(0.04);
  var_sig->Draw("hist");
  if(sigVsBkg) var_bkg->Draw("same hist");
  else var_bkg->Draw("same pe1");

  legend->Draw();
  text->Draw();
  
  TString fullname(namefile);
  fullname.ReplaceAll("/","Over");
  fullname.ReplaceAll("[","_");
  fullname.ReplaceAll("]","_");

  TString suffix = (sigVsBkg) ? "_SigVsBkg" : "_SimVsData";
  suffix += (applyOfflineHLT) ? "_triggering" : "_nontriggering";

  TString pdf = TString(outputPath_+"/") + TString(fullname) + suffix + TString(".pdf");
  TString png = TString(outputPath_+"/") + TString(fullname) + suffix + TString(".png");
  TString macro = TString(outputPath_+"/") + TString(fullname) + suffix + TString(".C");

  c1.SaveAs(pdf);
  c1.SaveAs(png);
  c1.SaveAs(macro);

}

void PlotIDVariables::compareDistributions(bool sigVsBkg, bool applyOfflineHLT) {

  // for sig/bkg comparison
  // if applyOfflineHLT=1 => signal = Z->ee (data), bkg = QCD fake rate sample (data). This is because with denom applied Z->ee is quite clean
  // if applyOfflineHLT=0 => signal = Z->ee (mc), bkg = W->ln + 1jet (data)

  string eospath("root://eoscms//eos/cms/store/group/phys_egamma/emanuele/eleid/cmsdasjan14/");
  string localpath("data/");

  string path = (localData_) ? localpath : eospath;
  
  gStyle->SetOptStat(0);

  TFile *fileSig, *fileBkg;
  TTree *treeSig, *treeBkg;
  fileSig = fileBkg = 0;
  treeSig = treeBkg = 0;

  fileSig = TFile::Open((path+"electrons_zeemc.root").c_str());
  if(sigVsBkg) {
    if(applyOfflineHLT) { /// this is the case for the triggering electrons
      fileBkg = TFile::Open((path+"fakes.root").c_str());
    } else { ///  this is the case for the non triggering electrons
      fileBkg = TFile::Open((path+"fakes-zll1e.root").c_str());
    }
  } else {
    fileBkg = TFile::Open((path+"electrons.root").c_str());
  }

  if( fileSig && fileBkg ) {
    fileSig->cd();
    treeSig = (TTree*)fileSig->Get("eleIDdir/T1");
    fileBkg->cd();
    treeBkg = (TTree*)fileBkg->Get("eleIDdir/T1");
  } else {
    cout << "File " << fileSig << " or " << fileBkg << " not existing !" << endl;
    return;
  }

  treeSig->AddFriend("eleIDdir/isoT1 = eleIDdir/T1", (path+"electrons_zeemc_hzzisoFriend.root").c_str());
  if(sigVsBkg) {
    if(applyOfflineHLT) { /// this is the case for the triggering electrons
      treeBkg->AddFriend("eleIDdir/isoT1 = eleIDdir/T1", (path+"fakes_hzzisoFriend.root").c_str());
    } else { ///  this is the case for the non triggering electrons
      treeBkg->AddFriend("eleIDdir/isoT1 = eleIDdir/T1", (path+"fakes-zll1e_hzzisoFriend.root").c_str());
    }
  } else {
    treeBkg->AddFriend("eleIDdir/isoT1 = eleIDdir/T1", (path+"electrons_hzzisoFriend.root").c_str());
  }
  
  if(!treeSig || !treeBkg) {
    cout << "Tree eleIDdir/T1 not existing inside signal or background files!" << endl;
    return;
  }

  cout << "All files OK!" << endl;

  vector<TString> cutBase;
  cutBase.push_back(TString("abs(eta)<1.0                   && pt<20"));
  cutBase.push_back(TString("abs(eta)>1.0 && abs(eta)<1.479 && pt<20"));
  cutBase.push_back(TString("abs(eta)>1.479                 && pt<20"));
  cutBase.push_back(TString("abs(eta)<1.0                   && pt>20"));
  cutBase.push_back(TString("abs(eta)>1.0 && abs(eta)<1.479 && pt>20"));
  cutBase.push_back(TString("abs(eta)>1.479                 && pt>20"));

  vector<TString> cutSignal;
  for(int i=0;i<(int)cutBase.size();++i) {
    if(applyOfflineHLT) cutSignal.push_back(cutBase[i]+TString("&& mcmatch && pt<35 && abs(mass-91.1876)<7.5 && DenomFake"));
    else cutSignal.push_back(cutBase[i]+TString("&& mcmatch && pt<35 && abs(mass-91.1876)<7.5")); /// pt<35 GeV is needed to suppress the EWK contamination
  }

  vector<TString> cutBackground;
  for(int i=0;i<(int)cutBase.size();++i) {
    if(applyOfflineHLT) cutBackground.push_back(cutBase[i] + TString("&& pt<35 && DenomFake") );
    else cutBackground.push_back(cutBase[i] + TString("&& pt<35") );
    if(!sigVsBkg) cutBackground[i] = cutBackground[i] + TString(" && abs(mass-91.1876)<7.5");
  }

  vector<TString> suffix;
  suffix.push_back("_inEB_LowPt");
  suffix.push_back("_outEB_LowPt");
  suffix.push_back("_EE_LowPt");
  suffix.push_back("_inEB_HighPt");
  suffix.push_back("_outEB_HighPt");
  suffix.push_back("_EE_HighPt");

  vector<TString> input;
  if(applyOfflineHLT) input.push_back("newbdthww[3]");
  else input.push_back("bdthzz[3]");
  input.push_back("EoP");
  input.push_back("deta");
  input.push_back("dphi");
  input.push_back("see");
  input.push_back("fbrem");
  input.push_back("EoPout");
  input.push_back("eleEoPout");
  input.push_back("IoEmIoP");
  input.push_back("HoE");
  input.push_back("eledeta");
  input.push_back("sep");
  input.push_back("spp");
  input.push_back("phiwidth");
  input.push_back("etawidth");
  input.push_back("missHits");
  input.push_back("nbrem");
  input.push_back("dist");
  input.push_back("dcot");
  input.push_back("d0");
  input.push_back("ip3d");
  input.push_back("ip3ds");
  input.push_back("kfhits");
  input.push_back("kfchi2");
  input.push_back("e1x5e5x5");
  input.push_back("ecaldriven");
  input.push_back("(trkIso+rho*TMath::Pi()*0.3*0.3)/pt"); // rho has been subtracted when making the ntuple
  input.push_back("(ecalIso+rho*TMath::Pi()*0.3*0.3)/pt");
  input.push_back("(hcalIso+rho*TMath::Pi()*0.3*0.3)/pt");
  input.push_back("chaPFIso[3]/pt");
  input.push_back("neuPFIso[3]/pt");
  input.push_back("phoPFIso[3]/pt");
  input.push_back("combPFIsoHWW/pt");
  input.push_back("combPFIsoHZZ/pt");
  input.push_back("vertices");

  vector<TString> title;
  if(applyOfflineHLT) title.push_back("Triggering BDT");
  else title.push_back("Non-Triggering BDT");
  title.push_back("E_{SC}/p_{in}");
  title.push_back("#Delta #eta");
  title.push_back("#Delta #phi");
  title.push_back("#sigma_{i#eta i#eta}");
  title.push_back("f_{brem}");
  title.push_back("E_{seed}/p_{out}");
  title.push_back("E_{match cluster}/p_{out}");
  title.push_back("1/E-1/P");
  title.push_back("H/E");
  title.push_back("#Delta #eta (track-cluster)");
  title.push_back("#sigma_{i#eta i#phi}");
  title.push_back("#sigma_{i#phi i#phi}");
  title.push_back("#phi width");
  title.push_back("#eta width");
  title.push_back("miss hits");
  title.push_back("n_{brem}");
  title.push_back("conv. dist");
  title.push_back("conv. ctg #theta");
  title.push_back("d_{0}");
  title.push_back("IP 3D");
  title.push_back("IP 3D sign.");
  title.push_back("KF hits");
  title.push_back("KF #chi^{2}");
  title.push_back("(E5x5-E1x5)/E5x5");
  title.push_back("ECAL seed");
  title.push_back("rel. tracker iso.");
  title.push_back("rel. ECAL iso.");
  title.push_back("rel. HCAL iso.");
  title.push_back("rel. charged PF iso.");
  title.push_back("rel. nh PF iso.");
  title.push_back("rel. #gamma PF iso.");
  title.push_back("rel. comb. PF iso. old");
  title.push_back("rel. comb. PF iso. new");
  title.push_back("n. vertices");

  vector< pair<float,float> > range;
  range.push_back(std::make_pair(-1.0,1.0)); // identification BDT
  range.push_back(std::make_pair(0.0,3.0)); // EoP
  range.push_back(std::make_pair(-0.01,0.01)); // deta in
  range.push_back(std::make_pair(-0.2,0.2)); // dphi in
  range.push_back(std::make_pair(0.0,0.04)); // see
  range.push_back(std::make_pair(-0.2,1.0)); // fbrem
  range.push_back(std::make_pair(0.0,5.0)); // EoPout
  range.push_back(std::make_pair(0.0,5.0)); // eleEoPout
  range.push_back(std::make_pair(-0.1,0.1)); // 1/E - 1/P
  range.push_back(std::make_pair(0.0,0.15)); // H/E
  range.push_back(std::make_pair(-0.03,0.03)); // ele deta
  range.push_back(std::make_pair(0.0,0.08)); // sep
  range.push_back(std::make_pair(0.0,0.08)); // spp
  range.push_back(std::make_pair(0.0,0.04)); // eta width
  range.push_back(std::make_pair(0.0,0.08)); // phi width
  range.push_back(std::make_pair(0.0,10)); // miss hits
  range.push_back(std::make_pair(0.0,5.0)); // nbrem
  range.push_back(std::make_pair(-2.0,2.0)); // dist
  range.push_back(std::make_pair(-20.0,20.)); // dcot
  range.push_back(std::make_pair(-0.04,0.04)); // d0
  range.push_back(std::make_pair(-0.05,0.05)); // ip3d
  range.push_back(std::make_pair(-10.0,10.0)); // ip3ds
  range.push_back(std::make_pair(0,20)); // kfhits
  range.push_back(std::make_pair(0,10)); // kfchi2
  range.push_back(std::make_pair(0,1)); // (e5x5-e1x5)/e5x5
  range.push_back(std::make_pair(0,1)); // ECAL seed
  range.push_back(std::make_pair(0,0.3)); // rel tk iso
  range.push_back(std::make_pair(-0.5,0.3)); // rel ECAL iso
  range.push_back(std::make_pair(0.,0.3)); // rel HCAL iso
  range.push_back(std::make_pair(0,2.0)); // cha PF iso
  range.push_back(std::make_pair(0,2.0)); // nh PF iso
  range.push_back(std::make_pair(0,2.0)); // pho PF iso
  range.push_back(std::make_pair(0,4.0)); // comb PF iso HWW
  range.push_back(std::make_pair(0,4.0)); // comb PF iso HZZ
  range.push_back(std::make_pair(0,30)); // vertices

  int maxVariables = (plotAllVariables_) ? input.size() : 6;

  for(int v=0;v<maxVariables;++v) {
    for(int i=0;i<(int)cutBase.size();++i) {
      TString filen(input[v]);
      filen.Append(suffix[i]);
      cout << filen.Data() << endl;
      makeInputVarDistributions(input[v],title[v],range[v],treeSig,treeBkg,cutSignal[i],cutBackground[i],filen,sigVsBkg,applyOfflineHLT);
    }
  }

}
