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
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>

#include "../cmsStyle/tdrStyle.C"
#include "../cmsStyle/CMS_lumi.C"

#include <sys/stat.h>
#include <sys/types.h>



void fillEfficiencyHistogramsByStream(muonHandler muTool, electronHandler elTool, jetMetHandler jetMetTool, TObjArray* array, string nameHLT, string stream="")
{
  // initialization
  TH1D* h0 = new TH1D();
  if (stream !=  "")
    stream = ("_" + stream).c_str();

  // ===  Method B: FAST  ===
  //cout << ("h_" + nameHLT + stream + "_el0_pt").c_str() << endl;
  h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + stream + "_el0_pt").c_str() );
  h0->Fill( elTool.leadPt );
  
  h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + stream + "_el0_eta").c_str() );
  h0->Fill( elTool.leadEta );
  
  h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + stream + "_mu0_pt").c_str() );
  h0->Fill( muTool.leadPt );
  
  h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + stream + "_mu0_eta").c_str() );
  h0->Fill( muTool.leadEta );
  
  h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + stream + "_njets").c_str() );
  h0->Fill( jetMetTool.nJets );
  
  h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + stream + "_met").c_str() );
  h0->Fill( jetMetTool.MET );

}

void fillEfficiencyHistograms(muonHandler muTool, electronHandler elTool, jetMetHandler jetMetTool, TObjArray* array, string nameHLT, bool splitStreams=false)
{
  //cout << "fillEfficiencyHistograms()" << endl;
  fillEfficiencyHistogramsByStream( muTool, elTool, jetMetTool, array, nameHLT);

  // fill histograms separated by stream if necessary --> should be needed for denominator in efficiency calculations
  if (splitStreams) {
    if ( elTool.passCuts ) fillEfficiencyHistogramsByStream( muTool, elTool, jetMetTool, array, nameHLT, "elStream");
    if ( muTool.passCuts ) fillEfficiencyHistogramsByStream( muTool, elTool, jetMetTool, array, nameHLT, "muStream");
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

void createEfficiencyHistograms(TObjArray* array, string nameHLT, string stream="")
{
  // Leading electron pT
  const Int_t nbins_pt = 7;
  Double_t edges_pt[nbins_pt + 1] = {20.0, 30.0, 40.0, 60.0, 80.0, 100.0, 200.0, 300.0};
  TH1D* h_el_pt = new TH1D( ("h_" + nameHLT + stream + "_el0_pt").c_str(),  ("h_" + nameHLT + stream + "_el0_pt").c_str(), nbins_pt, edges_pt );
  h_el_pt->SetXTitle("Leading Electron p_{T} [GeV]");
  h_el_pt->SetYTitle("Entries / Bin");
  TH1D* h_mu_pt = new TH1D( ("h_" + nameHLT + stream + "_mu0_pt").c_str(),  ("h_" + nameHLT + stream + "_mu0_pt").c_str(), nbins_pt, edges_pt );
  h_mu_pt->SetXTitle("Leading Muon p_{T} [GeV]");
  h_mu_pt->SetYTitle("Entries / Bin");

  // Leading electron eta
  const Int_t nbins_eta = 5;
  Double_t edges_eta[nbins_eta + 1] = {-2.5, -1.5, -0.75, 0.75, 1.5, 2.5};
  TH1D* h_el_eta = new TH1D( ("h_" + nameHLT + stream + "_el0_eta").c_str(),  ("h_" + nameHLT + stream + "_el0_eta").c_str(), nbins_eta, edges_eta );
  h_el_eta->SetMinimum(0.0);
  h_el_eta->SetXTitle("Leading Electron #eta");
  h_el_eta->SetYTitle("Entries / Bin");
  TH1D* h_mu_eta = new TH1D( ("h_" + nameHLT + stream + "_mu0_eta").c_str(),  ("h_" + nameHLT + stream + "_mu0_eta").c_str(), nbins_eta, edges_eta );
  h_mu_eta->SetMinimum(0.0);
  h_mu_eta->SetXTitle("Leading Muon #eta");
  h_mu_eta->SetYTitle("Entries / Bin");

  // N_jets
  TH1D* h_njets = new TH1D( ("h_" + nameHLT + stream + "_njets").c_str(),  ("h_" + nameHLT + stream + "_njets").c_str(), 12, 0, 12);
  h_njets->SetXTitle("N_{jets}");
  h_njets->SetYTitle("Entries / Bin");

  // MET
  const Int_t nbins_met = 11;
  Double_t edges_met[nbins_met + 1] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 125.0, 150.0, 175.0, 200.0, 250.0, 300.0};
  TH1D* h_met = new TH1D( ("h_" + nameHLT + stream + "_met").c_str(),  ("h_" + nameHLT + stream + "_met").c_str(), nbins_met, edges_met );
  h_met->SetXTitle("Missing Transverse Energy (MET) [GeV]");
  h_met->SetYTitle("Entries / Bin");

  // N_vtx
  //TH1D* h_nPV = new TH1D( ("h_" + nameHLT + stream + "_nPV").c_str(),  ("h_" + nameHLT + stream + "_nPV").c_str(), nbins_nPV, edges_nVtx );

  array->AddLast(h_el_pt);
  array->AddLast(h_el_eta);
  array->AddLast(h_mu_pt);
  array->AddLast(h_mu_eta);
  array->AddLast(h_njets);
  array->AddLast(h_met);
}


void initEfficiencyHistograms(TObjArray* array, string nameHLT, bool splitStreams)
{
  // always create general histograms
  createEfficiencyHistograms(array, nameHLT);
  if (splitStreams) { // create plots split by presence of good electron or muon if necessary --> should be needed for denominator in efficiency calculations
    createEfficiencyHistograms(array, nameHLT, "_elStream");
    createEfficiencyHistograms(array, nameHLT, "_muStream");
  }

}

void initializeHistograms(TObjArray* array, string nameHLT, bool splitStreams=false)
{
  init2DCorrelationHistograms(array, nameHLT);
  initEfficiencyHistograms(array, nameHLT, splitStreams);

  return;
}


void fill2DCorrHistogramsWithLabels(TH2D*& hist, int passHLT_lep, int passHLT_MET)
{
  if (passHLT_lep && passHLT_MET)
    hist->Fill("Fired", "Fired", 1);
  else if (passHLT_lep && !passHLT_MET)
    hist->Fill("Fired", "Not Fired", 1);
  else if (!passHLT_lep && passHLT_MET)
    hist->Fill("Not Fired", "Fired", 1);
  else // !passHLT_lep && !passHLT_MET)
    hist->Fill("Not Fired", "Not Fired", 1);

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

    if( metTriggers.at(i) == "HLT_PFHT500_PFMET100_PFMHT100_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFHT500_PFMET100_PFMHT100_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT500_PFMET110_PFMHT110_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFHT500_PFMET110_PFMHT110_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT700_PFMET85_PFMHT85_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFHT700_PFMET85_PFMHT85_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT700_PFMET95_PFMHT95_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFHT700_PFMET95_PFMHT95_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT800_PFMET75_PFMHT75_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFHT800_PFMET75_PFMHT75_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFHT800_PFMET85_PFMHT85_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFHT800_PFMET85_PFMHT85_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_CaloMET300_HBHECleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_CaloMET300_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_CaloMET350_HBHECleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_CaloMET350_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMET250_HBHECleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET250_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMET300_HBHECleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET300_HBHECleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMET200_HBHE_BeamHaloCleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET200_HBHE_BeamHaloCleaned_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMETTypeOne200_HBHE_BeamHaloCleaned_v_);
    else if( metTriggers.at(i) == "HLT_MET105_IsoTrk50")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_MET105_IsoTrk50_v_);
    else if( metTriggers.at(i) == "HLT_MET120_IsoTrk50")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_MET120_IsoTrk50_v_);
    else if( metTriggers.at(i) == "HLT_PFMET120_PFMHT120_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET120_PFMHT120_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMET130_PFMHT130_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET130_PFMHT130_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMET140_PFMHT140_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET140_PFMHT140_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET100_PFMHT100_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET110_PFMHT110_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_DiJet110_35_Mjj650_PFMET110")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_DiJet110_35_Mjj650_PFMET110_v_);
    else if( metTriggers.at(i) == "HLT_DiJet110_35_Mjj650_PFMET120")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_DiJet110_35_Mjj650_PFMET120_v_);
    else if( metTriggers.at(i) == "HLT_DiJet110_35_Mjj650_PFMET130")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_DiJet110_35_Mjj650_PFMET130_v_);
    else if( metTriggers.at(i) == "HLT_TripleJet110_35_35_Mjj650_PFMET110")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_TripleJet110_35_35_Mjj650_PFMET110_v_);
    else if( metTriggers.at(i) == "HLT_TripleJet110_35_35_Mjj650_PFMET120")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_TripleJet110_35_35_Mjj650_PFMET120_v_);
    else if( metTriggers.at(i) == "HLT_TripleJet110_35_35_Mjj650_PFMET130")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_TripleJet110_35_35_Mjj650_PFMET130_v_);
    else if( metTriggers.at(i) == "HLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET120_PFMHT120_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET130_PFMHT130_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET140_PFMHT140_IDTight_CaloBTagCSV_3p1_v_);
    else if( metTriggers.at(i) == "HLT_PFMET120_PFMHT120_IDTight_PFHT60")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMET120_PFMHT120_IDTight_PFHT60_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne120_PFMHT120_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMETTypeOne120_PFMHT120_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne130_PFMHT130_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMETTypeOne130_PFMHT130_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_PFMETTypeOne140_PFMHT140_IDTight")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_PFMETTypeOne140_PFMHT140_IDTight_v_);
    else if( metTriggers.at(i) == "HLT_CaloMET250_HBHECleaned")fill2DCorrHistogramsWithLabels(hist, passHLT, eve->passHLT_CaloMET250_HBHECleaned_v_);

  }

  
}

void draw1DHistograms(TObjArray* array, TCanvas* c0, string nameHLT, string var)
{

  TH1D *h0 = (TH1D*)array->FindObject( ("h_" + nameHLT + "_" + var).c_str() );
  // *** 3. Do the drawing
  c0->cd();
  h0->Sumw2();
  h0->Draw("e");

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

  //if (dumpFile)
  //  outfile->Write();
}
void plot1DHistograms(TObjArray* array, TCanvas* c0, string nameHLT)
{
  draw1DHistograms(array, c0, nameHLT, "el0_pt");
  draw1DHistograms(array, c0, nameHLT, "el0_eta");
  draw1DHistograms(array, c0, nameHLT, "mu0_pt");
  draw1DHistograms(array, c0, nameHLT, "mu0_eta");
  draw1DHistograms(array, c0, nameHLT, "njets");
  draw1DHistograms(array, c0, nameHLT, "met");

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

void drawEfficiencyHistograms(TCanvas* c0, TObjArray* a_numerator, string nameHLT_num, TObjArray* a_denominator, string nameHLT_denom, string var)
{

  // *** 1. Get the histograms
  TH1D* h_num   = (TH1D*)a_numerator->FindObject( ("h_" + nameHLT_num + "_" + var).c_str() );
  TH1D* h_denom = (TH1D*)a_denominator->FindObject( ("h_" + nameHLT_denom + "_" + var).c_str() );
  h_num->Sumw2();
  h_denom->Sumw2();
  
  cout << h_num->GetName() << " has " << h_num->GetEntries() << " entries." << endl;
  cout << h_denom->GetName() << " has " << h_denom->GetEntries() << " entries." << endl;

  // *** 2. Divide to get efficiency
  TH1D* h_eff = (TH1D*)h_num->Clone();
  string s_eff = h_num->GetName();
  s_eff = (s_eff + "_Eff").c_str();

  h_eff->SetName( s_eff.c_str() );
  h_eff->SetTitle( s_eff.c_str() );
  h_eff->Divide(h_denom);
  h_eff->SetYTitle("Efficiency / Bin");
  //if(h_eff->GetMaximum() < 1.0)
  h_eff->SetMaximum(1.1);
  //if(h_eff->GetMinimum() > 0.0)
  h_eff->SetMinimum(0.0);

  // *** 3. Do the drawing
  c0->cd();
  h_eff->Sumw2();
  h_eff->Draw("e");

  // *** 4. Setup LaTeX for printing correlation factor on plot
  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.25, 0.80, nameHLT_num.c_str() );

  // *** 5. Set CMS style
  //CMS_lumi( canv, iPeriod, iPos ); // <-- notes
  CMS_lumi( c0, 0, 33);
    
  // *** 6. Print plots
  struct stat sb;
  std::string tempDir = (topDir + nameHLT_num).c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << nameHLT_num << " subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  tempDir = (tempDir + "/efficiency1D/").c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << nameHLT_num << " subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  if( printPlots) {
    c0->Print( (tempDir + h_eff->GetName() + ".eps").c_str());
    c0->Print( (tempDir + h_eff->GetName() + ".png").c_str());
  }

}

void drawEfficiencyHistograms_v2(TCanvas* c0, TObjArray* a_numerator, string nameHLT_num, TObjArray* a_denominator, string nameHLT_denom, string var)
{

  // *** 1. Get the histograms
  TH1D* h_num   = (TH1D*)a_numerator->FindObject( ("h_" + nameHLT_num + "_" + var).c_str() );
  TH1D* h_denom = (TH1D*)a_denominator->FindObject( ("h_" + nameHLT_denom + "_" + var).c_str() );
  h_num->Sumw2();
  h_denom->Sumw2();
  
  cout << h_num->GetName() << " has " << h_num->GetEntries() << " entries." << endl;
  cout << h_denom->GetName() << " has " << h_denom->GetEntries() << " entries." << endl;

  // *** 2. Divide to get efficiency
  TH1D* h_eff = (TH1D*)h_num->Clone();
  string s_eff = h_num->GetName();
  s_eff = (s_eff + "_TH1").c_str();

  //h_eff->Divide(h_denom);
  h_eff->SetName( s_eff.c_str() );
  h_eff->SetTitle( s_eff.c_str() );
  h_eff->SetYTitle("Efficiency / Bin");
  h_eff->SetMaximum(1.1);
  h_eff->SetMinimum(0.0);

  TEfficiency* tEff = new TEfficiency(*h_eff, *h_denom);
  s_eff = h_num->GetName();
  s_eff = (s_eff + "_TEff").c_str();
  tEff->SetName( s_eff.c_str() );
  tEff->SetTitle( s_eff.c_str() );

  // *** 3. Do the drawing
  //h_eff->Sumw2();
  c0->cd();
  tEff->Draw();
  tEff->Paint("");
  TGraphAsymmErrors* gr = (TGraphAsymmErrors*)tEff->GetPaintedGraph();
  gr->SetMinimum(0);
  gr->SetMaximum(1.1);
  cout << s_eff << ", Min: " << h_eff->GetXaxis()->GetXmin()  << ", Max: " << h_eff->GetXaxis()->GetXmax() << endl;
  gr->GetXaxis()->SetRangeUser( h_eff->GetXaxis()->GetXmin(), h_eff->GetXaxis()->GetXmax() );
 
  tEff->Draw();
  //c0->Update();

  // *** 4. Setup LaTeX for printing correlation factor on plot
  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.25, 0.80, nameHLT_num.c_str() );

  // *** 5. Set CMS style
  //CMS_lumi( canv, iPeriod, iPos ); // <-- notes
  CMS_lumi( c0, 0, 33);
    
  // *** 6. Print plots
  struct stat sb;
  std::string tempDir = (topDir + nameHLT_num ).c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << nameHLT_num << " subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  tempDir = (tempDir + "/efficiency1D/").c_str();
  if (!(stat(tempDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << nameHLT_num << " subdirectory , " << tempDir << " , DNE. Creating now" << endl;
    mkdir(tempDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }
  if( printPlots) {
    c0->Print( (tempDir + tEff->GetName() + ".eps").c_str());
    c0->Print( (tempDir + tEff->GetName() + ".png").c_str());
  }
  
  // add to efficiency for storing in outfile
  a_Efficiencies->AddLast(tEff);
}

void makeEfficiencyHistograms(TCanvas* c0, TObjArray* a_numerator, string nameHLT_num, TObjArray* a_denominator, string nameHLT_denom)
{
  /*drawEfficiencyHistograms(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "el0_pt");
  drawEfficiencyHistograms(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "el0_eta");
  drawEfficiencyHistograms(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "mu0_pt");
  drawEfficiencyHistograms(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "mu0_eta");
  drawEfficiencyHistograms(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "njets");
  drawEfficiencyHistograms(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "met");
  */
  drawEfficiencyHistograms_v2(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "el0_pt");
  drawEfficiencyHistograms_v2(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "el0_eta");
  drawEfficiencyHistograms_v2(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "mu0_pt");
  drawEfficiencyHistograms_v2(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "mu0_eta");
  drawEfficiencyHistograms_v2(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "njets");
  drawEfficiencyHistograms_v2(c0, a_numerator, nameHLT_num, a_denominator, nameHLT_denom, "met");
  
}
