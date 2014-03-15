struct fitbinning {
  enum etaBins {kEB = 0,
                kEE};
};

//===================================
// make the RooDataSet from ROOT tree
//=================================== 
RooDataSet *getDataSet(const char *rootfile) {
  cout << "roofitting file " << rootfile << endl;

  // fit variables
  RooRealVar* mass = new RooRealVar("zmass",  "Mass [GeV]", 1.5, 3.8);

  // kinematic cut variables
  RooRealVar* l1pt = new RooRealVar("l1pt", "p_{T}^{1} [GeV]",7,1000);
  RooRealVar* l2pt = new RooRealVar("l2pt", "p_{T}^{2} [GeV]",7,1000);
  RooRealVar* l1eta = new RooRealVar("l1eta","#eta^{1}",-2.5,2.5);
  RooRealVar* l2eta = new RooRealVar("l2eta","#eta^{2}",-2.5,2.5);

  RooArgSet setkine(*mass,*l1pt,*l2pt,*l1eta,*l2eta);
  TFile *file = TFile::Open(rootfile);
  TTree *tree = (TTree*)file->Get("zeetree/probe_tree");

  RooDataSet *data = new RooDataSet("T1","dataset",tree,setkine);

  return data;
}

// Set Fit Options
MLOptions GetDefaultOptions() {
  MLOptions opts;
  // Fit configuration
  opts.addBoolOption("useinvMass", "Use invMass", kTRUE);        
  return opts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

MLFit theFit;

void myFit() {

  MLFit theFit;

  // Various fit options...
  MLOptions opts = GetDefaultOptions();
  
  // define the structure of the dataset
  RooRealVar* mass = new RooRealVar("zmass",  "Mass [GeV/c^{2}]" , 1.5, 3.8);
  
  theFit.AddFlatFileColumn(mass);
  
  // define a fit model
  theFit.addModel("myFit", "JPsitoEE");
  
  // define species
  theFit.addSpecies("myFit", "sig", "Signal Component");
  theFit.addSpecies("myFit", "bkg", "Bkg   Component");
  
  // mLL PDF
  if(opts.getBoolVal("useinvMass")) {
    theFit.addPdfWName("myFit", "sig" , "zmass",  "Cruijff", "sig_Mass");
    theFit.addPdfWName("myFit", "bkg" , "zmass",  "Poly2",   "bkg_Mass");
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fit a sample of Z events
void FitJPsiElectrons(int etabin=-1) {
  
  myFit();

  // Load the data
  char datasetname[200];
  sprintf(datasetname,"jpsiData2012.root");
  RooDataSet *data = getDataSet(datasetname);
  if(etabin==fitbinning::kEB) data = (RooDataSet*)data->reduce("abs(l1eta)<1.479 && abs(l2eta)<1.479");
  if(etabin==fitbinning::kEE) data = (RooDataSet*)data->reduce("abs(l1eta)>=1.479 && abs(l2eta)>=1.479");

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");
  
  // Initialize the fit...
  char fitconfig[200];
  sprintf(fitconfig,"fitconfig/fitJPsi-etabin0.config"); 
  theFit.initialize(fitconfig);
  
  // Print Fit configuration 
  myPdf->getParameters(data)->selectByAttrib("Constant",kTRUE)->Print("V");  
  myPdf->getParameters(data)->selectByAttrib("Constant",kFALSE)->Print("V");
  
  RooFitResult *fitres =  myPdf->fitTo(*data,
                                       RooFit::ConditionalObservables(theFit.getNoNormVars("myFit")),
                                       RooFit::FitOptions("MHTER"),
                                       RooFit::NumCPU(4));
  fitres->Print("V");
  
  // write the config file corresponding to the fit minimum
  char configfilename[200];
  sprintf(configfilename, "fitres/fitResults-etabin%d.config",etabin);
  theFit.writeConfigFile(configfilename);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlotJPsiElectrons(int etabin=-1, int nbins=19) {

  myFit();

  // Load the data
  char datasetname[200];
  sprintf(datasetname,"jpsiData2012.root");
  RooDataSet *data = getDataSet(datasetname);
  if(etabin==fitbinning::kEB) data = (RooDataSet*)data->reduce("abs(l1eta)<1.479 && abs(l2eta)<1.479");
  if(etabin==fitbinning::kEE) data = (RooDataSet*)data->reduce("abs(l1eta)>=1.479 && abs(l2eta)>=1.479");

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");

  // Initialize the fit...
  char configfilename[200];
  sprintf(configfilename, "fitres/fitResults-etabin%d.config",etabin);
  theFit.initialize(configfilename);

  TCanvas *c = new TCanvas("c","fitResult");
  char rootfilename[200];
  sprintf(rootfilename,"fitres/mjpsi-etabin%d.root",etabin);
  TFile *output = new TFile(rootfilename,"RECREATE");

  RooPlot* MassPlot = MakePlot("zmass", &theFit, data, nbins, false);    

  MassPlot->SetYTitle("Events");
  MassPlot->Draw();
  char epsfilename[200];
  sprintf(epsfilename,"fitres/mjpsi-etabin%d.pdf",etabin);
  c->SaveAs(epsfilename);
  sprintf(epsfilename,"fitres/mjpsi-etabin%d.png",etabin);
  c->SaveAs(epsfilename);
  MassPlot->Write();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Make the plot for a given variable
RooPlot *MakePlot(TString VarName, MLFit* theFit, RooDataSet* theData, int nbins, bool poissonError=true)
{
  RooRealVar* Var = theFit->RealObservable(VarName);
  double min=Var->getMin();
  double max=Var->getMax();
  RooPlot *plot = Var->frame(min,max,nbins);
  
  // plot the data
  if(poissonError)
    theData->plotOn(plot);
  else 
    theData->plotOn(plot, RooFit::DataError(RooAbsData::SumW2) );

  // plot the total likelihood
  RooAbsPdf *thePdf = theFit->getPdf("myFit");
  thePdf->plotOn(plot, RooFit::LineColor(kAzure-5));

  if(poissonError)
    theData->plotOn(plot);
  else 
    theData->plotOn(plot, RooFit::DataError(RooAbsData::SumW2) );

  double Ns = theFit->getRealPar("N_sig")->getVal();
  double Nb = theFit->getRealPar("N_bkg")->getVal();

  // plot (dashed) the bkg component
  theFit->getRealPar("N_sig")->setVal(0.);
  thePdf->plotOn(plot, RooFit::Normalization(Nb/(Ns+Nb)),RooFit::LineColor(kBlack),RooFit::LineStyle(kDashed));

  
  return plot;
}

// =========================================================
// *** Calculate the sWeights from the minimum of the LH
// =========================================================

void calcSWeight(int etabin=-1) {

  myFit();

  // Load the data
  // reduction has to be done hard on the tree because reduce does not propagate correctly the tree
  char datasetname[1000];
  sprintf(datasetname,"jpsiData2012.root");
  char cut[200];
  if(etabin==fitbinning::kEB) sprintf(cut,"abs(l1eta)<1.479 && abs(l2eta)<1.479");
  if(etabin==fitbinning::kEE) sprintf(cut,"abs(l1eta)>=1.479 && abs(l2eta)>=1.479");

  gROOT->LoadMacro("src/reduceTree.cc");
  char filesmall[200];
  sprintf(filesmall,"sPlots/jpsi_etabin%d.root",etabin);
  makeSmall(datasetname,"zeetree/probe_tree",cut,filesmall);
  RooDataSet *data = getDataSet(filesmall);

  // build the fit likelihood
  RooAbsPdf *myPdf = theFit.buildModel("myFit");
  
  // Initialize the fit: the first fit should be already done and the minimum config ready...
  char fitconfig[200];
  sprintf(fitconfig, "fitres/fitResults-etabin%d.config",etabin);
  theFit.initialize(fitconfig);

  // fix all parameters, float the yields and fit
  theFit._parameterSet.selectByName("*")->setAttribAll("Constant",kTRUE);
  (static_cast<RooRealVar*>(theFit._parameterSet.find("N_sig")))->setConstant(kFALSE) ;
  (static_cast<RooRealVar*>(theFit._parameterSet.find("N_bkg")))->setConstant(kFALSE) ;
  
  cout << "printing data: " << endl;
  cout << data->tree() << endl;
  data->Print("V");

  RooFitResult *fitres =  myPdf->fitTo(*data,
                                       RooFit::ConditionalObservables(theFit.getNoNormVars("myFit")),
                                       RooFit::FitOptions("MHTER"));
  fitres->Print("V");

  // add appropriate column to dataset
  RooArgList yieldsList;
  yieldsList.add(*theFit._fracList.find("N_sig"));
  yieldsList.add(*theFit._fracList.find("N_bkg"));
  cout << "number of entries in set to write: " << data->numEntries() << endl ;
  RooArgSet nonormvars;
  RooDataSet* dsnew = MLSPlot::addSWeightToData((RooSimultaneous*)(myPdf), yieldsList, *data, nonormvars) ;

  char filesweight[200];
  sprintf(filesweight,"sPlots/sweights_jpsi_etabin%d.root",etabin);
  TFile sPlotsTree(filesweight,"recreate");

  TreeFillerFromRooDataSet sweights(dsnew);
  sweights.addVar("N_sig_sw");
  sweights.addVar("N_bkg_sw");
  TTree *treewithweights = sweights.getTree();
  treewithweights->Write();
  sPlotsTree.Close();

}
