Directions taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/VHiggsBB#Ntuple_production_campaigns_AN1
To set up the environment on LPC... 

# Source LPC Scripts
source /cvmfs/cms.cern.ch/cmsset_default.csh

# Set up CMSSW release (preferably in some scratch space)
mkdir NtuplerWork
cd NtuplerWork

# [2016]: Use CMSSW_8_0_19
# [2015]:
cmsrel CMSSW_7_6_3_patch2
cd CMSSW_7_6_3_patch2/src/
cmsenv

#Check out appropriate tag, then compile
git cms-merge-topic andrewgodshalk:ZC2015
# check the upstream for updates (optional)
# git remote add perrozzi https://github.com/perrozzi/cmssw.git
# git fetch perrozzi
# git merge perrozzi/V21bis
scram b -j 16


# Different for 2016. Need to add.
# Compilation can take over an hour if using ZC2016.

# do a local test
cd VHbbAnalysis/Heppy/test
python vhbb_combined.py

# To run over crab...
- First mess with crab/heppy_crab_config.py to set up your own outLFNDirBase for output storage on EOS
cd crab
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init --voms cms --valid 168:00
# sh launchall.sh VHBBHeppyV21bis_datasets.txt
# submission command commented to avoid undesired submissions



