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


  h_jet_cutflow = new TH1D("h_jet_cutflow", "h_jet_cutflow", 4, 0, 4);
  h_jet_n       = new TH1D("h_jet_n", "h_jet_n", 12, 0, 12);
}

jetMetHandler::~jetMetHandler() {}

void jetMetHandler::test()
{
  std::cout << "Test!" << endl;
}

void jetMetHandler::applyMETCuts()
{
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

  // *** 2. Start handling business! (or at least jets and MET)
  applyMETCuts();
  if (nPreCutJets > 0 ) {
    applyJetCuts();
  }


}
