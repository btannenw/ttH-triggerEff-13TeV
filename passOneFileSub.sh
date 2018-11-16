python submitSampleToCondor.py --outputDir=ttbarMC_v7_oneFile_$1 --inputTXTfile=ttbarMC_v7_oneFile.txt --isMC=true
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunB_v7_oneFile_$1 --inputTXTfile=MET_Run2017B_v7_oneFile.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunC_v7_oneFile_$1 --inputTXTfile=MET_Run2017C_v7_oneFile.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunD_v7_oneFile_$1 --inputTXTfile=MET_Run2017D_v7_oneFile.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunE_v7_oneFile_$1 --inputTXTfile=MET_Run2017E_v7_oneFile.txt --isMC=false
sleep 10
python submitSampleToCondor.py --outputDir=MET_RunF_v7_oneFile_$1 --inputTXTfile=MET_Run2017F_v7_oneFile.txt --isMC=false
  

