# /usr/bin/python

#Author: Ben Tannenwald
#Date: March 28, 2018
#Purpose: Script to submit condor jobs for all files in data/mc sample

import os,sys, argparse

# *** 0. setup parser for command line
parser = argparse.ArgumentParser()
parser.add_argument("--outputDir", help="output directory for processed histograms and roofiles")
parser.add_argument("--inputTXTfile", help=".txt file containing list of input files for a given sample")
parser.add_argument("--isMC", help="flag whether sample is Monte Carlo (true) or data (false)")
args = parser.parse_args()

if (len(vars(args)) != 3): # 3 --> three for options
    os.system('python submitSampleToCondor.py -h')
    quit()

# ** A. Test output directory existence and create if DNE
if(args.outputDir is None):
    print "#### Need to specify output directory using --outputDir <desired output directory> ####\nEXITING"
    quit()
else:
    if ( not os.path.exists(args.outputDir) ):
        print "Specified input file ({0}) DNE.\nCREATING NOW".format(args.inputTXTfile)
        os.system("mkdir {0}".format(args.outputDir))
    print '-- Setting outputDir = {0}'.format(args.outputDir)

# ** B. Test input .txt file and exit if DNE
if(args.inputTXTfile is None):
    print "#### Need to specify input .txt file using --inputTXTfile <address to .txt file> ####\nEXITING"
    quit()
else:
    if ( not os.path.exists(args.inputTXTfile) ):
        print "#### Specified input file ({0}) DNE ####.\nEXITING".format(args.inputTXTfile)
        quit()
    else:
        print '-- Setting inputTXTfile = {0}'.format(args.inputTXTfile)

# ** C. Test isMC flag and exit if not sensible
if(args.isMC is None):
    print "#### Need to set isMC flag using --isMC <true/false> ####\nEXITING"
    quit()
else:
    if( not(args.isMC == "true" or args.isMC == "True" or args.isMC == "false" or args.isMC == "False") ):
        print "#### Please use true/True/false/False when setting --isMC <option>. Supplied value ({0}) does not match ####\nEXITING".format(args.isMC)
    else:
        print '-- Setting isMC = {0}'.format(args.isMC)


print "##########     Submitting Condor jobs     ##########\n"
# *** 1. Create temporary .pdl file for condor submission
txtfile = open(args.inputTXTfile, 'r')

for infile in txtfile:
    number = (infile.rsplit('_',1))[1].split('.root')[0] # get number of file

    jdl_filename = "submitOneFile_{0}_{1}.jdl".format(args.outputDir, number)
    os.system("touch {0}".format(jdl_filename))
    os.system("echo universe = vanilla > {0}".format(jdl_filename))
    os.system("echo Executable = runOneFile.sh {0} {1} {2} >> {3}".format(args.outputDir, args.isMC, infile, jdl_filename))
    os.system("echo Output = outfile_{0}.out >> {1}".format(number, jdl_filename))
    os.system("echo Error = outfile_{0}.err >> {1}".format(number, jdl_filename))
    os.system("echo Log = outfile_{0}.log >> {0}".format(number, jdl_filename))
    os.system("echo should_transfer_files = YES >> {0}".format(jdl_filename))
    os.system("echo when_to_transfer_output = ON_EXIT >> {0}".format(jdl_filename))
    os.system("echo transfer_input_files FIXME >> {0}".format(jdl_filename))

