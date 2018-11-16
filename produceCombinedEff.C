#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TEfficiency.h"
#include "TLegend.h"
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

void addOverflow(TH1D*& histo)
{
  // put overflow bin at end
  int maxBin = histo->GetNbinsX();
  histo->SetBinContent( maxBin, histo->GetBinContent( maxBin ) + histo->GetBinContent( maxBin+1 ) );
  histo->SetBinError  ( maxBin, sqrt( histo->GetBinError(maxBin)*histo->GetBinError(maxBin) + histo->GetBinError(maxBin+1)*histo->GetBinError(maxBin+1) ) );
  histo->SetBinContent( maxBin + 1, 0 );
  histo->SetBinError( maxBin + 1, 0 );

}

void print2DScaleFactorHistogramSimple(TCanvas* c0, TH2D* h2, string triggerSet, string variable)
{
  gStyle->SetPaintTextFormat("1.2f");

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

TH2D* get2DScaleFactorHistogram(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable)
{
  TEfficiency* t_ttbar = (TEfficiency*)ttbar->Get( ("h_" + triggerSet + "_" + variable + "_TEff2D").c_str() );
  TEfficiency* t_data = (TEfficiency*)data->Get( ("h_" + triggerSet + "_" + variable + "_TEff2D").c_str() );
  cout << "trying for " << ("h_" + triggerSet + "_" + variable ).c_str() << endl;
  TH2D* h_sf = (TH2D*)data->Get( ("h_" + triggerSet + "_" + variable ).c_str() )->Clone();

  double ratio = 0;
  double err = 0;
  for (int i =1; i < h_sf->GetNbinsX()+1; i++){
    for (int j =1; j < h_sf->GetNbinsY()+1; j++){
      // ** 1. Get central value of SF
      ratio = t_ttbar->GetEfficiency( t_ttbar->GetTotalHistogram()->GetBin(i,j) ) != 0 ? t_data->GetEfficiency( t_ttbar->GetTotalHistogram()->GetBin(i,j) )/t_ttbar->GetEfficiency( t_ttbar->GetTotalHistogram()->GetBin(i,j) ) : 0;
      
      // ** 2. Get biggest error and take that as symmetric envelope --> conservative approach. probably a more intelligent way (https://www.jstor.org/stable/2531405)
      err = t_data->GetEfficiencyErrorUp( t_ttbar->GetTotalHistogram()->GetBin(i,j) );
      if (t_data->GetEfficiencyErrorLow( t_ttbar->GetTotalHistogram()->GetBin(i,j) ) > err)   err = t_data->GetEfficiencyErrorLow( t_ttbar->GetTotalHistogram()->GetBin(i,j) );
      if (t_ttbar->GetEfficiencyErrorUp( t_ttbar->GetTotalHistogram()->GetBin(i,j) ) > err)   err = t_ttbar->GetEfficiencyErrorUp( t_ttbar->GetTotalHistogram()->GetBin(i,j) );
      if (t_ttbar->GetEfficiencyErrorLow( t_ttbar->GetTotalHistogram()->GetBin(i,j) ) > err)  err = t_ttbar->GetEfficiencyErrorLow( t_ttbar->GetTotalHistogram()->GetBin(i,j) );
      if (ratio == 0) err = 0;
      
      // ** 3. Set SF histogram content and error
      h_sf->SetBinContent(i, j, ratio);
      h_sf->SetBinError(i, j, err);
    }
  }

  //h_sf->Print("all");
  return h_sf;
}


TH2D* get2DScaleFactorDifferenceHistogram(TCanvas* c0, TObjArray* array, string triggerSet, string variable, string type)
{
  // *** 1. Set BCDEF file, get BCDEF histogram, and initialize difference histogram to 0s
  string hist = ("h_" + triggerSet + "_" + variable).c_str();
  TFile* f_all = (TFile*)array->FindObject( (topDir + "/tth_dileptonic_2DscaleFactors_2017BCDEF_" + date + ".root").c_str() );
  
  TH2D* h2_all     = (TH2D*)f_all->Get( hist.c_str() );
  TH2D* h2_fullDiff = (TH2D*)h2_all->Clone();
  TH2D* h2_tempDiff = new TH2D();
  for(int x_b=1; x_b < h2_all->GetNbinsX()+1; x_b++) {
    for(int y_b=1; y_b < h2_all->GetNbinsY()+1; y_b++) {
      h2_fullDiff->SetBinContent(x_b, y_b, 0);
      h2_fullDiff->SetBinError(x_b, y_b, 0);
    }
  }

  // *** 2A. Set up iterator over SF files and run over them (lumi)
  if (type.find("max")!=string::npos)  {
    TIter next(array); 
    TFile *f_SFfile = new TFile(); 
    while( (f_SFfile = (TFile*)next()) ) 
      { 
	string tempName = f_SFfile->GetName();
	if ( tempName.find("2017BCDEF")==string::npos ) {
	  h2_tempDiff = (TH2D*)f_SFfile->Get( hist.c_str() );
	  h2_tempDiff->Add(h2_all, -1); // get diff of BCDEF w.r.t. period under consideration
	  for(int x_b=1; x_b < h2_all->GetNbinsX()+1; x_b++) {
	    for(int y_b=1; y_b < h2_all->GetNbinsY()+1; y_b++) {
	      if( abs(h2_tempDiff->GetBinContent(x_b, y_b)) > abs(h2_fullDiff->GetBinContent(x_b, y_b))) {
		h2_fullDiff->SetBinContent(x_b, y_b, h2_tempDiff->GetBinContent(x_b, y_b));
		h2_fullDiff->SetBinError  (x_b, y_b, h2_tempDiff->GetBinError(x_b, y_b));
	      } // set max diff for bin
	    } // y_b loop
	  } // x_b loop
	} // end if statement protecting against BCDEF
      } // end loop over SF files
  }// end max option
  else if (type.find("lumi")!=string::npos) {
    TFile* f_B = (TFile*)array->FindObject( (topDir + "/tth_dileptonic_2DscaleFactors_2017B_" + date + ".root").c_str() );
    TFile* f_C = (TFile*)array->FindObject( (topDir + "/tth_dileptonic_2DscaleFactors_2017C_" + date + ".root").c_str() );
    TFile* f_D = (TFile*)array->FindObject( (topDir + "/tth_dileptonic_2DscaleFactors_2017D_" + date + ".root").c_str() );
    TFile* f_E = (TFile*)array->FindObject( (topDir + "/tth_dileptonic_2DscaleFactors_2017E_" + date + ".root").c_str() );
    TFile* f_F = (TFile*)array->FindObject( (topDir + "/tth_dileptonic_2DscaleFactors_2017F_" + date + ".root").c_str() );
    TH2D*  h_B = (TH2D*)f_B->Get(hist.c_str() );
    TH2D*  h_C = (TH2D*)f_C->Get(hist.c_str() );
    TH2D*  h_D = (TH2D*)f_D->Get(hist.c_str() );
    TH2D*  h_E = (TH2D*)f_E->Get(hist.c_str() );
    TH2D*  h_F = (TH2D*)f_F->Get(hist.c_str() );
    h_B->Scale(0.115);
    h_C->Scale(0.232);
    h_D->Scale(0.102);
    h_E->Scale(0.224);
    h_F->Scale(0.326);

    TH2D* lumiSum = (TH2D*)h_B->Clone();
    lumiSum->Add(h_C);
    lumiSum->Add(h_D);
    lumiSum->Add(h_E);
    lumiSum->Add(h_F);
  
    h2_fullDiff = (TH2D*)h2_all->Clone();
    h2_fullDiff->Add(lumiSum, -1);
  }//end lumi option

  string maxName = h2_fullDiff->GetName();
  h2_fullDiff->SetName(  (maxName + "_" + type + "Diff_periodDep").c_str() );
  h2_fullDiff->SetTitle( (maxName + "_" + type + "Diff_periodDep").c_str() );

  print2DScaleFactorHistogramSimple(c0, h2_fullDiff, triggerSet, (variable + "_" + type + "Diff_periodDep").c_str() );

  return h2_fullDiff;
}

void print2DScaleFactorHistogram(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable, bool dumpErrorHisto=false)
{
  gStyle->SetPaintTextFormat("1.2f");

  c0->cd();
  TH2D* h_sf = get2DScaleFactorHistogram(c0, ttbar, data, triggerSet, variable);
  //h_sf->GetYaxis()->SetRangeUser(0, 1.1);
  //h_sf->GetYaxis()->SetTitle("Scale Factor / Bin");
  //h_sf->SetLineColor(kBlue);
  //h_sf->SetMarkerStyle(0);
  h_sf->Draw("colz TEXT e");

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

  // print out 1D histogram of errors of 2D SFs
  if (dumpErrorHisto){
    TH1D* h_histErr = new TH1D( ("h_histErrs2D_" + triggerSet + "_" + variable).c_str(), ("h_histErrs2D_" + triggerSet + "_" + variable).c_str(), 10, 0, 20);
    for (int x = 1; x < h_sf->GetNbinsX()+1; x++) {
      for (int y = 1; y < h_sf->GetNbinsY()+1; y++) {
	h_histErr->Fill( h_sf->GetBinError(x, y)*100 );
      }
    }
    h_histErr->GetYaxis()->SetTitle("Bins / 2% Error");
    h_histErr->GetXaxis()->SetTitle("% Error");
    //h_histErr->Fit("gaus");
    addOverflow(h_histErr);
    c0->cd();
    h_histErr->Draw("HIST");
    ltx1.DrawLatex(0.6, 0.57, triggerSet.c_str());
    ltx1.DrawLatex(0.6, 0.52, variable.c_str());
    string s_mean;          // string which will contain the result
    ostringstream os_mean;   // stream used for the conversion
    os_mean << std::fixed;
    os_mean << setprecision(2) << h_histErr->GetMean() ;      
    s_mean = os_mean.str(); // set 'Result' to the contents of the stream

    ltx1.DrawLatex(0.6, 0.47, ("Mean Bin Err. = " + s_mean +"%").c_str() );
    c0->Print( (topDir + "/h_2DSF_" + triggerSet + "_" + variable + "_errorHisto.png").c_str() );
  }
}

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

void print1DScaleFactorHistogram(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable, bool dumpErrorHisto=true)
{
  gStyle->SetPaintTextFormat("1.3f");

  c0->cd();
  TH1D* h_sf = getScaleFactorHistogram(c0, ttbar, data, triggerSet, variable);
  h_sf->GetYaxis()->SetRangeUser(0, 1.1);
  h_sf->GetYaxis()->SetTitle("Scale Factor / Bin");
  h_sf->SetLineColor(kBlue);
  h_sf->SetMarkerStyle(0);
  h_sf->Draw("TEXT E");

  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.2, 0.52, triggerSet.c_str());
  
  TLine *line1=new TLine(h_sf->GetXaxis()->GetXmin(),1,h_sf->GetXaxis()->GetXmax(),1);
  line1->SetLineStyle(2);
  line1->SetLineWidth(3);
  line1->SetLineColor(kBlue+2);
  line1->Draw("same");

  CMS_lumi( c0, 0, 33);

  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/h_1DSF_" + triggerSet + "_" + variable + ".png").c_str() );

  // print out 1D histogram of errors of 2D SFs
  if (dumpErrorHisto){
    TH1D* h_histErr = new TH1D( ("h_histErrs1D_" + triggerSet + "_" + variable).c_str(), ("h_histErrs1D_" + triggerSet + "_" + variable).c_str(), 10, 0, 20);
    for (int x = 1; x < h_sf->GetNbinsX()+1; x++) {
      h_histErr->Fill( h_sf->GetBinError(x)*100 );
    }
    h_histErr->GetYaxis()->SetTitle("Bins / 2% Error");
    h_histErr->GetXaxis()->SetTitle("% Error");
    //h_histErr->Fit("gaus");
    addOverflow(h_histErr);
    c0->cd();
    h_histErr->Draw("HIST");
    ltx1.DrawLatex(0.6, 0.57, triggerSet.c_str());
    ltx1.DrawLatex(0.6, 0.52, variable.c_str());
    string s_mean;          // string which will contain the result
    ostringstream os_mean;   // stream used for the conversion
    os_mean << std::fixed;
    os_mean << setprecision(2) << h_histErr->GetMean() ;      
    s_mean = os_mean.str(); // set 'Result' to the contents of the stream
    ltx1.DrawLatex(0.6, 0.47, ("Mean Bin Err. = " + s_mean +"%").c_str() );
    c0->Print( (topDir + "/h_1DSF_" + triggerSet + "_" + variable + "_errorHisto.png").c_str() );
  }

}

void print1DScaleFactorHistogramCollection(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet)
{
  if (triggerSet.find("DoubleEl") != string::npos || triggerSet.find("EMu") != string::npos){
    print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "el0_pt");
    print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "el0_eta");
    print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "el0_relIso");
    if (triggerSet.find("DoubleEl") != string::npos){
      print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "el1_pt");
      print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "el1_eta");
    }
  }
  if (triggerSet.find("DoubleMu") != string::npos || triggerSet.find("EMu") != string::npos){
    print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "mu0_pt");
    print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "mu0_eta");
    print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "mu0_relIso");
    if (triggerSet.find("DoubleMu") != string::npos){
      print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "mu1_pt");
      print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "mu1_eta");
    }
  }
  print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "met");
  print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "njets");
  print1DScaleFactorHistogram(c0, ttbar, data, triggerSet, "nPV");

}

void drawDoubleEfficiency(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, string variable, bool addSF=false)
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

void drawDoubleEfficiencyCollection(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet, bool addSF=false)
{
  if (triggerSet.find("DoubleEl") != string::npos || triggerSet.find("EMu") != string::npos || triggerSet.find("SingleEl") != string::npos){
    drawDoubleEfficiency( c0, ttbar, data, triggerSet, "el0_pt", addSF);
    drawDoubleEfficiency( c0, ttbar, data, triggerSet, "el0_eta", addSF );
    if (triggerSet.find("DoubleEl") != string::npos){
      drawDoubleEfficiency( c0, ttbar, data, triggerSet, "el1_pt", addSF);
      drawDoubleEfficiency( c0, ttbar, data, triggerSet, "el1_eta", addSF );
    }
  }
  if (triggerSet.find("DoubleMu") != string::npos || triggerSet.find("EMu") != string::npos || triggerSet.find("SingleMu") != string::npos){
    drawDoubleEfficiency( c0, ttbar, data, triggerSet, "mu0_pt", addSF);
    drawDoubleEfficiency( c0, ttbar, data, triggerSet, "mu0_eta", addSF );
    if (triggerSet.find("DoubleMu") != string::npos){
      drawDoubleEfficiency( c0, ttbar, data, triggerSet, "mu1_pt", addSF);
      drawDoubleEfficiency( c0, ttbar, data, triggerSet, "mu1_eta", addSF );
    }
  }
  drawDoubleEfficiency( c0, ttbar, data, triggerSet, "met", addSF );
  drawDoubleEfficiency( c0, ttbar, data, triggerSet, "njets", addSF );
  drawDoubleEfficiency( c0, ttbar, data, triggerSet, "nPV", addSF );

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

void drawSingleTriggerTwoSamplesCollection(TCanvas* c0, TFile* ttbar, TFile* data, string triggerSet)
{
  if (triggerSet.find("DoubleEl") != string::npos || triggerSet.find("EMu") != string::npos){
    drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "el0_pt" );
    drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "el0_eta" );
    drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "el0_relIso" );
    if (triggerSet.find("DoubleEl") != string::npos){
      drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "el1_pt" );
      drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "el1_eta" );
    }
  }
  if (triggerSet.find("DoubleMu") != string::npos || triggerSet.find("EMu") != string::npos){
    drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "mu0_pt" );
    drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "mu0_eta" );
    drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "mu0_relIso" );
    if (triggerSet.find("DoubleMu") != string::npos){
      drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "mu1_pt" );
      drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "mu1_eta" );
    }
  }
  drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "met" );
  drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "njets" );
  drawSingleTriggerTwoSamples( c0, ttbar, data, triggerSet, "nPV" );
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
  // 09-05-18: cross-check of error and efficiency calculation
  double N1 = 0;
  double N2 = 0;
  double N3 = 0;
  double N4 = 0;
  TH2D* h_total = new TH2D();
  TH2D* h_lepFired = new TH2D();
  TH2D* h_metFired = new TH2D();
  TH2D* h_lepXmetFired = new TH2D();
  TEfficiency* tEff_lep;// = new TEfficiency();
  TEfficiency* tEff_met;// = new TEfficiency();
  TEfficiency* tEff_lepXmet;// = new TEfficiency();
  double Xcorr = 0;
  double Xcorr_err = 0;
  double Xeff_lep = 0;
  double Xeff_met = 0;
  double Xeff_lepXmet = 0;
  double Xeff_lep_err = 0;
  double Xeff_met_err = 0;
  double Xeff_lepXmet_err = 0;

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
    correlation_err = correlation * sqrt( (eff_lep_err/eff_lep)*(eff_lep_err/eff_lep) + (eff_met_err/eff_met)*(eff_met_err/eff_met) + (eff_lepXmet_err/eff_lepXmet)*(eff_lepXmet_err/eff_lepXmet) );

    // ** Step 3: Calculate errors [TO-DO]

    // ** Step 4: Cross-checks
    h_lepFired = (TH2D*)h_corr->Clone( ("hTemp_" + triggerSet + "__vs__" + metTriggers.at(i) + "_lep").c_str() );
    h_metFired = (TH2D*)h_corr->Clone( ("hTemp_" + triggerSet + "__vs__" + metTriggers.at(i) + "_met").c_str() );
    h_lepXmetFired = (TH2D*)h_corr->Clone( ("hTemp_" + triggerSet + "__vs__" + metTriggers.at(i) + "_lepXmet").c_str() );
    h_total    = (TH2D*)h_corr->Clone( ("hTemp_" + triggerSet + "__vs__" + metTriggers.at(i) + "_total").c_str() );
    // * a. set 0s where appropriate
    h_lepFired->SetBinContent(h_lepFired->GetXaxis()->FindBin("Not Fired"), h_lepFired->GetYaxis()->FindBin("Not Fired"), 0);
    h_lepFired->SetBinContent(h_lepFired->GetXaxis()->FindBin("Not Fired"), h_lepFired->GetYaxis()->FindBin("Fired"), 0);
    h_metFired->SetBinContent(h_metFired->GetXaxis()->FindBin("Fired"), h_metFired->GetYaxis()->FindBin("Not Fired"), 0);
    h_metFired->SetBinContent(h_metFired->GetXaxis()->FindBin("Not Fired"), h_metFired->GetYaxis()->FindBin("Not Fired"), 0);
    h_lepXmetFired->SetBinContent(h_lepXmetFired->GetXaxis()->FindBin("Not Fired"), h_lepXmetFired->GetYaxis()->FindBin("Not Fired"), 0);
    h_lepXmetFired->SetBinContent(h_lepXmetFired->GetXaxis()->FindBin("Not Fired"), h_lepXmetFired->GetYaxis()->FindBin("Fired"), 0);
    h_lepXmetFired->SetBinContent(h_lepXmetFired->GetXaxis()->FindBin("Fired"), h_lepXmetFired->GetYaxis()->FindBin("Not Fired"), 0);
    // * b. rebin
    h_lepFired->Rebin2D(2, 2);
    h_metFired->Rebin2D(2, 2);
    h_lepXmetFired->Rebin2D(2, 2);
    h_total->Rebin2D(2, 2);
    // * c. get efficiencies
    tEff_lep = new TEfficiency(*h_lepFired, *h_total);
    tEff_met = new TEfficiency(*h_metFired, *h_total);
    tEff_lepXmet = new TEfficiency(*h_lepXmetFired, *h_total);


    Xeff_lep = tEff_lep->GetEfficiency( tEff_lep->GetTotalHistogram()->GetBin(1,1) );
    Xeff_met = tEff_met->GetEfficiency( tEff_met->GetTotalHistogram()->GetBin(1,1) );
    Xeff_lepXmet = tEff_lepXmet->GetEfficiency( tEff_lepXmet->GetTotalHistogram()->GetBin(1,1) );
    Xeff_lep_err = tEff_lep->GetEfficiencyErrorLow( tEff_lep->GetTotalHistogram()->GetBin(1,1) ) > tEff_lep->GetEfficiencyErrorUp( tEff_lep->GetTotalHistogram()->GetBin(1,1) ) ? tEff_lep->GetEfficiencyErrorLow( tEff_lep->GetTotalHistogram()->GetBin(1,1) ) : tEff_lep->GetEfficiencyErrorUp( tEff_lep->GetTotalHistogram()->GetBin(1,1) );
    Xeff_met_err = tEff_met->GetEfficiencyErrorLow( tEff_met->GetTotalHistogram()->GetBin(1,1) ) > tEff_met->GetEfficiencyErrorUp( tEff_met->GetTotalHistogram()->GetBin(1,1) ) ? tEff_met->GetEfficiencyErrorLow( tEff_met->GetTotalHistogram()->GetBin(1,1) ) : tEff_met->GetEfficiencyErrorUp( tEff_met->GetTotalHistogram()->GetBin(1,1) );
    Xeff_lepXmet_err = tEff_lepXmet->GetEfficiencyErrorLow( tEff_lepXmet->GetTotalHistogram()->GetBin(1,1) ) > tEff_lepXmet->GetEfficiencyErrorUp( tEff_lepXmet->GetTotalHistogram()->GetBin(1,1) ) ? tEff_lepXmet->GetEfficiencyErrorLow( tEff_lepXmet->GetTotalHistogram()->GetBin(1,1) ) : tEff_lepXmet->GetEfficiencyErrorUp( tEff_lepXmet->GetTotalHistogram()->GetBin(1,1) );

    Xcorr = Xeff_lep * Xeff_met / Xeff_lepXmet;
    Xcorr_err = Xcorr * sqrt( (Xeff_lep_err/Xeff_lep)*(Xeff_lep_err/Xeff_lep) + (Xeff_met_err/Xeff_met)*(Xeff_met_err/Xeff_met) + (Xeff_lepXmet_err/Xeff_lepXmet)*(Xeff_lepXmet_err/Xeff_lepXmet) );
    //Xcorr_err = Xcorr * sqrt( (Xeff_lep_err/Xeff_lep)*(Xeff_lep_err/Xeff_lep) );
    //Xcorr_err = Xcorr * sqrt( (Xeff_met_err/Xeff_met)*(Xeff_met_err/Xeff_met) );
    //Xcorr_err = Xcorr * sqrt( (Xeff_lepXmet_err/Xeff_lepXmet)*(Xeff_lepXmet_err/Xeff_lepXmet) );

    // ** Step 5: Output results
    corrTXT << std::fixed;
    corrTXT << "Old Correlation between " << h_corr->GetXaxis()->GetTitle() << " and " << h_corr->GetYaxis()->GetTitle() << " = \t" << std::setprecision(3) << correlation << " +/- " << correlation_err << ",\t GetCorrelationFactor() = " << h_corr->GetCorrelationFactor() << endl;  
    corrTXT << "New Correlation between " << h_corr->GetXaxis()->GetTitle() << " and " << h_corr->GetYaxis()->GetTitle() << " = \t" << std::setprecision(3) << Xcorr << " +/- " << Xcorr_err << endl;
    corrTXT << std::setprecision(5) << "Old calculations,\t eff_lep = " << eff_lep << " +/- " << eff_lep_err << ",\t eff_met = " << eff_met << " +/- " << eff_met_err << ",\t eff_lepXmet = " << eff_lepXmet << " +/- " << eff_lepXmet_err << endl;
    corrTXT << std::setprecision(5) << "New calculations,\t eff_lep = " << Xeff_lep << " +/- " << Xeff_lep_err << ",\t eff_met = " << Xeff_met << " +/- " << Xeff_met_err << ",\t eff_lepXmet = " << Xeff_lepXmet << " +/- " << Xeff_lepXmet_err << endl;
    
  }
  
  corrTXT.close();
}

void drawPeriodComparison(TCanvas* c0, TFile* allPeriod, TFile* periodB, TFile* periodC, TFile* periodD, TFile* periodE, TFile* periodF, TFile* ttbar, string triggerSet, string variable)
{
  //cout << " +++ histo name: " << ("h_" + triggerSet + "_" + variable + "_TEff").c_str() << endl;
  
  TEfficiency* t_ttbar = (TEfficiency*)ttbar->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_all   = (TEfficiency*)allPeriod->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_B     = (TEfficiency*)periodB->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_C     = (TEfficiency*)periodC->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_D     = (TEfficiency*)periodD->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_E     = (TEfficiency*)periodE->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );
  TEfficiency* t_F     = (TEfficiency*)periodF->Get( ("h_" + triggerSet + "_" + variable + "_TEff").c_str() );

  c0->cd();
  // *** 1. Draw ttbar as reference and set ranges appropriately
  t_ttbar->SetMarkerStyle(0);
  t_ttbar->SetLineWidth(2);
  t_ttbar->Draw();
  t_ttbar->Paint("");
  TGraphAsymmErrors* gr = (TGraphAsymmErrors*)t_ttbar->GetPaintedGraph();
  gr->Draw("ep");
  gr->SetMinimum(0);
  gr->SetMaximum(1.1);
  TH1D* temp = (TH1D*)t_ttbar->GetCopyTotalHisto();
  gr->GetXaxis()->SetRangeUser( temp->GetXaxis()->GetXmin(), temp->GetXaxis()->GetXmax() );
  gr->GetYaxis()->SetTitle("Efficiency / Bin");
  gr->Draw("ep");

  // *** 2. Now draw data efficiencies
  t_all->SetLineColor(kRed);
  t_all->SetMarkerStyle(0);
  t_all->SetLineWidth(2);
  t_all->Draw("same");

  t_B->SetLineColor(kBlue);
  t_B->SetLineWidth(2);
  t_B->SetMarkerStyle(0);
  t_B->Draw("same");

  t_C->SetLineColor(kGreen+3);
  t_C->SetLineWidth(2);
  t_C->SetMarkerStyle(0);
  t_C->Draw("same");

  t_D->SetLineColor(kMagenta);
  t_D->SetLineWidth(2);
  t_D->SetMarkerStyle(0);
  t_D->Draw("same");

  t_E->SetLineColor(kOrange);
  t_E->SetLineWidth(2);
  t_E->SetMarkerStyle(0);
  t_E->Draw("same");

  t_F->SetLineColor(kGray);
  t_F->SetLineWidth(2);
  t_F->SetMarkerStyle(0);
  t_F->Draw("same");

  TLegend* leg = new TLegend(0.7, 0.4, .85, .5);
  leg->AddEntry(t_all, "All Data Periods", "el");
  leg->AddEntry(t_B, "Period B", "el");
  leg->AddEntry(t_C, "Period C", "el");
  leg->AddEntry(t_D, "Period D", "el");
  leg->AddEntry(t_E, "Period E", "el");
  leg->AddEntry(t_F, "Period F", "el");
  leg->AddEntry(t_ttbar, "t#bar{t} MC", "el");

  
  TLatex ltx1;
  ltx1.SetTextAlign(9);
  ltx1.SetTextFont(62);
  ltx1.SetTextSize(0.025);
  ltx1.SetNDC();
  ltx1.DrawLatex(0.2, 0.52, triggerSet.c_str());

  leg->Draw("same");
  CMS_lumi( c0, 0, 33);

  c0->SetLeftMargin(0.15);
  c0->SetRightMargin(0.05);
  c0->SetBottomMargin(0.15);

  c0->Print( (topDir + "/h_periodCompare_" + triggerSet + "_" + variable + ".png").c_str() );

}

void makePeriodComparison(TCanvas* c0, TFile* allPeriod, TFile* periodB, TFile* periodC, TFile* periodD, TFile* periodE, TFile* periodF, TFile* ttbar, string triggerSet)
{

  if (triggerSet.find("DoubleEl") != string::npos || triggerSet.find("EMu") != string::npos){
    drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "el0_pt");
    drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "el0_eta");
    drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "el0_relIso");
    if (triggerSet.find("DoubleEl") != string::npos){
      drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "el1_pt");
      drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "el1_eta");
    }
  }
  if (triggerSet.find("DoubleMu") != string::npos || triggerSet.find("EMu") != string::npos){
    drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "mu0_pt");
    drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "mu0_eta");
    drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "mu0_relIso");
    if (triggerSet.find("DoubleMu") != string::npos){
      drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "mu1_pt");
      drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "mu1_eta");
    }
  }
  drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "met");
  drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "njets");
  drawPeriodComparison( c0, allPeriod, periodB, periodC, periodD, periodE, periodF, ttbar, triggerSet, "nPV");
  
}

void produceCombinedEff()
{
  // now sourced from include/trigEffStudy_2017data.h
  date = "11-15-18";
  topDir = (date + "_files/").c_str();
  string recoVersion = "r7";
  string diffType = "max"; // types: lumi, max
  bool dumpSFfile = true;

  // *** 0. Input/output setup
  // ** I. Read files
  //TFile* mc_ttbar     = new TFile( (topDir + "/outfile_ttbarMC_v7_" + recoVersion + "_08-29-18.root").c_str(), "READ");
  //TFile* data_MET = new TFile( (topDir + "/outfile_MET_RunBCDEF_v7_" + recoVersion + ".root").c_str(), "READ");
  TFile* mc_ttbar = new TFile( (topDir + "/outfile_ttbarMC_DL_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TFile* data_MET = new TFile( (topDir + "/outfile_MET_RunBCDEF_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TFile* data_B   = new TFile( (topDir + "/outfile_MET_RunB_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TFile* data_C   = new TFile( (topDir + "/outfile_MET_RunC_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TFile* data_D   = new TFile( (topDir + "/outfile_MET_RunD_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TFile* data_E   = new TFile( (topDir + "/outfile_MET_RunE_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TFile* data_F   = new TFile( (topDir + "/outfile_MET_RunF_postMCsync_v0_" + recoVersion + "_" + date + ".root").c_str(), "READ");
  TObjArray* a_dataFiles = new TObjArray();
  a_dataFiles->AddLast(data_MET);
  a_dataFiles->AddLast(data_B);
  a_dataFiles->AddLast(data_C);
  a_dataFiles->AddLast(data_D);
  a_dataFiles->AddLast(data_E);
  a_dataFiles->AddLast(data_F);

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
  //drawDoubleEfficiency( c1, mc_ttbar, data_MET, "Ele35_WPTight_Gsf__X__PFMET120_PFMHT120_IDTight", "el0_pt");
  //drawDoubleEfficiency( c1, mc_ttbar, data_MET, "IsoMu27__X__PFMET120_PFMHT120_IDTight", "mu0_pt" );
  //drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "SingleEl__X__allMET");
  //drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "SingleMu__X__allMET");
  //drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "DoubleEl__X__allMET");
  //drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "DoubleMu__X__allMET");
  //drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "EMu__X__allMET");
  drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "DoubleEl_OR__X__allMET");
  drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "DoubleMu_OR__X__allMET");
  drawDoubleEfficiencyCollection(c1, mc_ttbar, data_MET, "EMu_OR__X__allMET");


  // *** 3. Compare trigger yields between two samples
  //drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_Ele35_WPTight_Gsf", "el0_pt");
  //drawSingleTriggerTwoSamples( c1, mc_ttbar, data_MET, "HLT_IsoMu27", "mu0_pt" );
  drawSingleTriggerTwoSamplesCollection(c1, mc_ttbar, data_MET, "HLT_DoubleMu_OR");
  drawSingleTriggerTwoSamplesCollection(c1, mc_ttbar, data_MET, "HLT_DoubleEl_OR");
  drawSingleTriggerTwoSamplesCollection(c1, mc_ttbar, data_MET, "HLT_EMu_OR");
  

  // *** 4. Dump some correlation madness
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_EMu_OR");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_DoubleMu_OR");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_DoubleEl_OR");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_DoubleMu_OR");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_DoubleEl_OR");
  dumpCorrelationNumbers(data_MET, "dataBCDEF",  "HLT_EMu_OR");
  /*dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_Ele35_WPTight_Gsf");
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
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_PFMET120_PFMHT120_IDTight");
  dumpCorrelationNumbers(mc_ttbar, "ttbar", "HLT_PFHT250");
  dumpCorrelationNumbers(data_MET, "dataBCDEF", "HLT_Ele35_WPTight_Gsf");  */

  
  // *** 5. Produce nominator + denominator plots
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "DoubleEl_OR__X__allMET", "HLT_allMET_elStreamDL", "met");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "DoubleEl_OR__X__allMET", "HLT_allMET_elStreamDL", "el0_pt");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "EMu_OR__X__allMET", "HLT_allMET_emuStreamDL", "met");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "EMu_OR__X__allMET", "HLT_allMET_emuStreamDL", "el0_pt");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "EMu_OR__X__allMET", "HLT_allMET_emuStreamDL", "njets");
  drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "EMu_OR__X__allMET", "HLT_allMET_emuStreamDL", "mu0_pt");
  //drawNomDenomHistSingleSample(c1, mc_ttbar, "ttbarMC", "SingleEl__X__allMET", "HLT_allMET_elStreamSL", "el0_pt");
  drawNomDenomHistSingleSample(c1, data_MET, "dataBCDEF", "DoubleEl_OR__X__allMET", "HLT_allMET_elStreamDL", "met");
  drawNomDenomHistSingleSample(c1, data_MET, "dataBCDEF", "DoubleEl_OR__X__allMET", "HLT_allMET_elStreamDL", "el0_pt");
  //drawNomDenomHistSingleSample(c1, data_MET, "dataBCDEF", "SingleEl__X__allMET", "HLT_allMET_elStreamSL", "el0_pt");


  // *** 6. Print 1D SF plots
  //print1DScaleFactorHistogramCollection(c1, mc_ttbar, data_MET, "SingleEl__X__allMET");
  //print1DScaleFactorHistogramCollection(c1, mc_ttbar, data_MET, "SingleMu__X__allMET");
  print1DScaleFactorHistogramCollection(c1, mc_ttbar, data_MET, "DoubleEl_OR__X__allMET");
  print1DScaleFactorHistogramCollection(c1, mc_ttbar, data_MET, "DoubleMu_OR__X__allMET");
  print1DScaleFactorHistogramCollection(c1, mc_ttbar, data_MET, "EMu_OR__X__allMET");

  // *** 7. Print 2D SF plots
  //print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "SingleEl__X__allMET", "el0_pt_vs_eta");
  //print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "SingleMu__X__allMET", "mu0_pt_vs_eta");
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleEl_OR__X__allMET", "el0_pt_vs_eta", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleEl_OR__X__allMET", "el1_pt_vs_eta", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleEl_OR__X__allMET", "el0_pt_vs_el1_pt", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleEl_OR__X__allMET", "el0_eta_vs_el1_eta", true);

  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleMu_OR__X__allMET", "mu0_pt_vs_eta", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleMu_OR__X__allMET", "mu1_pt_vs_eta", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleMu_OR__X__allMET", "mu0_pt_vs_mu1_pt", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "DoubleMu_OR__X__allMET", "mu0_eta_vs_mu1_eta", true);

  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "EMu_OR__X__allMET", "el0_pt_vs_eta", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "EMu_OR__X__allMET", "mu0_pt_vs_eta", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "EMu_OR__X__allMET", "mu0_pt_vs_el0_pt", true);
  print2DScaleFactorHistogram(c1, mc_ttbar, data_MET, "EMu_OR__X__allMET", "mu0_eta_vs_el0_eta", true);

  // *** 8. Print plots with period breakdown
  makePeriodComparison(c1, data_MET, data_B, data_C, data_D, data_E, data_F, mc_ttbar, "DoubleEl_OR__X__allMET");
  makePeriodComparison(c1, data_MET, data_B, data_C, data_D, data_E, data_F, mc_ttbar, "DoubleMu_OR__X__allMET");
  makePeriodComparison(c1, data_MET, data_B, data_C, data_D, data_E, data_F, mc_ttbar, "EMu_OR__X__allMET");

  // *** 9. Dump SF file for analyzers 

  if (dumpSFfile) {
    TFile* f_outfileSF = new TFile();
    TObjArray *a_SFfiles = new TObjArray();
    
    // ** A. Make SF files
    TIter next(a_dataFiles); 
    TFile *dataFile = new TFile(); 
    while( (dataFile = (TFile*)next()) ) 
      { 
	std::cout << "Processing SF outfile for " << dataFile->GetName() << std::endl;
	string dataFileName = dataFile->GetName();
	string period = "";
	if ( dataFileName.find("RunBCDEF")!=string::npos )  period = "2017BCDEF";
	else if ( dataFileName.find("RunB")!=string::npos ) period = "2017B";
	else if ( dataFileName.find("RunC")!=string::npos ) period = "2017C";
	else if ( dataFileName.find("RunD")!=string::npos ) period = "2017D";
	else if ( dataFileName.find("RunE")!=string::npos ) period = "2017E";
	else if ( dataFileName.find("RunF")!=string::npos ) period = "2017F";
	else
	  std::cout << "HALP, I DON'T KNOW WHAT RUN PERIOD I'M USING" << std::endl;
	
	f_outfileSF = new TFile( (topDir + "/tth_dileptonic_2DscaleFactors_" + period + "_" + date + ".root").c_str(), "RECREATE");
	f_outfileSF->cd();
	
	TH2D* h_mu0_pt_eta      = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleMu_OR__X__allMET", "mu0_pt_vs_eta");
	TH2D* h_mu1_pt_eta      = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleMu_OR__X__allMET", "mu1_pt_vs_eta");
	TH2D* h_mu0_eta_mu1_eta = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleMu_OR__X__allMET", "mu0_eta_vs_mu1_eta");
	TH2D* h_mu0_pt_mu1_pt   = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleMu_OR__X__allMET", "mu0_pt_vs_mu1_pt");
	TH2D* h_el0_pt_eta      = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleEl_OR__X__allMET", "el0_pt_vs_eta");
	TH2D* h_el1_pt_eta      = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleEl_OR__X__allMET", "el1_pt_vs_eta");
	TH2D* h_el0_eta_el1_eta = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleEl_OR__X__allMET", "el0_eta_vs_el1_eta");
	TH2D* h_el0_pt_el1_pt   = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "DoubleEl_OR__X__allMET", "el0_pt_vs_el1_pt");
	TH2D* h_emu_mu0_pt_eta      = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "EMu_OR__X__allMET", "mu0_pt_vs_eta");
	TH2D* h_emu_el0_pt_eta      = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "EMu_OR__X__allMET", "el0_pt_vs_eta");
	TH2D* h_emu_mu0_eta_el0_eta = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "EMu_OR__X__allMET", "mu0_eta_vs_el0_eta");
	TH2D* h_emu_mu0_pt_el0_pt   = get2DScaleFactorHistogram(c1, mc_ttbar, dataFile, "EMu_OR__X__allMET", "mu0_pt_vs_el0_pt");
	
	f_outfileSF->Write();
	a_SFfiles->AddLast(f_outfileSF);
	//f_outfileSF->Close();
      }
    
    // ** B. Make period dependent comparisons between SF files
    f_outfileSF = new TFile( (topDir + "/tth_dileptonic_2DscaleFactors_2017BCDEF_" + date + "_" + diffType + "Diff_periodDep.root").c_str(), "RECREATE");
    
    TH2D* diff_mu0_pt_mu1_pt   = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleMu_OR__X__allMET", "mu0_pt_vs_mu1_pt", diffType);
    TH2D* diff_mu0_eta_mu1_eta = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleMu_OR__X__allMET", "mu0_eta_vs_mu1_eta", diffType);
    TH2D* diff_mu0_pt_eta      = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleMu_OR__X__allMET", "mu0_pt_vs_eta", diffType);
    TH2D* diff_mu1_pt_eta      = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleMu_OR__X__allMET", "mu1_pt_vs_eta", diffType);
    
    TH2D* diff_el0_pt_el1_pt   = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleEl_OR__X__allMET", "el0_pt_vs_el1_pt", diffType);
    TH2D* diff_el0_eta_el1_eta = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleEl_OR__X__allMET", "el0_eta_vs_el1_eta", diffType);  
    TH2D* diff_el0_pt_eta      = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleEl_OR__X__allMET", "el0_pt_vs_eta", diffType);
    TH2D* diff_el1_pt_eta      = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "DoubleEl_OR__X__allMET", "el1_pt_vs_eta", diffType);
    
    TH2D* diff_EMu_mu0_pt_el0_pt   = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "EMu_OR__X__allMET", "mu0_pt_vs_el0_pt", diffType);
    TH2D* diff_EMu_mu0_eta_el0_eta = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "EMu_OR__X__allMET", "mu0_eta_vs_el0_eta", diffType);
    TH2D* diff_EMu_mu0_pt_eta      = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "EMu_OR__X__allMET", "mu0_pt_vs_eta", diffType);
    TH2D* diff_EMu_el0_pt_eta      = get2DScaleFactorDifferenceHistogram(c1, a_SFfiles, "EMu_OR__X__allMET", "el0_pt_vs_eta", diffType);

    f_outfileSF->Write();
    f_outfileSF->Close();

  } // END dumpSF LOOP
  
}
