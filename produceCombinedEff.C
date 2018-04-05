#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TEfficiency.h"

#include "../cmsStyle/tdrStyle.C"
#include "../cmsStyle/CMS_lumi.C"

#include <iostream>

void drawDoubleEfficiency(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable)
{
  TEfficiency* t_ttbar = (TEfficiency*)ttbar->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_data = (TEfficiency*)data->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  c0->cd();
  t_ttbar->SetMarkerStyle(0);
  t_ttbar->Draw();
  t_ttbar->Paint("");
  TGraphAsymmErrors* gr = (TGraphAsymmErrors*)t_ttbar->GetPaintedGraph();
  gr->SetMinimum(0);
  gr->SetMaximum(1.1);
  //cout << s_eff << ", Min: " << h_eff->GetXaxis()->GetXmin()  << ", Max: " << h_eff->GetXaxis()->GetXmax() << endl;
  //gr->GetXaxis()->SetRangeUser( h_eff->GetXaxis()->GetXmin(), h_eff->GetXaxis()->GetXmax() );
  t_data->SetLineColor(kRed);
  t_data->SetMarkerStyle(0);
  t_data->Draw("same");

  CMS_lumi( c0, 0, 33);
  
  c0->Print( ("h_" + triggerSet + "_" + variable + ".png").c_str() );
}


void produceCombinedEff()
{
  TFile* mc_ttbar     = new TFile("outfile_ttbarMC_full_04-04-18.root", "READ");
  TFile* data_el_runC = new TFile("outfile_singleElectron_Run2017C_full_04-04-18.root", "READ");
  TFile* data_mu_runC = new TFile("outfile_singleMuon_Run2017C_full_04-04-18.root", "READ");

  TCanvas* c1 = new TCanvas("c1", "c1", 1000, 1000);

  // ** D. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  

  drawDoubleEfficiency( c1, mc_ttbar, data_el_runC, "SingleEl__X__allMET", "el0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_el_runC, "SingleEl__X__allMET", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_el_runC, "SingleEl__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_el_runC, "SingleEl__X__allMET", "met" );

  drawDoubleEfficiency( c1, mc_ttbar, data_mu_runC, "SingleMu__X__allMET", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_mu_runC, "SingleMu__X__allMET", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_mu_runC, "SingleMu__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_mu_runC, "SingleMu__X__allMET", "njets" );
}

