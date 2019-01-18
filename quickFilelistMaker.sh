rm *triggerSF_newJECJER_v0*.txt -v

ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MC/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/crab_triggerSF_newJECJER_v0_TTTo2L2Nu/*/*/y*.root > TTTo2L2Nu_triggerSF_newJECJER_v0.txt
ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MC/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_triggerSF_newJECJER_v0_TTToSemiLeptonic/*/*/y*.root > TTToSemiLep_triggerSF_newJECJER_v0.txt

ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MET/MET/crab_triggerSF_newJECJER_v0_PeriodB*/*/*/y*.root > MET_PeriodB_triggerSF_newJECJER_v0.txt
ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MET/MET/crab_triggerSF_newJECJER_v0_PeriodC*/*/*/y*.root > MET_PeriodC_triggerSF_newJECJER_v0.txt
ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MET/MET/crab_triggerSF_newJECJER_v0_PeriodD*/*/*/y*.root > MET_PeriodD_triggerSF_newJECJER_v0.txt
ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MET/MET/crab_triggerSF_newJECJER_v0_PeriodE*/*/*/y*.root > MET_PeriodE_triggerSF_newJECJER_v0.txt
ls /eos/uscms/store/group/lpctthrun2/analysis2017Data/triggerSF/MET/MET/crab_triggerSF_newJECJER_v0_PeriodF*/*/*/y*.root > MET_PeriodF_triggerSF_newJECJER_v0.txt

sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' TTTo2L2Nu_triggerSF_newJECJER_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' TTToSemiLep_triggerSF_newJECJER_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodB_triggerSF_newJECJER_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodC_triggerSF_newJECJER_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodD_triggerSF_newJECJER_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodE_triggerSF_newJECJER_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodF_triggerSF_newJECJER_v0.txt
