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
#include <fstream>
#include <iomanip>      // std::setprecision
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void printProgBar( int percent );
void plot2Dcorr( TCanvas*& c0, TH2D*& h0, string xtitle, string ytitle);

void syncExercise_2017data(string p_topDir="", string p_isMC="", string p_passFile="")
{
  // *** 0. Set style, set file, set output directory
  // ** A. Set output directory and global bools
  topDir = "plots_10-01-18/";
  if (p_topDir != "") topDir = p_topDir;
  isMC = true;
  if (p_isMC != "") isMC = p_isMC=="true" ? true : false;
  singleFile = true;
  printCSV = true;
  ofstream csvFile;
  fileList="";
  printPlots = false;
  dumpFile = true;
  verbose = false;
  nJetsCutSL = 0;
  nJetsCutDL = 2;
  metCutSL = 40;
  metCutDL = 40;


  // ** B. Set input file
  TChain* fChain = new TChain("ttHTreeMaker/worldTree");
  if(singleFile) { // single file
    if (p_passFile==""){ // basically a local test
      if (isMC){
	fChain->AddFile("/afs/cern.ch/work/b/btannenw/ttH/yggdrasil2018/triggerEff/localUpdates/CMSSW_9_4_0_patch1/src/ttH-LeptonPlusJets/YggdrasilTreeMaker/test/yggdrasil_treeMaker_ttH_sync_10events.root");
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

  // ** E. Print header for csv file if requested
  if (printCSV) {
    csvFile.open( (topDir + "/csv_ttH_ben.csv").c_str() );
    csvFile << "run,lumi,event,is_e,is_mu,is_ee,is_emu,is_mumu,n_jets,n_btags,lep1_pt,lep1_eta,lep1_iso,lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed,lep2_pt,lep2_eta,lep2_iso,lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed,jet1_pt,jet1_eta,jet1_phi,jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal,jet1_csv,jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed,jet2_pt,jet2_eta,jet2_phi,jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal,jet2_csv,jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed,MET_pt,MET_phi,MET_pt_phiCor,MET_phi_phiCor,mll,ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output,isSLIsoFlip,isSLMETFlip,isSLQCDRegion" << endl;
  }

  // *** 1. Define histograms and canvasses
  TCanvas *c0 = new TCanvas("c0", "c0", 50, 50, 800, 600);

  initializeHistograms(a_HLT_allMET, "HLT_allMET", true);

  initializeHistograms(a_HLT_DoubleMu_OR, "HLT_DoubleMu_OR");
  initializeHistograms(a_HLT_DoubleEl_OR, "HLT_DoubleEl_OR");
  initializeHistograms(a_HLT_EMu_OR, "HLT_EMu_OR");
  initializeHistograms(a_DoubleMu_OR__X__allMET, "DoubleMu_OR__X__allMET");
  initializeHistograms(a_DoubleEl_OR__X__allMET, "DoubleEl_OR__X__allMET");
  initializeHistograms(a_EMu_OR__X__allMET, "EMu_OR__X__allMET");
 
  // *** 2. Set tree structure and variables to read
  eve=0;
  fChain->SetBranchAddress("eve.", &eve );
  //elTool = electronHandler();
  //elTool.setMCflag(isMC);
  //muTool = muonHandler();
  //muTool.setMCflag(isMC);
  lepTool = leptonHandler();
  lepTool.setFlags(isMC, p_passFile.c_str());
  
  jetMetTool = jetMetHandler();
 
 
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
    //elTool.Event(eve);
    lepTool.Event(eve);
    jetMetTool.Event(eve, lepTool);
    
    // ** II. 2D Correlations comparing SL triggers to MET triggers
    // * A. specific triggers
    // * B. logical OR of triggers
    

    // III. Fill efficiency histograms given some trigger qualifications
    // * A. Single triggers
    // * b. Cross triggers
    // * c. Use multiple triggers defined in handlers
    if ( jetMetTool.passAllMETTriggers) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_allMET, "HLT_allMET", true);
    
    // * d. Dilepton stuff   
    // I. using only DL triggers
    // II. using logical OR of SL and DL triggers
    // dilepton, ee
    if ( (lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && jetMetTool.nJets >= nJetsCutDL && jetMetTool.MET > metCutDL) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_DoubleEl_OR, "HLT_DoubleEl_OR");
    if ( (lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) && jetMetTool.passAllMETTriggers && lepTool.passDLCuts_el && jetMetTool.nJets >= nJetsCutDL && jetMetTool.MET > metCutDL) fillEfficiencyHistograms(lepTool, jetMetTool, a_DoubleEl_OR__X__allMET, "DoubleEl_OR__X__allMET");
    // dilepton, mumu
    if ( (lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && jetMetTool.nJets >= nJetsCutDL && jetMetTool.MET > metCutDL) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_DoubleMu_OR, "HLT_DoubleMu_OR");
    if ( (lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) && jetMetTool.passAllMETTriggers && lepTool.passDLCuts_mu && jetMetTool.nJets >= nJetsCutDL && jetMetTool.MET > metCutDL) fillEfficiencyHistograms(lepTool, jetMetTool, a_DoubleMu_OR__X__allMET, "DoubleMu_OR__X__allMET");
    // dilepton, emu
    if ( (lepTool.passSLtriggers_mu || lepTool.passSLtriggers_el || lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && jetMetTool.nJets >= nJetsCutDL && jetMetTool.MET > metCutDL) fillEfficiencyHistograms(lepTool, jetMetTool, a_HLT_EMu_OR, "HLT_EMu_OR");
    if ( (lepTool.passSLtriggers_mu || lepTool.passSLtriggers_el || lepTool.passDLtriggers_emu) && jetMetTool.passAllMETTriggers && lepTool.passDLCuts_emu && jetMetTool.nJets >= nJetsCutDL && jetMetTool.MET > metCutDL) fillEfficiencyHistograms(lepTool, jetMetTool, a_EMu_OR__X__allMET, "EMu_OR__X__allMET");

    // ** IV. print CSV lines if appropriate
    if(printCSV){
      csvFile << std::fixed;
      // * a. dilepton mumu
      if ((lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && (lepTool.nElectrons + lepTool.nMuons) == 2 && (lepTool.mll < 76 || lepTool.mll > 106) && jetMetTool.MET > metCutDL && jetMetTool.nJets >=2 && jetMetTool.nBTags >= 1) {
	csvFile << eve.run_ << "," << eve.lumi_ << "," <<,eve.evt_ << "0,0,0,0,1," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4) << 
	  lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed," <<
	  lepTool.subPt_mu << "," << lepTool.subEta_mu << "," << lepTool.subRelIso_mu << ",lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed," <<
	  jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
	  jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
	  jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor,"
	  lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output,isSLIsoFlip,isSLMETFlip,isSLQCDRegion" << endl;
      } // end mumu
      // * a. dilepton ee
      else if ((lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && (lepTool.nElectrons + lepTool.nMuons) == 2 && (lepTool.mll < 76 || lepTool.mll > 106) && jetMetTool.MET > metCutDL && jetMetTool.nJets >=2 && jetMetTool.nBTags >= 1) {
	csvFile << eve.run_ << "," << eve.lumi_ << "," <<,eve.evt_ << "0,0,1,0,0," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4) << 
	  lepTool.leadPt_el << "," << lepTool.leadEta_el << "," << lepTool.leadRelIso_el << ",lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed," <<
	  lepTool.subPt_el << "," << lepTool.subEta_el << "," << lepTool.subRelIso_el << ",lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed," <<
	  jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
	  jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
	  jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor,"
	  lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output,isSLIsoFlip,isSLMETFlip,isSLQCDRegion" << endl;
      }
      // * c. dilepton emu
      else if ((lepTool.passSLtriggers_el || lepTool.passSLtriggers_mu || lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && (lepTool.nElectrons + lepTool.nMuons) == 2 && jetMetTool.nJets >=2 && jetMetTool.nBTags >= 1) {
	csvFile << eve.run_ << "," << eve.lumi_ << "," <<,eve.evt_ << "0,0,0,1,0," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4);
	if(lepTool.leadPt_el > lepTool.leadPt_mu) { // electron is leading lepton
	  csvFile << lepTool.leadPt_el << "," << lepTool.leadEta_el << "," << lepTool.leadRelIso_el << ",lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed,";
	  csvFile << lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed,";
	}
	else { // muon is leading lepton
	  csvFile << lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed,";
	  csvFile << lepTool.leadPt_el << "," << lepTool.leadEta_el << "," << lepTool.leadRelIso_el << ",lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed,";
	}
	csvFile << jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
	  jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
	  jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor,"
	  lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output,isSLIsoFlip,isSLMETFlip,isSLQCDRegion" << endl;
      }    }// end CSV option
  } // end event loop
  
  // *** 4. Make plots
  if( dumpFile) outfile->cd();
  // ** A. 2D correlations 
  // ** B. 1D distributions
  plot1Dand2DHistograms( a_HLT_DoubleEl_OR, c0, "HLT_DoubleEl_OR");
  plot1Dand2DHistograms( a_HLT_DoubleMu_OR, c0, "HLT_DoubleMu_OR");
  plot1Dand2DHistograms( a_HLT_EMu_OR, c0, "HLT_EMu_OR");

  
  // ** C. 1D efficiencies
  makeEfficiencyHistograms( c0, a_DoubleMu_OR__X__allMET, "DoubleMu_OR__X__allMET", a_HLT_allMET, "HLT_allMET_muStreamDL");
  makeEfficiencyHistograms( c0, a_DoubleEl_OR__X__allMET, "DoubleEl_OR__X__allMET", a_HLT_allMET, "HLT_allMET_elStreamDL");
  makeEfficiencyHistograms( c0, a_EMu_OR__X__allMET, "EMu_OR__X__allMET", a_HLT_allMET, "HLT_allMET_emuStreamDL");

  
  // *** 5. Wrap up files
  if (dumpFile){
    outfile->Write();
    outfile->cd();     

    TIter next(a_Efficiencies);
    while ( TEfficiency *tEff = (TEfficiency*)next() )
      tEff->Write();
  }
  if (printCSV)
    csvFile.close();

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

