rm *postMCsync_v0*.txt -v

ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MC/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/crab_ICHEP18_postMCsync_v0_TTto2L2Nu/*/*/*.root > TTTo2L2Nu_postMCsync_v0.txt
ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MC/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/crab_ICHEP18_postMCsync_v0_TTtoSemiLep/*/*/y*.root > TTToSemiLep_postMCsync_v0.txt

ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MET/MET/crab_ICHEP18_postMCsync_v0_MET_PeriodB*/*/*/y*.root > MET_PeriodB_postMCsync_v0.txt
ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MET/MET/crab_ICHEP18_postMCsync_v0_MET_PeriodC*/*/*/y*.root > MET_PeriodC_postMCsync_v0.txt
ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MET/MET/crab_ICHEP18_postMCsync_v0_MET_PeriodD*/*/*/y*.root > MET_PeriodD_postMCsync_v0.txt
ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MET/MET/crab_ICHEP18_postMCsync_v0_MET_PeriodE*/*/*/y*.root > MET_PeriodE_postMCsync_v0.txt
ls /eos/uscms/store/group/lpctthrun2/UVA/ICHEP2018/MET/MET/crab_ICHEP18_postMCsync_v0_MET_PeriodF*/*/*/y*.root > MET_PeriodF_postMCsync_v0.txt

sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' TTTo2L2Nu_postMCsync_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' TTToSemiLep_postMCsync_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodB_postMCsync_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodC_postMCsync_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodD_postMCsync_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodE_postMCsync_v0.txt
sed -i 's/\/eos\/uscms/root:\/\/cmseos.fnal.gov\//g' MET_PeriodF_postMCsync_v0.txt
