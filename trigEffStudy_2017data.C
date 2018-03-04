// Author:  Ben Tannenwald
// Date:    Feb 27, 2018
// Purpose: Macro to make trigger correlation/efficiency plots

#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH2.h>
#include <TLatex.h>
#include <TString.h>
#include <TObjArray.h>

#include "include/YggdrasilEventVars.h"
#include "include/trigEffStudy_2017data.h"
#include "include/histogramHandler.h"

// Object Handlers
#include "src/electronHandler.C"
#include "src/muonHandler.C"
#include "src/jetMetHandler.C"

#include <iostream>
#include <iomanip>      // std::setprecision
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void printProgBar( int percent );
void plot2Dcorr( TCanvas*& c0, TH2D*& h0, string xtitle, string ytitle);

void trigEffStudy_2017data()
{
  // *** 0. Set style, set file, set output directory
  // ** A. Set input file
  //TFile* f0 = new TFile("rootfiles/yggdrasil_treeMaker_2kEvents_addUnprescaled2017TrigBranches.root", "READ");
  TFile* f0 = new TFile("rootfiles/yggdrasil_treeMaker_25kEvents_addUnprescaled2017TrigBranches.root", "READ");
  TTree* fTree = (TTree*) f0->Get("ttHTreeMaker/worldTree");

  // ** B. Set output directory
  topDir = "plots_030518/";
  printPlots = true;
  verbose = false;

  // check subdirectory structure for requested options and create directories if necessary
  struct stat sb;
  if (!(stat(topDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "TopDir, " << topDir << " , DNE. EXITING" << endl;
    exit(0);
  }
  std::string tempDir = (topDir + "corr2D" + "/").c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "corr2D subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }

  // ** B. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  

  // *** 1. Define histograms and canvasses
  TCanvas *c0 = new TCanvas("c0", "c0", 50, 50, 800, 600);

  initializeHistograms(a_HLT_IsoMu27, "HLT_IsoMu27");
  initializeHistograms(a_HLT_Ele32_WPTight_Gsf, "HLT_Ele32_WPTight_Gsf");
  initializeHistograms(a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", true);
  initializeHistograms(a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
  initializeHistograms(a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");

  // *** 2. Set tree structure and variables to read
  eve=0;
  fTree->SetBranchAddress("eve.", &eve );
  elTool = electronHandler();
  muTool = muonHandler();
  jetMetTool = jetMetHandler();
  //elTool.test();
  
  
  // *** 3. Start looping! 
  long t_entries = fTree->GetEntries();
  //long t_entries = 5000;
  cout << "Tree entries: " << t_entries << endl;
  
  for(int i = 0; i < t_entries; i++) {
    if ( t_entries > 100) {
      if ((i+1)%(5*t_entries/100)==0)  printProgBar(100*i/t_entries +1); }
    if (i == t_entries-1) {printProgBar(100); cout << endl;}
    
    fTree->GetEntry(i);
    
    // ** I. Get objects 
    muTool.Event(eve);
    elTool.Event(eve);
    jetMetTool.Event(eve);

    // ** II. 2D Correlations comparing SL triggers to MET triggers
    if (muTool.passCuts && jetMetTool.MET > 125) fill2DCorrHistograms(eve, a_HLT_IsoMu27, "HLT_IsoMu27", eve->passHLT_IsoMu27_v_ );
    if (elTool.passCuts && jetMetTool.MET > 125) fill2DCorrHistograms(eve, a_HLT_Ele32_WPTight_Gsf, "HLT_Ele32_WPTight_Gsf", eve->passHLT_Ele32_WPTight_Gsf_v_ );

    // III. Fill efficiency histograms given some trigger qualifications
    // * a. Single triggers
    if ( eve->passHLT_Ele32_WPTight_Gsf_v_ && elTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_Ele32_WPTight_Gsf, "HLT_Ele32_WPTight_Gsf");
    if ( eve->passHLT_IsoMu27_v_ && muTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_IsoMu27, "HLT_IsoMu27");
    // !!! FIXME --> how to require presence of lepton (both el and mu separately??)
    if ( eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", true);
    // * b. Cross triggers
    if ( eve->passHLT_IsoMu27_v_ && eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && muTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
    if ( eve->passHLT_Ele32_WPTight_Gsf_v_ && eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && elTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");

  }
  
  // *** 4. Make plots
  // ** A. 2D correlations
  plot2Dcorrelations( a_HLT_IsoMu27, c0, "HLT_IsoMu27");
  plot2Dcorrelations( a_HLT_Ele32_WPTight_Gsf, c0, "HLT_Ele32_WPTight_Gsf");
  
  // ** B. 1D distributions
  plot1DHistograms( a_HLT_Ele32_WPTight_Gsf, c0, "HLT_Ele32_WPTight_Gsf");
  plot1DHistograms( a_HLT_IsoMu27, c0, "HLT_IsoMu27");
  //plot1DHistograms( a_HLT_PFMET120_PFMHT120_IDTight, c0, "HLT_PFMET120_PFMHT120_IDTight");
  //plot1DHistograms( a_IsoMu27__X__PFMET120_PFMHT120_IDTight, c0, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
  //plot1DHistograms( a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, c0, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");
  
  // ** C. 1D efficiencies
  //makeEfficiencyHistograms( c0, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight");
  //makeEfficiencyHistograms( c0, a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight");
  makeEfficiencyHistograms( c0, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight_muStream");
  makeEfficiencyHistograms( c0, a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight_elStream");

  return;
}
    

void printProgBar( int percent )
{
  string bar;
  
  for(int i = 0; i < 50; i++){
    if( i < (percent/2)){
      bar.replace(i,1,"=");
    }else if( i == (percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }
  
  cout<< "\r" "[" << bar << "] ";
  cout.width( 3 );
  cout<< percent << "%     " << std::flush;
}

