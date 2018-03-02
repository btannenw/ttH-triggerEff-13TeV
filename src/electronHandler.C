#include "../include/electronHandler.h"




electronHandler::electronHandler()
{
  passCuts = false;
  leadPt = -99;
  leadEta = -99;
  leadIndex = -99;
  nLeptons = 0;
  nElectrons = 0;
 

  h_el_cutflow = new TH1D("h_el_cutflow", "h_el_cutflow", 4, 0, 4);
  h_el_n       = new TH1D("h_el_n", "h_el_n", 6, 0, 6);
}

electronHandler::~electronHandler() {}

void electronHandler::test()
{
  std::cout << "Test!" << endl;
}

void electronHandler::applyElectronCuts()
{
  h_el_cutflow->Fill("#geq 1 Electron", 1);
  h_el_n->Fill( nElectrons );

  // Cut 1: pT > 30 GeV
  if( ev->lepton_pt_[leadIndex] > 30 ) {
    h_el_cutflow->Fill("p_{T} > 30", 1);

    // Cut 2: |ETA| < 2.4
    if( abs(ev->lepton_eta_[leadIndex]) < 2.4 ) {
      h_el_cutflow->Fill("|#eta| < 2.4", 1);

      passCuts = true;
      leadPt = ev->lepton_pt_[leadIndex];
      leadEta = ev->lepton_eta_[leadIndex];
    }
  }

}

void electronHandler::findLeadingElectron()
{

  for (unsigned int l = 0; l < ev->lepton_pt_.size() + 1; l++) {
    if (ev->lepton_isMuon_[1] == 0) { // look only at electrons
      //if (ev->lepton_pt_[l] > 30)
      nElectrons++;
      
      if (l == 0) leadIndex=l; // first electron
      else { // not first electron
	if (ev->lepton_pt_[l] > ev->lepton_pt_[leadIndex])
	  leadIndex = l;
      }
    }// electron only condition

  }

  //cout << "1. leadIndex = " << leadIndex << endl;
}

void electronHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passCuts = false;
  nLeptons = ev->lepton_pt_.size();
  nElectrons = 0;  
  h_el_cutflow->Fill("Event", 1);

  // *** 2. Start handling business! (or at least electrons)
  if (nLeptons > 0 ) {
    findLeadingElectron();
    applyElectronCuts();
  }

}
