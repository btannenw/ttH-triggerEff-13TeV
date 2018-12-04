#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLine.h"
#include "TObjArray.h"

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
  gStyle->SetPaintTextFormat("1.3f");

  c0->cd();
  h2->Draw("colz TEXT");

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


TH2D* get2DScaleFactorDifferenceHistogram(TCanvas* c0, TH2D* h_nom, TH2D* h_nJetsHigh, TH2D* h_nJetsLow, TH2D* h_nPVHigh, TH2D* h_nPVLow, string triggerSet, string variable)
{
  TH2D* h_fullDiff = (TH2D*)h_nom->Clone();
  for(int x_b=1; x_b < h_nom->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < h_nom->GetNbinsY()+1; y_b++) {
      h_fullDiff->SetBinContent(x_b, y_b, 0);
      h_fullDiff->SetBinError(x_b, y_b, 0);
    }
  }

  TH2D* h_njh = (TH2D*)h_nom->Clone();
  h_njh->Add(h_nJetsHigh, -1);
  TH2D* h_njl = (TH2D*)h_nom->Clone();
  h_njl->Add(h_nJetsLow, -1);
  TH2D* h_npvh = (TH2D*)h_nom->Clone();
  h_npvh->Add(h_nPVHigh, -1);
  TH2D* h_npvl = (TH2D*)h_nom->Clone();
  h_npvl->Add(h_nPVLow, -1);

  for(int x_b=1; x_b < h_nom->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < h_nom->GetNbinsY()+1; y_b++) {
      
      if( abs(h_njh->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_njh->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_njh->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_njl->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_njl->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_njl->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_npvh->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_npvh->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_npvh->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_npvl->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_npvl->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_npvl->GetBinError(x_b, y_b));
      } // end if statement for nJets high

    } // y_b loop
  } // x_b loop

  
  string maxName = h_fullDiff->GetName();
  h_fullDiff->SetName(  (maxName + "_maxDiff_nJetsNPV").c_str() );
  h_fullDiff->SetTitle( (maxName + "_maxDiff_nJetsNPV").c_str() );
  
  print2DScaleFactorHistogramSimple(c0, h_fullDiff, triggerSet, (variable + "_maxDiff_nJetsNPV").c_str() );
  
  return h_fullDiff;

}



TH2D* get2DScaleFactorDifferenceHistogram_v2(TCanvas* c0, TH2D* h_nom, TH2D* h_nJetsHigh, TH2D* h_nJetsLow, TH2D* h_nPVHigh, TH2D* h_nPVLow, TH2D* h_METHigh, TH2D* h_METLow, string triggerSet, string variable)
{
  TH2D* h_fullDiff = (TH2D*)h_nom->Clone();
  for(int x_b=1; x_b < h_nom->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < h_nom->GetNbinsY()+1; y_b++) {
      h_fullDiff->SetBinContent(x_b, y_b, 0);
      h_fullDiff->SetBinError(x_b, y_b, 0);
    }
  }

  TH2D* h_njh = (TH2D*)h_nom->Clone();
  h_njh->Add(h_nJetsHigh, -1);
  TH2D* h_njl = (TH2D*)h_nom->Clone();
  h_njl->Add(h_nJetsLow, -1);
  TH2D* h_npvh = (TH2D*)h_nom->Clone();
  h_npvh->Add(h_nPVHigh, -1);
  TH2D* h_npvl = (TH2D*)h_nom->Clone();
  h_npvl->Add(h_nPVLow, -1);
  TH2D* h_meth = (TH2D*)h_nom->Clone();
  h_meth->Add(h_METHigh, -1);
  TH2D* h_metl = (TH2D*)h_nom->Clone();
  h_metl->Add(h_METLow, -1);

  for(int x_b=1; x_b < h_nom->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < h_nom->GetNbinsY()+1; y_b++) {
      
      if( abs(h_njh->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_njh->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_njh->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_njl->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_njl->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_njl->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_npvh->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_npvh->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_npvh->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_npvl->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_npvl->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_npvl->GetBinError(x_b, y_b));
      } // end if statement for nJets high

      if( abs(h_meth->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_meth->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_meth->GetBinError(x_b, y_b));
      } // end if statement for MET high

      if( abs(h_metl->GetBinContent(x_b, y_b)) > abs(h_fullDiff->GetBinContent(x_b, y_b))) {
	h_fullDiff->SetBinContent(x_b, y_b, h_metl->GetBinContent(x_b, y_b));
	h_fullDiff->SetBinError  (x_b, y_b, h_metl->GetBinError(x_b, y_b));
      } // end if statement for MET low

    } // y_b loop
  } // x_b loop

  
  string maxName = h_fullDiff->GetName();
  h_fullDiff->SetName(  (maxName + "_maxDiff_nJetsNPVMET").c_str() );
  h_fullDiff->SetTitle( (maxName + "_maxDiff_nJetsNPVMET").c_str() );
  
  print2DScaleFactorHistogramSimple(c0, h_fullDiff, triggerSet, (variable + "_maxDiff_nJetsNPVMET").c_str() );
  
  delete h_npvl;
  delete h_npvh;
  delete h_metl;
  delete h_meth;
  delete h_njl;
  delete h_njh;

  return h_fullDiff;

}


TH2D* make2DSFwithSysts(TCanvas* c0, TObjArray* array, string triggerSet, string variable){
  string hist = ("h_" + triggerSet + "_" + variable).c_str();
  TH2D* h_nom       = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r2/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );
  TH2D* h_periodDep = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r2/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18_lumiDiff_periodDep.root"))->Get( (hist + "_lumiDiff_periodDep").c_str() );
  TH2D* h_highNjets = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r4/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );
  TH2D* h_lowNjets  = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r5/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );
  TH2D* h_highNPV   = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r6/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );
  TH2D* h_lowNPV    = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r7/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );
  TH2D* h_lowMET    = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r8/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );
  TH2D* h_highMET   = (TH2D*) ((TFile*)array->FindObject("./11-18-18_files/r9/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root"))->Get( hist.c_str() );


  // *** A. Set correlation error
  double totalCorrelationBinError = 0.0;
  if (triggerSet.find("DoubleEl")!=string::npos)      totalCorrelationBinError = 0.001;
  else if (triggerSet.find("DoubleMu")!=string::npos) totalCorrelationBinError = 0.005;
  else if (triggerSet.find("EMu")!=string::npos)      totalCorrelationBinError = 0.001;
    
  // *** B. Get period dependence error (lumi-weighted)
  TH2D* nom_with_systs  = (TH2D*)h_nom->Clone();
  TH2D* syst_periodDep  = (TH2D*)h_periodDep->Clone();
  print2DScaleFactorHistogramSimple(c0, h_periodDep, triggerSet, (variable + "_lumiDiff_periodDep").c_str() );

  // *** C. first get max diff of high/low nJets/nPV w.r.t. nominal
  //TH2D* syst_highLowNjetsNPV = get2DScaleFactorDifferenceHistogram(c0, h_nom, h_highNjets, h_lowNjets, h_highNPV, h_lowNPV, triggerSet, variable);
  TH2D* syst_highLowNjetsNPVMET = get2DScaleFactorDifferenceHistogram_v2(c0, h_nom, h_highNjets, h_lowNjets, h_highNPV, h_lowNPV, h_highMET, h_lowMET, triggerSet, variable);


  // *** D. then calculate full error envelope
  double binError = 0;
  for(int x_b=1; x_b < nom_with_systs->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < nom_with_systs->GetNbinsY()+1; y_b++) {
      //binError = sqrt( nom_with_systs->GetBinError(x_b, y_b)*nom_with_systs->GetBinError(x_b, y_b) + syst_periodDep->GetBinContent(x_b, y_b)*syst_periodDep->GetBinContent(x_b, y_b) + syst_highLowNjetsNPV->GetBinContent(x_b, y_b)*syst_highLowNjetsNPV->GetBinContent(x_b, y_b) + totalCorrelationBinError*totalCorrelationBinError);
      binError = sqrt( nom_with_systs->GetBinError(x_b, y_b)*nom_with_systs->GetBinError(x_b, y_b) + syst_periodDep->GetBinContent(x_b, y_b)*syst_periodDep->GetBinContent(x_b, y_b) + syst_highLowNjetsNPVMET->GetBinContent(x_b, y_b)*syst_highLowNjetsNPVMET->GetBinContent(x_b, y_b) + totalCorrelationBinError*totalCorrelationBinError);
      nom_with_systs->SetBinError(x_b, y_b, binError);
    }
  }
  string hname = nom_with_systs->GetName();
  nom_with_systs->SetName(  (hname + "_withSysts").c_str() );
  nom_with_systs->SetTitle( (hname + "_withSysts").c_str() );

  print2DScaleFactorHistogramSimple(c0, nom_with_systs, triggerSet, (variable + "_withSysts").c_str() );
  
  delete syst_highLowNjetsNPVMET;
  delete syst_periodDep;

  return nom_with_systs;
}

/*
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
*/

void systCombiner()
{
  date = "11-18-18";
  topDir = (date + "_files/").c_str();
  recoVersion = "r2";
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
  TFile *f_nom       = new TFile("./11-18-18_files/r2/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");
  TFile *f_periodDep = new TFile("./11-18-18_files/r2/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18_lumiDiff_periodDep.root", "READ");
  TFile *f_highNjets = new TFile("./11-18-18_files/r4/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");
  TFile *f_lowNjets  = new TFile("./11-18-18_files/r5/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");
  TFile *f_highNPV   = new TFile("./11-18-18_files/r6/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");
  TFile *f_lowNPV    = new TFile("./11-18-18_files/r7/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");
  TFile *f_highMET   = new TFile("./11-18-18_files/r9/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");
  TFile *f_lowMET    = new TFile("./11-18-18_files/r8/tth_dileptonic_2DscaleFactors_2017BCDEF_11-18-18.root", "READ");

  TFile *f_outSysts  = new TFile("./11-18-18_files/r2/tth_dileptonic_2DscaleFactors_withSysts_2017BCDEF_11-26-18.root", "RECREATE");
  
  TObjArray* f_infiles = new TObjArray();
  f_infiles->AddLast(f_nom);
  f_infiles->AddLast(f_periodDep);
  f_infiles->AddLast(f_highNjets);
  f_infiles->AddLast(f_lowNjets);
  f_infiles->AddLast(f_highNPV);
  f_infiles->AddLast(f_lowNPV);
  f_infiles->AddLast(f_highMET);
  f_infiles->AddLast(f_lowMET);

  // *** 2. Get Histograms
  TH2D* h_DoubleMu_mu0_pt_eta      = make2DSFwithSysts(c1, f_infiles, "DoubleMu_OR__X__allMET", "mu0_pt_vs_eta");
  TH2D* h_DoubleMu_mu1_pt_eta      = make2DSFwithSysts(c1, f_infiles, "DoubleMu_OR__X__allMET", "mu1_pt_vs_eta");
  TH2D* h_DoubleMu_mu0_pt_mu1_pt   = make2DSFwithSysts(c1, f_infiles, "DoubleMu_OR__X__allMET", "mu0_pt_vs_mu1_pt");
  TH2D* h_DoubleMu_mu0_eta_mu1_eta = make2DSFwithSysts(c1, f_infiles, "DoubleMu_OR__X__allMET", "mu0_eta_vs_mu1_eta");

  TH2D* h_DoubleEl_el0_pt_eta      = make2DSFwithSysts(c1, f_infiles, "DoubleEl_OR__X__allMET", "el0_pt_vs_eta");
  TH2D* h_DoubleEl_el1_pt_eta      = make2DSFwithSysts(c1, f_infiles, "DoubleEl_OR__X__allMET", "el1_pt_vs_eta");
  TH2D* h_DoubleEl_el0_pt_el1_pt   = make2DSFwithSysts(c1, f_infiles, "DoubleEl_OR__X__allMET", "el0_pt_vs_el1_pt");
  TH2D* h_DoubleEl_el0_eta_el1_eta = make2DSFwithSysts(c1, f_infiles, "DoubleEl_OR__X__allMET", "el0_eta_vs_el1_eta");

  TH2D* h_EMu_mu0_pt_eta      = make2DSFwithSysts(c1, f_infiles, "EMu_OR__X__allMET", "mu0_pt_vs_eta");
  TH2D* h_EMu_el0_pt_eta      = make2DSFwithSysts(c1, f_infiles, "EMu_OR__X__allMET", "el0_pt_vs_eta");
  TH2D* h_EMu_mu0_pt_el0_pt   = make2DSFwithSysts(c1, f_infiles, "EMu_OR__X__allMET", "mu0_pt_vs_el0_pt");
  TH2D* h_EMu_mu0_eta_el0_eta = make2DSFwithSysts(c1, f_infiles, "EMu_OR__X__allMET", "mu0_eta_vs_el0_eta");

  /*
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
  */

  f_outSysts->Write();
  f_outSysts->Close();

  return;
}
