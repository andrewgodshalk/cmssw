#/usr/bin/env python

#-------------------------------------
# get_proj_reports.py
#
# created : 2016-01-06 - godshalk
# modified: 2016-01-06 - godshalk
#
# Script that calls crab report on all tasks in a project, creates a folder in
# the project directory, and stores the json outputs in that folder.
#
# Call with: 
# 
#     python get_proj_reporst.py <PROJECT DIRECTORY> [options]
# 


import os, sys
from timestamp import timestamp
from make_crab_task_list import make_crab_task_list

#=================================================================================================
def get_proj_reports(proj_dir, options) : 
    print ''
#    print options

    # Add a slash at the end of proj_dir, to be safe.
    if not proj_dir[-1] == '/' : proj_dir+='/'

    # Make sure the directory in question exists.
    if not os.path.isdir(proj_dir) :
        print "  get_proj_reports() : Directory not found: {}".format(proj_dir)
        return False

    # Create directory.
    report_dir = "{}reports_{}".format(proj_dir, timestamp())
    print " Creating report folder: %s" % report_dir
    os.system("mkdir {}".format(report_dir))

    # Create a task list file and extract its contents.
    f_task_list = open(make_crab_task_list(proj_dir, True))
    tasks = f_task_list.readlines()
    f_task_list.close()

    # For each task, dump that task's status to file with a bit of formatting.
    for task in tasks : 
        task = task.replace("\n","")
        task_dir = proj_dir+task
        # Create the report
        os.system("echo ====================================================================== ")
        os.system("echo \"{}\"".format(task_dir))
        os.system("crab report{} -d {}".format(options, task_dir))
        # Transfer the task into the reports folder.
        fn_report = "report_{}.json".format(task[5:])  # report name is that same as the report fikder
        print fn_report
        os.system("cp {}/results/inputDatasetLumis.json {}/{}".format(task_dir, report_dir, fn_report))
        os.system("echo \"\n\"")

    # Complete program
    print ''
    return True


#=================================================================================================
if __name__ == "__main__" :
    option = ""
    #if len(sys.argv) > 2 : option = sys.argv[2].replace('"', '').replace('\n','')
    for arg in sys.argv[2:] : option = " "+arg
    get_proj_reports(sys.argv[1], option)


