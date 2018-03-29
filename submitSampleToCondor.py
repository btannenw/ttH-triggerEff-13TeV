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

    if ( not os.path.exists( (args.outputDir + '/condor_logs/') ) ):
        os.system("mkdir {0}".format( (args.outputDir + '/condor_logs/')) )
    if ( not os.path.exists( (args.outputDir + '/condor_err/') ) ):
        os.system("mkdir {0}".format( (args.outputDir + '/condor_err/')) )
    if ( not os.path.exists( (args.outputDir + '/condor_out/') ) ):
        os.system("mkdir {0}".format( (args.outputDir + '/condor_out/')) )

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

# *** 1. Create .tar of directory and store in personal EOS
print "##########     Tarring workdir     ##########"
tarball_name = "{0}.tar.gz".format(args.outputDir)
os.system("tar -cvzf {0} ./ --exclude 'plots*' --exclude '.git' --exclude 'test*' --exclude 'submitOneFile_' --exclude '*.tar.gz'".format(tarball_name))
if ( not os.path.exists("/eos/uscms/store/user/benjtann/{0}/".format(args.outputDir)) ):
    os.system("mkdir /eos/uscms/store/user/benjtann/{0}/".format(args.outputDir))
os.system("xrdcp {0} root://cmseos.fnal.gov//store/user/benjtann/{0}/{1}".format(args.outputDir, tarball_name))
#os.system("rm {0}".format(tarball_name))

# *** 2. Create temporary .pdl file for condor submission
print "\n##########     Submitting Condor jobs     ##########\n"
txtfile = open(args.inputTXTfile, 'r')

for line in txtfile:
    number = (line.rsplit('_',1))[1].split('.root')[0] # get number of file
    infile = line.split('\n')[0]
    jdl_filename = "submitOneFile_{0}_{1}.jdl".format(args.outputDir, number)

    os.system("touch {0}".format(jdl_filename))
    os.system("echo universe = vanilla > {0}".format(jdl_filename))
    os.system("echo Executable = runOneFile.csh >> {0}".format(jdl_filename))
    os.system("echo Should_Transfer_Files = YES >> {0}".format(jdl_filename))
    os.system("echo WhenToTransferOutput = ON_EXIT >> {0}".format(jdl_filename))
    os.system("echo Transfer_Input_Files = runOneFile.csh, {0} >> {1}".format(tarball_name, jdl_filename))
    #os.system("echo notify_user = benjamin.tannenwald@CERN.CH >> {0}".format(jdl_filename))
    #os.system("notify_user = benjtann@FNAL.GOV >> {0}".format(jdl_filename))
    os.system("echo Output = {0}/condor_out/outfile_{1}.out  >> {2}".format(args.outputDir, number, jdl_filename))
    os.system("echo Error = {0}/condor_err/outfile_{1}.err >> {2}".format(args.outputDir, number, jdl_filename))
    os.system("echo Log = {0}/condor_logs/outfile_{1}.log >> {2}".format(args.outputDir, number, jdl_filename))
    os.system("echo x509userproxy = ${{X509_USER_PROXY}} >> {0}".format(jdl_filename))
    os.system("echo Arguments = {0} {1} {2} {3} >> {4}".format(args.outputDir, args.isMC, infile, tarball_name, jdl_filename))
    os.system("echo Queue 1 >> {0}".format(jdl_filename))   

    
    os.system("condor_submit {0}".format(jdl_filename))

