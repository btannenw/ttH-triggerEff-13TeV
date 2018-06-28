#ifndef _leptonHandler_h
#define _leptonHandler_h

#include <TH1.h>
#include <TH2.h>

#include <iostream>
#include <vector>
#include "YggdrasilEventVars.h"


#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif

#if !defined(__CINT__) && !defined(__MAKECINT__)


#endif

class leptonHandler{

   public: 
      // Constructor(s) and destructor
      leptonHandler();
      virtual ~leptonHandler();

      // === Functions === //
      void test();
      void setFlags(bool passMC, bool passIsMuon);
      void Event(yggdrasilEventVars* eve);

      // === Variables === //
      bool passCuts;
      bool passSLCuts;
      bool passDLCuts;
      bool passSLtriggers;
      bool passDLtriggers;
      int nLeptons;
      int nMuons;
      int nElectrons;
      double leadPt_el;
      double leadEta_el;
      double subPt_el;
      double subEta_el;
      double leadPt_mu;
      double leadEta_mu;
      double subPt_mu;
      double subEta_mu;
      double lepSF;
      bool isMC;
      bool isMuon;

      // === Variables === //
      TH1D* h_mu_cutflow;
      TH1D* h_mu_event_cutflow;
      TH1D* h_mu_n;
      TH1D* h_el_cutflow;
      TH1D* h_el_event_cutflow;
      TH1D* h_el_n;

   private:
      // === Functions === //
      void applyMuonCuts();
      void applyElectronCuts();
      void checkHLTTriggers();
      void checkSLCuts();
      void setLeadSubleadIndices(int l, int& lead, int& sub);

      // === Variables === //
      yggdrasilEventVars* ev;
      int leadIndex_el;
      int subIndex_el;
      int leadIndex_mu;
      int subIndex_mu;
      bool passMC;

}; // End of class prototype




#endif // _leptonHandler_h
