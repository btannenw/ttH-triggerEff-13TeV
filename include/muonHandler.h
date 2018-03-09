#ifndef _muonHandler_h
#define _muonHandler_h

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

class muonHandler{

   public: 
      // Constructor(s) and destructor
      muonHandler();
      virtual ~muonHandler();

      // === Functions === //
      void test();
      void Event(yggdrasilEventVars* eve);

      // === Variables === //
      bool passCuts;
      bool passSLtrigger;
      int nLeptons;
      int nMuons;
      double leadPt;
      double leadEta;
      
      // === Variables === //
      TH1D* h_mu_cutflow;
      TH1D* h_mu_n;

   private:
      // === Functions === //
      void findLeadingMuon();
      void applyMuonCuts();
      
      
      // === Variables === //
      yggdrasilEventVars* ev;
      int leadIndex;

}; // End of class prototype




#endif // _muonHandler_h