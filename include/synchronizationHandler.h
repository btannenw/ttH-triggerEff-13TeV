// Author:  Ben Tannenwald
// Date:    Dec 4, 2018
// Purpose: Make header for synchronization functions

#include <TFile.h>
#include <TDirectory.h>
#include <TString.h>
#include <TObjArray.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision

ofstream csvFile, missingEventsTXT;

void setSyncFiles(string topDir, string txtName, string csvTag = "")
{
  missingEventsTXT.open( (topDir + "/" + txtName + ".txt").c_str() );
  
  missingEventsTXT << "run,lumi,event,passDLtriggers_el,passDLtriggers_mu,passDLtriggers_emu,passDLCuts_el,passDLCuts_mu,passDLCuts_emu,n_jets,n_btags,el1_pt,el1_iso,el2_pt,el2_iso,mu1_pt,mu1_iso,mu2_pt,mu2_iso,jet1_pt,jet2_pt,jet1_csv,jet2_csv,MET_pt,mll,nElectrons,nMuons,MET_Type1xy,MET_Type1xy_sync" << endl;
  
  string csvName = csvTag=="" ? (topDir + "/csv_ttH_ben.csv").c_str() : (topDir + "/csv_ttH_ben_" + csvTag + ".csv").c_str();
  csvFile.open( csvName.c_str() );
  csvFile << "run,lumi,event,is_e,is_mu,is_ee,is_emu,is_mumu,n_jets,n_btags,lep1_pt,lep1_eta,lep1_iso,lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed,lep2_pt,lep2_eta,lep2_iso,lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed,jet1_pt,jet1_eta,jet1_phi,jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal,jet1_csv,jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed,jet2_pt,jet2_eta,jet2_phi,jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal,jet2_csv,jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed,MET_pt,MET_phi,MET_pt_phiCor,MET_phi_phiCor,mll,ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output" << endl;

}

void printEventToCSV(yggdrasilEventVars* eve, leptonHandler lepTool, jetMetHandler jetMetTool, bool isDebug)
{
  
  csvFile << std::fixed;
  missingEventsTXT << std::fixed;

  if (isDebug) {
    missingEventsTXT << eve->run_ << "," << eve->lumi_ << "," <<eve->evt_ << ","
		     << (lepTool.passSLtriggers_el || lepTool.passDLtriggers_el)  << "," << (lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) << "," << (lepTool.passSLtriggers_mu || lepTool.passSLtriggers_el || lepTool.passDLtriggers_emu) << "," << lepTool.passDLCuts_el << "," << lepTool.passDLCuts_mu << "," << lepTool.passDLCuts_emu << ","
		     << jetMetTool.nJets << "," << jetMetTool.nBTags << ",";
    
    missingEventsTXT << "EL1" << lepTool.leadPt_el << "," << lepTool.leadRelIso_el << ",";
    missingEventsTXT << "EL2" << lepTool.subPt_el << "," << lepTool.subRelIso_el << ",";
    missingEventsTXT << "MU1" << lepTool.leadPt_mu << "," << lepTool.leadRelIso_mu << ",";
    missingEventsTXT << "MU2" << lepTool.subPt_mu << "," << lepTool.subRelIso_mu << ",";
    
    missingEventsTXT << jetMetTool.leadPt << "," << jetMetTool.subPt << "," << jetMetTool.leadDeepCSV << "," << jetMetTool.subDeepCSV << ","
		     << jetMetTool.MET << "," << lepTool.mll << ","
		     << lepTool.nElectrons << "," << lepTool.nMuons << ","
		     << eve->MET_Type1xy_[0] << "," << eve->MET_Type1xy_sync_[0] << endl;
    
  }


  // * a. dilepton mumu
  if ((lepTool.passSLtriggers_mu || lepTool.passDLtriggers_mu) && lepTool.passDLCuts_mu && jetMetTool.passDLJetMetCuts) {
    csvFile << eve->run_ << "," << eve->lumi_ << "," <<eve->evt_ << ",0,0,0,0,1," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4) << 
      lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep1_pdgId,"<< lepTool.leadIDSF_mu << "," << lepTool.leadRecoIsoSF_mu <<",lep1_seed," <<
      lepTool.subPt_mu << "," << lepTool.subEta_mu << "," << lepTool.subRelIso_mu << ",lep2_pdgId,"<< lepTool.subIDSF_mu << "," << lepTool.subRecoIsoSF_mu <<",lep2_seed," <<
      jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
      jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
      jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor," <<
      lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output" << endl;
  } // end mumu
  // * a. dilepton ee
  else if ((lepTool.passSLtriggers_el || lepTool.passDLtriggers_el) && lepTool.passDLCuts_el && jetMetTool.passDLJetMetCuts) {
    csvFile << eve->run_ << "," << eve->lumi_ << "," <<eve->evt_ << ",0,0,1,0,0," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4) << 
      lepTool.leadPt_el << "," << lepTool.leadEta_el << "," << lepTool.leadRelIso_el << ",lep1_pdgId,"<< lepTool.leadIDSF_el << "," << lepTool.leadRecoIsoSF_el <<",lep1_seed," <<
      lepTool.subPt_el << "," << lepTool.subEta_el << "," << lepTool.subRelIso_el << ",lep2_pdgId,"<< lepTool.subIDSF_el << "," << lepTool.subRecoIsoSF_el <<",lep2_seed," <<
      jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
      jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
      jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor," <<
      lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output" << endl;
  }
  // * c. dilepton emu
  else if ((lepTool.passSLtriggers_el || lepTool.passSLtriggers_mu || lepTool.passDLtriggers_emu) && lepTool.passDLCuts_emu && jetMetTool.passDLJetMetCuts) {
    csvFile << eve->run_ << "," << eve->lumi_ << "," <<eve->evt_ << ",0,0,0,1,0," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4);
    if(lepTool.leadPt_el > lepTool.leadPt_mu) { // electron is leading lepton
      csvFile << lepTool.leadPt_el << "," << lepTool.leadEta_el << "," << lepTool.leadRelIso_el << ",lep1_pdgId,"<< lepTool.leadIDSF_el << "," << lepTool.leadRecoIsoSF_el <<",lep1_seed,";
      csvFile << lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep2_pdgId,"<< lepTool.leadIDSF_mu << "," << lepTool.leadRecoIsoSF_mu <<",lep2_seed,";
    }
    else { // muon is leading lepton
      csvFile << lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep1_pdgId,"<< lepTool.leadIDSF_mu << "," << lepTool.leadRecoIsoSF_mu <<",lep1_seed,";
      csvFile << lepTool.leadPt_el << "," << lepTool.leadEta_el << "," << lepTool.leadRelIso_el << ",lep2_pdgId,"<< lepTool.leadIDSF_el << "," << lepTool.leadRecoIsoSF_el <<",lep2_seed,";
    }
    csvFile << jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
      jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
      jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor," <<
      lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output" << endl;
  }
  // * d. single mu
  else if ( lepTool.passSLtriggers_mu && lepTool.passSLCuts_mu && (lepTool.nMuons == 1 && lepTool.nElectrons == 0) && jetMetTool.passSLJetMetCuts) {
    csvFile << eve->run_ << "," << eve->lumi_ << "," <<eve->evt_ << ",0,1,0,0,0," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4) << 
      lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed," <<
      lepTool.subPt_mu << "," << lepTool.subEta_mu << "," << lepTool.subRelIso_mu << ",lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed," <<
      jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
      jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
      jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor," <<
      lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output" << endl;
  } // end single mu
  // * e. single el
  else if ( lepTool.passSLtriggers_el && lepTool.passSLCuts_el && (lepTool.nMuons == 0 && lepTool.nElectrons == 1) && jetMetTool.passSLJetMetCuts) {
    csvFile << eve->run_ << "," << eve->lumi_ << "," <<eve->evt_ << ",1,0,0,0,0," << jetMetTool.nJets << "," << jetMetTool.nBTags << "," << setprecision(4) << 
      lepTool.leadPt_mu << "," << lepTool.leadEta_mu << "," << lepTool.leadRelIso_mu << ",lep1_pdgId,lep1_idSF,lep1_isoSF,lep1_seed," <<
      lepTool.subPt_mu << "," << lepTool.subEta_mu << "," << lepTool.subRelIso_mu << ",lep2_pdgId,lep2_idSF,lep2_isoSF,lep2_seed," <<
      jetMetTool.leadPt << "," << jetMetTool.leadEta << "," << jetMetTool.leadPhi << ",jet1_jesSF,jet1_jesSF_up,jet1_jesSF_down,jet1_jesSF_PileUpDataMC_down,jet1_jesSF_RelativeFSR_up,jet1_jerSF_nominal," << jetMetTool.leadDeepCSV << ",jet1_PUJetId,jet1_PUJetDiscriminant,jet1_seed," <<
      jetMetTool.subPt << "," << jetMetTool.subEta << "," << jetMetTool.subPhi << ",jet2_jesSF,jet2_jesSF_up,jet2_jesSF_down,jet2_jesSF_PileUpDataMC_down,jet2_jesSF_RelativeFSR_up,jet2_jerSF_nominal," << jetMetTool.subDeepCSV << ",jet2_PUJetId,jet2_PUJetDiscriminant,jet2_seed," <<
      jetMetTool.MET << "," << jetMetTool.MET_phi << ",MET_pt_phiCor,MET_phi_phiCor," <<
      lepTool.mll << ",ttHFCategory,ttHFGenFilterTag,n_interactions,puWeight,csvSF,csvSF_lf_up,csvSF_hf_down,csvSF_cErr1_down,pdf_up,pdf_down,me_up,me_down,triggerSF,top_pt_weight,bdt_output,dnn_ttH_output,dnn_ttbb_output" << endl;
  } // end single el
  
}

void closeCSVFiles()
{
  csvFile.close();
  missingEventsTXT.close();
}
