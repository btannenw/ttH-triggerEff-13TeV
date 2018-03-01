#!/usr/bin/env python

# Author:  Ben Tannenwald
# Date:    Mar 1, 2017
# Purpose: Python script to auto-generate text for histogram declaration and filling

analysisTrig = 'HLT_IsoMu27'

# print histogram declaration
metTrigFile = open('../prescaleInfo/unprescaled2017METtriggers_periodE.txt', 'r')    
for trig in metTrigFile:
    metTrig = trig.split('_v')[0]
    print  'TH2D* h_{0}__vs__{1} = new TH2D("h_{0}__vs__{1}", "h_{0}__vs__{1}", 2, 0, 2, 2, 0, 2);'.format(analysisTrig, metTrig)

# print histogram filling
metTrigFile = open('../prescaleInfo/unprescaled2017METtriggers_periodE.txt', 'r')    
for trig in metTrigFile:
    metTrig = trig.split('_v')[0]
    print 'h_{0}__vs__{1}->Fill(eve->pass{0}_v_, eve->pass{1}_v_);'.format(analysisTrig, metTrig)
    #plot2Dcorr( c0, h_passHLT_IsoMu24_v__vs__passHLT_IsoMu27_v, "HLT_IsoMu24", "HLT_IsoMu27" );

# print histogram printing
metTrigFile = open('../prescaleInfo/unprescaled2017METtriggers_periodE.txt', 'r')    
for trig in metTrigFile:
    metTrig = trig.split('_v')[0]
    print 'plot2Dcorr( c0, h_{0}__vs__{1}, "{0}", "{1}" );'.format(analysisTrig, metTrig)
