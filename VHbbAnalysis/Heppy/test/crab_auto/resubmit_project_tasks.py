#/usr/bin/env python

#-------------------------------------
# resubmit_project_tasks.py
#
# created : 2016-12-20 - godshalk
# modified: 2016-12-20 - godshalk
#
# Function/script that resubmits all crab tasks in an input project directory.
#
# Call with: 
# 
#     python resubmit_project_tasks.py <PROJECT DIRECTORY>
# 
# Will output all output to terminal. Can be saved to file by piping:
#
#     python check_proj_status.py <PROJECT DIRECTORY> &> status.txt
#


import os, sys
#from timestamp import timestamp
from make_crab_task_list import make_crab_task_list

def resubmit_project_tasks(proj_dir, options) : 
    print ''

    # Add a slash at the end of proj_dir, to be safe.
    if not proj_dir[-1] == '/' : proj_dir+='/'

    # Make sure the directory in question exists.
    if not os.path.isdir(proj_dir) :
        print "  resubmit_project_tasks() : Directory not found: {}".format(proj_dir)
        return False

    # Create a task list file and extract its contents.
    f_task_list = open(make_crab_task_list(proj_dir, True))
    tasks = f_task_list.readlines()
    f_task_list.close()

    # For each task, dump that task's status to file with a bit of formatting.
    for task in tasks : 
        os.system("echo ====================================================================== ")
        os.system("echo \"{}\"".format(proj_dir+task))
        os.system("crab resubmit {} -d {}".format(options, proj_dir+task))
        os.system("echo \"\n\"")
    return True

if __name__ == "__main__" :
    option = ""
    for arg in sys.argv[2:] : option = " "+arg
    resubmit_project_tasks(sys.argv[1], option) 
