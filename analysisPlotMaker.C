// Author:  Ben Tannenwald
// Date:    Nov 19, 2018
// Purpose: Macro to make plots for dileptonic ttH analysis

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
#include "include/analysisPlotMaker.h"
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


void analysisPlotMaker(string p_topDir="", string p_isMC="", string p_passFile="", string p_sampleTag="")
{
  // *** 0. Set style, set file, set output directory
  // ** A. Set output directory and global bools
  topDir = "plots_11-19-18/";
  if (p_topDir != "") topDir = p_topDir;

  sampleTag = "ttbar";
  if (p_sampleTag != "") sampleTag = p_sampleTag;

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
	fChain->AddFile("/uscms/home/benjtann/nobackup/sync/ttH-triggerEff-13TeV/yggdrasil_treeMaker_ttH_sync_11-06-18_v26_recipeTest.root");
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
    outfile = new TFile( (topDir + "/plotfile.root").c_str(), "RECREATE");
  else // add identifier so no overwrites
    outfile = new TFile( (topDir + "/plotfile" + p_passFile.substr(p_passFile.find_last_of("_"), p_passFile.find(".root")) ).c_str(), "RECREATE");

  // ** D. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  


  // *** 1. Define histograms and canvasses
  TCanvas *c0 = new TCanvas("c0", "c0", 50, 50, 800, 600);
  
  //initializeStackHistograms(a_ttlf,      "ttlf");
  //initializeStackHistograms(a_ttcc,      "ttcc");
  //initializeStackHistograms(a_ttbb,      "ttbb");
  //initializeStackHistograms(a_ttb,       "ttb");
  //initializeStackHistograms(a_tt2b,      "tt2b");
  //initializeStackHistograms(a_ttbar,      "ttbar");
  //initializeStackHistograms(a_vjets,     "vjets");
  //initializeStackHistograms(a_diboson,   "diboson");
  //initializeStackHistograms(a_ttv,       "ttv");
  //initializeStackHistograms(a_singleTop, "singleTop");
  
  initializeStackHistograms(a_sample, sampleTag.c_str() );

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

    // ** I. Get objects 
    lepTool.Event(eve, isDebug);
    jetMetTool.Event(eve, lepTool, isDebug);

    // ** II. Fill analysis plots
    if (sampleTag.find("ttbar")==string::npos) // simple, i.e. not ttbar
	fillStackHistogramsByChannel(lepTool, jetMetTool, a_sample, sampleTag);
    else { // split by additional jet flavor for ttbar
      if(jetMetTool.ttbarJetID == 0) 
	fillStackHistogramsByChannel(lepTool, jetMetTool, a_sample, "ttlf");
      if(jetMetTool.ttbarJetID == 41 || jetMetTool.ttbarJetID == 42 || jetMetTool.ttbarJetID == 43 || jetMetTool.ttbarJetID == 44 || jetMetTool.ttbarJetID == 45)
	fillStackHistogramsByChannel(lepTool, jetMetTool, a_sample, "ttcc");
      if(jetMetTool.ttbarJetID == 51) 
	fillStackHistogramsByChannel(lepTool, jetMetTool, a_sample, "ttb");
      if(jetMetTool.ttbarJetID == 52) 
	fillStackHistogramsByChannel(lepTool, jetMetTool, a_sample, "tt2b");
      if(jetMetTool.ttbarJetID == 53 || jetMetTool.ttbarJetID == 54 || jetMetTool.ttbarJetID == 55)
	fillStackHistogramsByChannel(lepTool, jetMetTool, a_sample, "ttbb");
    } // end ttbar splitting

  }

  // *** 4. Make plots
  //plot1Dand2DHistograms( a_HLT_EMu_OR, c0, "HLT_EMu_OR");

  // *** 5. Dump file if appropriate
  if (dumpFile){
    outfile->cd();
    outfile->Write();
    outfile->cd();     

    TIter next(a_Efficiencies);
    while ( TEfficiency *tEff = (TEfficiency*)next() )
      tEff->Write();
  }

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

