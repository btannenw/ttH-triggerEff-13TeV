#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TLegend.h"
#include "TLatex.h"

#include "../cmsStyle/tdrStyle.C"
#include "../cmsStyle/CMS_lumi.C"
#include "include/trigEffStudy_2017data.h"
// Object Handlers
#include "src/leptonHandler.C"
#include "src/jetMetHandler.C"

#include <iostream>
#include <iomanip>
#include <fstream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


TH1D* getScaleFactorHistogram(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable)
{
  TEfficiency* t_ttbar = (TEfficiency*)ttbar->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_data = (TEfficiency*)data->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TH1D* h_sf = (TH1D*)t_ttbar->GetTotalHistogram()->Clone();

  double ratio = 0;
  double err = 0;
  for (int i =1; i < h_sf->GetNbinsX()+1; i++){
    // ** 1. Get central value of SF
    ratio = t_ttbar->GetEfficiency(i) != 0 ? t_data->GetEfficiency(i)/t_ttbar->GetEfficiency(i) : 0;
    
    // ** 2. Get biggest error and take that as symmetric envelope --> conservative approach. probably a more intelligent way (https://www.jstor.org/stable/2531405)
    err = t_data->GetEfficiencyErrorUp(i);
    if (t_data->GetEfficiencyErrorLow(i) > err)   err = t_data->GetEfficiencyErrorLow(i);
    if (t_ttbar->GetEfficiencyErrorUp(i) > err)    err = t_ttbar->GetEfficiencyErrorUp(i);
    if (t_ttbar->GetEfficiencyErrorLow(i) > err)  err = t_ttbar->GetEfficiencyErrorLow(i);
    if (ratio == 0) err = 0;

    // ** 3. Set SF histogram content and error
    h_sf->SetBinContent(i, ratio);
    h_sf->SetBinError(i, err);
      
  }

  //h_sf->Print("all");
  return h_sf;
}

void drawDoubleEfficiency(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable, bool addSF=true)
{
  TEfficiency* t_ttbar = (TEfficiency*)ttbar->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_data = (TEfficiency*)data->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  c0->cd();
  t_ttbar->SetMarkerStyle(0);
  t_ttbar->Draw();
  t_ttbar->Paint("");
  TGraphAsymmErrors* gr = (TGraphAsymmErrors*)t_ttbar->GetPaintedGraph();
  gr->Draw("ep");
  gr->SetMinimum(0);
  gr->SetMaximum(1.1);
  TH1D* temp = (TH1D*)t_ttbar->GetCopyTotalHisto();
  //cout << s_eff << ", Min: " << h_eff->GetXaxis()->GetXmin()  << ", Max: " << h_eff->GetXaxis()->GetXmax() << endl;
  gr->GetXaxis()->SetRangeUser( temp->GetXaxis()->GetXmin(), temp->GetXaxis()->GetXmax() );
  gr->GetYaxis()->SetTitle("Efficiency / Bin");
  gr->Draw("ep");
  t_data->SetLineColor(kRed);
  t_data->SetMarkerStyle(0);
  t_data->Draw("same");
  //gr->GetHistogram()->GetYaxis()->SetTitle("Efficiency / Bin");

  TLegend* leg = new TLegend(0.7, 0.4, .85, .5);
  leg->AddEntry(t_data, "2017 RunBCDEF", "el");
  leg->AddEntry(t_ttbar, "t#bar{t} MC", "el");

  
  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.2, 0.52, triggerSet.c_str());

  //c0->Update();
  //gr->GetYaxis()->SetTitle("Efficiency / Bin");
  //c0->Modified();
  
  if(addSF){
    TH1D* h_sf = getScaleFactorHistogram(c0, ttbar, data, triggerSet, variable);
    h_sf->SetLineColor(kBlue);
    h_sf->SetMarkerStyle(0);
    h_sf->Draw("same ep");
    leg->AddEntry(h_sf, "Data/MC SF", "el");
  }

  leg->Draw("same");
  CMS_lumi( c0, 0, 33);

  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/h_" + triggerSet + "_" + variable + ".png").c_str() );
  //c0->Print( (topDir + "/h_" + triggerSet + "_" + variable + ".C").c_str() );
}

void drawSingleTriggerTwoSamples(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable)
{
  TH1D* t_ttbar = (TH1D*)ttbar->Get( ("h_" + triggerSet + "_" + variable).c_str() );
  TH1D* t_data = (TH1D*)data->Get( ("h_" + triggerSet + "_" + variable).c_str() );
  c0->cd();

  if (t_ttbar->GetBinContent(t_ttbar->GetMaximumBin())/t_ttbar->Integral() > t_data->GetBinContent(t_data->GetMaximumBin())/t_data->Integral() ) {
    //t_ttbar->SetMarkerStyle(0);
    t_ttbar->GetYaxis()->SetTitle ("Normalized Entries / Bin");
    t_ttbar->SetMarkerStyle(0);
    //t_ttbar->GetYaxis()->SetRangeUser(0, 1.1);
    t_ttbar->DrawNormalized("hist e");
    t_data->SetLineColor(kRed);
    t_data->SetMarkerStyle(0);
    t_data->SetMarkerColor(kRed);
    t_data->DrawNormalized("hist e same");
  }
  else {
    t_data->SetLineColor(kRed);
    t_data->SetMarkerColor(kRed);
    t_data->GetYaxis()->SetTitle ("Normalized Entries / Bin");
    t_data->SetMarkerStyle(0);
    t_data->DrawNormalized("hist e");
    t_ttbar->SetMarkerStyle(0);
    t_ttbar->DrawNormalized("hist e same");
  }

  TLegend* leg = new TLegend(0.7, 0.7, .85, .8);
  leg->AddEntry(t_data, "2017 RunBCDEF", "el");
  leg->AddEntry(t_ttbar, "t#bar{t} MC", "el");
  leg->Draw("same");
  
  //TLatex ltx1;
  //ltx1.SetTextAlign(9);
  //ltx1.SetTextFont(62);
  //ltx1.SetTextSize(0.025);
  //ltx1.SetNDC();
  //ltx1.DrawLatex(0.2, 0.52, triggerSet.c_str());

  
  CMS_lumi( c0, 0, 33);

  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetBottomMargin(0.15);
  //c0->SetTopMargin(0.05);

  c0->Print( (topDir + "/h_" + triggerSet + "_" + variable + "_TH1.png").c_str() );
}

void drawNomDenomHistSingleSample(TCanvas* c0, TFile* f0, string sample, string triggerNom, string triggerDenom, string variable)
{
  TH1D* h_nom   = (TH1D*)f0->Get( ("h_" + triggerNom + "_" + variable).c_str() );
  TH1D* h_denom = (TH1D*)f0->Get( ("h_" + triggerDenom + "_" + variable).c_str() );

  c0->cd();

  
  h_nom->SetMarkerColor(kBlack);
  h_nom->SetLineColor(kBlack);
  h_denom->SetLineColor(kGreen+2);
  h_denom->SetMarkerColor(kGreen+2);
  //h_nom->SetMarkerStyle(0);
  //h_denom->SetMarkerStyle(0);
  if (h_nom->GetMaximum() > h_denom->GetMaximum()) {
    h_nom->Draw("e");
    h_denom->Draw("e same");
  }
  else {
    h_denom->Draw("e");
    h_nom->Draw("e same");
  }

  cout << h_nom->GetTitle() << "->Integral() = " << h_nom->Integral() << "\t " << h_denom->GetTitle() << "->Integral() = " << h_denom->Integral() << endl;
  TLegend* leg = new TLegend(0.70, 0.75, .95, .85);
  leg->AddEntry(h_nom, triggerNom.c_str(), "el");
  leg->AddEntry(h_denom, triggerDenom.c_str(), "el");
  leg->Draw("same");

  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.023);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.80, 0.86, sample.c_str());

  c0->Print( (topDir + "/h_" + sample + "_Nom_" + triggerNom + "_Denom_" + triggerDenom + "_" + variable + ".png").c_str() );
 
}

void dumpCorrelationNumbers(TFile* f0, string sample, string triggerSet)
{
  ofstream corrTXT;
  corrTXT.open( (topDir + "/correlations2D_" + sample + "_" + triggerSet+ ".txt").c_str() );

  // variables that i may or may not need
  TH2D* h_corr = new TH2D();
  double d_lepFired_metFired = 0;
  double d_lepNotFired_metFired = 0;
  double d_lepFired_metNotFired = 0;
  double d_lepNotFired_metNotFired = 0;
  double e_lepFired_metFired = 0;
  double e_lepNotFired_metFired = 0;
  double e_lepFired_metNotFired = 0;
  double e_lepNotFired_metNotFired = 0;
  double eff_lep = 0;
  double eff_met = 0;
  double eff_lepXmet = 0;
  double eff_lep_err = 0;
  double eff_met_err = 0;
  double eff_lepXmet_err = 0;
  double total = 0;
  double total_err = 0;
  double correlation = 0;
  double correlation_err = 0;
  

  for (int i = 0; i < metTriggers.size(); i++) {
    //cout << metTriggers.at(i) << endl;
    h_corr = (TH2D*)f0->Get( ("h_" + triggerSet + "__vs__" + metTriggers.at(i)).c_str() );
    // Notes: x-axis is lepton trigger, y-axis is MET trigger
    //corrTXT << "Get correlation for " << ("h_" + triggerSet + "__vs__" + metTriggers.at(i)).c_str() << endl;
    //corrTXT << "X-axis: " << h_corr->GetXaxis()->GetTitle() << ", Y-axis: " << h_corr->GetYaxis()->GetTitle() << endl;

    // ** Step 1: Get bin values and errors from histogram
    d_lepFired_metFired = h_corr->GetBinContent(h_corr->GetXaxis()->FindBin("Fired"), h_corr->GetYaxis()->FindBin("Fired"));
    d_lepNotFired_metFired = h_corr->GetBinContent(h_corr->GetXaxis()->FindBin("Not Fired"), h_corr->GetYaxis()->FindBin("Fired"));
    d_lepFired_metNotFired = h_corr->GetBinContent(h_corr->GetXaxis()->FindBin("Fired"), h_corr->GetYaxis()->FindBin("Not Fired"));
    d_lepNotFired_metNotFired = h_corr->GetBinContent(h_corr->GetXaxis()->FindBin("Not Fired"), h_corr->GetYaxis()->FindBin("Not Fired"));
    e_lepFired_metFired = h_corr->GetBinError(h_corr->GetXaxis()->FindBin("Fired"), h_corr->GetYaxis()->FindBin("Fired"));
    e_lepNotFired_metFired = h_corr->GetBinError(h_corr->GetXaxis()->FindBin("Not Fired"), h_corr->GetYaxis()->FindBin("Fired"));
    e_lepFired_metNotFired = h_corr->GetBinError(h_corr->GetXaxis()->FindBin("Fired"), h_corr->GetYaxis()->FindBin("Not Fired"));
    e_lepNotFired_metNotFired = h_corr->GetBinError(h_corr->GetXaxis()->FindBin("Not Fired"), h_corr->GetYaxis()->FindBin("Not Fired"));

    // ** Step 2: Calculate efficiencies
    // * A. Intermediate
    total = (d_lepFired_metFired + d_lepFired_metNotFired + d_lepNotFired_metFired + d_lepNotFired_metNotFired);
    total_err = sqrt( e_lepFired_metFired*e_lepFired_metFired + e_lepFired_metNotFired*e_lepFired_metNotFired + e_lepNotFired_metFired*e_lepNotFired_metFired + e_lepNotFired_metNotFired*e_lepNotFired_metNotFired );

    // * B. Efficiencies and errors
    eff_lep = (d_lepFired_metFired + d_lepFired_metNotFired) / total;
    eff_met = (d_lepFired_metFired + d_lepNotFired_metFired) / total;
    eff_lepXmet = (d_lepFired_metFired) / total;
    eff_lep_err = eff_lep * sqrt( (total_err/total)*(total_err/total) + ((e_lepFired_metFired*e_lepFired_metFired) + (e_lepFired_metNotFired*e_lepFired_metNotFired))/((d_lepFired_metFired + d_lepFired_metNotFired)*(d_lepFired_metFired + d_lepFired_metNotFired)) );
    eff_met_err = eff_met * sqrt( (total_err/total)*(total_err/total) + ((e_lepFired_metFired*e_lepFired_metFired) + (e_lepNotFired_metFired*e_lepNotFired_metFired))/((d_lepFired_metFired + d_lepNotFired_metFired)*(d_lepFired_metFired + d_lepNotFired_metFired)) );
    eff_lepXmet_err = eff_lepXmet * sqrt( (total_err/total)*(total_err/total) + ((e_lepFired_metFired*e_lepFired_metFired))/((d_lepFired_metFired*d_lepFired_metFired)) );

    correlation = eff_lep * eff_met / eff_lepXmet;
    correlation_err = correlation * sqrt( (eff_lep_err/eff_lep)*(eff_lep_err/eff_lep) + (eff_met/eff_met)*(eff_met_err/eff_met) + (eff_lepXmet_err/eff_lepXmet)*(eff_lepXmet_err/eff_lepXmet) );

    // ** Step 3: Calculate errors [TO-DO]

    // ** Step 4: Output results
    corrTXT << std::fixed;
    corrTXT << "Correlation between " << h_corr->GetXaxis()->GetTitle() << " and " << h_corr->GetYaxis()->GetTitle() << " = \t" << std::setprecision(3) << correlation << " +/- " << correlation_err << ",\t GetCorrelationFactor() = " << h_corr->GetCorrelationFactor() << endl;  
  }

  corrTXT.close();
}

void produceCombinedEff()
{
  // now sourced from include/trigEffStudy_2017data.h
  topDir = "08-24-18_files/";
  string recoVersion = "r3";
  
  // *** 0. Input/output setup
  // ** I. Read files
  TFile* mc_ttbar     = new TFile( (topDir + "/outfile_ttbarMC_v7_" + recoVersion + "_08-24-18.root").c_str(), "READ");
  TFile* data_MET = new TFile( (topDir + "/outfile_MET_RunBCDEF_v7_" + recoVersion + ".root").c_str(), "READ");



  // ** II. Check subdirectory structure for requested options and create directories if necessary
  topDir = (topDir + "/" + recoVersion + "/").c_str(); 
  // Check if topdir exists
  struct stat sb;
  if (!(stat(topDir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
    cout << "top-level director, " << topDir << " , DNE. Creating now" << endl;
    mkdir(topDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
  }


  // ** III. Drawing options
  TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);
  // * A. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  
  cmsTextSize = 0.3;
  lumiTextSize = 0.3;
  
  
  // *** 2. Draw efficiencies and Scale Factors (sometimes)
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleEl__X__allMET", "el0_pt");
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleEl__X__allMET", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleEl__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleEl__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleEl__X__allMET", "nPV" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleMu__X__allMET", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleMu__X__allMET", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleMu__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleMu__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "SingleMu__X__allMET", "nPV" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "el0_pt");
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "el1_pt");
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "el1_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleEl__X__allMET", "nPV" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "mu1_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "mu1_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "DoubleMu__X__allMET", "nPV" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "el0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "EMu__X__allMET", "nPV" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "el0_pt");
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "met" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "njets" );
  

  // *** 3. Compare trigger yields between two samples
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "el0_pt");
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "el0_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "njets" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "met" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "nPV" );

  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "mu0_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "mu0_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "met" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "njets" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "nPV" );

  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu0_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu0_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu1_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu1_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "met" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "njets" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "nPV" );

  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el0_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el0_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el1_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el1_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "met" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "njets" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "nPV" );

  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "el0_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "el0_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "mu0_pt" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "mu0_eta" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "met" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "njets" );
  drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_EMu", "nPV" );

  // *** 4. Dump some correlation madness
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Ele35_WPTight_Gsf");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_IsoMu27");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_IsoMu24_eta2p1");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ");
  //dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_PFMET120_PFMHT120_IDTight");
  //dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_PFHT250");
  dumpCorrelationNumbers(data_MET, "dataBCDEF", "HLT_Ele35_WPTight_Gsf");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_IsoMu27");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_IsoMu24_eta2p1");

  // *** 5. Produce nominator + denominator plots
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "DoubleEl__X__allMET", "HLT_allMET_elStreamDL", "met");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "DoubleEl__X__allMET", "HLT_allMET_elStreamDL", "el0_pt");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "SingleEl__X__allMET", "HLT_allMET_elStreamSL", "el0_pt");
  drawNomDenomHistSingleSample(c1, data_MET, "dataBCDEF", "DoubleEl__X__allMET", "HLT_allMET_elStreamDL", "met");
  drawNomDenomHistSingleSample(c1, data_MET, "dataBCDEF", "DoubleEl__X__allMET", "HLT_allMET_elStreamDL", "el0_pt");
  drawNomDenomHistSingleSample(c1, data_MET, "dataBCDEF", "SingleEl__X__allMET", "HLT_allMET_elStreamSL", "el0_pt");


}
