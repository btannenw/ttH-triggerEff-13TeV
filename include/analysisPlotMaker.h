// Author:  Ben Tannenwald
// Date:    Nov 19, 2018
// Purpose: Header for analysis plotting code

#include <string>
#include <TH2.h>
#include <TFile.h>
#include <TObjArray.h>
#include "YggdrasilEventVars.h"
#include "leptonHandler.h"
#include "jetMetHandler.h"


// top level vars
std::string topDir, fileList, date, sampleTag;
bool printPlots, dumpFile, verbose, isMC, singleFile, isDebug;

// user tools
yggdrasilEventVars *eve;
leptonHandler lepTool;
jetMetHandler jetMetTool;

// user file
TFile* outfile = new TFile();

// user vars
//TObjArray* a_ttbar     = new TObjArray();
//TObjArray* a_ttlf    = new TObjArray();
//TObjArray* a_ttcc    = new TObjArray();
//TObjArray* a_ttbb    = new TObjArray();
//TObjArray* a_tt2b    = new TObjArray();
//TObjArray* a_ttb     = new TObjArray();
//TObjArray* a_vjets     = new TObjArray();
//TObjArray* a_diboson   = new TObjArray();
//TObjArray* a_ttv       = new TObjArray();
//TObjArray* a_singletop = new TObjArray();
TObjArray* a_sample = new TObjArray();
TObjArray* a_Efficiencies = new TObjArray();

std::vector <std::string> metTriggers = {};
