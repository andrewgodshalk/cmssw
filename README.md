Directions taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/VHiggsBB#Ntuple_production_campaigns_AN1 /
To set up the environment on LPC... 

##### Source LPC Scripts
```
source /cvmfs/cms.cern.ch/cmsset_default.csh
```

##### Set up CMSSW release (preferably in some scratch space)
```
mkdir NtuplerWork
cd NtuplerWork
```
[ZC2016]: Use CMSSW_8_0_19 \
[ZC2015]: Use CMSSW_7_6_3_patch2
```
cmsrel CMSSW_7_6_3_patch2
cd CMSSW_7_6_3_patch2/src/
cmsenv
```

##### Check out appropriate tag
```
git cms-merge-topic andrewgodshalk:ZC2015
```

##### Check the upstream for updates (optional, for development purposes)
For ZC2015:
```
git remote add perrozzi https://github.com/perrozzi/cmssw.git
git fetch perrozzi
git merge perrozzi/V21bis
```

For ZC2016:
```
**TO DO: ADD UPSTREAM DIRECTIONS**
```

##### Compile
```
scram b -j 16
```
NOTE: Compilation can take over an hour if using ZC2016.

##### TO DO: ADD DIRECTIONS, EXACT LOCATIONS TO CHANGE IN CONFIG.
Directions go here.

##### Do a local test.
```
cd VHbbAnalysis/Heppy/test
python vhbb_combined.py
```

##### To run over crab...
- First mess with crab/heppy_crab_config.py to set up your own outLFNDirBase for output storage on EOS /
```
cd crab
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init --voms cms --valid 168:00
# sh launchall.sh VHBBHeppyV21bis_datasets.txt
# submission command commented to avoid undesired submissions
```

##### TO DO: ADD DIRECTIONS EOS DIRECTIONS, FOR PROCESSING JOB OUTPUT, CHECKING QUOTA, ETC.
