python submitSampleToCondor.py --outputDir=ttbarMC_v7_$1 --inputTXTfile=ttbarMC_v7.txt --isMC=true
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunB_v7_$1 --inputTXTfile=MET_Run2017B_v7.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunC_v7_$1 --inputTXTfile=MET_Run2017C_v7.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunD_v7_$1 --inputTXTfile=MET_Run2017D_v7.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunE_v7_$1 --inputTXTfile=MET_Run2017E_v7.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunF_v7_$1 --inputTXTfile=MET_Run2017F_v7.txt --isMC=false
  

