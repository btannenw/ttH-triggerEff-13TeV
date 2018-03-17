#ifndef _electronHandler_h
#define _electronHandler_h

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

class electronHandler{

   public: 
      // Constructor(s) and destructor
      electronHandler();
      virtual ~electronHandler();

      // === Functions === //
      void test();
      void Event(yggdrasilEventVars* eve);

      // === Variables === //
      bool passCuts;
      bool passSLCuts;
      bool passSLtriggers;
      int nLeptons;
      int nElectrons;
      double leadPt;
      double leadEta;
      
      // === Variables === //
      TH1D* h_el_cutflow;
      TH1D* h_el_event_cutflow;
      TH1D* h_el_n;

   private:
      // === Functions === //
      void findLeadingElectron();
      void applyElectronCuts();
      void checkHLTTriggers();
      void checkSLCuts();
      
      // === Variables === //
      yggdrasilEventVars* ev;
      int leadIndex;

}; // End of class prototype




#endif // _electronHandler_h
