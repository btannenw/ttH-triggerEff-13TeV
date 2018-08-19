#include "../include/leptonHandler.h"


leptonHandler::leptonHandler()
{
  passSLCuts_el = false;
  passSLCuts_mu = false;
  passDLCuts_el = false;
  passDLCuts_mu = false;
  passDLCuts_emu = false;
  passSLtriggers_el = false;
  passSLtriggers_mu = false;
  passDLtriggers_el = false;
  passDLtriggers_mu = false;
  passDLtriggers_emu = false;
  nLeptons = 0;
  nMuons = 0;
  leadPt_mu = -99;
  leadEta_mu = -99;
  leadRelIso_mu = -99;
  leadIndex_mu = -99;
  subPt_mu = -99;
  subEta_mu = -99;
  subIndex_mu = -99;
  nElectrons = 0;
  leadPt_el = -99;
  leadEta_el = -99;
  leadIndex_el = -99;
  subPt_el = -99;
  subEta_el = -99;
  subIndex_el = -99;
  mll = -99;

  lepSF = 1.;
  isMC = false;
  dataPeriod = "";
  b_periodDep__HLT_IsoMu24_2p1 = false;
  b_periodDep__doubleMu_noMass = false;
  b_periodDep__doubleMu_withMass = false;


  h_mu_cutflow = new TH1D("h_mu_cutflow", "h_mu_cutflow", 6, 0, 6);
  h_mu_event_cutflow = new TH1D("h_mu_event_cutflow", "h_mu_event_cutflow", 6, 0, 6);
  h_mu_n       = new TH1D("h_mu_n", "h_mu_n", 6, 0, 6);
  h_el_cutflow = new TH1D("h_el_cutflow", "h_el_cutflow", 5, 0, 5);
  h_el_event_cutflow = new TH1D("h_el_event_cutflow", "h_el_event_cutflow", 11, 0, 11);
  h_el_n       = new TH1D("h_el_n", "h_el_n", 6, 0, 6);
}

leptonHandler::~leptonHandler() {}

void leptonHandler::test()
{
  std::cout << "Test!" << endl;
}


void leptonHandler::setFlags(bool passMC, string inputFile)
{
  isMC = passMC;
  /*  if (isMC)
    std::cout << "muTool.isMC = true!" << endl;
    else
    std::cout << "muTool.isMC = false!" << endl;*/
  if (!isMC){
    if (inputFile.find("Run2017A") != string::npos) dataPeriod = "A";
    else if (inputFile.find("Run2017B") != string::npos) dataPeriod = "B";
    else if (inputFile.find("Run2017C") != string::npos) dataPeriod = "C";
    else if (inputFile.find("Run2017D") != string::npos) dataPeriod = "D";
    else if (inputFile.find("Run2017E") != string::npos) dataPeriod = "E";
    else if (inputFile.find("Run2017F") != string::npos) dataPeriod = "F";
    else                                                 dataPeriod = "";
  }
  
}

void leptonHandler::applyMuonCuts()
{
  //if (leadIndex == -99) return; // protection when no muon in event

  for (unsigned int l = 0; l < ev->lepton_pt_.size(); l++) {
  
    // Cut 0: is muon
    if ( !(ev->lepton_isMuon_[l] == 1) ) continue;
    h_mu_cutflow->Fill("Total Muons", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Total Muons", 1);
    
    // Cut 1: pT > 15 GeV --> use SL veto as threshold for counting muons. Apply higher pT cut later
    if ( !(ev->lepton_pt_[l] > 15) ) continue;
    h_mu_cutflow->Fill("p_{T} > 15", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("p_{T} > 15", 1);
	
    // Cut 2: |ETA| < 2.4
    if ( !(abs(ev->lepton_eta_[l]) < 2.4) ) continue;
    h_mu_cutflow->Fill("|#eta| < 2.4", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("|#eta| < 2.4", 1);

    // Cut 3: tight ID
    if ( !(ev->lepton_isTight_[l] == 1) ) continue;
    h_mu_cutflow->Fill("Tight", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Tight", 1);


    // Cut 4: Isolation < 0.25
    if ( !(ev->lepton_relIso_[l] < 0.25) ) continue;
    h_mu_cutflow->Fill("Isolation < 0.25", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Isolation < 0.25", 1);
    nMuons++;    

    // Cut 5: Trigger
    if ( ev->passHLT_IsoMu27_v_ && leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Trigger", 1);

    // set lepton scale factors if MC
    if (isMC && lepSF!=1) {
      lepSF = ev->lepton_IDSF_ * ev->lepton_IsoRecoSF_;
      //std::cout << "MU lepton SF!" << std::endl;
    }

    // set leading/subleading indices if appropriate
    setLeadSubleadIndices(l, leadIndex_mu, subIndex_mu);
    //std::cout << "lepton pt = " << ev->lepton_pt_[l] << std::endl;

  } // loop over muons

  // *** ?. Wrap-up
  h_mu_n->Fill( nMuons );
  if (leadIndex_mu != -99){
    leadPt_mu = ev->lepton_pt_[leadIndex_mu];
    leadEta_mu = ev->lepton_eta_[leadIndex_mu];
    leadRelIso_mu = ev->lepton_relIso_[leadIndex_mu];
  }
  if (subIndex_mu != -99){
    subPt_mu = ev->lepton_pt_[subIndex_mu];
    subEta_mu = ev->lepton_eta_[subIndex_mu];
  }

  if (nMuons > 0 && leadIndex_mu == -99)
    cout << "applyMuonCuts: NAHHHH" << endl;

  if (subPt_mu > leadPt_mu)
    cout << "subPt_mu: " << subPt_mu << "\tleadPt_mu: " << leadPt_mu << endl;

}

void leptonHandler::applyElectronCuts()
{
  //if (leadIndex == -99) return; // protection when no electron in event
  //bool pass0, pass1, pass2, pass3, pass4 = false;
  bool pass0 = false;
  bool pass1 = false;
  bool pass2 = false;
  bool pass3 = false;
  bool pass4 = false;
  bool pass5 = false;
  bool pass6 = false;
  bool pass7 = false;

  for (unsigned int l = 0; l < ev->lepton_pt_.size(); l++) {
    //if ( ev->lepton_pt_.size() == 0) continue ; // protection against events w/o leptons
    //if ( l > 0) continue ; // only for testing

     // Cut 0: Is electron
    if ( !(ev->lepton_isMuon_[l] == 0) ) continue;
    h_el_cutflow->Fill("Total Electrons", 1);
    if (!pass0) h_el_event_cutflow->Fill("Total Electrons", 1);
    pass0 = true;
        
    // Cut 1: pT > 15 GeV
    if ( !(ev->lepton_pt_[l] > 15) ) continue;
    h_el_cutflow->Fill("p_{T} > 15", 1);
    if (!pass1) h_el_event_cutflow->Fill("p_{T} > 15", 1);
    pass1 = true;
    
    // Cut 2: |ETA| < 2.4
    if ( !(abs(ev->lepton_eta_[l]) < 2.4) ) continue;
    h_el_cutflow->Fill("|#eta| < 2.4", 1);
    if (!pass2) h_el_event_cutflow->Fill("|#eta| < 2.4", 1);
    pass2 = true;
    
    // Cut 3: Reject cluster |ETA| in crack ( < 1.4442, > 1.566 )
    if ( !(abs(ev->lepton_scEta_[l]) < 1.4442 || abs(ev->lepton_scEta_[l]) > 1.566) ) continue;
    h_el_cutflow->Fill("Reject Crack Cluster #eta", 1);
    if (!pass3) h_el_event_cutflow->Fill("Reject Crack Cluster #eta", 1);
    pass3 = true;
    
    // Cut 4: tight ID
    if ( !(ev->lepton_isTight_[l] == 1) ) continue;
    h_el_cutflow->Fill("Tight ID", 1);
    if (!pass4) h_el_event_cutflow->Fill("Tight ID", 1);
    pass4 = true;
    
    // Trigger cuts 
    if ( ev->passHLT_Ele35_WPTight_Gsf_v_ ) {
      if (!pass5) h_el_event_cutflow->Fill("El Trigger", 1);
      pass5 = true;
    }
    if ( ev->passHLT_PFMET120_PFMHT120_IDTight_v_ ) {
      if (!pass6) h_el_event_cutflow->Fill("MET Trigger", 1);
      pass6 = true;
    }
    if ( ev->passHLT_Ele35_WPTight_Gsf_v_ && ev->passHLT_PFMET120_PFMHT120_IDTight_v_ ) {
      if (!pass7) h_el_event_cutflow->Fill("MET+Lep Trigger", 1);
      pass7 = true;
    }

    nElectrons++;

    // set lepton scale factors if MC
    if (isMC && lepSF != 1) {
      lepSF = ev->lepton_IDSF_ * ev->lepton_IsoRecoSF_;
      //std::cout << "EL lepton SF!" << std::endl;
    }

    // set leading/subleading indices if appropriate
    setLeadSubleadIndices(l, leadIndex_el, subIndex_el);
    
  } // loop over electrons

  h_el_n->Fill( nElectrons );
  if (nElectrons == 1) h_el_event_cutflow->Fill("==1 Electron", 1);
  if (nElectrons == 1 && ev->MET_[0] > 125 && ev->passHLT_PFMET120_PFMHT120_IDTight_v_) h_el_event_cutflow->Fill("==1 Electron RefTrig", 1);
  if (nElectrons == 1 && ev->MET_[0] > 125 && ev->passHLT_Ele35_WPTight_Gsf_v_ && ev->passHLT_PFMET120_PFMHT120_IDTight_v_) h_el_event_cutflow->Fill("==1 Electron AllTrig", 1);
  if (leadIndex_el != -99){
    leadPt_el = ev->lepton_pt_[leadIndex_el];
    leadEta_el = ev->lepton_eta_[leadIndex_el];
  }
  if (subIndex_el != -99){
    subPt_el = ev->lepton_pt_[subIndex_el];
    subEta_el = ev->lepton_eta_[subIndex_el];
  }

  if (nElectrons > 0 && leadIndex_el == -99)
    cout << "applyElectronCuts: NAHHHH" << endl;

  if (subPt_el > leadPt_el)
    cout << "subPt_el: " << subPt_el << "\tleadPt_el: " << leadPt_el << endl;

}


void leptonHandler::setLeadSubleadIndices(int l, int& lead, int& sub)
{
  // *** 0. Temporary variables
  double leadPt = (lead == -99) ? -99 : ev->lepton_pt_[lead];
  double subPt  = (sub == -99) ? -99  : ev->lepton_pt_[sub];

  // *** 1. Set leading lepton if appropriate
  if (ev->lepton_pt_[l] > leadPt) {
    // ** A. If already have first good lepton, set old lead lepton to sub
    if (leadPt != -99){ 
      sub = lead;
    }
    // ** B. set new leading lepton to leading
    lead = l;
  }
  else if (ev->lepton_pt_[l] < leadPt && ev->lepton_pt_[l] > subPt) { 
    // ** C. New lepton not greater than leading but greater than sub
    sub = l;
  }
  
} 


void leptonHandler::checkCategoryCuts()
{
  if (nMuons == 1 && nElectrons==0 && leadPt_mu >= 30 && leadRelIso_mu < 0.15){
    passSLCuts_mu = true;
    //cout << "IT'S HAPPENING!!!!!!" << endl;
  }
  if (nMuons >= 2 && leadPt_mu >= 25 && subPt_mu >= 15){
    mll = calculateDileptonMass(leadIndex_mu, subIndex_mu);
    if (mll > 20)
      passDLCuts_mu = true;
  }

  if (nElectrons == 1 && nMuons == 0 && leadPt_el >= 40)
    passSLCuts_el = true;

  if (nElectrons >= 2 && leadPt_el >= 25 && subPt_el >= 15){
    mll = calculateDileptonMass(leadIndex_el, subIndex_el);
    if (mll > 20)
      passDLCuts_el = true;
    //cout << "HEY! You're part of it!! (passDLCuts_el)" << endl;
  }

  if ( (nMuons>=1 && nElectrons>=1) && (leadPt_mu >= 25 && leadPt_el >= 15) || (leadPt_el >= 25 && leadPt_mu >= 15)){
    //if ( (leadPt_mu >= 25 && leadPt_el >= 15) || (leadPt_el >= 25 && leadPt_mu >= 15)){
    mll = calculateDileptonMass(leadIndex_mu, leadIndex_el);
    if (mll > 20)
      passDLCuts_emu = true;
    //if (passDLtriggers_emu)
    //  cout << "HEY! You're part of it!! (passDLCuts_emu)" << endl;
  }
  
}

double leptonHandler::calculateDileptonMass(int index_lead, int index_sub)
{
  TLorentzVector *l1 = new TLorentzVector();
  TLorentzVector *l2 = new TLorentzVector();
  l1->SetPtEtaPhiE( ev->lepton_pt_[index_lead], ev->lepton_eta_[index_lead], ev->lepton_phi_[index_lead], ev->lepton_e_[index_lead] );
  l2->SetPtEtaPhiE( ev->lepton_pt_[index_sub], ev->lepton_eta_[index_sub], ev->lepton_phi_[index_sub], ev->lepton_e_[index_sub] );

  TLorentzVector *ll = new TLorentzVector();
  *ll = *l1 + *l2;

  return ll->M();

}


void leptonHandler::checkHLTTriggers()
{
  b_periodDep__HLT_IsoMu24_2p1   = isMC || (!isMC && (dataPeriod == "A" || dataPeriod == "B" || dataPeriod == "C" || dataPeriod == "D") ) ? true : false; // should be true if MC || if is data and period is A, B, C, or D
  b_periodDep__doubleMu_noMass   = isMC || (!isMC && (dataPeriod == "A" || dataPeriod == "B") ) ? true : false; // should be true if MC || if sample is data and period is A or B
  b_periodDep__doubleMu_withMass = isMC || (!isMC && (dataPeriod == "C" || dataPeriod == "D" || dataPeriod == "E" || dataPeriod == "F") ) ? true : false; // should be true if MC || if sample is data and period is C, D, E, or F
  
  // muon triggers
  passSLtriggers_mu = ev->passHLT_IsoMu27_v_ || (ev->passHLT_IsoMu24_2p1_v_ && b_periodDep__HLT_IsoMu24_2p1) ? true : false;
  passDLtriggers_mu = (ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_ && b_periodDep__doubleMu_noMass) || ((ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_ || ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_) && b_periodDep__doubleMu_withMass) ? true : false;
  
  /*
  // **** WARNING: SET ALL True FOR TESTING ONLY *** [FIXME]
  bool b_periodDep__HLT_IsoMu24_2p1   = true;
  bool b_periodDep__doubleMu_noMass   = true;
  bool b_periodDep__doubleMu_withMass = true;
  */
  /*
  // **** CLASSIC --> no period dep
  // muon triggers
  passSLtriggers_mu = ev->passHLT_IsoMu27_v_ || ev->passHLT_IsoMu24_2p1_v_ ? true : false;
  passDLtriggers_mu = ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_ || ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_ || ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_ ? true : false;
  */
  // electron triggers
  passSLtriggers_el = ev->passHLT_Ele35_WPTight_Gsf_v_ ? true : false;
  passDLtriggers_el = ev->passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_ || ev->passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ ? true : false;
 
  //if (passDLtriggers_el)
  //  cout << "HEY! You're part of it!! (DLtriggers_el)" << endl;

  passDLtriggers_emu = ev->passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_ || ev->passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ || ev->passHLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ || ev->passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ ? true : false;

  //if (passDLtriggers_emu)
  //  cout << "HEY! You're part of it!! (DLtriggers_emu)" << endl;

}

void leptonHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passSLCuts_el = false;
  passSLCuts_mu = false;
  passDLCuts_el = false;
  passDLCuts_mu = false;
  passDLCuts_emu = false;
  nLeptons = ev->lepton_pt_.size();
  nMuons = 0;  
  leadPt_mu = -99;
  leadEta_mu = -99;
  leadRelIso_mu = -99;
  leadIndex_mu = -99;
  subPt_mu = -99;
  subEta_mu = -99;
  subIndex_mu = -99;
  nElectrons = 0;
  leadPt_el = -99;
  leadEta_el = -99;
  leadIndex_el = -99;
  subPt_el = -99;
  subEta_el = -99;
  subIndex_el = -99;
  mll = -99;
  b_periodDep__HLT_IsoMu24_2p1 = false;
  b_periodDep__doubleMu_noMass = false;
  b_periodDep__doubleMu_withMass = false;

  //h_mu_cutflow->Fill("Event", 1);

  // *** 2. Check trigger
  checkHLTTriggers();

  // *** 3. Start handling business! (or at least muons)
  if (nLeptons > 0 ) {
    // ** A. Apply object selection
    applyMuonCuts();
    applyElectronCuts();
    // ** B. Check what category an event falls into: el/mu, SL/DL
    checkCategoryCuts();
  }

}
