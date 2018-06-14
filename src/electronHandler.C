#include "../include/electronHandler.h"




electronHandler::electronHandler()
{
  passCuts = false;
  passSLCuts = false;
  passSLtriggers = false;
  leadPt = -99;
  leadEta = -99;
  leadIndex = -99;
  nLeptons = 0;
  nElectrons = 0;
  lepSF = 1.;
  isMC = false;
  

  h_el_cutflow = new TH1D("h_el_cutflow", "h_el_cutflow", 5, 0, 5);
  h_el_event_cutflow = new TH1D("h_el_event_cutflow", "h_el_event_cutflow", 11, 0, 11);
  h_el_n       = new TH1D("h_el_n", "h_el_n", 6, 0, 6);
}

electronHandler::~electronHandler() {}

void electronHandler::test()
{
  std::cout << "Test!" << endl;
}


void electronHandler::setMCflag(bool passMC)
{
  isMC = passMC;
  if (isMC)
    std::cout << "elTool.isMC = true!" << endl;
  else
    std::cout << "elTool.isMC = false!" << endl;
}

void electronHandler::checkSLCuts()
{
  //if (nElectrons != 1)
  //passCuts = false;
  //if (passCuts && nElectrons == 1)
  //  passSLCuts = true;
  //if (nLeptons > 1 && passCuts && ev->passHLT_PFMET120_PFMHT120_IDTight_v_ && ev->MET_[0] > 125)
  //  nLeptons_int++;
  
}

void electronHandler::applyElectronCuts()
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
        
    // Cut 1: pT > 30 GeV
    if ( !(ev->lepton_pt_[l] > 40) ) continue;
    h_el_cutflow->Fill("p_{T} > 40", 1);
    if (!pass1) h_el_event_cutflow->Fill("p_{T} > 40", 1);
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

    passCuts = true;
    nElectrons++;

    // set lepton scale factors if MC
    if (isMC)
      lepSF = ev->lepton_IDSF_ * ev->lepton_IsoRecoSF_;

    // set leading lepton if appropriate
    if (ev->lepton_pt_[l] > leadPt) {
      leadPt = ev->lepton_pt_[l];
      leadEta = ev->lepton_eta_[l];
      leadIndex = l;
    }

  } // loop over electrons

  h_el_n->Fill( nElectrons );
  if (passCuts && nElectrons == 1) h_el_event_cutflow->Fill("==1 Electron", 1);
  if (passCuts && nElectrons == 1 && ev->MET_[0] > 125 && ev->passHLT_PFMET120_PFMHT120_IDTight_v_) h_el_event_cutflow->Fill("==1 Electron RefTrig", 1);
  if (passCuts && nElectrons == 1 && ev->MET_[0] > 125 && ev->passHLT_Ele35_WPTight_Gsf_v_ && ev->passHLT_PFMET120_PFMHT120_IDTight_v_) h_el_event_cutflow->Fill("==1 Electron AllTrig", 1);

}

void electronHandler::findLeadingElectron()
{

  for (unsigned int l = 0; l < ev->lepton_pt_.size() + 1; l++) {
    if (ev->lepton_isMuon_[l] == 0) { // look only at electrons
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

void electronHandler::checkHLTTriggers()
{
  passSLtriggers = ev->passHLT_Ele35_WPTight_Gsf_v_ ? true : false;
}

void electronHandler::Event(yggdrasilEventVars* eve)
{
  // *** 1. Intialize some things
  ev = eve;
  passCuts = false;
  passSLCuts = false;
  leadEta = -99;
  leadPt = -99;
  leadIndex = -99;
 
  nLeptons = ev->lepton_pt_.size();

  nElectrons = 0;  
  //h_el_cutflow->Fill("Event", 1);

  // *** 2. Check trigger
  checkHLTTriggers();

  // *** 3. Start handling business! (or at least electrons)

  if (nLeptons > 0 ) {
    //findLeadingElectron();
    applyElectronCuts();
    //checkSLCuts();
  }

}
