#!/bin/python

#------------------------------------------------------------------------------
# Small script to print value of "Counts" histogram of all ntuples in an
# input directory
#
# Created : 2017-01-10 godshalk
#
#------------------------------------------------------------------------------

#from ROOT import gROOT, TFile, TH1
import os, sys

os.system("xrdfs root://cmseos.fnal.gov ls {} > tmp.txt".format(sys.argv[1]))
os.system("root -l get_event_counts_from_ntuple.C")
#os.system("rm tmp.txt")
