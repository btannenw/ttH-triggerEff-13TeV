#include "../include/jetMetHandler.h"


jetMetHandler::jetMetHandler()
{
  passCuts = false;
  leadPt = -99;
  leadEta = -99;
  MET = -99;
  nPV = -99;
  leadIndex = -99;
  nPreCutJets = 0;
  nJets = 0;
  passOneMETTrigger = false;
  passAllMETTriggers = false;
  metXTriggerBits = "";

  h_jet_cutflow  = new TH1D("h_jet_cutflow", "h_jet_cutflow", 4, 0, 4);
  h_jet_n        = new TH1D("h_jet_n", "h_jet_n", 12, 0, 12);
  h_met_passXtriggers = new TH1D("h_met_passXtriggers", "h_met_passXtriggers", 8, 0, 8);
  h_met_passOnlyXtrigger = new TH1D("h_met_passOnlyXtrigger", "h_met_passOnlyXtrigger", 8, 0, 8);
}

jetMetHandler::~jetMetHandler() {}

void jetMetHandler::test()
{
  std::cout << "Test!" << endl;
}

void jetMetHandler::parseMETTriggerLogic()
{

  // store all trigger bits in single string
  ev->passHLT_PFMET120_PFMHT120_IDTight_v_ ? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMETTypeOne120_PFMHT120_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_CaloMET250_HBHECleaned_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMET250_HBHECleaned_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMET200_HBHE_BeamHaloCleaned_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  
  // Fill plots
  if ( metXTriggerBits.at(0) == '1' ) h_met_passXtriggers->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.at(1) == '1' ) h_met_passXtriggers->Fill("PFMETTypeOne120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.at(2) == '1' ) h_met_passXtriggers->Fill("PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.at(3) == '1' ) h_met_passXtriggers->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.at(4) == '1' ) h_met_passXtriggers->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.at(5) == '1' ) h_met_passXtriggers->Fill("CaloMET250_HBHECleaned", 1);
  if ( metXTriggerBits.at(6) == '1' ) h_met_passXtriggers->Fill("PFMET250_HBHECleaned", 1);
  if ( metXTriggerBits.at(7) == '1' ) h_met_passXtriggers->Fill("PFMET200_HBHE_BeamHaloCleaned", 1);

  // Fill plots
  if ( metXTriggerBits.find("10000000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.find("01000000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMETTypeOne120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.find("00100000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.find("00010000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.find("00001000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.find("00000100") != string::npos ) h_met_passOnlyXtrigger->Fill("CaloMET250_HBHECleaned", 1);
  if ( metXTriggerBits.find("00000010") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET250_HBHECleaned", 1);
  if ( metXTriggerBits.find("00000001") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET200_HBHE_BeamHaloCleaned", 1);

  // set boolean of passing desired X triggers
  passAllMETTriggers = metXTriggerBits.find('1') != string::npos ? true : false; // pass any X trigger
  
  passOneMETTrigger = metXTriggerBits.at(0) == '1' ? true : false; // pass PFMET120_PFMHT120_IDTight
  
  /*passMETTriggers = 
    metXTriggerBits.at(0) == '1' || // pass PFMET120_PFMHT120_IDTight
    metXTriggerBits.at(1) == '1'    // pass PFMETTypeOne120_PFMHT120_IDTight
    ? true : false; 
  */
}


void jetMetHandler::parseMETTriggerLogic_v2() // New version (07-01-18) loosely following 5-trigger bit from 2016 à la Carmen's detective work
{

  // *** FROM CARMEN *** 07-01-18 --> triggers used in 2016 efficiency calculations
  //"HLT_PFHT300_PFMET110_v" "HLT_MET200_v" "HLT_PFMET300_v" "HLT_PFMET120_PFMHT120_IDTight_v" "HLT_PFMET170_HBHECleaned_v"

  // store all trigger bits in single string

  ev->passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMET120_PFMHT120_IDTight_v_ ? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMET250_HBHECleaned_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  
  // Fill plots
  if ( metXTriggerBits.at(0) == '1' ) h_met_passXtriggers->Fill("PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.at(1) == '1' ) h_met_passXtriggers->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.at(2) == '1' ) h_met_passXtriggers->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.at(3) == '1' ) h_met_passXtriggers->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.at(4) == '1' ) h_met_passXtriggers->Fill("PFMET250_HBHECleaned", 1);

  // Fill plots
  if ( metXTriggerBits.find("10000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.find("01000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.find("00100") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.find("00010") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.find("00001") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET250_HBHECleaned", 1);

  // set boolean of passing desired X triggers
  passAllMETTriggers = metXTriggerBits.find('1') != string::npos ? true : false; // pass any X trigger
  
  passOneMETTrigger = metXTriggerBits.at(3) == '1' ? true : false; // pass PFMET120_PFMHT120_IDTight
  
  /*passMETTriggers = 
    metXTriggerBits.at(0) == '1' || // pass PFMET120_PFMHT120_IDTight
    metXTriggerBits.at(1) == '1'    // pass PFMETTypeOne120_PFMHT120_IDTight
    ? true : false; 
  */
}


void jetMetHandler::parseMETTriggerLogic_v3() // New version (08-24-18) loosely following 5-trigger bit from 2016 à la Carmen's detective work + single HT trigger
{

  // *** FROM CARMEN *** 07-01-18 --> triggers used in 2016 efficiency calculations
  //"HLT_PFHT300_PFMET110_v" "HLT_MET200_v" "HLT_PFMET300_v" "HLT_PFMET120_PFMHT120_IDTight_v" "HLT_PFMET170_HBHECleaned_v"

  // store all trigger bits in single string

  ev->passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMET120_PFMHT120_IDTight_v_ ? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFMET250_HBHECleaned_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  ev->passHLT_PFHT250_v_? metXTriggerBits.append("1") : metXTriggerBits.append("0");
  
  // Fill plots
  if ( metXTriggerBits.at(0) == '1' ) h_met_passXtriggers->Fill("PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.at(1) == '1' ) h_met_passXtriggers->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.at(2) == '1' ) h_met_passXtriggers->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.at(3) == '1' ) h_met_passXtriggers->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.at(4) == '1' ) h_met_passXtriggers->Fill("PFMET250_HBHECleaned", 1);
  if ( metXTriggerBits.at(5) == '1' ) h_met_passXtriggers->Fill("PFHT250", 1);

  // Fill plots
  if ( metXTriggerBits.find("100000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.find("010000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.find("001000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.find("000100") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.find("000010") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET250_HBHECleaned", 1);
  if ( metXTriggerBits.find("000001") != string::npos ) h_met_passOnlyXtrigger->Fill("PFH250", 1);

  // set boolean of passing desired X triggers
  passAllMETTriggers = metXTriggerBits.find('1') != string::npos ? true : false; // pass any X trigger
  
  passOneMETTrigger = metXTriggerBits.at(3) == '1' ? true : false; // pass PFMET120_PFMHT120_IDTight
  
  /*passMETTriggers = 
    metXTriggerBits.at(0) == '1' || // pass PFMET120_PFMHT120_IDTight
    metXTriggerBits.at(1) == '1'    // pass PFMETTypeOne120_PFMHT120_IDTight
    ? true : false; 
  */
}



void jetMetHandler::applyMETCuts()
{

  // parse MET trigger logic --> store bits in string, fill inclusive/exclusive plots
  //parseMETTriggerLogic();
  parseMETTriggerLogic_v2(); // new 07-01-18
  //parseMETTriggerLogic_v3(); // new 08-24-18 // add HLT_PFHT250 to allMET collection

  // set MET publically accessible MET
  MET = ev->MET_[0];
      
}

void jetMetHandler::applyJetCuts()
{
  for (unsigned int j = 0; j < ev->jet_pt_[0].size() + 1; j++){ 
    h_jet_cutflow->Fill("Jet", 1);
    // Cut 1: pT > 25 GeV
    if( ev->jet_pt_[0][j] > 25 ) {
      h_jet_cutflow->Fill("p_{T} > 25", 1);
      
      // Cut 2: |ETA| < 3.0
      if( abs(ev->jet_eta_[0][j]) < 3.0 ) {
	h_jet_cutflow->Fill("|#eta| < 3.0", 1);
	
	passCuts = true;
	nJets++;
	
	if (leadIndex == -1) leadIndex=j; // first jet passing cuts
	else { // not first jet
	  if ( ev->jet_pt_[0][j] > ev->jet_pt_[0][leadIndex])
	    leadIndex = j;
	}
	leadPt = ev->jet_pt_[0][leadIndex];
	leadEta = ev->jet_eta_[0][leadIndex];
      } // eta cut
    } // pt cut
  } // jet loop

  h_jet_n->Fill(nJets);

}

void jetMetHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passCuts = false;
  nPreCutJets = ev->jet_pt_[0].size();
  nJets = 0;
  leadIndex = -1;
  MET = -99;
  nPV = ev->numPVs_;
  passOneMETTrigger = false;
  passAllMETTriggers = false;
  metXTriggerBits = "";

  // *** 2. Start handling business! (or at least jets and MET)
  applyMETCuts();
  if (nPreCutJets > 0 ) {
    applyJetCuts();
  }


}
