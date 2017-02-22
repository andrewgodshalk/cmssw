#
# Python script that sets up condor running conditions.
# Run directly after make_file_lists.py
# Creates a python script that can be run in condor.
#
# WARNING: Need to set condor/condor_config.script's Queue number manually.
# 

from os import system

# Do initial cleanup to remove existing files.
system('rm tmp.txt condor/hadd_script.py')

fn_out_script = "condor/hadd_script.py"
filelist_dir = "merge_filelists"
parent_path_output = "root://cmseos.fnal.gov://store/group/leptonjets/noreplica/godshalk/2017-02_ZJNtuples2016/"

# Set up temporary file with output of ls command on merge_filelists folder.
system('ls -1 '+filelist_dir+' >> tmp.txt')

# Get contents of list temp file, then get rid of the file.
f = open('tmp.txt')
fileList = f.readlines()
f.close()
system('rm tmp.txt')

# Write the script header:
f_script = open(fn_out_script, 'w')
f_script.write("#!/usr/bin/env python\n")
f_script.write("import os, sys\n")
f_script.write("cmd_list = [\n")

for l in fileList : 
	l = l.replace(".txt\n", '')
	cmd = '    \"hadd -f %s%s.root `cat %s/%s.txt`\",\n' % (parent_path_output,l,filelist_dir,l) 
	f_script.write(cmd)

f_script.write(']\n\n')

# Add to script command that actually runs hadd, then close
f_script.write('os.system(cmd_list[int(sys.argv[1])])\n')
f_script.close()

# Tar the file lists to pass to condor
filelist_tar = "condor/%s.tgz" % filelist_dir
system('rm %s' % filelist_tar) 
system('tar czv --file=%s %s/*' % (filelist_tar, filelist_dir))

# OUTPUT NUMBER OF FILES TO ADD TO CONDOR CONFIG
print '\n CHANGE QUEUE NUMBER IN condor/condor_config.script: %d\n' % len(fileList)

