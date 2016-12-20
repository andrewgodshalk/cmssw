Directions taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/VHiggsBB#Ntuple_production_campaigns_AN1

## To set up the environment on LPC... 

##### Source LPC Scripts
```
source /cvmfs/cms.cern.ch/cmsset_default.csh
```

##### Set up CMSSW release in your scratch space scratch space (directory name not important)
```
mkdir NtuplerWorkDir
cd NtuplerWorkDir
```
[ZC2016]: Use CMSSW_8_0_19 \
[ZC2015]: Use CMSSW_7_6_3_patch2
```
cmsrel CMSSW_8_0_19
cd CMSSW_8_0_19/src/
cmsenv
```

##### Check out software from github
```
git cms-merge-topic andrewgodshalk:ZC2016
```
For more information about CMSSW on git, see http://cms-sw.github.io/

##### Check the upstream for updates (optional, for development purposes)
For ZC2015:
```
git remote add perrozzi https://github.com/perrozzi/cmssw.git
git fetch perrozzi
git merge perrozzi/V21bis
```

For ZC2016: taken from https://github.com/vhbb/cmssw/tree/V24
See more documentation at https://twiki.cern.ch/twiki/bin/viewauth/CMS/VHiggsBB#Ntuple_production_campaigns_for
```
git remote add vhbb https://github.com/vhbb/cmssw.git
git fetch vhbb
git diff vhbb/V24
```
vhbb/V24 is the latest branch as of this writing. Check the documentation, or with the VHbb group, to see if there is a more recent version. Resolve conflicts, commit, and continue:
```
git merge vhbb/V24
```

##### Compile
```
scram b -j 16
```
NOTE: Compilation may take up to an hour if using ZC2016.

##### Modify Configuration Files
- Move to the [VHbbAnalysis/Heppy/test](https://github.com/andrewgodshalk/cmssw/tree/ZC2016/VHbbAnalysis/Heppy/test) directory. Most of the work will be done from here.
- Modify [crab/heppy_crab_config.py](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py) for running over simulated datasets with crab (line numbers subject to change):
  - [~L6](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L6): Change config.General.requestName name to something meaningful.
  - [~L7](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L7): Set up config.General.workArea to point to a new, meaninfully named directory.
  - [~L48](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L48): config.Data.splitting to FileBased. Can also be LumiBased, but it's a bit easier to deal with problem files as opposed to problem Lumis.
  - [~L49](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L49): Change config.Data.unitsPerJob to 1. May increase if desired, but found we've had problems with memory leaks that kill jobs with more units on some T2s.
  - [~L51](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L51): config.Data.totalUnits = 1 : Set to run a single job. Good for testing submission. Comment out when ready to run over full datasets.
  - [~L54](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L54): set config.Data.outLFNDirBase to point to an EOS storage space.
  - [~L56](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L56): set config.Data.outputDatasetTag to a meaningful, unique identifier.
- Need to make similar modifications to run over DATA datasets: [crab_data/heppy_crab_config.py](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab_data/heppy_crab_config.py)
- Modify [vhbb.py](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/vhbb.py#L491) and [vhbb_combined_data.py](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/vhbb_combined_data.py) with new source files (an MC and DATA file, respectively) for local testing.

##### Do a local test.
```
cd VHbbAnalysis/Heppy/test
python vhbb_combined.py
python vhbb_combined_data.py
```
Check resulting Loop_* folders for resulting tree.root.

##### To run over crab...
```
cd crab (or cd crab_data)
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init --voms cms --valid 168:00
sh launchall.sh ZPJ_datasets_MC2016_SPRING16_TEST.txt
```

##### TO DO: ADD DIRECTIONS EOS DIRECTIONS, FOR PROCESSING JOB OUTPUT, CHECKING QUOTA, ETC.


##### Pushing changes to repo
```
git push my-cmssw from-CMSSW_8_0_19:ZC2016
```
Only works if you have access to remote "my-cmssw".

##### Pulling changes from github
```
git fetch my-cmssw
git merge my-cmssw/ZC2016 
```

See documentation on [git-fetch](https://git-scm.com/docs/git-fetch) and [git-merge](https://git-scm.com/docs/git-merge) for more info.
