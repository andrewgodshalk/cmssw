import FWCore.ParameterSet.Config as cms

def addVBTFSelection(var):
    vbtf = cms.PSet(
        eid = cms.string('test_bit(sourcePtr().electronID(\"%s\"),0)' % var),
        iso = cms.string('test_bit(sourcePtr().electronID(\"%s\"),1)' % var),
        conversionVeto = cms.string('test_bit(sourcePtr().electronID(\"%s\"),2)' % var)
    )
    return vbtf
    
vbtfelectron60 = addVBTFSelection('simpleEleId60relIso')
vbtfelectron70 = addVBTFSelection('simpleEleId70relIso')
vbtfelectron80 = addVBTFSelection('simpleEleId80relIso')
vbtfelectron90 = addVBTFSelection('simpleEleId90relIso')
vbtfelectron95 = addVBTFSelection('simpleEleId95relIso')
