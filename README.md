Directions taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/VHiggsBB#Ntuple_production_campaigns_AN1

## Setting up software on LPC

##### Source LPC Environment Scripts
```
source /cvmfs/cms.cern.ch/cmsset_default.csh
```

##### Set up CMSSW release in your scratch space scratch space (directory name not important)
```
mkdir NtuplerWorkDir
cd NtuplerWorkDir
```
[V25]: Use CMSSW_8_0_25 \
[ZC2016]: Use CMSSW_8_0_19 \
[ZC2015]: Use CMSSW_7_6_3_patch2
```
cmsrel CMSSW_8_0_25
cd CMSSW_8_0_25/src/
cmsenv
```

##### Check out software from github
```
git cms-merge-topic -u andrewgodshalk:V25
```
For more information about CMSSW on git, see http://cms-sw.github.io/

##### Compile
```
scram b -j 16
```

##### Check the upstream for updates (optional, for development purposes)
Taken from https://github.com/vhbb/cmssw/tree/V25
See more documentation at https://twiki.cern.ch/twiki/bin/viewauth/CMS/VHiggsBB#Ntuple_production_campaigns_for
```
git remote add vhbb https://github.com/vhbb/cmssw.git
git fetch vhbb
git diff vhbb/V25
```
vhbb/V25 is the latest branch as of this writing. Check the documentation, or with the VHbb group, to see if there is a more recent version. Resolve conflicts, commit, and continue:
```
git merge vhbb/V25
```

## Processing with CRAB

##### Modify Configuration Files
- Move to the [VHbbAnalysis/Heppy/test](https://github.com/andrewgodshalk/cmssw/tree/ZC2016/VHbbAnalysis/Heppy/test) directory. Most of the work will be done from here.
- Modify [crab/heppy_crab_config.py](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py) for running over simulated datasets with crab (line numbers subject to change):
  - [~L5](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L5): Change config.General.requestName name to something meaningful.
  - [~L6](https://github.com/andrewgodshalk/cmssw/blob/ZC2016/VHbbAnalysis/Heppy/test/crab/heppy_crab_config.py#L6): Set up config.General.workArea to point to a new, meaninfully named directory.
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
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -valid 168:00
python vhbb_combined.py
python vhbb_combined_data.py
```
Check resulting Loop_* folders for resulting tree.root.

##### Record Dataset Information

TO DO: Include information about running automation scripts, recording DAS information.
TO DO: Include information about datasets, where lists are created from.

##### To run over crab...
```
cd crab (or cd crab_data)
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -valid 168:00
./launchall.sh dataset_lists/ZPJ_datasets_MC2016_SPRING16_TEST.txt
```
A crab project folder based on your specifications in heppy_crab_config.py will be created in the crab directory. launchall.sh will create a crab task for each dataset in the input dataset_list.txt file.

##### Check Job Status
For an individual task:
```
crab status -d <TASK DIRECTORY>
```

For all tasks in a project, from VHbbAnalysis/Heppy/test/:
```
python crab_auto/check_proj_status.py <CRAB PROJECT DIRECTORY> [OPTIONS]
```
Script will output status of all crab jobs located in \<CRAB PROJECT DIRECTORY>\. Options for the crab status command may be included after the dataset. It may be useful to pipe output into a temporary file for easier browsing:
```
python crab_auto/check_proj_status.py <CRAB PROJECT DIRECTORY> > status_output.txt
```

On the web, check the listing for your name on the [Task Monitoring Dashboard](http://dashb-cms-job.cern.ch/dashboard).

##### Resubmitting Failed Jobs
For an individual task:
```
crab resubmit -d <TASK DIRECTORY> [OPTIONS]
```

For all tasks in a project, from VHbbAnalysis/Heppy/test/:
```
python crab_auto/resubmit_project_tasks.py <CRAB PROJECT DIRECTORY>
```

## Calculating Processed Luminosity (DATA ONLY)
2017-01-12 - This currently CAN NOT WORK if the dataset hasn't been COMPLETELY processed. Due to the Framework Job Report not being set up correctly for Heppy, crab report is unable to do the proper calculations. See [conversation](https://hypernews.cern.ch/HyperNews/CMS/get/computing-tools/2515.html) in computing hypernews for details.

Alright. Your data projects are in some state of completion. Time to calculate the integratd luminosity you succesfully processed. You should be able to run crab report even if your project has some failed jobs or has completed running.

NOTE: It might be best to calculate luminosity after all jobs have completed and you've already merged your ntuples, so there is no discrepancy caused by jobs completing after one or the other step.

##### Run crab report

For all tasks in a project, from VHbbAnalysis/Heppy/test/:
```
python crab_auto/get_proj_reports.py <CRAB PROJECT DIRECTORY>
```

This command will run "crab report" for all tasks in the project directory (the terminal output is informative as well), then copies all of resulting JSON files (from PROJ_DIR/TASK_DIR/results/) to a timestamped reports folder in the PROJ_DIR.

##### Merge report JSON with Golden JSON and Run Brilcalc
NOTE: Following steps completed on LXPLUS. Have not tested on LPC.

Transfer lumicalc scripts folder and raw JSON files from the crab reports to lxplus. Set up a CMSSW environment, run the setup script, then run the python script with the raw JSON file as input:
```
cmsrel CMSSW_8_0_19
cd CMSSW_8_0_19/src
cmsenv
cd -
source brilcalc_setup.sh
python get_brilcalc_report.py <RAW JSON FILE>
```

The golden JSON file is hardcoded into get_brilcalc_report.py and may need to be updated.

##### Document, Store Results

Upload results to EOS, record information on data processing sheet.


## Merging, storing Ntuples

##### Script nonsense

Scripts for using condor to merge ntuples into larger files can be found in the [merge_scripts](https://github.com/andrewgodshalk/cmssw/tree/ZC2016/VHbbAnalysis/Heppy/test/) folder. Before beginning, it is a good idea to remove any old working files from the files_and_sizes and merged_filelists directories.

Create lists of files to merge. Open make_file_lists.py and modify the following variables (around line 12 to 20):
- file_size_max: maximum size of combined ntuples. Set to 5GB by default.
- parent_path_output: Directory where output, combined ntuples will be stored.
- parent_path_input: Directory where raw output ntuples from crab are stored.
- dataset_directories: List of all datasets directories in parent_path_input that you'd like to combine into ntuples.

Also modify the parent_path_output on line 16 of setup_condor.py (with the appropriate file/server prefix this time).

Once modifications have been made, run the following scripts to set up condor:
```
python make_file_lists.py
python setup_condor.py
```

Follow the final direction given by setup_condor.py and change condor/condor_config.script file. Then submit to condor:
```
condor_submit condor_confit.script
```

Monitor status using condor_q. More information about running on condor on LPC can be found [here](http://uscms.org/uscms_at_work/physics/computing/setup/batch_systems.shtml).

##### More information to record
Get the number of events processed for each ntuple by using merge_scripts/get_event_counts_from_ntuple.py with the ntuple file location as an option. Working from the merge_scripts folder, for example:
```
python get_event_counts_from_ntuple.py /store/group/leptonjets/noreplica/godshalk/2017-01_ZJNtuples2016/
```
The script will output bin value of the "Count" histogram and the file name of each ntuple in the input directory. The counts are important for calculating weights for MC.

## Code Housekeeping

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
See documentation on [git-fetch](https://git-scm.com/docs/git-fetch) and [git-merge](https://git-scm.com/docs/git-merge) for more info on pulling changes from github and rectifying conflicts.

## Other useful links
- [EOS at LPC Information](http://uscms.org/uscms_at_work/computing/LPC/usingEOSAtLPC.shtml)
- [DAS](https://cmsweb.cern.ch/das/) - Dataset database web lookup.
- [CRAB Documentation](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCrab)
