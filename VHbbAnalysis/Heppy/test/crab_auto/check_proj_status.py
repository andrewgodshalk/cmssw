#/usr/bin/env python

#-------------------------------------
# check_proj_status.py
#
# created : 2016-12-09 - godshalk
# modified: 2016-12-22 - godshalk
#
# Function that creates a file containing the output of "crab status"
#   for all tasks in an input project directory.
#
# Call with: 
# 
#     python check_proj_status.py <PROJECT DIRECTORY> [options]
# 
# Will output all output to terminal. Can be saved to file by piping:
#
#     python check_proj_status.py <PROJECT DIRECTORY> [options] &> status.txt
#
# Most of commented code is work done to automatically save to file. Found
# it was difficult to save the output of another python script (CRAB3 command
# is python based), so abandoned for the time being.
#
# For future nonsense along this line, take a look at importing and using a 
# context manager to mess with argv. See:
#   http://stackoverflow.com/questions/3781851/run-a-python-script-from-another-python-script-passing-in-args
#


import os, sys
#from timestamp import timestamp
from make_crab_task_list import make_crab_task_list

def check_proj_status(proj_dir, options) : 
    print ''
#    print options

    # Add a slash at the end of proj_dir, to be safe.
    if not proj_dir[-1] == '/' : proj_dir+='/'

    # Make sure the directory in question exists.
    if not os.path.isdir(proj_dir) :
        print "  check_proj_status() : Directory not found: {}".format(proj_dir)
        return False

    # Create a task list file and extract its contents.
    f_task_list = open(make_crab_task_list(proj_dir, True))
    tasks = f_task_list.readlines()
    f_task_list.close()

    # Create a new file for the crab status dump.
    #fn_status_dump = proj_dir+"status_{}.txt".format(timestamp())

    # For each task, dump that task's status to file with a bit of formatting.
    #sys.stdout = open(fn_status_dump, 'w')
    for task in tasks : 
        os.system("echo ====================================================================== ")
        os.system("echo \"{}\"".format(proj_dir+task))
        os.system("crab status {} -d {}".format(options, proj_dir+task))
        os.system("echo \"\n\"")
        #os.system("echo ====================================================================== >> {}".format(fn_status_dump))
        #os.system("echo \"{}\" >> {}".format(proj_dir+task, fn_status_dump))
        #os.system("{} {} status -d {} &>> {}".format(cmd_python, cmd_crab, proj_dir+task, fn_status_dump))
        #os.system("echo \"\n\" >> {}".format(fn_status_dump))
    return True

if __name__ == "__main__" :
    option = ""
    #if len(sys.argv) > 2 : option = sys.argv[2].replace('"', '').replace('\n','')
    for arg in sys.argv[2:] : option = " "+arg
    check_proj_status(sys.argv[1], option)


