# ttH-triggerEff-13TeV
ttH studies using 2017 data

## Commands
condor_q = check status of condor queue
python submitSampleToCondor.py [-h] [--outputDir OUTPUTDIR] [--inputTXTfile INPUTTXTFILE] [--isMC ISMC]

#General Notes

tar -cvzf testTar.tar.gz ./ --exclude "testMC" --exclude ".git"

=======  To remove all condor jobs with 'Held' status  =========
condor_rm -const 'jobstatus==5'

=======  to see what files tracked by git  ========
git ls-tree -r master --name-only

======   to make repo push-able from FNAL  ======
check 

>$ git remote -v

and if not using git username

>$ git remote set-url origin https://btannenw@github.com/<REPO>

or

>$ git remote set-url origin git@github.com:btannenw/<REPO>.git