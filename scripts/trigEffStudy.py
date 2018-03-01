#!/usr/bin/env python

# Author:  Ben Tannenwald
# Date:    Feb 27, 2017
# Purpose: Make python script to make trigger correlation/efficiency plots that should really be done in a .C macro

import ROOT

f = ROOT.TFile('yggdrasil_treeMaker_test2kEvents_add2017TrigBranches.root', 'READ')
t=f.Get('ttHTreeMaker/worldTree')

print "worldTree has {0} entries".format( t.GetEntries() )

#t.Print()

#for event in t.eve.:
#    print 'Event {0}: passHLT_IsoMu24- {1}'.format(event.evt_, event.passHLT_IsoMu24_v_)

for event in f.

    
