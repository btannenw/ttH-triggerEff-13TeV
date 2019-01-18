source /cvmfs/sft.cern.ch/lcg/views/LCG_89/x86_64-slc6-gcc62-opt/setup.sh
cd ${_CONDOR_SCRATCH_DIR}
xrdcp -s root://cmseos.fnal.gov//store/user/benjtann/condor_tarballs/$5 .
tar -xf $5
rm $5
root -l -q -b 'trigEffStudy_2017data.C("'${1}'","'${2}'","'${3}'","'${4}'")'
### Now that the run is over, there is one or more root files created
echo "List all root files = "
ls $1/*/*.root
echo "List all files"
ls 
echo "*******************************************"
OUTDIR=root://cmseos.fnal.gov//store/user/benjtann/$1/
echo "xrdcp output for condor"
for FILE in $1/*/*.root
do
  echo "xrdcp -f ${FILE} ${OUTDIR}/${FILE}"
  xrdcp -f ${FILE} ${OUTDIR}/${FILE} 2>&1
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    rm *.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi
  rm ${FILE}
done
