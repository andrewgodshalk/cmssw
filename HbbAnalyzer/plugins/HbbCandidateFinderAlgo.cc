#include "VHbbAnalysis/HbbAnalyzer/interface/HbbCandidateFinderAlgo.h"
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidateTools.h"

#include <iostream>
#include<cstdlib>

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




void HbbCandidateFinderAlgo::run (const VHbbEvent* event, std::vector<VHbbCandidate>  & candidates){
  //
  // first find the jets
  //

  VHbbCandidateTools selector(verbose_);

  VHbbEvent::SimpleJet j1,j2;
  std::vector<VHbbEvent::SimpleJet> addJets;
  bool foundJets = findDiJets(event->simpleJets2,j1,j2,addJets) ;

  if (verbose_){
    std::cout <<" Found Dijets: "<<foundJets<< " Additional: "<<addJets.size()<< std::endl;
  }
  
  if (foundJets == false) return;
  //
  // search for leptons
  //
  std::vector<VHbbEvent::MuonInfo> mu;
  findMuons(event->muInfo,mu);
  std::vector<VHbbEvent::ElectronInfo> ele;
 findElectrons(event->eleInfo,ele);
  
  std::vector<VHbbEvent::METInfo> met;
  findMET(event->pfmet, met);

  if (verbose_){
    std::cout <<" Electrons: "<< ele.size()<<std::endl;
    std::cout <<" Muons    : "<< mu.size()<<std::endl;
    std::cout <<" MET      : "<< met.size()<<std::endl;
  }
  if (ele.size()<1 && mu.size() < 1 && met.size()<1) return;

  //
  // fill!
  //
  VHbbCandidate temp;
  temp.H.jets.push_back(j1);
  temp.H.jets.push_back(j2);
  temp.H.fourMomentum = (j1).fourMomentum+(j2).fourMomentum;
  TVector3 higgsBoost;
  higgsBoost = (temp.H.fourMomentum).BoostVector();
  temp.H.helicities.push_back(selector.getHelicity(j1,higgsBoost));
  temp.H.helicities.push_back(selector.getHelicity(j2,higgsBoost));
  temp.H.deltaTheta = selector.getDeltaTheta(j1,j2);
  temp.additionalJets = addJets;
  temp.V.mets = met;
  temp.V.muons = mu;
  temp.V.electrons = ele;
  
  //
  // now see which kind of andidate this can be
  // 

  VHbbCandidate result;
  bool ok = false;
  //
  // first: hZmumu
  //
  
  if (verbose_){
    std::cout <<" START SELECTION "<<std::endl;
  }
  
  result = selector.getHZmumuCandidate(temp,ok);
  if ( ok == true ){
    result.setCandidateType(VHbbCandidate::Zmumu);
    candidates.push_back(result);
  }else{
    //      HZee
    result = selector. getHZeeCandidate(temp,ok);
    if ( ok == true ){
      result.setCandidateType(VHbbCandidate::Zee);
      candidates.push_back(result);
      return;
    }else{
      //HWmunu
      result = selector. getHWmunCandidate(temp,ok);
      if ( ok == true ){
	result.setCandidateType(VHbbCandidate::Wmun);
	candidates.push_back(result);
	return;
      }else{
	// HWenu
	result = selector. getHWenCandidate(temp,ok);
	if ( ok == true ){
	  result.setCandidateType(VHbbCandidate::Wen);
	  candidates.push_back(result);
	  return;
	}else{
	  // HZnn
	  result = selector. getHZnnCandidate(temp,ok);
	  if ( ok == true ){
	    result.setCandidateType(VHbbCandidate::Znn);
	    candidates.push_back(result);
	    return;
	  }
	}
      }
    }
  }
  return;
}

void HbbCandidateFinderAlgo::findMET(const VHbbEvent::METInfo & met, std::vector<VHbbEvent::METInfo>& out){
  //

  //  just preselection: met significance > 2 

    if (met.metSig >2 ) out.push_back(met);
    if (verbose_){
      std::cout <<" CandidateFinder: Input MET = "<<met.metSig<<" Output MET = "<<out.size()<<std::endl;
    }
    
}


bool HbbCandidateFinderAlgo::findDiJets (const std::vector<VHbbEvent::SimpleJet>& jets, VHbbEvent::SimpleJet& j1, VHbbEvent::SimpleJet& j2,std::vector<VHbbEvent::SimpleJet>& addJets){
  
 std::vector<VHbbEvent::SimpleJet> tempJets;

 if (verbose_){
   std::cout <<" CandidateFinder: Input Jets = "<<jets.size()<<std::endl;
 }

 for (unsigned int i=0 ; i< jets.size(); ++i){
   if (jets[i].fourMomentum.Pt()> jetPtThreshold)
     tempJets.push_back(jets[i]);
 }
 
 CompareBTag  bTagComparator;
 CompareJetPt ptComparator;
 
 if (verbose_){
   std::cout <<" CandidateFinder: Intermediate Jets = "<<tempJets.size()<<std::endl;
 }


 if (tempJets.size()<2) return false;
 
 std::sort(tempJets.begin(), tempJets.end(), bTagComparator);
 
if (tempJets[0].fourMomentum.Pt()>(tempJets[1].fourMomentum.Pt())){
   j1 = tempJets[0];
   j2 = tempJets[1];
 }else{
   j2 = tempJets[0];
   j1 = tempJets[1]; 
 }

//
 // additional jets
 //
 if (tempJets.size()>2){
   for (unsigned int i=2 ; i< tempJets.size(); ++i){
     addJets.push_back(tempJets[i]);
   }
 }

 if (verbose_){
   std::cout <<" CandidateFinder: Output Jets = "<<2<<" Additional = "<<addJets.size()<<std::endl;
 }


  std::sort(addJets.begin(), addJets.end(), ptComparator);
 return true;


}

void HbbCandidateFinderAlgo::findMuons(const std::vector<VHbbEvent::MuonInfo>& muons, std::vector<VHbbEvent::MuonInfo>& out){
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
	(*it). globChi2<10 &&
	(*it).nPixelHits>= 1 &&
	(*it).globNHits >= 1 &&
	(*it).nHits >= 10 &&
	(*it).cat ==1 && 
	(*it).validMuStations >=2 &&
	(*it).ipDb<.2 &&
	((*it).hIso+(*it).eIso+(*it).tIso)/(*it).fourMomentum.Pt()<.15 &&
	(*it).fourMomentum.Eta()<2.4 &&
	(*it).fourMomentum.Pt()>15) {
      out.push_back(*it);
    }
  }

    if (verbose_){
      std::cout <<" CandidateFinder: Input Muons = "<<muons.size()<<" Output Muons = "<<out.size()<<std::endl;
    }



}


void HbbCandidateFinderAlgo::findElectrons(const std::vector<VHbbEvent::ElectronInfo>& electrons, std::vector<VHbbEvent::ElectronInfo>& out){
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
	fabs((*it).fourMomentum.Eta()) < 2.5 &&
	!( 	fabs((*it).fourMomentum.Eta()) < 1.57 &&	fabs((*it).fourMomentum.Eta()) > 1.44) &&
	(*it).fourMomentum.Pt()>15 //  I use the minimum ok for both Z and W
	){
      out.push_back(*it);
    }  
  }
    if (verbose_){
      std::cout <<" CandidateFinder: Input Electrons = "<<electrons.size()<<" Output Electrons = "<<out.size()<<std::endl;
    }


}

