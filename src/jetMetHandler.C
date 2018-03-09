#include "../include/jetMetHandler.h"


jetMetHandler::jetMetHandler()
{
  passCuts = false;
  leadPt = -99;
  leadEta = -99;
  MET = -99;
  leadIndex = -99;
  nPreCutJets = 0;
  nJets = 0;
  passMETXtriggers = false;
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
  if ( metXTriggerBits.at(2) == '1' ) h_met_passXtriggers->Fill("HLT_PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.at(3) == '1' ) h_met_passXtriggers->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.at(4) == '1' ) h_met_passXtriggers->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.at(5) == '1' ) h_met_passXtriggers->Fill("CaloMET250_HBHECleaned", 1);
  if ( metXTriggerBits.at(6) == '1' ) h_met_passXtriggers->Fill("PFMET250_HBHECleaned", 1);
  if ( metXTriggerBits.at(7) == '1' ) h_met_passXtriggers->Fill("PFMET200_HBHE_BeamHaloCleaned", 1);

  // Fill plots
  if ( metXTriggerBits.find("10000000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.find("01000000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMETTypeOne120_PFMHT120_IDTight", 1);
  if ( metXTriggerBits.find("00100000") != string::npos ) h_met_passOnlyXtrigger->Fill("HLT_PFHT500_PFMET100_PFMHT100_IDTight", 1);
  if ( metXTriggerBits.find("00010000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT700_PFMET85_PFMHT85_IDTight", 1);
  if ( metXTriggerBits.find("00001000") != string::npos ) h_met_passOnlyXtrigger->Fill("PFHT800_PFMET75_PFMHT75_IDTight", 1);
  if ( metXTriggerBits.find("00000100") != string::npos ) h_met_passOnlyXtrigger->Fill("CaloMET250_HBHECleaned", 1);
  if ( metXTriggerBits.find("00000010") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET250_HBHECleaned", 1);
  if ( metXTriggerBits.find("00000001") != string::npos ) h_met_passOnlyXtrigger->Fill("PFMET200_HBHE_BeamHaloCleaned", 1);

  // set boolean of passing desired X triggers
  //passMETXtriggers = metXTriggerBits.find('1') != string::npos ? true : false; // pass any X trigger
  
  passMETXtriggers = metXTriggerBits.at(0) == '1' ? true : false; // pass PFMET120_PFMHT120_IDTight
  
  /*passMETXtriggers = 
    metXTriggerBits.at(0) == '1' || // pass PFMET120_PFMHT120_IDTight
    metXTriggerBits.at(1) == '1'    // pass PFMETTypeOne120_PFMHT120_IDTight
    ? true : false; 
  */
}

void jetMetHandler::applyMETCuts()
{

  // parse MET trigger logic --> store bits in string, fill inclusive/exclusive plots
  parseMETTriggerLogic();

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
  passMETXtriggers = false;
  metXTriggerBits = "";

  // *** 2. Start handling business! (or at least jets and MET)
  applyMETCuts();
  if (nPreCutJets > 0 ) {
    applyJetCuts();
  }


}
