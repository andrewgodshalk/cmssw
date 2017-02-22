#!/usr/bin/env python

#-------------------------------------
# make_crab_task_list.py
#
# created : 2016-12-09 - godshalk
# modified: 2016-12-09 - godshalk
#
# Function that creates a text list of crab task directories for an
#   input project directory.
# Excludes entries in folder not beginning with "crab_" (i.e. skips
# existing task lists, report folders, etc.).
#

import os, sys

def make_crab_task_list( proj_dir , silent = False) :
    if not silent: print ''

    # Add a slash at the end of proj_dir, to be safe.
    if not proj_dir[-1] == '/' : proj_dir+='/'

    # Make sure the directory in question exists.
    if not os.path.isdir(proj_dir) :
        if not silent: print "  make_crab_task_list() : Directory not found: {}".format(proj_dir)
        return "INVALID_DIR"

    # Check to see if a task list file exists. If so, delete.
    fn_task_list = proj_dir+"task_dir_list.txt"
    if os.path.isfile(fn_task_list) :
        if not silent: print "  make_crab_task_list(): task list already exists. Recreating."
        os.system("rm {}".format(fn_task_list))

    # Use ls, grep, and piping system commands to create a list of directories.
    os.system("ls -1 {} | grep crab_ > {}".format(proj_dir, fn_task_list))

    # Open file, print all the stuff!
    f_task_list = open(fn_task_list)
    if not silent: print "  make_crab_task_list(): task list created at {}\n".format(fn_task_list)
    if not silent: print f_task_list.read()
    f_task_list.close()

    return fn_task_list 

if __name__ == "__main__" : 
    # Use a command line input directory as input 
    make_crab_task_list( sys.argv[1] )
