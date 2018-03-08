#ifndef _jetMetHandler_h
#define _jetMetHandler_h

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

class jetMetHandler{

   public: 
      // Constructor(s) and destructor
      jetMetHandler();
      virtual ~jetMetHandler();

      // === Functions === //
      void test();
      void Event(yggdrasilEventVars* eve);

      // === Variables === //
      bool passCuts;
      int nPreCutJets;
      int nJets;
      double leadPt;
      double leadEta;
      double MET;
      bool passMETXtriggers;

      // === Variables === //
      TH1D* h_jet_cutflow;
      TH1D* h_jet_n;
      TH1D* h_met_passXtriggers;

   private:
      // === Functions === //
      void applyJetCuts();
      void applyMETCuts();
      
      
      // === Variables === //
      yggdrasilEventVars* ev;
      int leadIndex;

}; // End of class prototype




#endif // _electronHandler_h
