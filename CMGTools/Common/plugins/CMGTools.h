#ifndef __CMGTools_Common_CMGTools_h__
#define __CMGTools_Common_CMGTools_h__

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/Common/interface/GenericHistograms.h"
#include "CMGTools/Common/interface/JetHistograms.h"
#include "CMGTools/Common/interface/LorentzHistograms.h"
#include "CMGTools/Common/interface/CompoundFactories.h"
#include "CMGTools/Common/interface/GenericFactories.h"
#include "CMGTools/Common/interface/ElectronFactory.h"
#include "CMGTools/Common/interface/MuonFactory.h"
#include "CMGTools/Common/interface/BaseJetFactory.h"
#include "CMGTools/Common/interface/PFJetFactory.h"
#include "CMGTools/Common/interface/BaseMETFactory.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

//COLIN why not in plugins? 
#include "CMGTools/Common/interface/CutSummaryAnalyzer.h"
#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"
#include "CMGTools/Common/plugins/LeadingObjectsSelector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

//colin: should add cmg to the type names where applicable

typedef PhysicsObjectProducer<cmg::DiElectronFactory> DiElectronPOProducer;
typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
typedef PhysicsObjectProducer<cmg::DiJetFactory> DiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiPFJetFactory> DiPFJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiTauFactory> DiTauPOProducer;
typedef PhysicsObjectProducer<cmg::ElectronFactory> ElectronPOProducer;
typedef PhysicsObjectProducer<cmg::MuonFactory> MuonPOProducer;
typedef PhysicsObjectProducer<cmg::TauFactory> TauPOProducer;
typedef PhysicsObjectProducer<cmg::QuadMuonFactory> QuadMuonPOProducer;
typedef PhysicsObjectProducer<cmg::BaseJetFactory> BaseJetPOProducer;
typedef PhysicsObjectProducer<cmg::PFJetFactory> PFJetPOProducer;
typedef PhysicsObjectProducer<cmg::BaseMETFactory> BaseMETPOProducer;
typedef PhysicsObjectProducer<cmg::PFCandFactory> PFCandidatePOProducer;
typedef PhysicsObjectProducer<cmg::WENuFactory> WENuPOProducer;
typedef PhysicsObjectProducer<cmg::WMuNuFactory> WMuNuPOProducer;
typedef PhysicsObjectProducer<cmg::WTauNuFactory> WTauNuPOProducer;

typedef cmg::HistogramAnalyzer<cmg::LorentzHistogramsFF> LorentzHistogramsAnalyzer;
typedef cmg::HistogramAnalyzer<cmg::JetHistograms> PFJetHistogramsAnalyzer;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::Muon> > GenericMuonHistograms;
typedef cmg::HistogramAnalyzer<cmg::GenericHistograms<cmg::BaseMET> > GenericBaseMETHistograms;
typedef cmg::RunInfoAccountingAnalyzer RunInfoAccountingAnalyzer;
typedef cmg::CutSummaryAnalyzer CutSummaryAnalyzer;


// leading objects selectors 

typedef cmg::LeadingObjectsSelector<cmg::AbstractPhysicsObject> LeadingCMGPOSelector;
typedef cmg::LeadingObjectsSelector<cmg::BaseJet> LeadingCMGBaseJetSelector;
typedef cmg::LeadingObjectsSelector<cmg::PFJet> LeadingCMGPFJetSelector;

// single object selectors

typedef SingleObjectSelector<
  std::vector<cmg::PFJet>,
  StringCutObjectSelector<cmg::PFJet>
  > CmgPFJetSelector;

typedef SingleObjectSelector<
  std::vector<cmg::BaseJet>,
  StringCutObjectSelector<cmg::BaseJet>
  > CmgBaseJetSelector;


#endif
