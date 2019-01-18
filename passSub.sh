python submitSampleToCondor.py --outputDir=ttbarMC_DL_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/TTTo2L2Nu_triggerSF_newJECJER_v0.txt --isMC=true --trigSF
sleep 10
python submitSampleToCondor.py --outputDir=ttbarMC_SL_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/TTToSemiLep_triggerSF_newJECJER_v0.txt --isMC=true --trigSF
sleep 10

python submitSampleToCondor.py --outputDir=MET_RunB_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/MET_PeriodB_triggerSF_newJECJER_v0.txt --isMC=false --trigSF
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunC_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/MET_PeriodC_triggerSF_newJECJER_v0.txt --isMC=false --trigSF
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunD_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/MET_PeriodD_triggerSF_newJECJER_v0.txt --isMC=false --trigSF
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunE_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/MET_PeriodE_triggerSF_newJECJER_v0.txt --isMC=false --trigSF
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunF_triggerSF_newJECJER_v0_$1 --inputTXTfile=./fileLists/triggerSF_newJECJER_v0/MET_PeriodF_triggerSF_newJECJER_v0.txt --isMC=false --trigSF
  

