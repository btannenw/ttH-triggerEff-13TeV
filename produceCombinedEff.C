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


void drawDoubleEfficiency(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable)
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
  leg->Draw("same");
  
  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.2, 0.52, triggerSet.c_str());

  //c0->Update();
  //gr->GetYaxis()->SetTitle("Efficiency / Bin");
  //c0->Modified();
  
  CMS_lumi( c0, 0, 33);

  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/h_" + triggerSet + "_" + variable + ".png").c_str() );
  //c0->Print( (topDir + "/h_" + triggerSet + "_" + variable + ".C").c_str() );
}

void drawDoubleHist(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable)
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
  topDir = "08-19-18_files/";

  //TFile* mc_ttbar     = new TFile( (topDir + "/outfile_ttbarMC_pres_04-16-18.root").c_str(), "READ");
  //TFile* data_el_runC = new TFile( (topDir + "/outfile_singleElectron_Run2017C_pres_04-16-18.root").c_str(), "READ");
  //TFile* data_mu_runC = new TFile( (topDir + "/outfile_singleMuon_Run2017C_pres_04-16-18.root").c_str(), "READ");
  TFile* mc_ttbar     = new TFile( (topDir + "/outfile_ttbarMC_v7_r5_08-19-18.root").c_str(), "READ");
  TFile* data_MET = new TFile( (topDir + "/outfile_MET_RunBCDEF_v7_r5.root").c_str(), "READ");
  //TFile* data_mu_runC = new TFile( (topDir + "/outfile_MET_v7_r0_06-11-18.root").c_str(), "READ");

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);

  // ** D. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  
  cmsTextSize = 0.3;
  lumiTextSize = 0.3;
  
  
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
  

  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "el0_pt");
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "el0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "njets" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "met" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "nPV" );

  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "mu0_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "mu0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "met" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "njets" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "nPV" );

  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu0_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu1_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "mu1_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "met" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "njets" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleMu", "nPV" );

  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el0_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el1_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "el1_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "met" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "njets" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_DoubleEl", "nPV" );

  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "el0_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "el0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "mu0_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "mu0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "met" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "njets" );
  drawDoubleHist( c1, mc_ttbar, data_MET, "HLT_EMu", "nPV" );

  // dump some correlation madness
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Ele35_WPTight_Gsf");
  dumpCorrelationNumbers(mc_ttbar, "ttbar",  "HLT_IsoMu27");
  dumpCorrelationNumbers(mc_ttbar, "ttbar",  "HLT_IsoMu24_eta2p1");
  dumpCorrelationNumbers(data_MET, "dataBCDEF", "HLT_Ele35_WPTight_Gsf");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_IsoMu27");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_IsoMu24_eta2p1");
}
