// Author:  Ben Tannenwald
// Date:    Mar 1, 2018
// Purpose: Header for trigger efficiency code

#include <string>
#include <TH2.h>
#include <TFile.h>
#include <TObjArray.h>
#include "YggdrasilEventVars.h"
#include "leptonHandler.h"
#include "jetMetHandler.h"

// top level vars
std::string topDir, fileList;
bool printPlots, dumpFile, verbose, isMC, singleFile, printCSV, isDebug;
int nJetsCutSL;
int nJetsCutDL;
int metCutSL;
int metCutDL;

// user tools
yggdrasilEventVars *eve;
//electronHandler elTool;
//muonHandler muTool;
leptonHandler lepTool;
jetMetHandler jetMetTool;


// user file
TFile* outfile = new TFile();

// user vars
TObjArray* a_HLT_IsoMu27 = new TObjArray();
TObjArray* a_HLT_IsoMu24_eta2p1 = new TObjArray();
TObjArray* a_HLT_Ele35_WPTight_Gsf = new TObjArray();
TObjArray* a_HLT_PFMET120_PFMHT120_IDTight = new TObjArray();
TObjArray* a_HLT_PFHT250 = new TObjArray();
TObjArray* a_IsoMu27__X__PFMET120_PFMHT120_IDTight = new TObjArray();
TObjArray* a_Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight = new TObjArray();

TObjArray* a_HLT_SingleMu = new TObjArray();
TObjArray* a_HLT_SingleEl = new TObjArray();
TObjArray* a_HLT_allMET = new TObjArray();
TObjArray* a_SingleMu__X__allMET = new TObjArray();
TObjArray* a_SingleEl__X__allMET = new TObjArray();

TObjArray* a_HLT_DoubleMu = new TObjArray();
TObjArray* a_HLT_DoubleEl = new TObjArray();
TObjArray* a_HLT_EMu = new TObjArray();
TObjArray* a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = new TObjArray();
TObjArray* a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 = new TObjArray();
TObjArray* a_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8 = new TObjArray();
TObjArray* a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL = new TObjArray();
TObjArray* a_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = new TObjArray();
TObjArray* a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = new TObjArray();
TObjArray* a_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = new TObjArray();
TObjArray* a_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ = new TObjArray();
TObjArray* a_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ = new TObjArray();

TObjArray* a_DoubleMu__X__allMET = new TObjArray();
TObjArray* a_DoubleEl__X__allMET = new TObjArray();
TObjArray* a_EMu__X__allMET = new TObjArray();

TObjArray* a_HLT_DoubleMu_OR = new TObjArray();
TObjArray* a_HLT_DoubleEl_OR = new TObjArray();
TObjArray* a_HLT_EMu_OR = new TObjArray();
TObjArray* a_DoubleMu_OR__X__allMET = new TObjArray();
TObjArray* a_DoubleEl_OR__X__allMET = new TObjArray();
TObjArray* a_EMu_OR__X__allMET = new TObjArray();


TObjArray* a_Efficiencies = new TObjArray();

std::vector <std::string> metTriggers = 
  {
    "HLT_PFHT500_PFMET100_PFMHT100_IDTight", 
    "HLT_PFHT500_PFMET110_PFMHT110_IDTight", 
    "HLT_PFHT700_PFMET85_PFMHT85_IDTight", 
    "HLT_PFHT700_PFMET95_PFMHT95_IDTight", 
    "HLT_PFHT800_PFMET75_PFMHT75_IDTight", 
    "HLT_PFHT800_PFMET85_PFMHT85_IDTight", 
    "HLT_CaloMET300_HBHECleaned", 
    "HLT_CaloMET350_HBHECleaned", 
    "HLT_PFMET250_HBHECleaned", 
    "HLT_PFMET300_HBHECleaned", 
    "HLT_PFMET200_HBHE_BeamHaloCleaned", 
    "HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned", 
    "HLT_MET105_IsoTrk50", 
    "HLT_MET120_IsoTrk50", 
    "HLT_PFMET120_PFMHT120_IDTight", 
    "HLT_PFMET130_PFMHT130_IDTight", 
    "HLT_PFMET140_PFMHT140_IDTight", 
    "HLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1", 
    "HLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1", 
    "HLT_DiJet110_35_Mjj650_PFMET110", 
    "HLT_DiJet110_35_Mjj650_PFMET120", 
    "HLT_DiJet110_35_Mjj650_PFMET130", 
    "HLT_TripleJet110_35_35_Mjj650_PFMET110", 
    "HLT_TripleJet110_35_35_Mjj650_PFMET120", 
    "HLT_TripleJet110_35_35_Mjj650_PFMET130", 
    "HLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1", 
    "HLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1", 
    "HLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1", 
    "HLT_PFMET120_PFMHT120_IDTight_PFHT60", 
    "HLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60", 
    "HLT_PFMETTypeOne120_PFMHT120_IDTight", 
    "HLT_PFMETTypeOne130_PFMHT130_IDTight", 
    "HLT_PFMETTypeOne140_PFMHT140_IDTight", 
    "HLT_CaloMET250_HBHECleaned", 
    "HLT_PFHT250", 
    "HLT_allMET" 
   };

// Lepton Triggers
// Single Muon: HLT_IsoMu27
// Single Electron: HLT_Ele35_WPTight_Gsf
