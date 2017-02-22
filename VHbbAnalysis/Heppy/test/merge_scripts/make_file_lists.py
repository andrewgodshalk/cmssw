# Script for combining smaller crab-output root files from Ntupler into larger ntuples.
# Limits size of ntuples to make processing more managable.
# Most of work done by functions in eos_hadd_functions.py
#
# Created: 2016-12-05 - godshalk
# Created: 2016-12-05 - godshalk
#

import os
from eos_hadd_functions import make_files_and_sizes_list, make_partitioned_file_lists 

# RUN PARAMETERS

file_size_max = 5*10**9   # Maximimum size of combined ntuple files.
# Input and output paths.
# parent_path_input  = "/store/group/leptonjets/noreplica/godshalk/2017-02_ZJNtuples2016/crab_output/"
parent_path_input  = "/store/group/leptonjets/noreplica/godshalk/CRABTEST/DoubleMuon/"
parent_path_output = "/store/group/leptonjets/noreplica/godshalk/2017-02_ZJNtuples2016/"
dataset_directories = [
            "ZC2017_V25_DATA_RUNG_DoubleMuon__Run2016G-23Sep2016-v1",
            #"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
            #"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            #"TT_TuneCUETP8M2T4_13TeV-powheg-pythia8",
            #"WW_TuneCUETP8M1_13TeV-pythia8",
            #"WZ_TuneCUETP8M1_13TeV-pythia8",
            #"ZZ_TuneCUETP8M1_13TeV-pythia8",
        ]

# Should include proper processing date, will be removed from file name.

# Handle command line input
    # Meh. I think it's fine as is.

# Make directories for lists if they don't already exist.
for dir in ['./files_and_sizes', './merge_filelists'] :
    if not os.path.isdir(dir) : os.system('mkdir {}'.format(dir))

# Lay out initialization.
print "\n==========hadd_eos_into_parts.py=========="
print "Max output files size: ", file_size_max/10**9, "GB"
print "Directories to merge: "
for dd in dataset_directories:
    print "    ", dd

for dataset_directory in dataset_directories :
    fn_dataset_directory = dataset_directory.split('/')[0]
    
    # Make list of ROOT files and sizes in crab output folder to process.
    fn_files_and_sizes = "files_and_sizes/"+fn_dataset_directory+".txt"
    make_files_and_sizes_list(parent_path_input+dataset_directory, fn_files_and_sizes)

    # Partition files into smaller groups to process.
    fn_merged_filelist_prefix = "merge_filelists/"+fn_dataset_directory
    make_partitioned_file_lists(fn_files_and_sizes, fn_merged_filelist_prefix, parent_path_output, file_size_max)


print "\n"

