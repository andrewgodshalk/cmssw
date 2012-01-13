import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

############### Jets
# count high pt jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
#require 6 jets offline, of which 2 must be hard
#QuadJet trigger cuts at pt >80, eta < 5.0
razorMJPFJetSel30 = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>30 && abs(eta)<3.0' )
razorMJPFJetSel80 = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = 'pt()>80' )

#we start by filtering very jetty events as we can't cope with them
razorPFJetSelCount = cmgCandCount.clone( src = 'cmgPFJetSel', minNumber = 20 )
#
razorMJPFJetSel30Count = cmgCandCount.clone( src = 'razorMJPFJetSel30', minNumber = 6 )
razorMJPFJetSel80Count = cmgCandCount.clone( src = 'razorMJPFJetSel80', minNumber = 2 )

# id the jets
#used to veto event - the number of jets that fail loose jet ID
razorMJPFJetSelID = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = '(!getSelection("cuts_looseJetId"))' )
razorMJPFJetIDCount = cmgCandCount.clone( src = 'razorMJPFJetSelID', minNumber = 1 ) #filter inverted below

############### Trigger
#make a skim on the HLT - should match all multi triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
# triggers from the MultiJet, ElectronHad and MuHad PDs
razorMJTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_.*Mu[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$")'\
                                            )
razorMJTriggerCount = cmgCandCount.clone( src = 'razorMJTriggerSel', minNumber = 1 )

razorMJL1TriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_L1MultiJet_v[0-9]+$")'\
                                            )
razorMJL1TriggerCount = cmgCandCount.clone( src = 'razorMJL1TriggerSel', minNumber = 1 )

############### MR and R
#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

razorMJHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorMJPFJetSel30")
      ),
      balanceAlgorithm = cms.uint32(1),#use the MassBalance algo
      maxCand = cms.uint32(50)
    )
)

razorMJDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBox'),
    leg2Collection = cms.InputTag('razorMJHemiHadBox'),
    metCollection = cms.InputTag('cmgPFMET')                  
    )    
)

razorMJHemiHadBoxTop = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorMJPFJetSel30")
      ),
      balanceAlgorithm = cms.uint32(2),#use the TopMassBalance algo
      maxCand = cms.uint32(50)
    )
)

razorMJDiHemiHadBoxTop = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBoxTop'),
    leg2Collection = cms.InputTag('razorMJHemiHadBoxTop'),
    metCollection = cms.InputTag('cmgPFMET')                  
    )    
)

############### Run the sequences
razorMJTriggerSequence = cms.Sequence(
    razorMJTriggerSel+
    razorMJL1TriggerSel
    )

razorMJJetSequence = cms.Sequence(                             
    razorMJPFJetSel30*
    razorMJPFJetSelID*
    razorMJPFJetSel80
)

razorMJHemiSequence = cms.Sequence(
    razorMJHemiHadBox*
    razorMJDiHemiHadBox+
    razorMJHemiHadBoxTop*
    razorMJDiHemiHadBoxTop     
)

razorMJObjectSequence = cms.Sequence(
    razorMJJetSequence + 
    razorMJTriggerSequence+
    razorMJHemiSequence                            
    )


razorMJSequence = cms.Sequence(
    razorMJObjectSequence
    )

#offline based selection
razorMJSkimSequence = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #the selection 
    razorMJPFJetSel30Count+
    razorMJPFJetSel80Count+
    #filter is inverted
    ~razorMJPFJetIDCount+
    #apply trigger also
    razorMJTriggerCount
    )

#just take all events that pass the trigger
razorMJTriggerSkimSequence = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    razorMJTriggerCount
    )

#take events which have the L1 passthrough
razorMJL1TriggerSkimSequence = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    razorMJL1TriggerCount
    )
