# Author:  Ben Tannenwald
# Date:    Feb 27, 2017
# Purpose: Lazy check of working triggers and if they exist in online good run list

yFile = open('pass_noPass_triggers.txt','r')
#grFile = open('online_grun__cmssw940patch1.txt','r')

yTriggers = []

for line in yFile:
    if '=' not in line and line !='\n':
        #print line.strip('\n')
        yTriggers.append(line.strip('\n'))

for hlt in yTriggers:
    found = False
    grFile = open('online_grun__cmssw940patch1.txt','r')

    for line in grFile:
        if hlt in line:
            print hlt, "FOUND"
#            print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
            found = True
            break
    if found == False:
        print hlt, "FAIL"

print '\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n'

grFile = open('online_grun__cmssw940patch1.txt','r')
for trig in grFile:
    hlt = trig.strip('\n')
    if 'MET' in hlt and 'Mu' not in hlt and 'Tau' not in hlt and 'Ele' not in hlt and 'Photon' not in hlt and 'Not' not in hlt:
        print hlt.split('*')[0]
    # Check for subsets
    test = 'Pho' # or Photon or Ele or Mu or Tau
    #if test in hlt and 'MET' in hlt:
    #    print hlt
    
