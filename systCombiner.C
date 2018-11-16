#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLine.h"

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

string recoVersion;

void print2DScaleFactorHistogramSimple(TCanvas* c0, TH2D* h2, string triggerSet, string variable)
{
  gStyle->SetPaintTextFormat("1.2f");

  c0->cd();
  h2->Draw("colz TEXT e");

  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.2, 0.57, triggerSet.c_str());
  
  CMS_lumi( c0, 0, 33);

  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/h_2DSF_" + triggerSet + "_" + variable + ".png").c_str() );

}

TH2D* make2DSFwithSysts(TCanvas* c0, TFile* nom, TFile* periodDep, TFile* highMET, TFile* lowMET, string triggerSet, string variable){
  string hist = ("h_" + triggerSet + "_" + variable).c_str();
  TH2D* h_nom       = (TH2D*)nom->Get( hist.c_str() );
  TH2D* h_periodDep = (TH2D*)periodDep->Get( (hist + "_maxDiff_periodDep").c_str() );
  TH2D* h_highMET   = (TH2D*)highMET->Get( hist.c_str() );
  TH2D* h_lowMET    = (TH2D*)lowMET->Get( hist.c_str() );

  TH2D* nom_with_systs  = (TH2D*)h_nom->Clone();
  TH2D* syst_periodDep  = (TH2D*)h_periodDep->Clone();
  TH2D* syst_highLowMET = (TH2D*)h_highMET->Clone();
  syst_highLowMET->Add(h_lowMET, -1);
  syst_highLowMET->Scale(0.5);

  double binError = 0;
  for(int x_b=1; x_b < nom_with_systs->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < nom_with_systs->GetNbinsY()+1; y_b++) {
      binError = sqrt( nom_with_systs->GetBinError(x_b, y_b)*nom_with_systs->GetBinError(x_b, y_b) + syst_periodDep->GetBinContent(x_b, y_b)*syst_periodDep->GetBinContent(x_b, y_b) + syst_highLowMET->GetBinContent(x_b, y_b)*syst_highLowMET->GetBinContent(x_b, y_b) );
      nom_with_systs->SetBinError(x_b, y_b, binError);
    }
  }
  string hname = nom_with_systs->GetName();
  nom_with_systs->SetName(  (hname + "_withSysts").c_str() );
  nom_with_systs->SetTitle( (hname + "_withSysts").c_str() );

  print2DScaleFactorHistogramSimple(c0, nom_with_systs, triggerSet, (variable + "_withSysts").c_str() );

  return nom_with_systs;
}
void systCombiner()
{
  date = "11-15-18";
  topDir = (date + "_files/").c_str();
  recoVersion = "r5";
  topDir = (topDir + "/" + recoVersion + "/").c_str(); 


  // ** 0. Drawing options
  TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);
  // * A. CMS Style
  setTDRStyle();
  writeExtraText = true;       // if extra text                                                
  //extraText  = "Internal";  // default extra text is "Preliminary"
  lumi_sqrtS = "#sqrt{s} = 13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)  
  cmsTextSize = 0.3;
  lumiTextSize = 0.3;

  // *** 1. Set input and output files
  TFile *f_nom       = new TFile("./11-15-18_files/r5/tth_dileptonic_2DscaleFactors_2017BCDEF_11-15-18.root", "READ");
  TFile *f_periodDep = new TFile("./11-15-18_files/r5/tth_dileptonic_2DscaleFactors_2017BCDEF_11-15-18_maxDiff_periodDep.root", "READ");
  TFile *f_highMET   = new TFile("./11-15-18_files/r6/tth_dileptonic_2DscaleFactors_2017BCDEF_11-15-18.root", "READ");
  TFile *f_lowMET    = new TFile("./11-15-18_files/r7/tth_dileptonic_2DscaleFactors_2017BCDEF_11-15-18.root", "READ");
  TFile *f_outSysts  = new TFile("./11-15-18_files/r5/tth_dileptonic_2DscaleFactors_withSysts_2017BCDEF_11-15-18.root", "RECREATE");

  // *** 2. Get Histograms
  TH2D* h_DoubleMu_mu0_pt_eta      = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleMu_OR__X__allMET", "mu0_pt_vs_eta");
  TH2D* h_DoubleMu_mu1_pt_eta      = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleMu_OR__X__allMET", "mu1_pt_vs_eta");
  TH2D* h_DoubleMu_mu0_pt_mu1_pt   = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleMu_OR__X__allMET", "mu0_pt_vs_mu1_pt");
  TH2D* h_DoubleMu_mu0_eta_mu1_eta = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleMu_OR__X__allMET", "mu0_eta_vs_mu1_eta");

  TH2D* h_DoubleEl_el0_pt_eta      = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleEl_OR__X__allMET", "el0_pt_vs_eta");
  TH2D* h_DoubleEl_el1_pt_eta      = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleEl_OR__X__allMET", "el1_pt_vs_eta");
  TH2D* h_DoubleEl_el0_pt_el1_pt   = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleEl_OR__X__allMET", "el0_pt_vs_el1_pt");
  TH2D* h_DoubleEl_el0_eta_el1_eta = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "DoubleEl_OR__X__allMET", "el0_eta_vs_el1_eta");

  TH2D* h_EMu_mu0_pt_eta      = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "EMu_OR__X__allMET", "mu0_pt_vs_eta");
  TH2D* h_EMu_el0_pt_eta      = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "EMu_OR__X__allMET", "el0_pt_vs_eta");
  TH2D* h_EMu_mu0_pt_el0_pt   = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "EMu_OR__X__allMET", "mu0_pt_vs_el0_pt");
  TH2D* h_EMu_mu0_eta_el0_eta = make2DSFwithSysts(c1, f_nom, f_periodDep, f_highMET, f_lowMET, "EMu_OR__X__allMET", "mu0_eta_vs_el0_eta");

  f_outSysts->Write();
  f_outSysts->Close();

  return;
}
