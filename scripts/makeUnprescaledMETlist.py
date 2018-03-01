# Author:  Ben Tannenwald
# Date:    Feb 27, 2017
# Purpose: Lazy check of working triggers and if they exist in online good run list

unprescaled = open('unprescaled2017METtiggers_prescaleInfo_periodE.txt','r')
allTriggers = open('all2017METtriggers_prescaleInfo_periodE.txt','r')

uniqueTriggers = []

for line in allTriggers:
    #print line
    if '|' in line:
        hlt = line.split('|')[5].split('/')[0]
        totPrescale = line.split('|')[4].split(' ')[1]

        if 'HLT' in hlt and int(totPrescale) == 1:
            #print hlt, totPrescale
            if hlt not in uniqueTriggers and 'Mu' not in hlt and 'Ele' not in hlt and 'Photon' not in hlt and 'Tau' not in hlt:
                uniqueTriggers.append(hlt)

print len(uniqueTriggers)
for trigger in uniqueTriggers:
    print trigger

    
