#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TLegend.h"
#include "TLatex.h"

#include "../cmsStyle/tdrStyle.C"
#include "../cmsStyle/CMS_lumi.C"

#include <iostream>

std::string topDir = "06-11-18_files/";

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
  leg->AddEntry(t_data, "2017 RunC", "el");
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
  leg->AddEntry(t_data, "2017 RunC", "el");
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


void produceCombinedEff()
{
  //TFile* mc_ttbar     = new TFile( (topDir + "/outfile_ttbarMC_pres_04-16-18.root").c_str(), "READ");
  //TFile* data_el_runC = new TFile( (topDir + "/outfile_singleElectron_Run2017C_pres_04-16-18.root").c_str(), "READ");
  //TFile* data_mu_runC = new TFile( (topDir + "/outfile_singleMuon_Run2017C_pres_04-16-18.root").c_str(), "READ");
  TFile* mc_ttbar     = new TFile( (topDir + "/outfile_ttbar_v7_r0_06-11-18.root").c_str(), "READ");
  TFile* data_MET_runC = new TFile( (topDir + "/outfile_MET_v7_r0_06-11-18.root").c_str(), "READ");
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
  
  
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleEl__X__allMET", "el0_pt");
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleEl__X__allMET", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleEl__X__allMET", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleEl__X__allMET", "met" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleMu__X__allMET", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleMu__X__allMET", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleMu__X__allMET", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "SingleMu__X__allMET", "njets" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "el0_pt");
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "el0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "njets" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "met" );

  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "mu0_pt" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "mu0_eta" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "met" );
  drawDoubleEfficiency( c1, mc_ttbar, data_MET_runC, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "njets" );
  

  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_Ele35_WPTight_Gsf", "el0_pt");
  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_Ele35_WPTight_Gsf", "el0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_Ele35_WPTight_Gsf", "njets" );
  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_Ele35_WPTight_Gsf", "met" );

  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_IsoMu27", "mu0_pt" );
  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_IsoMu27", "mu0_eta" );
  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_IsoMu27", "met" );
  drawDoubleHist( c1, mc_ttbar, data_MET_runC, "HLT_IsoMu27", "njets" );
  
}

