#include "../include/sampleHandler.h"

sampleHandler::sampleHandler()
{
  xSec = 99999;
  c_plotColor = 0;
  sampleName = "PENELOPE";
  nEvents = 1;
  sampleLumi = 1;
  genEff = 1;
  lumiWeight = 1;
}

sampleHandler::~sampleHandler() {}

void sampleHandler::test() {  std::cout << "Test!" << endl; }

void sampleHandler::calculateLumiScale()
{
  // *** 0. Equation: L = N / (sigma*eff)
  
  // *** 1. Luminosity of data --> what we're scaling the MC to
  const double totalIntLumi_2017 = 41.35 * 1e3; // fb * fb->pb factor
  
  // *** 2. Calculate effective luminosity of sample
  sampleLumi = nEvents / (xSec * genEff) ;

  // *** 3. Calculate weight to apply to each event to properly scale effective luminosity
  lumiScale = totalIntLumi_2017 / sampleLumi;

  return;
}

void sampleHandler::SetSample(string sample)
{

void setSampleVariables(string filename, double xSec, Color_t c_plotColor, string sampleName)
{
  // *** 1. Branching ratios
  const double BR_Hbb   = 0.5824;
  const double BR_Whad  = 0.6741; 
  const double BR_Wlep  = 1 - BR_Whad; 
  const double BR_Zclep = 3 * 0.033658; 

  // *** 2. Cross-sections
  // !!!!!!!!!!!!!!!    ALL IN [pb]    !!!!!!!!!!!!!!!
  const double xSec_ttHTobb          = 0.5071 * BR_Hbb;
  const double xSec_TTToSemiLeptonic = 831.76 * 2 * BR_Whad * BR_Wlep;
  const double xSec_TTTo2L2Nu        = 831.76 * BR_Wlep * BR_Wlep;
  const double xSec_TTToHadronic     = 831.76 * BR_Whad * BR_Whad;
  const double xSec_ST_s_channel_4f_leptonDecays            = 11.36 * BR_Wlep;
  const double xSec_ST_t_channel_top_4f_inclusiveDecays     = 136.02;
  const double xSec_ST_t_channel_antitop_4f_inclusiveDecays = 80.95;
  const double xSec_ST_tW_top_5f_inclusiveDecays = 35.85;
  const double xSec_ST_tW_antitop_5f_inclusiveDecays = 35.85;
  const double xSec_WJetsToLNu = 61526.7;
  const double xSec_WW = 118.7;
  const double xSec_WZ = (0.106 + 0.0663) * 3*3 / BR_Wlep / BR_Zclep;
  const double xSec_ZZ = 0.0719 * (3 / BR_Zclep)*(3 / BR_Zclep) / 2;
  const double xSec_DYJetsToLL_M_10to50 = 18610;
  const double xSec_DYJetsToLL_M_50 = 3*1921.8;
  const double xSec_TTWJetsToLNu = 0.2043;
  const double xSec_TTWJetsToQQ = 0.4062;
  const double xSec_TTZToLLNuNu_M_10 = 0.2529;
  const double xSec_TTZToQQ = 0.5297;
  
  // *** 3. Colors
  const Color_t c_ttH        = kBlue;
  const Color_t c_ttbar_SL   = kRed+3;
  const Color_t c_ttbar_DL   = kRed-7;
  const Color_t c_ttbar_had  = kRed-2;
  const Color_t c_singleTop  = kMagenta;
  const Color_t c_Vjets      = kGreen+1;
  const Color_t c_diboson    = kBlue;
  const Color_t c_ttV        = kBlue-10;
  

  // *** 2. Sample information
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


  calculateLumiScale();
}



