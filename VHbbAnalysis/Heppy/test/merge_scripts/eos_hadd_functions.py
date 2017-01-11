# eos_hadd_functions.py
# Functions for combining smaller crab-output root files from Ntupler into larger ntuples.
# Limits size of ntuples to make processing more managable.
#
# Created: 2016-12-05 - godshalk
# Created: 2016-12-05 - godshalk
#

import os

# =============================================================================
# make_files_and_sizes_list()
#  path: path with desired root files.
#  fn_outputFileList: file where output is stored.
# Create a file with a list of ntuple files and their sizes. File has format:
#   filename1.root size1
#   filename2.root size2
#   filename3.root size3
#   ...
# 
def make_files_and_sizes_list(path, fn_outputFileList, eos=True):
    print "get_files_and_sizes() called with:"
    print "  path:", path
    print "  fn_outputFileslists:", fn_outputFileList

    # If told to look in a folder with "log" or "failed" in the title, return.
    if path.find('failed') != -1 or path.find('log') != -1 : return 1

    # Pipe output of "ls -l" into temporary file.
    if os.path.isfile('./tmp.txt') : os.system('rm tmp.txt')
    if eos: os.system('xrdfs root://cmseos.fnal.gov ls -lu -l ' + path + '/ > tmp.txt')
    else:   os.system('ls -l ' + path + '/ > tmp.txt')

    # Get contents of ls command from tmp file.
    f_temp = open('tmp.txt')
    rawFileList = f_temp.readlines()
    f_temp.close()
    if len(rawFileList) == 0: return 0

    # Split file info into only file and size.
    fileAndSize = []
    #if os.path.isfile('./{}'.format(fn_outputFileList)) : os.system('rm '+fn_outputFileList)
    for fileInfo in rawFileList:
	fileInfo = fileInfo.replace('\n','')   # Get rid of return characters.
	if fileInfo.find('log.tar.gz') != -1: continue   # skip log files.
        splitInfo = fileInfo.split()
	if fileInfo.find('.root') != -1 : 
            if eos : filePrefix = 'root://cmseos.fnal.gov/'   # Set up a prefix for eos or local processing.
            else   : filePrefix = path + '/'
	    #fileAndSize.append([filePrefix+splitInfo[4], splitInfo[3]])   # Save only file name and size.
            #print splitInfo[3], filePrefix+splitInfo[4]
	    os.system('echo \'' + filePrefix + splitInfo[4] + ' ' + splitInfo[3] + '\' >> ' + fn_outputFileList)
        else :   # Look in daughter directories for root files.
            newPath = splitInfo[4]
	    #print newPath
	    make_files_and_sizes_list(newPath, fn_outputFileList, eos)
    # cleanup, return
    os.system('rm tmp.txt')
    return 1



# =============================================================================
# make_partitioned_files_list()
#  fn_files_and_sizes: filename with filename and file size per line.
#  fn_merged_filelist_prefix: prefix of eventual hadd'd file. Ended by _XXof##.root, or just .root if only one file is needed.
#  parent_path_output: final location of merged files. 
#  file_size_max: maximum size of each partition of files, in bytes.
# Creates a files with a lists of ntuple files, broken up into chunks of size less than file_size_max.
# Format of output:
#
# [fn_merged_filelist_prefix]_01of23.root
# filename1
# filename2
# ...
# 
def make_partitioned_file_lists(fn_files_and_sizes, fn_merged_filelist_prefix, parent_path_output, file_size_max, eos=True) : 
    print "make_partioned_files_list() called with:"
    print "  fn_files_and_sizes:", fn_files_and_sizes
    print "  fn_merged_filelist_prefix:", fn_merged_filelist_prefix
    print "  parent_path_output:", parent_path_output
    print "  file_size_max:", file_size_max

    # Get filenames, sizes from file.
    f_input = open(fn_files_and_sizes)
    fileAndSize = []
    for line in f_input.readlines() :
        line = line.replace('\n', '')
        fileAndSize.append(line.split())
    #for fs in fileAndSize : print fs[0], fs[1]
    #for file,size in fileAndSize : print size, file

    # Cycle through the files and sizes. Add files to a list until the max size is met.
    accumulatedSize = 0
    nFiles = len(fileAndSize)
    fileLists = []
    currentFileList = []
    print "Cycling through %i files." % nFiles 
    for file, size in fileAndSize :
        accumulatedSize += int(size)
        if accumulatedSize > file_size_max : 
            fileLists.append(currentFileList)
            currentFileList = []
            accumulatedSize = int(size)
        currentFileList.append(file)
    fileLists.append(currentFileList)   # Add the last list to the list.

    # Split last two lists.
    if len(fileLists) > 1 :
        tempList = fileLists.pop() + fileLists.pop()
	sz = len(tempList)
        fileLists.append(tempList[:int(round(float(sz)/2)) ])
        fileLists.append(tempList[ int(round(float(sz)/2)):])

        # Set up and output file lists.  
        #os.system('rm %s' % fn_partitioned_files )
        numberOfMerges = len(fileLists)
        digitsForStr = len(str(numberOfMerges))
        fmtStr = '{:0%id}of{}' % digitsForStr
        # For every merge file...
        for i in range(numberOfMerges) :
            #print fmtStr.format(i+1, numberOfMerges)
            fn_out = fn_merged_filelist_prefix + "_" + fmtStr.format(i+1, numberOfMerges) + ".txt"
            if os.path.isfile('./{}'.format(fn_out)) : os.system('rm {}'.format(fn_out))   # Delete the file if it already exists.
            for file in fileLists[i] :
                os.system('echo \'' + file + '\' >> ' + fn_out)

    # If all files combined are less than max file size, just make one.
    elif len(fileLists) == 1 :
        fn_out = fn_merged_filelist_prefix + ".txt"
        if os.path.isfile('./{}'.format(fn_out)) : os.system('rm '+fn_out)   # Delete the file if it already exists.
        for file in fileLists[0] :
            os.system('echo \'' + file + '\' >> ' + fn_out)
    return 1


