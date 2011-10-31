import FWCore.ParameterSet.Types as CfgTypes
import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

#fileNames   = cms.vstring('file:2l2bMetEdmNtuples.root'),         ## mandatory
fname = "BestCSV_ZH_ZToLL_HToBB_M-110_7TeV-powheg-herwigpp_split_00.root";

process.fwliteInput = cms.PSet(
    fileName   = cms.string(fname),
    PUmcfileName = cms.string("ttbarPU_36bins.root"),
    PUmcfileName2011B= cms.string("PU1D2011B.root"),
    PUdatafileName2011B = cms.string("Cert_175832-178078_7TeV_PromptReco_Collisons11_JSON.pileupTruth_v2.root"),
    PUdatafileName = cms.string("Pileup_2011_to_173692_LPLumiScale_68mb_36bins.root"),
    maxEvents   = cms.int32(-1),                             ## optional
    skipEvents   = cms.int32(0),                             ## optional
    outputEvery = cms.uint32(0),                            ## optional
    )

fnameOut = "Updated_"+fname
process.fwliteOutput = cms.PSet(
    fileName  = cms.string(fnameOut),## mandatory
)

process.Analyzer = cms.PSet(
    replaceWeights = cms.bool(True),
    redoPU = cms.bool(True),
    idMuFileName = cms.string("ScaleEffs42.root"),
    hltMuFileName = cms.string("ScaleFactor_muonEffsOnlyIsoToHLT2.2fb_efficiency.root"),
    hltEle1FileName = cms.string("Ele17.root"),
    hltEle2FileName = cms.string("Ele8NotEle17.root"),
    hltEle1AugFileName = cms.string("Ele17Aug5PromptRecoV6.root"),
    hltEle2AugFileName = cms.string("Ele8NotEle17Aug5PromptRecoV6.root"),
    idEle80FileName = cms.string("PFElectronToWP80.root"),
    idEle95FileName = cms.string("PFElectronToWP95.root"),
    hltJetEle1FileName = cms.string("TriggerEfficiency_Jet30_PromptV4Aug05PromptV6.root"),
    hltJetEle2FileName = cms.string("TriggerEfficiency_JetNo30_Jet25_PromptV4Aug05PromptV6.root"),
    recoEleFileName = cms.string("EleReco.root"),
    hltSingleEleMayFileName = cms.string("TriggerEfficiency_Electrons_May10.root"),
    hltSingleEleV4FileName = cms.string("TriggerEfficiency_Electrons_PromptV4Aug05PromptV6.root"),
    idEleFileName = cms.string("ScaleFactor_PFElectrons_DataMontecarlo.root"),
    hltMuOr30FileName =  cms.string("ScaleFactor_muonEffsIsoToHLT2.2fb_efficiency.root"),
    btagEffFileName = cms.string("btag_generic.txt")
    )

    
  
    

