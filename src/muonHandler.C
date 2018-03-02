#include "../include/muonHandler.h"




muonHandler::muonHandler()
{
  passCuts = false;
  leadPt = -99;
  leadEta = -99;
  leadIndex = -99;
  nLeptons = 0;
  nMuons = 0;
 

  h_mu_cutflow = new TH1D("h_mu_cutflow", "h_mu_cutflow", 4, 0, 4);
  h_mu_n       = new TH1D("h_mu_n", "h_mu_n", 6, 0, 6);
}

muonHandler::~muonHandler() {}

void muonHandler::test()
{
  std::cout << "Test!" << endl;
}

void muonHandler::applyMuonCuts()
{
  h_mu_cutflow->Fill("#geq 1 Muon", 1);
  h_mu_n->Fill( nMuons );

  // Cut 1: pT > 30 GeV
  if( ev->lepton_pt_[leadIndex] > 30 ) {
    h_mu_cutflow->Fill("p_{T} > 30", 1);

    // Cut 2: |ETA| < 2.4
    if( abs(ev->lepton_eta_[leadIndex]) < 2.4 ) {
      h_mu_cutflow->Fill("|#eta| < 2.4", 1);

      passCuts = true;
      leadPt = ev->lepton_pt_[leadIndex];
      leadEta = ev->lepton_eta_[leadIndex];
    }
  }

}

void muonHandler::findLeadingMuon()
{

  for (unsigned int l = 0; l < ev->lepton_pt_.size() + 1; l++) {
    if (ev->lepton_isMuon_[1] == 1) { // look only at muons
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

void muonHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passCuts = false;
  nLeptons = ev->lepton_pt_.size();
  nMuons = 0;  
  h_mu_cutflow->Fill("Event", 1);

  // *** 2. Start handling business! (or at least muons)
  if (nLeptons > 0 ) {
    findLeadingMuon();
    applyMuonCuts();
  }

}
