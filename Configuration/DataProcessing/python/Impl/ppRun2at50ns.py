#!/usr/bin/env python
"""
_ppRun2at50ns_

Scenario supporting proton collisions

"""

import os
import sys

from Configuration.DataProcessing.Reco import Reco
import FWCore.ParameterSet.Config as cms

class ppRun2at50ns(Reco):
    def __init__(self):
        Reco.__init__(self)
        self.recoSeq=''
        self.cbSc='pp'
    """
    _ppRun2at50ns_

    Implement configuration building for data processing for proton
    collision data taking for Run2

    """


    def promptReco(self, globalTag, **args):
        """
        _promptReco_

        Proton collision data taking prompt reco

        """
        if not 'skims' in args:
            args['skims']=['@allForPrompt']

        if not 'customs' in args:
            args['customs']=['Configuration/DataProcessing/RecoTLR.customisePromptRun2Deprecated_50ns']
        else:
            args['customs'].append('Configuration/DataProcessing/RecoTLR.customisePromptRun2Deprecated_50ns')

        process = Reco.promptReco(self,globalTag, **args)

        return process


    def expressProcessing(self, globalTag, **args):
        """
        _expressProcessing_

        Proton collision data taking express processing

        """
        if not 'skims' in args:
            args['skims']=['@allForExpress']

        if not 'customs' in args:
            args['customs']=['Configuration/DataProcessing/RecoTLR.customiseExpressRun2Deprecated_50ns']
        else:
            args['customs'].append('Configuration/DataProcessing/RecoTLR.customiseExpressRun2Deprecated_50ns')

        process = Reco.expressProcessing(self,globalTag, **args)
        
        return process

    def visualizationProcessing(self, globalTag, **args):
        """
        _visualizationProcessing_

        Proton collision data taking visualization processing

        """
        if not 'customs' in args:
            args['customs']=['Configuration/DataProcessing/RecoTLR.customiseExpressRun2Deprecated_50ns']
        else:
            args['customs'].append('Configuration/DataProcessing/RecoTLR.customiseExpressRun2Deprecated_50ns')

        process = Reco.visualizationProcessing(self,globalTag, **args)
        
        return process

    def alcaHarvesting(self, globalTag, datasetName, **args):
        """
        _alcaHarvesting_

        Proton collisions data taking AlCa Harvesting

        """


        if not 'skims' in args and not 'alcapromptdataset' in args:
            args['skims']=['BeamSpotByRun',
                           'BeamSpotByLumi',
                           'SiStripQuality']
            
        return Reco.alcaHarvesting(self, globalTag, datasetName, **args)

