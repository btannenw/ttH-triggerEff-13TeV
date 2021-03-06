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
#include "src/leptonHandler.C"
#include "src/jetMetHandler.C"

#include <iostream>
#include <iomanip>      // std::setprecision
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void printProgBar( int percent );
void plot2Dcorr( TCanvas*& c0, TH2D*& h0, string xtitle, string ytitle);

void trigEffStudy_2017data(string p_topDir="", string p_isMC="", string p_passFile="", string p_trigSF="")
{
  // *** 0. Set style, set file, set output directory
  // ** A. Set output directory and global bools
  topDir = "plots_01-17-19/";
  if (p_topDir != "") topDir = p_topDir;
  isMC = true;
  if (p_isMC != "") isMC = (p_isMC=="true" || p_isMC=="True") ? true : false;
  if (p_trigSF != "") isTrigSF = (p_trigSF=="true" || p_trigSF=="True") ? true : false;

  if(isTrigSF) cout<<"### p_trigSF=="<<p_trigSF<<" is TRUE!!!"<<endl;
  else         cout<<"### p_trigSF=="<<p_trigSF<<" is FALSE!!!"<<endl; 

  singleFile = true;
  fileList="";
  printPlots = true;
  dumpFile = true;
  verbose = false;
  printCSV = true;
  // clean up output if grid submission for trigSF
  if(isTrigSF) {
    printCSV=false;
    //printPlots = false;
  }

  // ** B. Set input file
  TChain* fChain = new TChain("ttHTreeMaker/worldTree");
  if(singleFile) { // single file
    if (p_passFile==""){ // basically a local test
      if (isMC){
	fChain->AddFile("/uscms/home/benjtann/nobackup/ttH-triggerEff-13TeV/updateRootFiles_12-2018/yggdrasil_treeMaker_ttH_sync_01-08-19_v0_oldV1_oldJEC_newBTagSF.root");
	//fChain->AddFile("/uscms/home/benjtann/nobackup/ttH-triggerEff-13TeV/updateRootFiles_12-2018/yggdrasil_treeMaker_ttH_sync_12-04-18_v27_allOld.root");
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

  // * iv. Set CSV outputs if doing sync
  if (printCSV)  setSyncFiles(topDir, "updateCheck_v28_addV2_newBTagSF", "addV2_newBTagSF");


  // *** 1. Define histograms and canvasses
  TCanvas *c0 = new TCanvas("c0", "c0", 50, 50, 800, 600);
  /*
  initializeHistograms(a_HLT_IsoMu27, "HLT_IsoMu27");
  initializeHistograms(a_HLT_IsoMu24_eta2p1, "HLT_IsoMu24_eta2p1");
  initializeHistograms(a_HLT_Ele35_WPTight_Gsf, "HLT_Ele35_WPTight_Gsf");
  initializeHistograms(a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", true);
  initializeHistograms(a_HLT_PFHT250, "HLT_PFHT250", true);
  initializeHistograms(a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight");
  initializeHistograms(a_Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight");

  initializeHistograms(a_HLT_SingleMu, "HLT_SingleMu");
  initializeHistograms(a_HLT_SingleEl, "HLT_SingleEl");
  initializeHistograms(a_SingleMu__X__allMET, "SingleMu__X__allMET");
  initializeHistograms(a_SingleEl__X__allMET, "SingleEl__X__allMET");
  */

  /*initializeHistograms(a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ");
  initializeHistograms(a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8");
  initializeHistograms(a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8");
  initializeHistograms(a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
  initializeHistograms(a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
  initializeHistograms(a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL, "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL");
  initializeHistograms(a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
  initializeHistograms(a_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
  initializeHistograms(a_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
  */

  initializeHistograms(a_HLT_allMET, "HLT_allMET", true);

  initializeHistograms(a_HLT_DoubleMu, "HLT_DoubleMu");
  initializeHistograms(a_HLT_DoubleEl, "HLT_DoubleEl");
  initializeHistograms(a_HLT_EMu, "HLT_EMu");
  initializeHistograms(a_DoubleMu__X__allMET, "DoubleMu__X__allMET");
  initializeHistograms(a_DoubleEl__X__allMET, "DoubleEl__X__allMET");
  initializeHistograms(a_EMu__X__allMET, "EMu__X__allMET");

  initializeHistograms(a_HLT_DoubleMu_OR, "HLT_DoubleMu_OR");
  initializeHistograms(a_HLT_DoubleEl_OR, "HLT_DoubleEl_OR");
  initializeHistograms(a_HLT_EMu_OR, "HLT_EMu_OR");
  initializeHistograms(a_DoubleMu_OR__X__allMET, "DoubleMu_OR__X__allMET");
  initializeHistograms(a_DoubleEl_OR__X__allMET, "DoubleEl_OR__X__allMET");
  initializeHistograms(a_EMu_OR__X__allMET, "EMu_OR__X__allMET");
 
  // *** 2. Set tree structure and variables to read
  eve=0;
  fChain->SetBranchAddress("eve.", &eve );
  lepTool = leptonHandler();
  lepTool.setFlags(isMC, p_passFile.c_str());
  jetMetTool = jetMetHandler();

  
  // *** 3. Start looping! 
  long t_entries = fChain->GetEntries();
  //long t_entries = 500;
  cout << "Chain entries: " << t_entries << endl;
  
  for(int i = 0; i < t_entries; i++) {
    if ( t_entries > 100) {
      if ((i+1)%(5*t_entries/100)==0)  printProgBar(100*i/t_entries +1); }
    if (i == t_entries-1) {printProgBar(100); cout << endl;}
    
    fChain->GetEntry(i);
    
    // ** 0. Set debug flags per event
    if ( !isTrigSF && ( eve->evt_ == 3280938 || eve->evt_ == 6895311 || eve->evt_ == 7772097 || eve->evt_ == 6896348 || eve->evt_ == 7984706 || eve->evt_ == 7247651 || eve->evt_ == 6896671)
	//eve->evt_ == 34837976 || eve->evt_ == 227079011 || eve->evt_ == 4511372
	//eve->evt_ == 7893371 || eve->evt_ == 3690607 || eve->evt_ == 7773934 || eve->evt_ ==7646645 || eve->evt_ == 7774148 || eve->evt_ == 7772809 // n_jets and n_btags mismatch in MC [FIXED]
	 ) 
      isDebug = true;
    else
      isDebug = false;

    // ** I. Get objects 
    lepTool.Event(eve, isDebug, isTrigSF);
    jetMetTool.Event(eve, lepTool, isDebug, isTrigSF);


    // ** II. 2D Correlations comparing SL triggers to MET triggers
    // * A. specific triggers
    /*if (lepTool.passSLCuts_mu && jetMetTool.passSLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_IsoMu27, "HLT_IsoMu27", eve->passHLT_IsoMu27_v_, jetMetTool );
    if (lepTool.passSLCuts_mu && jetMetTool.passSLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_IsoMu24_eta2p1, "HLT_IsoMu24_eta2p1", eve->passHLT_IsoMu24_eta2p1_v_, jetMetTool );
    if (lepTool.passSLCuts_el && jetMetTool.passSLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Ele35_WPTight_Gsf, "HLT_Ele35_WPTight_Gsf", eve->passHLT_Ele35_WPTight_Gsf_v_, jetMetTool );
    if (lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_, jetMetTool );
    if (lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_, jetMetTool );
    if (lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8", eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_, jetMetTool );
    if (lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", eve->passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_, jetMetTool);
    if (lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", eve->passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_, jetMetTool);
    if (lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL, "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL", eve->passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_, jetMetTool);
    if (lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", eve->passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_, jetMetTool);
    if (lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ", eve->passHLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_, jetMetTool);
    if (lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ, "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ", eve->passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_, jetMetTool);
    if ( (lepTool.passSLCuts_el || lepTool.passSLCuts_mu || lepTool.passSLCuts_el || lepTool.passDLCuts_mu) && jetMetTool.passSLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", eve->passHLT_PFMET120_PFMHT120_IDTight_v_, jetMetTool );
    if ( (lepTool.passSLCuts_el || lepTool.passSLCuts_mu || lepTool.passSLCuts_el || lepTool.passDLCuts_mu) && jetMetTool.passSLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_PFHT250, "HLT_PFHT250", eve->passHLT_PFHT250_v_, jetMetTool );
    */
    // * B. logical OR of triggers
    if (lepTool.passSLCuts_el && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_SingleEl, "HLT_SingleEl", lepTool.passSLtriggers_el , jetMetTool );
    if (lepTool.passSLCuts_mu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_SingleMu, "HLT_SingleMu", lepTool.passSLtriggers_mu, jetMetTool );
    if (lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_DoubleEl, "HLT_DoubleEl", lepTool.passDLtriggers_el , jetMetTool );
    if (lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_DoubleMu, "HLT_DoubleMu", lepTool.passDLtriggers_mu , jetMetTool );
    if (lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_EMu, "HLT_EMu", lepTool.passDLtriggers_emu, jetMetTool );
    if (lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_DoubleEl_OR, "HLT_DoubleEl_OR", (lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) , jetMetTool );
    if (lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_DoubleMu_OR, "HLT_DoubleMu_OR", (lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu), jetMetTool );
    if (lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fill2DCorrHistograms(eve, a_HLT_EMu_OR, "HLT_EMu_OR", (lepTool.passSLtriggers_el || lepTool.passSLtriggers_mu || lepTool.passDLtriggers_emu), jetMetTool );

    // III. Fill efficiency histograms given some trigger qualifications
    // * A. Single triggers
    /*if ( eve->passHLT_Ele35_WPTight_Gsf_v_ && lepTool.passSLCuts_el && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_Ele35_WPTight_Gsf, "HLT_Ele35_WPTight_Gsf", p_passFile.c_str());
    if ( eve->passHLT_IsoMu27_v_ && lepTool.passSLCuts_mu && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_IsoMu27, "HLT_IsoMu27", p_passFile.c_str());
    if ( eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight", p_passFile.c_str(), true);
    if ( eve->passHLT_PFHT250_v_ && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_PFHT250, "HLT_PFHT250", p_passFile.c_str(), true);
    if ( (lepTool.b_periodDep__doubleMu_noMass && eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", p_passFile.c_str());
    if ( (lepTool.b_periodDep__doubleMu_withMass && eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8" , p_passFile.c_str());
    if ( (lepTool.b_periodDep__doubleMu_withMass && eve->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8" , p_passFile.c_str());
    
    // * b. Cross triggers
    if ( eve->passHLT_IsoMu27_v_ && eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && lepTool.passSLCuts_mu && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight", p_passFile.c_str());
    if ( eve->passHLT_Ele35_WPTight_Gsf_v_ && eve->passHLT_PFMET120_PFMHT120_IDTight_v_ && lepTool.passSLCuts_el && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", p_passFile.c_str());
    
    // * c. Use multiple triggers defined in handlers
    if ( lepTool.passSLtriggers_el && lepTool.passSLCuts_el && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_SingleEl, "HLT_SingleEl", p_passFile.c_str());
    if ( lepTool.passSLtriggers_mu && lepTool.passSLCuts_mu && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_SingleMu, "HLT_SingleMu", p_passFile.c_str());

    if ( lepTool.passSLtriggers_mu && jetMetTool.passAllMETTriggers && lepTool.passSLCuts_mu && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_SingleMu__X__allMET, "SingleMu__X__allMET", p_passFile.c_str());
    if ( lepTool.passSLtriggers_el && jetMetTool.passAllMETTriggers && lepTool.passSLCuts_el && jetMetTool.passSLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_SingleEl__X__allMET, "SingleEl__X__allMET", p_passFile.c_str());
    */

    // * d. Dilepton stuff   
    if ( jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_allMET, "HLT_allMET", p_passFile.c_str(), true);
    // I. using only DL triggers
    // dilepton, ee
    if ((lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_DoubleEl, "HLT_DoubleEl", p_passFile.c_str());
    if ((lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts && jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_DoubleEl__X__allMET, "DoubleEl__X__allMET", p_passFile.c_str());
    // dilepton, mumu
    if ((lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_DoubleMu, "HLT_DoubleMu", p_passFile.c_str());
    if ((lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts && jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_DoubleMu__X__allMET, "DoubleMu__X__allMET", p_passFile.c_str());
    // dilepton, emu
    if ((lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_EMu, "HLT_EMu", p_passFile.c_str());
    if ((lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts && jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_EMu__X__allMET, "EMu__X__allMET", p_passFile.c_str());


    // II. using logical OR of SL and DL triggers
    // dilepton, ee
    if ((lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_DoubleEl_OR, "HLT_DoubleEl_OR", p_passFile.c_str());
    if ((lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts && jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_DoubleEl_OR__X__allMET, "DoubleEl_OR__X__allMET", p_passFile.c_str());
    // dilepton, mumu
    if ((lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_DoubleMu_OR, "HLT_DoubleMu_OR", p_passFile.c_str());
    if ((lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts && jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_DoubleMu_OR__X__allMET, "DoubleMu_OR__X__allMET", p_passFile.c_str());
    // dilepton, emu
    if ((lepTool.passSLtriggers_el || lepTool.passSLtriggers_mu || lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_EMu_OR, "HLT_EMu_OR", p_passFile.c_str());
    if ((lepTool.passSLtriggers_el || lepTool.passSLtriggers_mu || lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts && jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_EMu_OR__X__allMET, "EMu_OR__X__allMET", p_passFile.c_str());

    // ** C. print CSV lines if appropriate
    if(printCSV) printEventToCSV( eve, lepTool, jetMetTool, isDebug);
      
  }
  
  // *** 4. Make plots
  if( dumpFile) outfile->cd();
  /*
  // ** A. 2D correlations
  plot2Dcorrelations( a_HLT_IsoMu27, c0, "HLT_IsoMu27");
  plot2Dcorrelations( a_HLT_Ele35_WPTight_Gsf, c0, "HLT_Ele35_WPTight_Gsf");
  plot2Dcorrelations( a_HLT_SingleEl, c0, "HLT_SingleEl");
  plot2Dcorrelations( a_HLT_SingleMu, c0, "HLT_SingleMu");
  plot2Dcorrelations( a_HLT_DoubleEl, c0, "HLT_DoubleEl");
  plot2Dcorrelations( a_HLT_DoubleMu, c0, "HLT_DoubleMu");
  plot2Dcorrelations( a_HLT_EMu, c0, "HLT_EMu");
  plot2Dcorrelations( a_HLT_DoubleEl_OR, c0, "HLT_DoubleEl_OR");
  plot2Dcorrelations( a_HLT_DoubleMu_OR, c0, "HLT_DoubleMu_OR");
  plot2Dcorrelations( a_HLT_EMu_OR, c0, "HLT_EMu_OR");
  plot2Dcorrelations( a_HLT_IsoMu24_eta2p1, c0, "HLT_IsoMu24_eta2p1");
  plot2Dcorrelations( a_HLT_PFMET120_PFMHT120_IDTight, c0, "HLT_PFMET120_PFMHT120_IDTight");
  plot2Dcorrelations( a_HLT_PFHT250, c0, "HLT_PFHT250");
  plot2Dcorrelations( a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ, c0, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ");
  plot2Dcorrelations( a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, c0, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8");
  plot2Dcorrelations( a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8, c0, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8");
  plot2Dcorrelations( a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL, c0, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
  plot2Dcorrelations( a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, c0, "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
  plot2Dcorrelations( a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL, c0, "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL");
  plot2Dcorrelations( a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, c0, "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
  plot2Dcorrelations( a_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ, c0, "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
  plot2Dcorrelations( a_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ, c0, "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
  */
  // ** B. 1D distributions
  //plot1Dand2DHistograms( a_HLT_Ele35_WPTight_Gsf, c0, "HLT_Ele35_WPTight_Gsf");
  //plot1Dand2DHistograms( a_HLT_IsoMu27, c0, "HLT_IsoMu27");
  //plot1Dand2DHistograms( a_HLT_SingleEl, c0, "HLT_SingleEl");
  //plot1Dand2DHistograms( a_HLT_SingleMu, c0, "HLT_SingleMu");
  //plot1Dand2DHistograms( a_HLT_PFMET120_PFMHT120_IDTight, c0, "HLT_PFMET120_PFMHT120_IDTight");
  //plot1Dand2DHistograms( a_HLT_PFHT250, c0, "HLT_PFHT250");
  plot1Dand2DHistograms( a_HLT_DoubleEl, c0, "HLT_DoubleEl");
  plot1Dand2DHistograms( a_HLT_DoubleMu, c0, "HLT_DoubleMu");
  plot1Dand2DHistograms( a_HLT_EMu, c0, "HLT_EMu");
  plot1Dand2DHistograms( a_HLT_DoubleEl_OR, c0, "HLT_DoubleEl_OR");
  plot1Dand2DHistograms( a_HLT_DoubleMu_OR, c0, "HLT_DoubleMu_OR");
  plot1Dand2DHistograms( a_HLT_EMu_OR, c0, "HLT_EMu_OR");
  
  
  // ** C. 1D efficiencies
  /*makeEfficiencyHistograms( c0, a_IsoMu27__X__PFMET120_PFMHT120_IDTight, "IsoMu27__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight_muStreamSL");
  makeEfficiencyHistograms( c0, a_Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", a_HLT_PFMET120_PFMHT120_IDTight, "HLT_PFMET120_PFMHT120_IDTight_elStreamSL");
  
  makeEfficiencyHistograms( c0, a_SingleMu__X__allMET, "SingleMu__X__allMET", a_HLT_allMET, "HLT_allMET_muStreamSL");
  makeEfficiencyHistograms( c0, a_SingleEl__X__allMET, "SingleEl__X__allMET", a_HLT_allMET, "HLT_allMET_elStreamSL");*/

  makeEfficiencyHistograms( c0, a_DoubleMu__X__allMET, "DoubleMu__X__allMET", a_HLT_allMET, "HLT_allMET_muStreamDL");
  makeEfficiencyHistograms( c0, a_DoubleEl__X__allMET, "DoubleEl__X__allMET", a_HLT_allMET, "HLT_allMET_elStreamDL");
  makeEfficiencyHistograms( c0, a_EMu__X__allMET, "EMu__X__allMET", a_HLT_allMET, "HLT_allMET_emuStreamDL");
  
  makeEfficiencyHistograms( c0, a_DoubleMu_OR__X__allMET, "DoubleMu_OR__X__allMET", a_HLT_allMET, "HLT_allMET_muStreamDL");
  makeEfficiencyHistograms( c0, a_DoubleEl_OR__X__allMET, "DoubleEl_OR__X__allMET", a_HLT_allMET, "HLT_allMET_elStreamDL");
  makeEfficiencyHistograms( c0, a_EMu_OR__X__allMET, "EMu_OR__X__allMET", a_HLT_allMET, "HLT_allMET_emuStreamDL");


  if (dumpFile){
    outfile->Write();
    outfile->cd();     

    TIter next(a_Efficiencies);
    while ( TEfficiency *tEff = (TEfficiency*)next() )
      tEff->Write();
  }

  if (printCSV) closeCSVFiles();

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

