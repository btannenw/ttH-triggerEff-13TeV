#include "../include/muonHandler.h"




muonHandler::muonHandler()
{
  passCuts = false;
  passSLtriggers = false;
  passDLtriggers = false;
  leadPt = -99;
  leadEta = -99;
  leadIndex = -99;
  subPt = -99;
  subEta = -99;
  subIndex = -99;
  nLeptons = 0;
  nMuons = 0;
  lepSF = 1.;
  isMC = false;

  h_mu_cutflow = new TH1D("h_mu_cutflow", "h_mu_cutflow", 4, 0, 4);
  h_mu_n       = new TH1D("h_mu_n", "h_mu_n", 6, 0, 6);
}

muonHandler::~muonHandler() {}

void muonHandler::test()
{
  std::cout << "Test!" << endl;
}


void muonHandler::setMCflag(bool passMC)
{
  isMC = passMC;
  /*  if (isMC)
    std::cout << "muTool.isMC = true!" << endl;
  else
  std::cout << "muTool.isMC = false!" << endl;*/
}

void muonHandler::applyMuonCuts()
{
  //if (leadIndex == -99) return; // protection when no muon in event

  for (unsigned int l = 0; l < ev->lepton_pt_.size(); l++) {
  
    // Cut 0: is muon
    if ( !(ev->lepton_isMuon_[l] == 1) ) continue;
    h_mu_cutflow->Fill("Total Muons", 1);
      
    // Cut 1: pT > 30 GeV
    if ( !(ev->lepton_pt_[l] > 30) ) continue;
    h_mu_cutflow->Fill("p_{T} > 30", 1);
	
    // Cut 2: |ETA| < 2.4
    if ( !(abs(ev->lepton_eta_[l]) < 2.4) ) continue;
    h_mu_cutflow->Fill("|#eta| < 2.4", 1);
	
    // Cut 3: Isolation < 0.15
    if ( !(ev->lepton_relIso_[l] < 0.15) ) continue;
    h_mu_cutflow->Fill("Isolation < 0.15", 1);
    nMuons++;    
    passCuts = true;

    // set lepton scale factors if MC
    if (isMC) {
      lepSF = ev->lepton_IDSF_ * ev->lepton_IsoRecoSF_;
      //std::cout << "MU lepton SF!" << std::endl;
    }
    // set leading lepton if appropriate
    if (ev->lepton_pt_[l] > leadPt) {
      leadPt = ev->lepton_pt_[l];
      leadEta = ev->lepton_eta_[l];
      leadIndex = l;
    }

  } // loop over muons

  h_mu_n->Fill( nMuons );
}

void muonHandler::findLeadingMuon()
{

  for (unsigned int l = 0; l < ev->lepton_pt_.size() + 1; l++) {
    if (ev->lepton_isMuon_[l] == 1) { // look only at muons
      //if (ev->lepton_pt_[l] > 30)
      nMuons++;
      
      if (l == 0) leadIndex=l; // first muon
      else { // not first muon
	if (ev->lepton_pt_[l] > ev->lepton_pt_[leadIndex])
	  leadIndex = l;
      }
    }// muon only condition

  }

  //cout << "1. leadIndex = " << leadIndex << endl;
}

void muonHandler::checkHLTTriggers()
{
  passSLtriggers = ev->passHLT_IsoMu27_v_ ? true : false;
  passDLtriggers = ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_ || ev->passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_ ? true : false;
}

void muonHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passCuts = false;
  nLeptons = ev->lepton_pt_.size();
  leadPt = -99;
  leadEta = -99;
  leadIndex = -99;
  subPt = -99;
  subEta = -99;
  subIndex = -99;
  nMuons = 0;  
  //h_mu_cutflow->Fill("Event", 1);

  // *** 2. Check trigger
  checkHLTTriggers();

  // *** 3. Start handling business! (or at least muons)
  if (nLeptons > 0 ) {
    //findLeadingMuon();
    applyMuonCuts();
  }

}
