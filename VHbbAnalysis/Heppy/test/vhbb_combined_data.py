#! /usr/bin/env python

# First import basic vhbb so we have the sample available
# Need to set sample.isMC/isData correctly before importing vhbb_combined
# so vhbb_combined knows which modules to schedule 

from vhbb import *

sample.isMC=False
sample.isData=True

from vhbb_combined import *
sample.json="json.txt"
sample.files=[
    # SAMPLE FILES FROM DATA - ENTERED 2016-12-20
        # /SingleMuon/Run2016C-PromptReco-v2/MINIAOD
        "root://cmsxrootd.fnal.gov//store/data/Run2016C/SingleMuon/MINIAOD/PromptReco-v2/000/275/657/00000/AE21C45F-703B-E611-9B97-02163E011C23.root",
        # /DoubleEG/Run2016D-PromptReco-v2/MINIAOD
        #"root://cmsxrootd.fnal.gov//store/data/Run2016D/DoubleEG/MINIAOD/PromptReco-v2/000/276/318/00000/005511B0-2045-E611-81E7-02163E0142DC.root",
    ]

TriggerObjectsAna.triggerObjectInputTag = ('selectedPatTrigger','','RECO')
FlagsAna.processName='RECO'
TrigAna.triggerBits = triggerTableData
L1TriggerAna.processName = 'RECO'

# and the following runs the process directly 
if __name__ == '__main__':
    from PhysicsTools.HeppyCore.framework.looper import Looper 
    looper = Looper( 'Loop', config, nPrint = 1, nEvents = 1000)

    import time
    import cProfile
    p = cProfile.Profile(time.clock)
    p.runcall(looper.loop)
    p.print_stats()
    looper.write()
