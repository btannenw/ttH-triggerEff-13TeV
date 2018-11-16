python submitSampleToCondor.py --outputDir=ttbarMC_DL_postMCsync_v0_$1 --inputTXTfile=TTTo2L2Nu_postMCsync_v0.txt --isMC=true
sleep 10
python submitSampleToCondor.py --outputDir=ttbarMC_SL_postMCsync_v0_$1 --inputTXTfile=TTToSemiLep_postMCsync_v0.txt --isMC=true
sleep 10

python submitSampleToCondor.py --outputDir=MET_RunB_postMCsync_v0_$1 --inputTXTfile=MET_PeriodB_postMCsync_v0.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunC_postMCsync_v0_$1 --inputTXTfile=MET_PeriodC_postMCsync_v0.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunD_postMCsync_v0_$1 --inputTXTfile=MET_PeriodD_postMCsync_v0.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunE_postMCsync_v0_$1 --inputTXTfile=MET_PeriodE_postMCsync_v0.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunF_postMCsync_v0_$1 --inputTXTfile=MET_PeriodF_postMCsync_v0.txt --isMC=false
  

