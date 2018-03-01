#ifndef ttH_LeptonPlusJets_AnalysisCode_YggdrasilEventVars_h
#define ttH_LeptonPlusJets_AnalysisCode_YggdrasilEventVars_h

//
// Dependencies (#includes)
//
#include <iostream>
#include <vector>
#include "TLorentzVector.h"

#ifdef __MAKECINT__
#pragma link C++ class std::vector< TLorentzVector >+; 
#endif

using namespace std;



typedef std::vector<std::vector<double> > vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double> vdouble;
typedef std::vector<float> vfloat;
typedef std::vector<string> vstring;
typedef std::vector<bool> vbool;
typedef std::vector<int> vint;
typedef std::vector< TLorentzVector > vecTLorentzVector;

//
// Utility Class for Handling Event Variables
//

const Int_t rNumSys = 25 + 27 * 2 ;
// 25     : As of 2016 HC analysis.
// 27 * 2 : JES decomposed. (up/down)

struct yggdrasilEventVars{


  //////////////////////////////////////////////////////////////////////////
  ///  Tree branches/leaves
  //////////////////////////////////////////////////////////////////////////

  explicit yggdrasilEventVars(){ }
  Int_t   additionalJetEventId_;
  Int_t   higgsDecayType_;
  Int_t   ttbarDecayType_;

  int passHLT_Ele27_eta2p1_WP85_Gsf_HT200_v1_;
  
  int passHLT_IsoMu24_v_;
  int passHLT_IsoTkMu24_v_;
  int passHLT_IsoMu20_eta2p1_v_;

  int passHLT_Ele27_eta2p1_WPTight_Gsf_v_;
  int passHLT_Ele27_WPTight_Gsf_v;
  int passHLT_IsoMu22_v_;
  int passHLT_IsoTkMu22_v_;

  int passHLT_Ele27_WP85_Gsf_v_;
  int passHLT_Ele27_eta2p1_WPLoose_Gsf_v_;
  int passHLT_Ele27_eta2p1_WP75_Gsf_v_;

  int passHLT_Ele27_eta2p1_WP85_Gsf_HT200_v_;
  int passHLT_Ele27_eta2p1_WPLoose_Gsf_HT200_v_;

  int passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_;

  int passHLT_Mu30_TkMu11_v_;
  int passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v_;
  int passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v_;
  
  int passHLT_Ele25WP60_SC4_Mass55_v_;

  /* int passHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_; */
  /* int passHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_; */
  /* int passHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v_; */
  int passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v_;

  int passHLT_Ele28_eta2p1_WPTight_Gsf_HT150_v_                      ;
  int passHLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v_  ;
  int passHLT_IsoMu24_2p1_v_                                   	    ;
  int passHLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_         ;
  int passHLT_PFHT380_SixJet32_DoubleBTagCSV_p075_v_           	    ;
  int passHLT_PFHT380_SixPFJet32_DoublePFBTagCSV_2p2_v_              ;  
  int passHLT_PFHT380_SixPFJet32_DoublePFBTagDeepCSV_2p2_v_          ;  
  int passHLT_PFHT430_SixJet40_BTagCSV_p080_v_                       ;  
  int passHLT_PFHT430_SixPFJet40_PFBTagCSV_1p5_v_                    ;  

  // BBT [Added Feb 26, 2018]: add 2016 branches for trigger efficiency study
  /* 
  //NOTE [2-27-18]: these don't work!!
  int passHLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1_ ;
  int passHLT_PFHT350_PFMET120_NoiseCleaned_v1_ ;
  int passHLT_PFMET120_NoiseCleaned_BTagCSV07_v1_ ;
  int passHLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ ;
  int passHLT_PFMET170_NoiseCleaned_v1_ ;
  int passHLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ ;
  int passHLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ ;
  int passHLT_Photon135_PFMET40_v1_ ;
  int passHLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ ;
  int passHLT_Photon150_PFMET40_v1_ ;
  int passHLT_Photon160_PFMET40_v1_ ;
  int passHLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ ;
  int passHLT_Photon250_NoHE_PFMET40_v1_ ;
  int passHLT_Photon300_NoHE_PFMET40_v1_ ;
  */
  // 2-27-18: add all 2017 branches for trigger efficiency study, need to weed out later to remove prescaled HLT and L1
  // 2-28-18: unprescaled known... just gonna keep them all for sanity's sake. also, adding 2017 single and double lepton triggers
  // 2017 MET triggers
  int passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_ ;
  int passHLT_PFHT500_PFMET110_PFMHT110_IDTight_v_ ;
  int passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_ ;
  int passHLT_PFHT700_PFMET95_PFMHT95_IDTight_v_ ;
  int passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_ ;
  int passHLT_PFHT800_PFMET85_PFMHT85_IDTight_v_ ;
  int passHLT_PFMET110_PFMHT110_IDTight_v_ ;
  int passHLT_PFMET120_PFMHT120_IDTight_v_ ;
  int passHLT_PFMET130_PFMHT130_IDTight_v_ ;
  int passHLT_PFMET140_PFMHT140_IDTight_v_ ;
  int passHLT_PFMETTypeOne110_PFMHT110_IDTight_v_ ;
  int passHLT_PFMETTypeOne120_PFMHT120_IDTight_v_ ;
  int passHLT_PFMETTypeOne130_PFMHT130_IDTight_v_ ;
  int passHLT_PFMETTypeOne140_PFMHT140_IDTight_v_ ;
  int passHLT_DiJet110_35_Mjj650_PFMET110_v_ ;
  int passHLT_DiJet110_35_Mjj650_PFMET120_v_ ;
  int passHLT_DiJet110_35_Mjj650_PFMET130_v_ ;
  int passHLT_TripleJet110_35_35_Mjj650_PFMET110_v_ ;
  int passHLT_TripleJet110_35_35_Mjj650_PFMET120_v_ ;
  int passHLT_TripleJet110_35_35_Mjj650_PFMET130_v_ ;
  int passHLT_MET105_IsoTrk50_v_ ;
  int passHLT_MET120_IsoTrk50_v_ ;
  int passHLT_PFMET120_PFMHT120_IDTight_PFHT60_v_ ;
  int passHLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60_v_ ;
  int passHLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1_v_ ;
  int passHLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1_v_ ;
  int passHLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1_v_ ;
  int passHLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1_v_ ;
  int passHLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1_v_ ;
  int passHLT_CaloMET100_HBHECleaned_v_ ;
  int passHLT_CaloMET250_HBHECleaned_v_ ;
  int passHLT_CaloMET300_HBHECleaned_v_ ;
  int passHLT_CaloMET350_HBHECleaned_v_ ;
  int passHLT_CaloMET70_HBHECleaned_v_ ;
  int passHLT_CaloMET80_HBHECleaned_v_ ;
  int passHLT_CaloMET90_HBHECleaned_v_ ;
  int passHLT_PFMET200_HBHE_BeamHaloCleaned_v_ ;
  int passHLT_PFMET200_HBHECleaned_v_ ;
  int passHLT_PFMET250_HBHECleaned_v_ ;
  int passHLT_PFMET300_HBHECleaned_v_ ;
  int passHLT_PFMETTypeOne200_HBHE_BeamHaloCleaned_v_ ;
  int passHLT_PFMET100_PFMHT100_IDTight_PFHT60_v_ ;
  int passHLT_PFMETTypeOne100_PFMHT100_IDTight_PFHT60_v_ ;

  // 2017 singleLepton and diLepton triggers
  int passHLT_IsoMu27_v_ ;    
  int passHLT_IsoMu24_eta2p1_v_;
  int passHLT_Ele32_WPTight_Gsf_v_ ;  
  int passHLT_Ele35_WPTight_Gsf_v_ ;  
  int passHLT_Ele38_WPTight_Gsf_v_ ;  
  int passHLT_Ele40_WPTight_Gsf_v_ ;  
  int passHLT_Ele32_WPTight_Gsf_L1DoubleEG_v_ ;
  int passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_;
  int passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_ ;
  int passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_ ;
  int passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_;
  int passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_;
  int passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_;
  int passHLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ ;
  int passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ ; 

  // END

  int run_;
  int lumi_;
  long evt_;



  float   numTruePV_;
  int     numGenPV_;
  
  bool    GoodFirstPV_;
  
  Int_t   numPVs_;
  Int_t   numSys_;

  vint lepton_isMuon_;
  vint lepton_charge_;
  vint lepton_isTight_;
  vint lepton_isLoose_;
  vint lepton_isLooseAlt_;
  vdouble lepton_pt_;
  vdouble lepton_eta_;
  vdouble lepton_phi_;
  vdouble lepton_e_;
  vdouble lepton_relIso_;
  vdouble lepton_puppirelIso_;
  vdouble lepton_dbiso_CH_;
  vdouble lepton_dbiso_NH_;
  vdouble lepton_dbiso_PH_;
  vdouble lepton_dbiso_PU_;
  vdouble lepton_puppiIsoWithLep_CH_    ;
  vdouble lepton_puppiIsoWithLep_NH_    ;
  vdouble lepton_puppiIsoWithLep_PH_    ;
  vdouble lepton_puppiIsoWithoutLep_CH_ ;
  vdouble lepton_puppiIsoWithoutLep_NH_ ;
  vdouble lepton_puppiIsoWithoutLep_PH_ ;
  vdouble lepton_scEta_;
  vdouble lepton_dRSingleLepTrig_;

  vfloat truth_pt_;
  vfloat truth_eta_;
  vfloat truth_phi_; 
  vfloat truth_m_ ;
  vint    truth_pdgid_;
  vint truth_parentIdx_;

  Float_t wgt_lumi_;
  Float_t wgt_xs_;
  Float_t wgt_nGen_;
  double  wgt_generator_;
  double  wgt_[rNumSys];
  double  wgt_csvSF_[rNumSys];  

  double weight_q2_upup_ ;
  double weight_q2_downdown_ ;

  double weight_PDF_CT14nlo_up_ ;
  double weight_PDF_CT14nlo_down_ ;

  double weight_PDF_NNPDF30NLO_up_ ;
  double weight_PDF_NNPDF30NLO_down_ ;

  double weight_topPt_ ; 

  Float_t MET_[rNumSys];
  Float_t uMET_[rNumSys];
  Float_t MET_phi_[rNumSys];
  Float_t uMET_phi_[rNumSys];

  Float_t MET_Type1xy_[rNumSys];
  Float_t MET_Type1xy_phi_[rNumSys];

  Float_t PUPPIMET_[rNumSys];
  Float_t PUPPIMET_phi_[rNumSys];

  Float_t PUPPIMET_Type1xy_[rNumSys];
  Float_t PUPPIMET_Type1xy_phi_[rNumSys];

  vdouble jet_combinedMVABJetTags_[rNumSys];
  vdouble jet_combinedInclusiveSecondaryVertexV2BJetTags_[rNumSys];
  vdouble jet_combinedMVABJetTags_HIP_[rNumSys];
  vdouble jet_combinedInclusiveSecondaryVertexV2BJetTags_HIP_[rNumSys];

  vdouble jet_DeepCSV_b_ [rNumSys];
  vdouble jet_DeepCSV_bb_[rNumSys];

  vdouble jet_vtxMass_[rNumSys];
  vdouble jet_vtxNtracks_[rNumSys];
  vdouble jet_vtx3DVal_[rNumSys];
  vdouble jet_vtx3DSig_[rNumSys];


  vdouble jet_pt_[rNumSys];
  vdouble jet_eta_[rNumSys];
  vdouble jet_phi_[rNumSys];
  vdouble jet_m_[rNumSys];


  vdouble  jet_precorr_pt_  [rNumSys];
  vdouble  jet_precorr_phi_  [rNumSys];

  vdouble jet_AssociatedGenJet_pt_[rNumSys];
  vdouble jet_AssociatedGenJet_eta_[rNumSys];
  vdouble jet_AssociatedGenJet_phi_[rNumSys];
  vdouble jet_AssociatedGenJet_m_[rNumSys];

  vint jet_genId_[rNumSys];
  vint jet_partonflavour_[rNumSys];
  vint jet_flavour_[rNumSys];
  vint jet_genParentId_[rNumSys];
  vint jet_genGrandParentId_[rNumSys];

  vdouble genjet_pt_[rNumSys];
  vdouble genjet_eta_[rNumSys];
  vdouble genjet_phi_[rNumSys];
  vdouble genjet_m_[rNumSys];
  vint    genjet_BhadronMatch_[rNumSys];

  ///boosted jets
  vvdouble topfatJet_vect_TLV_;
  vvdouble nonW_vect_TLV_;
  vvdouble W1_vect_TLV_;
  vvdouble W2_vect_TLV_;
  vbool isTopTag_;
  int numTopTags_;

  int numHiggsTags_;
  vvdouble higgsfatJet_vect_TLV_;
  std::vector<vvdouble> higgsfilterjet_all_vect_TLV_;
  vvdouble csv_filterjet_all_;
  
  //Only used in slimTree Code
  
  vint lepnums_;
  
  

  void initialize();

};


typedef std::vector<yggdrasilEventVars> vyggdrasilEventVars;


void yggdrasilEventVars::initialize(){
  additionalJetEventId_ = -1;
  higgsDecayType_      = -1;
  ttbarDecayType_      = 0 ;

  passHLT_Ele27_eta2p1_WP85_Gsf_HT200_v1_ = -99;
  
  passHLT_IsoMu24_v_ = -99;
  passHLT_IsoTkMu24_v_ = -99;
  passHLT_IsoMu20_eta2p1_v_ = -99;

  passHLT_Ele27_eta2p1_WPTight_Gsf_v_ = -99 ;
  passHLT_IsoMu22_v_ = -99 ;
  passHLT_IsoTkMu22_v_ = -99 ;
  
  passHLT_Ele27_WP85_Gsf_v_ = -99;
  passHLT_Ele27_WPTight_Gsf_v = -99;
  passHLT_Ele27_eta2p1_WPLoose_Gsf_v_ = -99;
  passHLT_Ele27_eta2p1_WP75_Gsf_v_ = -99;

  passHLT_Ele27_eta2p1_WP85_Gsf_HT200_v_ = -99;
  passHLT_Ele27_eta2p1_WPLoose_Gsf_HT200_v_ = -99;

  passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_ = -99;

  passHLT_Mu30_TkMu11_v_ = -99;
  passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v_ = - 99;
  passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v_ = -99;
  
  passHLT_Ele25WP60_SC4_Mass55_v_ = -99;

  /* passHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ = -99 ; */
  /* passHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_ = -99 ; */
  /* passHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v_ = -99 ; */
  passHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v_ = -99 ;


  // BBT [Added Feb 26, 2018]: add branches for trigger efficiency study
  /*
  // 2-27-18: these are 2016 and don't work!
  passHLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1_ = -99 ;
  passHLT_PFHT350_PFMET120_NoiseCleaned_v1_ = -99 ;
  passHLT_PFMET120_NoiseCleaned_BTagCSV07_v1_ = -99 ;
  passHLT_Photon22_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ = -99 ;
  passHLT_PFMET170_NoiseCleaned_v1_ = -99 ;
  passHLT_Photon36_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ = -99 ;
  passHLT_Photon50_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ = -99 ;
  passHLT_Photon135_PFMET40_v1_ = -99 ;
  passHLT_Photon75_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ = -99 ;
  passHLT_Photon150_PFMET40_v1_ = -99 ;
  passHLT_Photon160_PFMET40_v1_ = -99 ;
  passHLT_Photon90_R9Id90_HE10_Iso40_EBOnly_PFMET40_v1_ = -99 ;
  passHLT_Photon250_NoHE_PFMET40_v1_ = -99 ;
  passHLT_Photon300_NoHE_PFMET40_v1_ = -99 ;
  */
  // 2-27-18: add 2017 branhces
  passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_ = -99 ;
  passHLT_PFHT500_PFMET110_PFMHT110_IDTight_v_ = -99 ;
  passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_ = -99 ;
  passHLT_PFHT700_PFMET95_PFMHT95_IDTight_v_ = -99 ;
  passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_ = -99 ;
  passHLT_PFHT800_PFMET85_PFMHT85_IDTight_v_ = -99 ;
  passHLT_PFMET110_PFMHT110_IDTight_v_ = -99 ;
  passHLT_PFMET120_PFMHT120_IDTight_v_ = -99 ;
  passHLT_PFMET130_PFMHT130_IDTight_v_ = -99 ;
  passHLT_PFMET140_PFMHT140_IDTight_v_ = -99 ;
  passHLT_PFMETTypeOne110_PFMHT110_IDTight_v_ = -99 ;
  passHLT_PFMETTypeOne120_PFMHT120_IDTight_v_ = -99 ;
  passHLT_PFMETTypeOne130_PFMHT130_IDTight_v_ = -99 ;
  passHLT_PFMETTypeOne140_PFMHT140_IDTight_v_ = -99 ;
  passHLT_DiJet110_35_Mjj650_PFMET110_v_ = -99 ;
  passHLT_DiJet110_35_Mjj650_PFMET120_v_ = -99 ;
  passHLT_DiJet110_35_Mjj650_PFMET130_v_ = -99 ;
  passHLT_TripleJet110_35_35_Mjj650_PFMET110_v_ = -99 ;
  passHLT_TripleJet110_35_35_Mjj650_PFMET120_v_ = -99 ;
  passHLT_TripleJet110_35_35_Mjj650_PFMET130_v_ = -99 ;
  passHLT_MET105_IsoTrk50_v_ = -99 ;
  passHLT_MET120_IsoTrk50_v_ = -99 ;
  passHLT_PFMET120_PFMHT120_IDTight_PFHT60_v_ = -99 ;
  passHLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60_v_ = -99 ;
  passHLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1_v_ = -99 ;
  passHLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1_v_ = -99 ;
  passHLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1_v_ = -99 ;
  passHLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1_v_ = -99 ;
  passHLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1_v_ = -99 ;
  passHLT_CaloMET100_HBHECleaned_v_ = -99 ;
  passHLT_CaloMET250_HBHECleaned_v_ = -99 ;
  passHLT_CaloMET300_HBHECleaned_v_ = -99 ;
  passHLT_CaloMET350_HBHECleaned_v_ = -99 ;
  passHLT_CaloMET70_HBHECleaned_v_ = -99 ;
  passHLT_CaloMET80_HBHECleaned_v_ = -99 ;
  passHLT_CaloMET90_HBHECleaned_v_ = -99 ;
  passHLT_PFMET200_HBHE_BeamHaloCleaned_v_ = -99 ;
  passHLT_PFMET200_HBHECleaned_v_ = -99 ;
  passHLT_PFMET250_HBHECleaned_v_ = -99 ;
  passHLT_PFMET300_HBHECleaned_v_ = -99 ;
  passHLT_PFMETTypeOne200_HBHE_BeamHaloCleaned_v_ = -99 ;
  passHLT_PFMET100_PFMHT100_IDTight_PFHT60_v_ = -99 ;
  passHLT_PFMETTypeOne100_PFMHT100_IDTight_PFHT60_v_ = -99 ;

  // 2017 singleLepton and diLepton triggers
  passHLT_IsoMu27_v_ = -99 ;    
  passHLT_IsoMu24_eta2p1_v_ = -99 ;
  passHLT_Ele32_WPTight_Gsf_v_ = -99 ;  
  passHLT_Ele35_WPTight_Gsf_v_ = -99 ;  
  passHLT_Ele38_WPTight_Gsf_v_ = -99 ;  
  passHLT_Ele40_WPTight_Gsf_v_ = -99 ;  
  passHLT_Ele32_WPTight_Gsf_L1DoubleEG_v_ = -99 ;
  passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v_ = -99 ;
  passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_ = -99 ;
  passHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_ = -99 ;
  passHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ = -99 ;
  passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_ = -99 ;
  passHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_ = -99 ;
  passHLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ = -99 ;
  passHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_ = -99 ; 
  // END

  run_  = -99;
  lumi_ = -99;
  evt_ = -99;


  numTruePV_ = -99;
  numGenPV_ = -99;
  
  GoodFirstPV_ = false;

  numPVs_ = -99;
  numSys_ = -99;

  truth_pt_.clear();
  truth_eta_.clear();
  truth_phi_.clear();
  truth_m_ .clear();
  
  truth_pdgid_.clear();
  truth_parentIdx_.clear();

  lepton_isMuon_.clear();
  lepton_charge_.clear();
  lepton_isTight_.clear();
  lepton_isLoose_.clear();
  lepton_isLooseAlt_.clear();
  lepton_pt_.clear();
  lepton_eta_.clear();
  lepton_phi_.clear();
  lepton_e_.clear();
  lepton_relIso_.clear();
  lepton_puppirelIso_.clear();

  lepton_dbiso_CH_ .clear();
  lepton_dbiso_NH_ .clear();
  lepton_dbiso_PH_ .clear();
  lepton_dbiso_PU_ .clear();

  lepton_puppiIsoWithLep_CH_   .clear() ;
  lepton_puppiIsoWithLep_NH_   .clear() ;
  lepton_puppiIsoWithLep_PH_   .clear() ;
  lepton_puppiIsoWithoutLep_CH_.clear() ;
  lepton_puppiIsoWithoutLep_NH_.clear() ;
  lepton_puppiIsoWithoutLep_PH_.clear() ;
  lepton_scEta_.clear();
  lepton_dRSingleLepTrig_.clear();

  wgt_generator_        = -99.9;
  wgt_lumi_             = -99.9;
  wgt_xs_               = -99.9;
  wgt_nGen_             = -99.9;

  weight_q2_upup_     = -99.9;
  weight_q2_downdown_ = -99.9;
 
  weight_PDF_CT14nlo_up_    = -99.9 ;
  weight_PDF_CT14nlo_down_  = -99.9 ;
  weight_PDF_NNPDF30NLO_up_    = -99.9 ;
  weight_PDF_NNPDF30NLO_down_ = -99.9 ;

  weight_topPt_ = -99.9;

  for(int iSys=0; iSys<rNumSys; iSys++){

    wgt_[iSys] = -99.9;  

    MET_[iSys]                            = -99.9;
    uMET_[iSys]                           = -99.9;
    MET_phi_[iSys]                        = -99.9;
    uMET_phi_[iSys]                       = -99.9;

    PUPPIMET_[iSys]                            = -99.9;
    PUPPIMET_phi_[iSys]                        = -99.9;

    MET_Type1xy_[iSys]                        = -99.9;
    MET_Type1xy_phi_[iSys]                        = -99.9;

    PUPPIMET_Type1xy_[iSys]                            = -99.9;
    PUPPIMET_Type1xy_phi_[iSys]                        = -99.9;

    jet_combinedMVABJetTags_[iSys].clear();
    jet_combinedInclusiveSecondaryVertexV2BJetTags_[iSys].clear();
    jet_combinedMVABJetTags_HIP_[iSys].clear();
    jet_combinedInclusiveSecondaryVertexV2BJetTags_HIP_[iSys].clear();
    jet_vtxMass_[iSys].clear();
    jet_vtxNtracks_[iSys].clear();
    jet_vtx3DVal_[iSys].clear();
    jet_vtx3DSig_[iSys].clear();

    jet_pt_ [iSys].clear();
    jet_phi_[iSys].clear();
    jet_eta_[iSys].clear();
    jet_m_  [iSys].clear();

    jet_precorr_pt_  [iSys].clear();
    jet_precorr_phi_  [iSys].clear();

    jet_AssociatedGenJet_pt_[iSys].clear();
    jet_AssociatedGenJet_eta_[iSys].clear();
    jet_AssociatedGenJet_phi_[iSys].clear();
    jet_AssociatedGenJet_m_[iSys].clear();
    
    genjet_pt_ [iSys].clear();
    genjet_phi_[iSys].clear();
    genjet_eta_[iSys].clear();
    genjet_m_  [iSys].clear();
    genjet_BhadronMatch_[iSys].clear();

    jet_partonflavour_[iSys].clear();
    jet_flavour_[iSys].clear();
    jet_genId_[iSys].clear();
    jet_genParentId_[iSys].clear();
    jet_genGrandParentId_[iSys].clear();

  }

  //boosted jets
  topfatJet_vect_TLV_.clear();
  nonW_vect_TLV_.clear();
  W1_vect_TLV_.clear();
  W2_vect_TLV_.clear();
  isTopTag_.clear();
  numTopTags_ = -99;

  numHiggsTags_ = -99;
  higgsfatJet_vect_TLV_.clear();
  higgsfilterjet_all_vect_TLV_.clear();
  csv_filterjet_all_.clear();
  
  lepnums_.clear();

  return;
}

  

#endif
