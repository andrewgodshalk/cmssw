#!/bin/bash

# Set up env and unpack all files
export SCRAM_ARCH="slc6_amd64_gcc530"
cd ${_CONDOR_SCRATCH_DIR}
pwd
cd /uscms_data/d2/godshalk/root6SetupDir/CMSSW_8_0_19/src
eval `scramv1 runtime -sh`
cd -

tar -zxf merge_filelists.tgz
python hadd_script.py $1

rm merge_filelists.tgz
rm -r merge_filelists/
