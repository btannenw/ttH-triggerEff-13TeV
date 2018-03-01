// Author:  Ben Tannenwald
// Date:    Mar 1, 2018
// Purpose: Header for trigger efficiency code

#include <string>
#include <TH2.h>
#include <TObjArray.h>
#include "YggdrasilEventVars.h"

yggdrasilEventVars *eve;

std::string topDir;
bool printPlots, verbose;

TObjArray* a_HLT_IsoMu27 = new TObjArray();
TObjArray* a_HLT_Ele32_WPTight_Gsf = new TObjArray();

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
    "HLT_CaloMET250_HBHECleaned" 
  };

// Lepton Triggers
// Single Muon: HLT_IsoMu27
// Single Electron: HLT_Ele32_WPTight_Gsf
