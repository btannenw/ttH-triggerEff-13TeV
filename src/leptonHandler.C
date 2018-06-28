#include "../include/leptonHandler.h"


leptonHandler::leptonHandler()
{
  passCuts = false;
  passSLtriggers = false;
  passDLtriggers = false;
  nLeptons = 0;
  nMuons = 0;
  leadPt_mu = -99;
  leadEta_mu = -99;
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

  lepSF = 1.;
  isMC = false;
  isMuon = false;

  h_mu_cutflow = new TH1D("h_mu_cutflow", "h_mu_cutflow", 5, 0, 5);
  h_mu_event_cutflow = new TH1D("h_mu_event_cutflow", "h_mu_event_cutflow", 5, 0, 5);
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


void leptonHandler::setFlags(bool passMC, bool passIsMuon)
{
  isMC = passMC;
  /*  if (isMC)
    std::cout << "muTool.isMC = true!" << endl;
  else
  std::cout << "muTool.isMC = false!" << endl;*/
  isMuon = passIsMuon;
  
}

void leptonHandler::applyMuonCuts()
{
  //if (leadIndex == -99) return; // protection when no muon in event

  for (unsigned int l = 0; l < ev->lepton_pt_.size(); l++) {
  
    // Cut 0: is muon
    if ( !(ev->lepton_isMuon_[l] == 1) ) continue;
    h_mu_cutflow->Fill("Total Muons", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Total Muons", 1);
    
    // Cut 1: pT > 30 GeV
    if ( !(ev->lepton_pt_[l] > 30) ) continue;
    h_mu_cutflow->Fill("p_{T} > 30", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("p_{T} > 30", 1);
	
    // Cut 2: |ETA| < 2.4
    if ( !(abs(ev->lepton_eta_[l]) < 2.4) ) continue;
    h_mu_cutflow->Fill("|#eta| < 2.4", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("|#eta| < 2.4", 1);

    // Cut 3: Isolation < 0.15
    if ( !(ev->lepton_relIso_[l] < 0.25) ) continue;
    h_mu_cutflow->Fill("Isolation < 0.25", 1);
    if (leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Isolation < 0.25", 1);
    nMuons++;    
    passCuts = true;

    // Cut 4: Trigger
    if ( ev->passHLT_IsoMu27_v_ && leadIndex_mu == -99)     h_mu_event_cutflow->Fill("Trigger", 1);

    // set lepton scale factors if MC
    if (isMC && lepSF!=1) {
      lepSF = ev->lepton_IDSF_ * ev->lepton_IsoRecoSF_;
      //std::cout << "MU lepton SF!" << std::endl;
    }

    // set leading/subleading indices if appropriate
    setLeadSubleadIndices(l, leadIndex_mu, subIndex_mu);
    //std::cout << "lepton pt = " << ev->lepton_pt_[l] << std::endl;

    /*
    if (ev->lepton_pt_[l] > leadPt) {
      if (leadPt != -99){ // if already have first good lepton, set old lead lepton to sub
	subIndex = leadIndex;
	subPt = ev->lepton_pt_[subIndex];
	subEta = ev->lepton_eta_[subIndex];
      }
      // set new leading lepton to leading
      leadPt = ev->lepton_pt_[l];
      leadEta = ev->lepton_eta_[l];
      leadIndex = l;
    }
    else if (ev->lepton_pt_[l] < leadPt && ev->lepton_pt_[l] > subPt) { // new lepton not greater than leading but greater than sub
      subIndex = l;
      subPt = ev->lepton_pt_[l];
      subEta = ev->lepton_eta_[l];
    }
    */
  } // loop over muons

  // *** ?. Wrap-up
  h_mu_n->Fill( nMuons );
  if (leadIndex_mu != -99){
    leadPt_mu = ev->lepton_pt_[leadIndex_mu];
    leadEta_mu = ev->lepton_eta_[leadIndex_mu];
  }
  if (subIndex_mu != -99){
    subPt_mu = ev->lepton_pt_[subIndex_mu];
    subEta_mu = ev->lepton_eta_[subIndex_mu];
  }

  if (nMuons > 0 && leadIndex_mu == -99)
    cout << "NAHHHH" << endl;

  if (subPt_mu > leadPt_mu)
    cout << "subPt_mu: " << subPt_mu << "\tleadPt_mu: " << leadPt_mu << endl;

}

void leptonHandler::setLeadSubleadIndices(int l, int& lead, int& sub)
{
  // *** 0. Temporary variables
  double leadPt = ev->lepton_pt_[lead];
  double subPt = ev->lepton_pt_[sub];

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


void leptonHandler::checkSLCuts()
{
  //if (nElectrons != 1)
  //passCuts = false;
  //if (passCuts && nElectrons == 1)
  //  passSLCuts = true;
  //if (nLeptons > 1 && passCuts && ev->passHLT_PFMET120_PFMHT120_IDTight_v_ && ev->MET_[0] > 125)
  //  nLeptons_int++;
  
}


void leptonHandler::checkHLTTriggers()
{
  passSLtriggers = ev->passHLT_IsoMu27_v_ ? true : false;
  passDLtriggers = ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_ || ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_ ? true : false;
}

void leptonHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passCuts = false;
  nLeptons = ev->lepton_pt_.size();
  nMuons = 0;  
  leadPt_mu = -99;
  leadEta_mu = -99;
  leadIndex_mu = -99;
  subPt_mu = -99;
  subEta_mu = -99;
  subIndex_mu = -99;
  nElectrons = -99;
  leadPt_el = -99;
  leadEta_el = -99;
  leadIndex_el = -99;
  subPt_el = -99;
  subEta_el = -99;
  subIndex_el = -99;

  //h_mu_cutflow->Fill("Event", 1);

  // *** 2. Check trigger
  checkHLTTriggers();

  // *** 3. Start handling business! (or at least muons)
  if (nLeptons > 0 ) {
    //findLeadingMuon();
    applyMuonCuts();
  }

}
