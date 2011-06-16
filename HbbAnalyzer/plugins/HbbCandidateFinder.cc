#include "VHbbAnalysis/HbbAnalyzer/interface/HbbCandidateFinder.h"

struct CompareJetPt {
  bool operator()( const VHbbEvent::SimpleJet& j1, const  VHbbEvent::SimpleJet& j2 ) const {
    return j1.fourMomentum.Pt() > j2.fourMomentum.Pt();
  }
};

  struct CompareBTag {
    bool operator()(const  VHbbEvent::SimpleJet& j1, const  VHbbEvent::SimpleJet& j2 ) const {
      return j1.csv > j2.csv;
    }
  };

HbbCandidateFinder::HbbCandidateFinder(const edm::ParameterSet& iConfig):   vhbbevent_(iConfig.getParameter<edm::InputTag>("VHbbEventLabel")), verbose_ (iConfig.getParameter<bool>("verbose")), jetPtThreshold(iConfig.getParameter<double>("jetPtThreshold")){
  produces<std::vector<VHbbCandidate > >();
}

HbbCandidateFinder::~HbbCandidateFinder(){}

void HbbCandidateFinder::beginJob(){}
void HbbCandidateFinder::endJob(){}


float HbbCandidateFinder::getDeltaTheta( VHbbEvent::SimpleJet * j1, VHbbEvent::SimpleJet * j2 ){return -1.;}



void HbbCandidateFinder::produce( edm::Event& iEvent, const edm::EventSetup& iEventSetup){
  
  std::auto_ptr<std::vector<VHbbCandidate> >  vHbbCandidates( new std::vector<VHbbCandidate>  );

  edm::Handle<VHbbEvent>  vHbbEvent;
  //  iEvent.getByLabel(vhbbevent_, vHbbEvent);
  iEvent.getByType(vHbbEvent);
  

  //
  // start searching for candidates
  //

  //  hbbCandidateFinderAlgo(vHbbCandidates, vHbbEvent-> result());
  // do nothing for a test
  
  run(vHbbEvent.product(),vHbbCandidates);


  if (verbose_)
    std::cout <<" Pushing VHbb candidates: "<<vHbbCandidates->size()<<std::endl;

  iEvent.put(vHbbCandidates);  

}

void HbbCandidateFinder::run (const VHbbEvent* event, std::auto_ptr<std::vector<VHbbCandidate> > & candidates){
  //
  // first find the jets
  //

  VHbbEvent::SimpleJet j1,j2;
  std::vector<VHbbEvent::SimpleJet> addJets;
  bool foundJets = findDiJets(event->simpleJets2,j1,j2,addJets) ;

  if (verbose_){
    std::cout <<" Found Dijets: "<<foundJets<< " Additional: "<<addJets.size()<< std::endl;
  }
  
  if (foundJets == false) return;
  //
  // search for a dilepton - just 
  //
  std::vector<VHbbEvent::MuonInfo> mu;
  findMuons(event->muInfo,mu);
  std::vector<VHbbEvent::ElectronInfo> ele;
  findElectrons(event->eleInfo,ele);
  
  if (verbose_){
    std::cout <<" Electrons: "<< ele.size()<<std::endl;
    std::cout <<" Muons    : "<< mu.size()<<std::endl;
  }
  if (ele.size()<1 && mu.size() < 1 ) return;

  //
  // fill!
  //
  VHbbCandidate tempMu;
  VHbbCandidate tempE;
  tempMu.H.jets.push_back(j1);
  tempMu.H.jets.push_back(j2);
  tempMu.H.fourMomentum = (j1).fourMomentum+(j2).fourMomentum;
  tempMu.additionalJets = addJets;
  tempE = tempMu;
  TLorentzVector pMu,pE;

  if (mu.size()){
    for (std::vector<VHbbEvent::MuonInfo>::iterator it = mu.begin(); it !=mu.end(); ++it){
      tempMu.V.muons.push_back(*it);
   }
  }
  if (ele.size()){
    for (std::vector<VHbbEvent::ElectronInfo>::iterator it = ele.begin(); it !=ele.end(); ++it){
      tempE.V.electrons.push_back(*it);
   }
  }

  if (tempMu.V.muons.size()){
   candidates->push_back(tempMu);
  }
  if (tempE.V.electrons.size()){
   candidates->push_back(tempE);
  }
}

bool HbbCandidateFinder::findDiJets (const std::vector<VHbbEvent::SimpleJet>& jets, VHbbEvent::SimpleJet& j1, VHbbEvent::SimpleJet& j2,std::vector<VHbbEvent::SimpleJet>& addJets){
  
 std::vector<VHbbEvent::SimpleJet> tempJets;
 
 for (unsigned int i=0 ; i< jets.size(); ++i){
   if (jets[i].fourMomentum.Pt()> jetPtThreshold)
     tempJets.push_back(jets[i]);
 }
 
 CompareBTag  bTagComparator;

 if (tempJets.size()<2) return false;
 
 std::sort(tempJets.begin(), tempJets.end(), bTagComparator);
 
 j1 = tempJets[0];
 j2 = tempJets[1];
 //
 // additional jets
 //
 if (tempJets.size()>2){
   for (unsigned int i=2 ; i< tempJets.size(); ++i){
     addJets.push_back(tempJets[i]);
   }
 }
  CompareJetPt ptComparator;

  std::sort(addJets.begin(), addJets.end(), ptComparator);
 return true;
}

void HbbCandidateFinder::findMuons(const std::vector<VHbbEvent::MuonInfo>& muons, std::vector<VHbbEvent::MuonInfo>& out){
  /* Use:
For both W -> mu nu and Z -> mu mu, we adopt the standard VBTF muon selection described in VbtfWmunuBaselineSelection. The explicit cuts are reproduced here:

    We use RECO Muons that are both Global and Tracker
    chi2/ndof < 10 for the global muon fit
    The track associated to the muon must have
        >= 1 pixel hits
        >= 10 pixel + strip hits
        >= 1 valid hit in the muon chambers
        >= 2 muon stations
        |dxy| < 0.2
        |eta| < 2.4 
    Relative combined isolation (R) is required to be < 0.15
        R = [Sum pT(trks) + Et(ECAL) + Et(HCAL)] / pT(mu) computed in a cone of radius 0.3 in eta-phi 
    pT(mu) > 20 GeV 
  */
  for (std::vector<VHbbEvent::MuonInfo>::const_iterator it = muons.begin(); it!= muons.end(); ++it){
    if (
	(*it).nPixelHits>= 1 &&
	(*it).nHits +(*it).nPixelHits >= 10 &&
	//
	// sta muon not saved????
	(*it).ipDb<.2 &&
	((*it).hIso+(*it).eIso+(*it).tIso)/(*it).fourMomentum.Pt()<.15 &&
	(*it).fourMomentum.Eta()<2.4 &&
	(*it).fourMomentum.Pt()>20) {
      out.push_back(*it);
    }
  }
}


void HbbCandidateFinder::findElectrons(const std::vector<VHbbEvent::ElectronInfo>& electrons, std::vector<VHbbEvent::ElectronInfo>& out){
  /*
We adopt the standard cut-based selection from VBTF described in detail here.

    Z -> ee
        gsf electrons
        VBTF WP95
        |eta|<2.5, excluding the gap 1.44 < |eta| < 1.57
        pT(e) > 20 

    W -> e nu
        gsf electrons
        VBTF WP80
        |eta|<2.5, excluding the gap 1.44 < |eta| < 1.57
        pT(e) > 30 
  */

  for (std::vector<VHbbEvent::ElectronInfo>::const_iterator it = electrons.begin(); it!= electrons.end(); ++it){
    if (
	// fake
	//	(*it).id95>  &&
	std::abs((*it).fourMomentum.Eta()) < 2.5 &&
	!( 	std::abs((*it).fourMomentum.Eta()) < 1.57 &&	std::abs((*it).fourMomentum.Eta()) > 1.44) &&
	(*it).fourMomentum.Pt()>30
	){
      out.push_back(*it);
    }  
  }
}


//define this as a plug-in
DEFINE_FWK_MODULE(HbbCandidateFinder);
