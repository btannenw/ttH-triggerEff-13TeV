// Author:  Ben Tannenwald
// Date:    Mar 1, 2018
// Purpose: Make header for histogramming functions

#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH2.h>
#include <TLatex.h>
#include <TString.h>
#include <TObjArray.h>

#include "../cmsStyle/tdrStyle.C"
#include "../cmsStyle/CMS_lumi.C"

#include <sys/stat.h>
#include <sys/types.h>



//void fillEfficiencyHistograms(electronHandler elTool, TObjArray* array, string nameHLT)
void fillEfficiencyHistograms(electronHandler elTool, jetMetHandler jetMetTool, TObjArray* array, string nameHLT)
{
  //cout << "fillEfficiencyHistograms()" << endl;

  TH1D* h0 = new TH1D();

  if ( elTool.passCuts ) { // check electron passing cuts
    // ===  Method B: FAST  ===
    h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + "_el0_pt").c_str() );
    h0->Fill( elTool.leadPt );
    
    h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + "_el0_eta").c_str() );
    h0->Fill( elTool.leadEta );

    h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + "_njets").c_str() );
    h0->Fill( jetMetTool.nJets );

    h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + "_met").c_str() );
    h0->Fill( jetMetTool.MET );
  }

}

void init2DCorrelationHistograms(TObjArray* array, string nameHLT)
{
  TH2D* h_2dCorr = new TH2D();

  for (int i = 0; i < metTriggers.size(); i++) {
    //cout << metTriggers.at(i) << endl;
    h_2dCorr = new TH2D(("h_" + nameHLT + "__vs__" + metTriggers.at(i)).c_str(), ("h_" + nameHLT + "__vs__" + metTriggers.at(i)).c_str(), 2, 0, 2, 2, 0, 2);
    array->AddLast(h_2dCorr); // add histo to TObjArray
  }

  //cout << "number " << nameHLT << " histos: " << array->GetSize() << endl;

}

void initEfficiencyHistograms(TObjArray* array, string nameHLT)
{
  // Leading lepton pT
  const Int_t nbins_pt = 7;
  Double_t edges_pt[nbins_pt + 1] = {20.0, 30.0, 40.0, 60.0, 80.0, 100.0, 200.0, 300.0};
  TH1D* h_pt = new TH1D( ("h_" + nameHLT + "_el0_pt").c_str(),  ("h_" + nameHLT + "_el0_pt").c_str(), nbins_pt, edges_pt );
  h_pt->SetXTitle("Leading Electron p_{T} [GeV]");
  h_pt->SetYTitle("Entries / Bin");

  // Leading lepton eta
  const Int_t nbins_eta = 5;
  Double_t edges_eta[nbins_eta + 1] = {-2.5, -1.5, -0.75, 0.75, 1.5, 2.5};
  TH1D* h_eta = new TH1D( ("h_" + nameHLT + "_el0_eta").c_str(),  ("h_" + nameHLT + "_el0_eta").c_str(), nbins_eta, edges_eta );
  h_eta->SetMinimum(0.0);
  h_eta->SetXTitle("Leading Electron #eta");
  h_eta->SetYTitle("Entries / Bin");

  // N_jets
  TH1D* h_njets = new TH1D( ("h_" + nameHLT + "_njets").c_str(),  ("h_" + nameHLT + "_njets").c_str(), 12, 0, 12);
  h_njets->SetXTitle("N_{jets}");
  h_njets->SetYTitle("Entries / Bin");

  // MET
  const Int_t nbins_met = 11;
  Double_t edges_met[nbins_met + 1] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 125.0, 150.0, 175.0, 200.0, 250.0, 300.0};
  TH1D* h_met = new TH1D( ("h_" + nameHLT + "_met").c_str(),  ("h_" + nameHLT + "_met").c_str(), nbins_met, edges_met );
  h_met->SetXTitle("Missing Transverse Energy (MET) [GeV]");
  h_met->SetYTitle("Entries / Bin");

  // N_vtx
  //TH1D* h_nPV = new TH1D( ("h_" + nameHLT + "_nPV").c_str(),  ("h_" + nameHLT + "_nPV").c_str(), nbins_nPV, edges_nVtx );

  array->AddLast(h_pt);
  array->AddLast(h_eta);
  array->AddLast(h_njets);
  array->AddLast(h_met);

}

void initializeHistograms(TObjArray* array, string nameHLT)
{
  init2DCorrelationHistograms(array, nameHLT);
  initEfficiencyHistograms(array, nameHLT);

  return;
}


void fill2DCorrHistograms(yggdrasilEventVars* eve, TObjArray*& array, string nameHLT, int passHLT)
{
  TH2D* temp = new TH2D();
  TH2D* hist = new TH2D();
  string hname = "";

  for (int i = 0; i < metTriggers.size(); i++) {
    
    hname = ("h_" + nameHLT + "__vs__" + metTriggers.at(i)).c_str();
    // *** 1. get histogram from array
    /*
    // ===  Method A: SAFE  ===
    temp = new TH2D();
    if( array->FindObject( hname.c_str()) )
      temp = (TH2D*)array->FindObject( hname.c_str() );
    else {
      if(verbose) cout<<"returnArrayHisto, NOT FOUND: "<<hname<<endl;
    }
    hist = (TH2D*)temp->Clone(); // store histogram and avoid mem issues (hopefully)
    */

    // ===  Method B: FAST  ===
    hist = (TH2D*)array->FindObject( hname.c_str() );


    // *** 2. Fill histograms depending on variable
    if( metTriggers.at(i) == "HLT_PFHT500_PFMET100_PFMHT100_IDTight") hist->Fill(passHLT, eve->passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT500_PFMET110_PFMHT110_IDTight") hist->Fill(passHLT, eve->passHLT_PFHT500_PFMET110_PFMHT110_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT700_PFMET85_PFMHT85_IDTight") hist->Fill(passHLT, eve->passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT700_PFMET95_PFMHT95_IDTight") hist->Fill(passHLT, eve->passHLT_PFHT700_PFMET95_PFMHT95_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT800_PFMET75_PFMHT75_IDTight") hist->Fill(passHLT, eve->passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT800_PFMET85_PFMHT85_IDTight") hist->Fill(passHLT, eve->passHLT_PFHT800_PFMET85_PFMHT85_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_CaloMET300_HBHECleaned") hist->Fill(passHLT, eve->passHLT_CaloMET300_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_CaloMET350_HBHECleaned") hist->Fill(passHLT, eve->passHLT_CaloMET350_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMET250_HBHECleaned") hist->Fill(passHLT, eve->passHLT_PFMET250_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMET300_HBHECleaned") hist->Fill(passHLT, eve->passHLT_PFMET300_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMET200_HBHE_BeamHaloCleaned") hist->Fill(passHLT, eve->passHLT_PFMET200_HBHE_BeamHaloCleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned") hist->Fill(passHLT, eve->passHLT_PFMETTypeOne200_HBHE_BeamHaloCleaned_v_);
    else if( metTriggers.at(i) == "HLT_MET105_IsoTrk50") hist->Fill(passHLT, eve->passHLT_MET105_IsoTrk50_v_);
    else if( metTriggers.at(i) == "HLT_MET120_IsoTrk50") hist->Fill(passHLT, eve->passHLT_MET120_IsoTrk50_v_);
    else if( metTriggers.at(i) == "HLT_PFMET120_PFMHT120_IDTight") hist->Fill(passHLT, eve->passHLT_PFMET120_PFMHT120_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMET130_PFMHT130_IDTight") hist->Fill(passHLT, eve->passHLT_PFMET130_PFMHT130_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMET140_PFMHT140_IDTight") hist->Fill(passHLT, eve->passHLT_PFMET140_PFMHT140_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1") hist->Fill(passHLT, eve->passHLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1") hist->Fill(passHLT, eve->passHLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_DiJet110_35_Mjj650_PFMET110") hist->Fill(passHLT, eve->passHLT_DiJet110_35_Mjj650_PFMET110_v_);
    else if( metTriggers.at(i) == "HLT_DiJet110_35_Mjj650_PFMET120") hist->Fill(passHLT, eve->passHLT_DiJet110_35_Mjj650_PFMET120_v_);
    else if( metTriggers.at(i) == "HLT_DiJet110_35_Mjj650_PFMET130") hist->Fill(passHLT, eve->passHLT_DiJet110_35_Mjj650_PFMET130_v_);
    else if( metTriggers.at(i) == "HLT_TripleJet110_35_35_Mjj650_PFMET110") hist->Fill(passHLT, eve->passHLT_TripleJet110_35_35_Mjj650_PFMET110_v_);
    else if( metTriggers.at(i) == "HLT_TripleJet110_35_35_Mjj650_PFMET120") hist->Fill(passHLT, eve->passHLT_TripleJet110_35_35_Mjj650_PFMET120_v_);
    else if( metTriggers.at(i) == "HLT_TripleJet110_35_35_Mjj650_PFMET130") hist->Fill(passHLT, eve->passHLT_TripleJet110_35_35_Mjj650_PFMET130_v_);
    else if( metTriggers.at(i) == "HLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1") hist->Fill(passHLT, eve->passHLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1") hist->Fill(passHLT, eve->passHLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1") hist->Fill(passHLT, eve->passHLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET120_PFMHT120_IDTight_PFHT60") hist->Fill(passHLT, eve->passHLT_PFMET120_PFMHT120_IDTight_PFHT60_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60") hist->Fill(passHLT, eve->passHLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne120_PFMHT120_IDTight") hist->Fill(passHLT, eve->passHLT_PFMETTypeOne120_PFMHT120_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne130_PFMHT130_IDTight") hist->Fill(passHLT, eve->passHLT_PFMETTypeOne130_PFMHT130_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne140_PFMHT140_IDTight") hist->Fill(passHLT, eve->passHLT_PFMETTypeOne140_PFMHT140_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_CaloMET250_HBHECleaned") hist->Fill(passHLT, eve->passHLT_CaloMET250_HBHECleaned_v_);

  }

  
}

void draw1DEfficiencyHistograms(TObjArray* array, TCanvas* c0, string nameHLT, string var)
{

  TH1D *h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + "_" + var).c_str() );
  // *** 3. Do the drawing
  c0->cd();
  h0->Draw();
    
  // *** 4. Set CMS style
  //CMS_lumi( canv, iPeriod, iPos ); // <-- notes
  CMS_lumi( c0, 0, 33);
    
  // *** 5. Print plots
  struct stat sb;
  std::string tempDir = (topDir + nameHLT).c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << nameHLT << " subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  if( printPlots) {
    c0->Print( (tempDir + "/" + h0->GetName() + ".eps").c_str());
    c0->Print( (tempDir + "/" + h0->GetName() + ".png").c_str());
  }

}
void plot1DEfficiencyHistograms(TObjArray* array, TCanvas* c0, string nameHLT)
{
  draw1DEfficiencyHistograms(array, c0, nameHLT, "el0_pt");
  draw1DEfficiencyHistograms(array, c0, nameHLT, "el0_eta");
  draw1DEfficiencyHistograms(array, c0, nameHLT, "njets");
  draw1DEfficiencyHistograms(array, c0, nameHLT, "met");

}

void plot2Dcorrelations(TObjArray* array, TCanvas* c0, string nameHLT)
{
  
  TH2D* temp = new TH2D();
  
  for (int i = 0; i < metTriggers.size(); i++) {
    // *** 0. Create histogram name
    string hname = ("h_" + nameHLT + "__vs__" + metTriggers.at(i)).c_str();

    // *** 1. Get histogram from array
    temp = new TH2D();
    if( array->FindObject( hname.c_str()) )
      temp = (TH2D*)array->FindObject( hname.c_str() );
    else {
      if(verbose) cout<<"returnArrayHisto, NOT FOUND: "<<hname<<endl;
    }
    TH2D* h0 = (TH2D*)temp->Clone();

    h0->SetXTitle( nameHLT.c_str() );
    h0->SetYTitle( metTriggers.at(i).c_str() );
 
    // *** 2. Setup LaTeX for printing correlation factor on plot
    TLatex ltx1;
    ltx1.SetTextAlign(9);
    ltx1.SetTextFont(62);
    ltx1.SetTextSize(0.04);
    ltx1.SetNDC();
    
    string s_corr;          // string which will contain the result
    std::ostringstream os_corr;  // stream used for the conversion
    os_corr << std::fixed;
    os_corr << std::setprecision(2) << h0->GetCorrelationFactor();
    s_corr = os_corr.str(); // set 'Result' to the contents of the stream
    
    
    // *** 3. Do the drawing
    c0->cd();
    h0->Draw("colz");
    ltx1.DrawLatex(0.55, 0.80, ("Corr. Factor = " + s_corr).c_str());
    
    cout << "Correlation Factor (" << nameHLT.c_str() << " vs " << metTriggers.at(i).c_str() << "): " << s_corr.c_str() << endl;
    
    // *** 4. Set CMS style
    //CMS_lumi( canv, iPeriod, iPos ); // <-- notes
    CMS_lumi( c0, 0, 11);
    
    
    // *** 5. Print plots
    struct stat sb;
    std::string tempDir = (topDir + "corr2D" + "/" + nameHLT).c_str();
    if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
      cout << nameHLT << " subdirectory , " << tempDir << " , DNE. Creating now" << endl;
      mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
    }
    if( printPlots) {
      c0->Print( (tempDir + "/" + h0->GetName() + ".eps").c_str());
      c0->Print( (tempDir + "/" + h0->GetName() + ".png").c_str());
    }
    
  }    // end metTriggers for loop

  return;
}
