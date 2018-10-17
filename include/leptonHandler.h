#ifndef _leptonHandler_h
#define _leptonHandler_h

#include <TH1.h>
#include <TH2.h>
#include <TLorentzVector.h>

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
      void setFlags(bool passMC, string inputFile);
      void Event(yggdrasilEventVars* eve);

      // === Variables === //
      bool passSLCuts_el;
      bool passSLCuts_mu;
      bool passDLCuts_el;
      bool passDLCuts_mu;
      bool passDLCuts_emu;
      bool passSLtriggers_el;
      bool passSLtriggers_mu;
      bool passDLtriggers_el;
      bool passDLtriggers_mu;
      bool passDLtriggers_emu;
      int nLeptons;
      int nMuons;
      int nElectrons;
      double leadPt_el;
      double leadEta_el;
      double leadCharge_el;
      double subPt_el;
      double subEta_el;
      double subCharge_el;
      double leadPt_mu;
      double leadEta_mu;
      double leadCharge_mu;
      double leadRelIso_mu;
      double leadRelIso_el;
      double subRelIso_mu;
      double subRelIso_el;
      double subPt_mu;
      double subEta_mu;
      double subCharge_mu;
      double mll;
      double lepSF;
      bool isMC;
      string dataPeriod;
      bool b_periodDep__HLT_IsoMu24_eta2p1;
      bool b_periodDep__doubleMu_noMass;
      bool b_periodDep__doubleMu_withMass;
      int leadIndex_el;
      int subIndex_el;
      int leadIndex_mu;
      int subIndex_mu;

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
      void checkCategoryCuts();
      void setLeadSubleadIndices(int l, int& lead, int& sub);
      double calculateDileptonMass(int index_lead, int index_sub);

      // === Variables === //
      yggdrasilEventVars* ev;
      bool passMC;

}; // End of class prototype




#endif // _leptonHandler_h
