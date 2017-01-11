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
parent_path_input  = "/store/user/yokugawa/ZC_2016_no2/DoubleEG/"
parent_path_output = "/store/group/leptonjets/noreplica/godshalk/2017-01_ZJNtuples2016/"
dataset_directories = [
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016B-PromptReco-v1",
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016B-PromptReco-v2",
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016C-PromptReco-v2",
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016D-PromptReco-v2",
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016E-PromptReco-v2",
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016F-PromptReco-v1",
        "ZC2016_11_Run2016_no2_DoubleEG__Run2016G-PromptReco-v1",
]

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
    # Make list of ROOT files and sizes in crab output folder to process.
    fn_files_and_sizes = "files_and_sizes/"+dataset_directory+".txt"
    make_files_and_sizes_list(parent_path_input+dataset_directory, fn_files_and_sizes)

    # Partition files into smaller groups to process.
    fn_merged_filelist_prefix = "merge_filelists/"+dataset_directory
    make_partitioned_file_lists(fn_files_and_sizes, fn_merged_filelist_prefix, parent_path_output, file_size_max)


print "\n"

