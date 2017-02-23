#/usr/bin/env python

#-------------------------------------
# get_datset_info_from_das.py
#
# created : 2017-01-05 - godshalk
# modified: 2017-01-05 - godshalk
#
# Function that takes an dataset list input (from text file) and outputs a 
# deliminated list of info about that set. Utilizes the das_client.py 
# 
# To use...
#
# []$ python get_dataset_info_from_das.py <DATASET_LIST_FILE> [DELIMINATOR]
#
# Outputs information from DAS into terminal in a table deliminated by the
# given input (tab by default).
# 


import os, sys

DEFAULT_DELIMINATOR = "\t"

def get_dataset_info_from_das(dataset_name, deliminator=DEFAULT_DELIMINATOR) : 
    #print ''
    #print "get_dataset_info_from_das(", dataset_name, ",", deliminator, ")"

    # Set up tmp file.
    #os.system("rm tmp_info.txt")

    # Call das_query
    os.system("echo \"{}\" >> tmp_info.txt".format(dataset_name))
    os.system("das_client --query=\"summary dataset={}\" >> tmp_info.txt".format(dataset_name))
    os.system("echo \"\n\" >> tmp_info.txt")
    return True

#==============================================================================================
def get_info_for_dataset_list(dataset_list, deliminator=DEFAULT_DELIMINATOR) :
    #print ''
    #print "get_info_for_dataset_list(", dataset_list, ",", deliminator, ")"
    
    # Create a task list file and extract its contents.
    f_ds_list = open(dataset_list)
    datasets = f_ds_list.readlines()
    f_ds_list.close()
    
    # Set up tmp file.
    os.system("rm tmp_info.txt")

    # Format each dataset name and get info.
    for ds in datasets : 
        ds = ds.replace('\n', '')
        get_dataset_info_from_das(ds, deliminator)

    # Input info from temp file.
    f_ds_info = open("tmp_info.txt")
    ds_raw_info = f_ds_info.readlines()
    f_ds_info.close()

    # Run through lines and extract info.
    ds_info = []
    cur_ds_start_index = 0
    while cur_ds_start_index < len(ds_raw_info) :
        ds_info.append( [ 
            ds_raw_info[cur_ds_start_index+0].replace('\n',''),   #  1 /DYJetsToLL_M-50_TuneCUETP8M1_..../MINIAODSIM
            ds_raw_info[cur_ds_start_index+4].split()[-1]     ,   #  5 nfiles   : 657
            ds_raw_info[cur_ds_start_index+5].split()[-1]     ,   #  6 nevents  : 91350867
            ds_raw_info[cur_ds_start_index+8].split()[-1]     ,   #  9 file_size: 2446135000192
            float(ds_raw_info[cur_ds_start_index+8].split()[-1])/10.0**9 , 
                      ] )
        cur_ds_start_index += 12
    
    # Print results
    for ds in ds_info : 
        print "{1}{0}{2}{0}{3}{0}{4}{0}{5:6.2f}".format(deliminator, ds[0], ds[1], ds[2], ds[3], ds[4])

    return True


#==============================================================================================
# HANDLE COMMAND LINE INPUT.
if __name__ == "__main__" :
    if len(sys.argv) < 2 : 
        print "ERROR: Please input a dataset name or list."
    # Second option is deliminator
    delim = DEFAULT_DELIMINATOR
    if len(sys.argv) == 3 : delim = sys.argv[2]

    # Decide whether input is a dataset list or a single set.
    fn = sys.argv[1]
    if fn[-4:] == ".txt" : get_info_for_dataset_list(fn, delim)
    else : get_dataset_info_from_das(fn, delim)

