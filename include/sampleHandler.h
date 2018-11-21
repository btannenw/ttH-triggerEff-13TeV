// Author:  Ben Tannenwald
// Date:    Nov 16, 2018
// Purpose: Header for process cross-sections

#include <TString.h>

// *** 0. Global variables per sample
double xSec = 999999;
Color_t c_plotColor = 0;
string sampleName = "PENELOPE";
double nEvents = 1;

// *** 1. Branching ratios
double BR_Hbb   = 0.5824;
double BR_Whad  = 0.6741; 
double BR_Wlep  = 1 - BR_Whad; 
double BR_Zclep = 3 * 0.033658; 

// *** 2. Cross-sections
// !!!!!!!!!!!!!!!    ALL IN [pb]    !!!!!!!!!!!!!!!
double xSec_ttHTobb          = 0.5071 * BR_Hbb;
double xSec_TTToSemiLeptonic = 831.76 * 2 * BR_Whad * BR_Wlep;
double xSec_TTTo2L2Nu        = 831.76 * BR_Wlep * BR_Wlep;
double xSec_TTToHadronic     = 831.76 * BR_Whad * BR_Whad;
double xSec_ST_s_channel_4f_leptonDecays            = 11.36 * BR_Wlep;
double xSec_ST_t_channel_top_4f_inclusiveDecays     = 136.02;
double xSec_ST_t_channel_antitop_4f_inclusiveDecays = 80.95;
double xSec_ST_tW_top_5f_inclusiveDecays = 35.85;
double xSec_ST_tW_antitop_5f_inclusiveDecays = 35.85;
double xSec_WJetsToLNu = 61526.7;
double xSec_WW = 118.7;
double xSec_WZ = (0.106 + 0.0663) * 3*3 / BR_Wlep / BR_Zclep;
double xSec_ZZ = 0.0719 * (3 / BR_Zclep)*(3 / BR_Zclep) / 2;
double xSec_DYJetsToLL_M_10to50 = 18610;
double xSec_DYJetsToLL_M_50 = 3*1921.8;
double xSec_TTWJetsToLNu = 0.2043;
double xSec_TTWJetsToQQ = 0.4062;
double xSec_TTZToLLNuNu_M_10 = 0.2529;
double xSec_TTZToQQ = 0.5297;

// *** 3. Colors
Color_t c_ttH        = kBlue;
Color_t c_ttbar_SL   = kRed+3;
Color_t c_ttbar_DL   = kRed-7;
Color_t c_ttbar_had  = kRed-2;
Color_t c_singleTop  = kMagenta;
Color_t c_Vjets      = kGreen+1;
Color_t c_diboson    = kBlue;
Color_t c_ttV        = kBlue-10;

void setSampleVariables(string filename, double xSec, Color_t c_plotColor, string sampleName)
{
  if( filename.find("ttHTobb") != string::npos){
    xSec = xSec_ttHTobb;
    sampleName = "ttHTobb";
    c_plotColor = c_ttH;
    nEvents = 8000000;
  }
  else if( filename.find("TTToSemiLeptonic") != string::npos ) {
    xSec = xSec_TTToSemiLeptonic;
    sampleName = "TTToSemiLeptonic";
    c_plotColor = c_ttbar_SL;
    nEvents = ;
  }
  else if( filename.find("TTTo2L2Nu") != string::npos ) {
    xSec = xSec_TTTo2L2Nu;
    sampleName = "TTTo2L2Nu";
    c_plotColor = c_ttbar_DL;
    nEvents = ;
  }
  else if( filename.find("TTToHadronic") != string::npos ) {
    xSec = xSec_TTToHadronic;
    sampleName = "TTToHadronic";
    c_plotColor = c_ttbar_had;
    nEvents = ;
  }
  else if( filename.find("ST_s_channel_4f_leptonDecays") != string::npos ) {
    xSec = xSec_ST_s_channel_4f_leptonDecays;
    sampleName = "ST_s_channel_4f_leptonDecays";
    c_plotColor = c_singleTop;
    nEvents = ;
  }
  else if( filename.find("ST_t_channel_top_4f_inclusiveDecays") != string::npos ) {     
    xSec = xSec_ST_t_channel_top_4f_inclusiveDecays;
    sampleName = "ST_t_channel_top_4f_inclusiveDecays";
    c_plotColor = c_singleTop;
    nEvents = ;
  }
  else if( filename.find("ST_t_channel_antitop_4f_inclusiveDecays") != string::npos ) {
    xSec = xSec_ST_t_channel_antitop_4f_inclusiveDecays;
    sampleName = "ST_t_channel_antitop_4f_inclusiveDecays";
    c_plotColor = c_singleTop;
    nEvents = ;
  }
  else if( filename.find("ST_tW_top_5f_inclusiveDecays") != string::npos ) {
    xSec = xSec_ST_tW_top_5f_inclusiveDecays;
    sampleName = "ST_tW_top_5f_inclusiveDecays";
    c_plotColor = c_singleTop;
    nEvents = ;
  }
  else if( filename.find("ST_tW_antitop_5f_inclusiveDecays") != string::npos ) {
    xSec = xSec_ST_tW_antitop_5f_inclusiveDecays;
    sampleName = "ST_tW_antitop_5f_inclusiveDecays";
    c_plotColor = c_singleTop;
    nEvents = ;
  }
  else if( filename.find("WJetsToLNu") != string::npos ) {
    xSec = xSec_WJetsToLNu;
    sampleName = "WJetsToLNu";
    c_plotColor = c_Vjets;
    nEvents = ;
  }
  else if( filename.find("WW") != string::npos ) {
    xSec = xSec_WW;
    sampleName = "WW";
    c_plotColor = c_diboson;
    nEvents = ;
  }
  else if( filename.find("WZ") != string::npos ) {
    xSec = xSec_WZ;
    sampleName = "WZ";
    c_plotColor = c_diboson;
    nEvents = ;
  }
  else if( filename.find("ZZ") != string::npos ) {
    xSec = xSec_ZZ;
    sampleName = "ZZ";
    c_plotColor = c_diboson;
    nEvents = ;
  }
  else if( filename.find("DYJetsToLL_M_10to50") != string::npos ) {
    xSec = xSec_DYJetsToLL_M_10to50;
    sampleName = "DYJetsToLL_M_10to50";
    c_plotColor = c_Vjets;
    nEvents = ;
  }
  else if( filename.find("DYJetsToLL_M_50") != string::npos ) {
    xSec = xSec_DYJetsToLL_M_50;
    sampleName = "DYJetsToLL_M_50";
    c_plotColor = c_Vjets;
    nEvents = ;
  }
  else if( filename.find("TTWJetsToLNu") != string::npos ) {
    xSec = xSec_TTWJetsToLNu;
    sampleName = "TTWJetsToLNu";
    c_plotColor = c_ttV;
    nEvents = ;
  }
  else if( filename.find("TTWJetsToQQ") != string::npos ) {
    xSec = xSec_TTWJetsToQQ;
    sampleName = "TTWJetsToQQ";
    c_plotColor = c_ttV;
    nEvents = ;
  }
  else if( filename.find("TTZToLLNuNu_M_10") != string::npos ) {
    xSec = xSec_TTZToLLNuNu_M_10;
    sampleName = "TTZToLLNuNu_M_10";
    c_plotColor = c_ttV;
    nEvents = ;
  }
  else if( filename.find("TTZToQQ") != string::npos ) {
    xSec = xSec_TTZToQQ;
    sampleName = "TTZToQQ";
    c_plotColor = c_ttV;
    nEvents = ;
  }
  else { 
    xSec = 999999;
    c_plotColor = 0;
    sampleName = "PENELOPE";
    nEvents = ;
  }
}
