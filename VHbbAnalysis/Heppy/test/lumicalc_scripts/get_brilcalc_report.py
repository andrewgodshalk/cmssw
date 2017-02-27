#!/bin/python
#------------------------------------------------------------------------------
# get_brilcalc_report.py
# 
# To use, setup a CMSSW release environment (use "cmsenv" in a CMSSW release
# src folder), run the setup script brilcalc_script.sh, then:
# 
#   python get_brilcalc_report.py <RAW JSON FILE>
#
# The merged json files and brilcalc results will be automatically saved to 
# folders created by brilcalc_setup.sh.
#
# Directions for comparing JSON: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGoodLumiSectionsJSONFile#How_to_compare_Good_Luminosity_f
# Directions for using Brilcalc to calculate integrated luminosity from a json file.
#   http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html 
#
# 2017-02-27 - Updated to reflect ReReco JSONs and recalculated luminosities.
# See hypernews post about updates: https://hypernews.cern.ch/HyperNews/CMS/get/physics-announcements/4495.html
#
#------------------------------------------------------------------------------

from os import system
from sys import argv

# ---------------------------------------------------------
# STEP 1: Combine crab results json with golden JSON.

# Golden JSON file: copied from https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGoodLumiSectionsJSONFile
#fn_goldenJSON = "Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt"
fn_goldenJSON = "Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt"

# Get file name from end of input
input_path = argv[1]
fn = input_path.split('/')[-1]
#merge_path = argv[2]
merge_path = "mergedJSON"
brilcalc_result_path = "brilcalcResults"

#system("compareJSON.py --and golden.json rawJSON/report_ZC2017_01_PR2016_MUONEG_MuonEG__Run2016G-PromptReco-v1.json mergedJSON/report_ZC2017_01_PR2016_MUONEG_MuonEG__Run2016G-PromptReco-v1.json")
system("compareJSON.py --and {} {} {}/{}".format(fn_goldenJSON, input_path, merge_path, fn))


#----------------------------------------------------------
# STEP 2: Use Brilcalc to get a report on the integrated luminosity

#system("brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -i {}/{} -u /pb > {}/{}".format(merge_path, fn, brilcalc_result_path, fn.split('.')[0]+'.txt'))
system("brilcalc lumi -b \"STABLE BEAMS\" --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_DATACERT.json -i {}/{} -u /pb > {}/{}".format(merge_path, fn, brilcalc_result_path, fn.split('.')[0]+'.txt'))


