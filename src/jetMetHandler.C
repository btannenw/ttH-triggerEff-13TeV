#include "../include/jetMetHandler.h"


jetMetHandler::jetMetHandler()
{
  passLeadingJetCuts = false;
  passDLJetMetCuts = false;
  passSLJetMetCuts = false;
  leadPt = -99;
  leadEta = -99;
  leadPhi = -99;
  leadDeepCSV = -99;
  subPt = -99;
  subEta = -99;
  subPhi = -99;
  subDeepCSV = -99;
  MET = -99;
  MET_phi = -99;
  nPV = -99;
  leadIndex = -99;
  subIndex = -99;
  nPreCutJets = 0;
  nJets = 0;
  nBTags = 0;
  passOneMETTrigger = false;
  passAllMETTriggers = false;
  metXTriggerBits = "";

  h_jet_cutflow  = new TH1D("h_jet_cutflow", "h_jet_cutflow", 4, 0, 4);
  h_jet_n        = new TH1D("h_jet_n", "h_jet_n", 12, 0, 12);
  h_btags_n      = new TH1D("h_btags_n", "h_btags_n", 12, 0, 12);
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
  //MET = ev->MET_[0];
  //MET_phi = ev->MET_phi_[0];
  //MET = ev->MET_Type1xy_[0];
  //MET_phi = ev->MET_Type1xy_phi_[0];
  MET = ev->MET_Type1xy_sync_[0];
  MET_phi = ev->MET_Type1xy_phi_sync_[0];
      

}

void jetMetHandler::applyJetCuts()
{
  /*if (isDebug) {
  //  cout << "\n ev->evt = " << ev->evt_ << " and el1 has pt " << ev->lepton_pt_[lTool.leadIndex_el] << " , eta " << ev->lepton_eta_[lTool.leadIndex_el] << " , phi " << ev->lepton_phi_[lTool.leadIndex_el] << " , iso " << ev->lepton_relIso_[lTool.leadIndex_el] << endl;
  //  cout << "\n ev->evt = " << ev->evt_ << " and mu1 has pt " << ev->lepton_pt_[lTool.leadIndex_mu] << " , eta " << ev->lepton_eta_[lTool.leadIndex_mu] << " , phi " << ev->lepton_phi_[lTool.leadIndex_mu] << " , iso " << ev->lepton_relIso_[lTool.leadIndex_mu] << endl;
  }*/

  for (unsigned int j = 0; j < ev->jet_pt_[0].size(); j++){ 
    
    if (isDebug) 
      cout << "\n ev->evt = " << ev->evt_ << " and jet " << j << " has pt " << ev->jet_pt_[0][j] << " , eta " << ev->jet_eta_[0][j] << " , phi " << ev->jet_phi_[0][j] << " , csv " << (ev->jet_DeepCSV_b_[0][j]+ev->jet_DeepCSV_bb_[0][j]) << " , puid " << ev->jet_puid_[0][j] << endl;
    
    h_jet_cutflow->Fill("Jet", 1);
    // Cut 1: pT > 25 GeV
    if( !(ev->jet_pt_[0][j] > 20) ) continue;
    h_jet_cutflow->Fill("p_{T} > 20", 1);
    
    // Cut 2: |ETA| < 2.4
    if( !(abs(ev->jet_eta_[0][j]) < 2.4) ) continue;
    h_jet_cutflow->Fill("|#eta| < 2.4", 1);
    
    // Cut 3: Lepton overlap removal (R=0.4)
    if ( vetoLeptonJetOverlapRemoval(j) ) continue;
    h_jet_cutflow->Fill("dR(l, j) > 0.4", 1);
    
    // Cut 4: PU JetID
    if( !(ev->jet_puid_[0][j] == 7) ) continue;
    h_jet_cutflow->Fill("PUID Tight", 1);
  
    nJets++;

    //if (isDebug) 
    //  cout << "\n ev->evt = " << ev->evt_ << " and jet " << j << " has pt " << ev->jet_pt_[0][j] << " , eta " << ev->jet_eta_[0][j] << " , phi " << ev->jet_phi_[0][j] << " , csv " << (ev->jet_DeepCSV_b_[0][j]+ev->jet_DeepCSV_bb_[0][j]) << " , puid " << ev->jet_puid_[0][j] << endl;
    
    // set leading/subleading indices if appropriate
    setLeadSubleadIndices(j, leadIndex, subIndex);
    
    // Cut 5. DeepCSV > 0.491 (medium WP)
    if( !((ev->jet_DeepCSV_b_[0][j] + ev->jet_DeepCSV_bb_[0][j])>0.4941) ) continue;
    nBTags++;
    
  }
	
  // *** ?. Wrap-up
  h_jet_n->Fill( nJets );
  h_btags_n->Fill( nBTags );
  if (leadIndex != -99){
    leadPt      = ev->jet_pt_[0][leadIndex];
    leadEta     = ev->jet_eta_[0][leadIndex];
    leadPhi     = ev->jet_phi_[0][leadIndex];
    leadDeepCSV = ( ev->jet_DeepCSV_b_[0][leadIndex] + ev->jet_DeepCSV_bb_[0][leadIndex] );
  }
  if (subIndex != -99){
    subPt      = ev->jet_pt_[0][subIndex];
    subEta     = ev->jet_eta_[0][subIndex];
    subPhi     = ev->jet_phi_[0][subIndex];
    subDeepCSV = ( ev->jet_DeepCSV_b_[0][subIndex] + ev->jet_DeepCSV_bb_[0][subIndex] );
  }

  if (leadPt >= 30 && subPt >= 30) {
    passLeadingJetCuts = true;
    if (nJets >= 2 && nBTags >= 1 && ( ((lTool.passDLCuts_mu || lTool.passDLCuts_el) && MET>40) || (lTool.passDLCuts_emu) )) passDLJetMetCuts = true;
    if (nJets >= 4 && nBTags >= 2 && MET > 20) passSLJetMetCuts = true;
  }

  if (nJets >= 3 && ( ((lTool.passDLCuts_mu || lTool.passDLCuts_el) && MET>40) || (lTool.passDLCuts_emu) )) passDLJetMetCuts = true; // FIXME, only for trig study systematic [11-15-18]  
}

void jetMetHandler::Event(yggdrasilEventVars* eve, leptonHandler lep, bool passDebug)
{
  // *** 1. Intialize some things
  ev = eve;
  lTool = lep;
  isDebug = passDebug;
  passLeadingJetCuts = false;
  passDLJetMetCuts = false;
  passSLJetMetCuts = false;
  nPreCutJets = ev->jet_pt_[0].size();
  nJets = 0;
  nBTags = 0;
  leadIndex = -1;
  subIndex = -1;
  leadPt = -99;
  leadEta = -99;
  leadPhi = -99;
  leadDeepCSV = -99;
  subPt = -99;
  subEta = -99;
  subPhi = -99;
  subDeepCSV = -99;
  MET = -99;
  MET_phi = -99;
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

void jetMetHandler::setLeadSubleadIndices(int l, int& lead, int& sub)
{
  // *** 0. Temporary variables
  double d_leadPt = (lead == -99) ? -99 : ev->jet_pt_[0][lead];
  double d_subPt  = (sub == -99) ? -99  : ev->jet_pt_[0][sub];

  // *** 1. Set leading lepton if appropriate
  if (ev->jet_pt_[0][l] > d_leadPt) {
    // ** A. If already have first good jet, set old lead lepton to sub
    if (d_leadPt != -99){ 
      sub = lead;
    }
    // ** B. set new leading jet to leading
    lead = l;
  }
  else if (ev->jet_pt_[0][l] < d_leadPt && ev->jet_pt_[0][l] > d_subPt) { 
    // ** C. New lepton not greater than leading but greater than sub
    sub = l;
  }
  
} 

bool jetMetHandler::vetoLeptonJetOverlapRemoval(int j)
{
  double OverlapCut = 0.4;

  TLorentzVector *v_lead = new TLorentzVector();
  TLorentzVector *v_sub  = new TLorentzVector();
  TLorentzVector *v_jet  = new TLorentzVector();

  int i_lead = -99;
  int i_sub = -99;
  
  if (lTool.passDLCuts_mu) {
    i_lead = lTool.leadIndex_mu;
    i_sub  = lTool.subIndex_mu;
  }
  if (lTool.passDLCuts_el) {
    i_lead = lTool.leadIndex_el;
    i_sub  = lTool.subIndex_el;
  }
  if (lTool.passDLCuts_emu) { // take leading index from each lepton category, internal i_* name misleading in this case for i_sub
    i_lead = lTool.leadIndex_el;
    i_sub  = lTool.leadIndex_mu;
  }

  v_lead->SetPtEtaPhiE( ev->lepton_pt_[i_lead], ev->lepton_eta_[i_lead], ev->lepton_phi_[i_lead], ev->lepton_e_[i_lead] );
  v_sub->SetPtEtaPhiE( ev->lepton_pt_[i_sub], ev->lepton_eta_[i_sub], ev->lepton_phi_[i_sub], ev->lepton_e_[i_sub] );
  v_jet->SetPtEtaPhiM( ev->jet_pt_[0][j], ev->jet_eta_[0][j], ev->jet_phi_[0][j], ev->jet_m_[0][j] );

  double deltaR_j_lead = v_lead->DeltaR(*v_jet);
  double deltaR_j_sub  = v_sub->DeltaR(*v_jet);

  if( deltaR_j_lead < OverlapCut || deltaR_j_sub < OverlapCut)
    return true;
  else
    return false;

  return false;
}
