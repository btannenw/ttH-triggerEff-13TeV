// Author:  Ben Tannenwald
// Date:    Feb 27, 2018
// Purpose: Macro to make trigger correlation/efficiency plots

#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TChain.h>
#include <TFileCollection.h>
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

void trigEffStudy_2017data(string p_topDir="", string p_isMC="", string p_passFile="")
{
  // *** 0. Set style, set file, set output directory
  // ** A. Set output directory and global bools
  topDir = "plots_032618/";
  if (p_topDir != "") topDir = p_topDir;
  isMC = true;
  if (p_isMC != "") isMC = p_isMC=="true" ? true : false;
  singleFile = true;
  fileList="";
  printPlots = true;
  dumpFile = true;
  verbose = false;

  // ** B. Set input file
  TChain* fChain = new TChain("ttHTreeMaker/worldTree");
  if(singleFile) { // single file
    if (p_passFile==""){ // basically a local test
      if (isMC){
	fChain->AddFile("rootfiles/yggdrasil_treeMaker_25kEvents_addUnprescaled2017TrigBranches.root");
      }
      else{ // data!
	fChain->AddFile("rootfiles/data/SingleElectron_Run2017B-17Nov2017-v1_treeMaker_5.root");
      }
    }
    else // pass name of analysis file --> this probably means Condor submission
      fChain->AddFile( p_passFile.c_str() );
  }
  else{ // entire directory 
    if (isMC)
      fileList="rootfiles/data/allFiles_032418.txt";
    else // data!
      fileList="rootfiles/data/allFiles_032418.txt";

    // check to make sure fileList
    if(gSystem->AccessPathName(fileList.c_str()) )
      {
	cout<<"Input list file /"<<fileList.c_str()<<" DNE."<<endl;
	gSystem->Exit(0);
      }
    TFileCollection *fc= new TFileCollection("fc","files",fileList.c_str());
    fChain->AddFileInfoList((TCollection*)fc->GetList());
  }


  // ** C. Check subdirectory structure for requested options and create directories if necessary
  // * i. Check if topdir exists
  struct stat sb;
  if (!(stat(topDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "top-level director, " << topDir << " , DNE. Creating now" << endl;
    mkdir(topDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  /*
    // old code for exiting if top-level director DNE. can't use via condor, but maybe this isn't even necessary anymore...?
    if (!(stat(topDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "TopDir, " << topDir << " , DNE. EXITING" << endl;
    exit(0);
  }*/


  // * ii. Create corr2D subdir
  std::string sampleDir = "";
  if(isMC)
    sampleDir = "/MC";
  else
    sampleDir = "/data";

  topDir = (topDir + sampleDir + "/").c_str();
  if (!(stat(topDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "sample subdirectory , " << topDir << " , DNE. Creating now" << endl;
    mkdir(topDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  
  // * iii. Set output file
  if(p_passFile == "")
    outfile = new TFile( (topDir + "/outfile.root").c_str(), "RECREATE");
  else // add identifier so no overwrites
    outfile = new TFile( (topDir + "/outfile" + p_passFile.substr(p_passFile.find_last_of("_"), p_passFile.find(".root")) ).c_str(), "RECREATE");

  
  // * iv. Create corr2D subdir
  std::string tempDir = (topDir + "corr2D" + "/").c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "corr2D subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }

  // ** D. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  



  // *** 1. Define histograms and canvasses
  TCanvas *c0 = new TCanvas("c0", "c0", 50, 50, 800, 600);

  initializeHistograms(a_HLT_IsoMu27, "HLT_IsoMu27");
  initializeHistograms(a_HLT_Ele32_WPTight_Gsf, "HLT_Ele32_WPTight_Gsf");
  //initializeHistograms(a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", true);
  initializeHistograms(a_HLT_PFMET120_PFMHT120_IDTight_elStream, "HLT_PFMET120_PFMHT120_IDTight_elStream");
  initializeHistograms(a_HLT_PFMET120_PFMHT120_IDTight_muStream, "HLT_PFMET120_PFMHT120_IDTight_muStream");
  initializeHistograms(a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
  initializeHistograms(a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");

  initializeHistograms(a_HLT_SingleMu, "HLT_SingleMu");
  initializeHistograms(a_HLT_SingleEl, "HLT_SingleEl");
  initializeHistograms(a_HLT_allMET, "HLT_allMET", true);
  initializeHistograms(a_SingleMu__X__allMET, "SingleMu__X__allMET");
  initializeHistograms(a_SingleEl__X__allMET, "SingleEl__X__allMET");
 
  // *** 2. Set tree structure and variables to read
  eve=0;
  fChain->SetBranchAddress("eve.", &eve );
  elTool = electronHandler();
  muTool = muonHandler();
  jetMetTool = jetMetHandler();
  //elTool.test();
  
  
  // *** 3. Start looping! 
  long t_entries = fChain->GetEntries();
  //long t_entries = 25000;
  cout << "Chain entries: " << t_entries << endl;
  
  for(int i = 0; i < t_entries; i++) {
    if ( t_entries > 100) {
      if ((i+1)%(5*t_entries/100)==0)  printProgBar(100*i/t_entries +1); }
    if (i == t_entries-1) {printProgBar(100); cout << endl;}
    
    fChain->GetEntry(i);

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
    //if ( eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", true);

    if ( eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && jetMetTool.MET > 125 && muTool.passCuts) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_PFMET120_PFMHT120_IDTight_muStream, "HLT_PFMET120_PFMHT120_IDTight_muStream");
    //if ( eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && jetMetTool.MET > 125 && elTool.passCuts) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_PFMET120_PFMHT120_IDTight_elStream, "HLT_PFMET120_PFMHT120_IDTight_elStream");
    if ( eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && jetMetTool.MET > 125 && elTool.passCuts) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_PFMET120_PFMHT120_IDTight_elStream, "HLT_PFMET120_PFMHT120_IDTight_elStream");
    // * b. Cross triggers
    if ( eve->passHLT_IsoMu27_v_ && eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && muTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
    if ( eve->passHLT_Ele32_WPTight_Gsf_v_ && eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && elTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");


    // * c. Use multiple triggers defined in handlers
    if ( elTool.passSLtriggers && elTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_SingleEl, "HLT_SingleEl");
    if ( muTool.passSLtriggers && muTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_SingleMu, "HLT_SingleMu");
    if ( jetMetTool.passMETTriggers && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_HLT_allMET, "HLT_allMET", true);
    if ( muTool.passSLtriggers && jetMetTool.passMETTriggers && muTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_SingleMu__X__allMET, "SingleMu__X__allMET");
    if ( elTool.passSLtriggers && jetMetTool.passMETTriggers && elTool.passCuts && jetMetTool.MET > 125) fillEfficiencyHistograms(muTool, elTool, jetMetTool, a_SingleEl__X__allMET, "SingleEl__X__allMET");


  }
  
  // *** 4. Make plots
  if( dumpFile) outfile->cd();
  // ** A. 2D correlations
  //plot2Dcorrelations( a_HLT_IsoMu27, c0, "HLT_IsoMu27");
  //plot2Dcorrelations( a_HLT_Ele32_WPTight_Gsf, c0, "HLT_Ele32_WPTight_Gsf");
  
  // ** B. 1D distributions
  plot1DHistograms( a_HLT_Ele32_WPTight_Gsf, c0, "HLT_Ele32_WPTight_Gsf");
  plot1DHistograms( a_HLT_IsoMu27, c0, "HLT_IsoMu27");
  //plot1DHistograms( a_HLT_PFMET120_PFMHT120_IDTight, c0, "HLT_PFMET120_PFMHT120_IDTight");
  //plot1DHistograms( a_IsoMu27__X__PFMET120_PFMHT120_IDTight, c0, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
  //plot1DHistograms( a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, c0, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");
  
  // ** C. 1D efficiencies
  makeEfficiencyHistograms( c0, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight_muStream, "HLT_PFMET120_PFMHT120_IDTight_muStream");
  makeEfficiencyHistograms( c0, a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight_elStream, "HLT_PFMET120_PFMHT120_IDTight_elStream");
  //makeEfficiencyHistograms( c0, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight_muStream");
  //makeEfficiencyHistograms( c0, a_Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele32_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight_elStream");

  makeEfficiencyHistograms( c0, a_SingleMu__X__allMET, "SingleMu__X__allMET", a_HLT_allMET, "HLT_allMET_muStream");
  makeEfficiencyHistograms( c0, a_SingleEl__X__allMET, "SingleEl__X__allMET", a_HLT_allMET, "HLT_allMET_elStream");


  if (dumpFile)
    outfile->Write();

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

