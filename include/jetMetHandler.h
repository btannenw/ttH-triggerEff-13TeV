#ifndef _jetMetHandler_h
#define _jetMetHandler_h

#include <TH1.h>
#include <TH2.h>

#include <iostream>
#include <vector>
#include "YggdrasilEventVars.h"
#include "TLorentzVector.h"
//#include "leptonHandler.h"


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
      void Event(yggdrasilEventVars* eve, leptonHandler lep, bool passDebug);

      // === Variables === //
      bool passLeadingJetCuts;
      bool passDLJetMetCuts;
      bool passSLJetMetCuts;
      int nPreCutJets;
      int nJets;
      int nBTags;
      double leadPt;
      double leadEta;
      double leadPhi;
      double leadDeepCSV;
      double subPt;
      double subEta;
      double subPhi;
      double subDeepCSV;
      double MET;
      double MET_phi;
      double nPV;
      double ttbarJetID;
      bool passOneMETTrigger;
      bool passAllMETTriggers;
      bool isDebug;

      // === Variables === //
      TH1D* h_jet_cutflow;
      TH1D* h_jet_n;
      TH1D* h_btags_n;
      TH1D* h_met_passXtriggers;
      TH1D* h_met_passOnlyXtrigger;

   private:
      // === Functions === //
      void applyJetCuts();
      void applyMETCuts();
      void parseMETTriggerLogic();
      void parseMETTriggerLogic_v2();
      void parseMETTriggerLogic_v3();
      void setLeadSubleadIndices(int l, int& lead, int& sub);
      bool vetoLeptonJetOverlapRemoval(int j);
      
      
      // === Variables === //
      yggdrasilEventVars* ev;
      leptonHandler lTool;
      int leadIndex;
      int subIndex;
      string metXTriggerBits;
      bool passDebug;

}; // End of class prototype




#endif // _electronHandler_h
