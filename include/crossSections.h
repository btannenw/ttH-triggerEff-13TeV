// Author:  Ben Tannenwald
// Date:    Nov 16, 2018
// Purpose: Header for process cross-sections

#include <TString.h>

// *** 0. Global variables per sample
double xSec = 999999;
Color_t c_plotColor = 0;
string sampleName = "PENELOPE";

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
double xSec_WZ = (0.106 + 0.0663) * 3**2 / BR_Wlep / BR_Zclep;
double xSec_ZZ = 0.0719 * (3 / BR_Zclep)**2 / 2;
double xSec_DYJetsToLL_M_10to50 = 18610;
double xSec_DYJetsToLL_M_50 = 3*1921.8;
double xSec_TTWJetsToLNu = 0.2043;
double xSec_TTWJetsToQQ = 0.4062;
double xSec_TTZToLLNuNu_M_10 = 0.2529;
double xSec_TTZToQQ = 0.5297;

// *** 3. Colors
Color_t c_ttH        = ;
Color_t c_ttbar_SL   = ;
Color_t c_ttbar_DL   = ;
Color_t c_ttbar_had  = ;
Color_t c_singleTop  = ;
Color_t c_Wjets      = ;
Color_t c_Zjets      = ;
Color_t c_WW         = ;
Color_t c_WZ         = ;
Color_t c_ZZ         = ;
Color_t c_ttW        = ;
Color_t c_ttZ        = ;

void setSampleVariables(string filename)
{
  if( sample.find("ttHTobb") != string::npos){
    xSec = xSec_ttHTobb;
    sample = "ttHTobb";
    c_plotColor = c_;
  }
  else if( sample.find("TTToSemiLeptonic") != string::npos ) {
    xSec = xSec_TTToSemiLeptonic;
    sample = "TTToSemiLeptonic";
    c_plotColor = c_;
  }
  else if( sample.find("TTTo2L2Nu") != string::npos ) {
    xSec = xSec_TTTo2L2Nu;
    sample = "TTTo2L2Nu";
    c_plotColor = c_;
  }
  else if( sample.find("TTToHadronic") != string::npos ) {
    xSec = xSec_TTToHadronic;
    sample = "TTToHadronic";
    c_plotColor = c_;
  }
  else if( sample.find("ST_s_channel_4f_leptonDecays") != string::npos ) {
    xSec = xSec_ST_s_channel_4f_leptonDecays;
    sample = "ST_s_channel_4f_leptonDecays";
    c_plotColor = c_;
  }
  else if( sample.find("ST_t_channel_top_4f_inclusiveDecays") != string::npos ) {     
    xSec = xSec_ST_t_channel_top_4f_inclusiveDecays;
    sample = "ST_t_channel_top_4f_inclusiveDecays";
    c_plotColor = c_;
  }
  else if( sample.find("ST_t_channel_antitop_4f_inclusiveDecays") != string::npos ) {
    xSec = xSec_ST_t_channel_antitop_4f_inclusiveDecays;
    sample = "ST_t_channel_antitop_4f_inclusiveDecays";
    c_plotColor = c_;
  }
  else if( sample.find("ST_tW_top_5f_inclusiveDecays") != string::npos ) {
    xSec = xSec_ST_tW_top_5f_inclusiveDecays;
    sample = "ST_tW_top_5f_inclusiveDecays";
    c_plotColor = c_;
  }
  else if( sample.find("ST_tW_antitop_5f_inclusiveDecays") != string::npos ) {
    xSec = xSec_ST_tW_antitop_5f_inclusiveDecays;
    sample = "ST_tW_antitop_5f_inclusiveDecays";
    c_plotColor = c_;
  }
  else if( sample.find("WJetsToLNu") != string::npos ) {
    xSec = xSec_WJetsToLNu;
    sample = "WJetsToLNu";
    c_plotColor = c_;
  }
  else if( sample.find("WW") != string::npos ) {
    xSec = xSec_WW;
    sample = "WW";
    c_plotColor = c_;
  }
  else if( sample.find("WZ") != string::npos ) {
    xSec = xSec_WZ;
    sample = "WZ";
    c_plotColor = c_;
  }
  else if( sample.find("ZZ") != string::npos ) {
    xSec = xSec_ZZ;
    sample = "ZZ";
    c_plotColor = c_;
  }
  else if( sample.find("DYJetsToLL_M_10to50") != string::npos ) {
    xSec = xSec_DYJetsToLL_M_10to50;
    sample = "DYJetsToLL_M_10to50";
    c_plotColor = c_;
  }
  else if( sample.find("DYJetsToLL_M_50") != string::npos ) {
    xSec = xSec_DYJetsToLL_M_50;
    sample = "DYJetsToLL_M_50";
    c_plotColor = c_;
  }
  else if( sample.find("TTWJetsToLNu") != string::npos ) {
    xSec = xSec_TTWJetsToLNu;
    sample = "TTWJetsToLNu";
    c_plotColor = c_;
  }
  else if( sample.find("TTWJetsToQQ") != string::npos ) {
    xSec = xSec_TTWJetsToQQ;
    sample = "TTWJetsToQQ";
    c_plotColor = c_;
  }
  else if( sample.find("TTZToLLNuNu_M_10") != string::npos ) {
    xSec = xSec_TTZToLLNuNu_M_10;
    sample = "TTZToLLNuNu_M_10";
    c_plotColor = c_;
  }
  else if( sample.find("TTZToQQ") != string::npos ) {
    xSec = xSec_TTZToQQ;
    c_plotColor = c_;
    sample = "TTZToQQ";
  }

}
