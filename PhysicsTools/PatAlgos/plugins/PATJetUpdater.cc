//


#include "PhysicsTools/PatAlgos/plugins/PATJetUpdater.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"

#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "FWCore/Utilities/interface/transform.h"

///////////////////////////////
//TEMP small hack
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/BTauReco/interface/CandIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputerRecord.h"
///////////////////////////////

#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

using namespace pat;


PATJetUpdater::PATJetUpdater(const edm::ParameterSet& iConfig) :
  useUserData_(iConfig.exists("userData")),
  printWarning_(true)
{
  // initialize configurables
  jetsToken_ = consumes<edm::View<reco::Jet> >(iConfig.getParameter<edm::InputTag>( "jetSource" ));
  addJetCorrFactors_ = iConfig.getParameter<bool>( "addJetCorrFactors" );
  if( addJetCorrFactors_ ) {
    jetCorrFactorsTokens_ = edm::vector_transform(iConfig.getParameter<std::vector<edm::InputTag> >( "jetCorrFactorsSource" ), [this](edm::InputTag const & tag){return mayConsume<edm::ValueMap<JetCorrFactors> >(tag);});
  }
  addBTagInfo_ = iConfig.getParameter<bool>( "addBTagInfo" ); 
  addDiscriminators_ = iConfig.getParameter<bool>( "addDiscriminators" );
  discriminatorTags_ = iConfig.getParameter<std::vector<edm::InputTag> >( "discriminatorSources" );
  discriminatorTokens_ = edm::vector_transform(discriminatorTags_, [this](edm::InputTag const & tag){return mayConsume<reco::JetFloatAssociation::Container>(tag);});
  addTagInfos_ = iConfig.getParameter<bool>( "addTagInfos" );
  tagInfoTags_ = iConfig.getParameter<std::vector<edm::InputTag> >( "tagInfoSources" );
  tagInfoTokens_ =edm::vector_transform(tagInfoTags_, [this](edm::InputTag const & tag){return mayConsume<edm::View<reco::BaseTagInfo> >(tag);});
  if (discriminatorTags_.empty()) {
    addDiscriminators_ = false;
  } else {
    for (std::vector<edm::InputTag>::const_iterator it = discriminatorTags_.begin(), ed = discriminatorTags_.end(); it != ed; ++it) {
        std::string label = it->label();
        std::string::size_type pos = label.find("JetTags");
        if ((pos !=  std::string::npos) && (pos != label.length() - 7)) {
            label.erase(pos+7); // trim a tail after "JetTags"
        }
				if(it->instance().size()) {
					std::stringstream name;
					name << label << ":" << it->instance();
					label = name.str();
				}
        discriminatorLabels_.push_back(label);
    }
  }
  if (tagInfoTags_.empty()) {
    addTagInfos_ = false;
  } else {
    for (std::vector<edm::InputTag>::const_iterator it = tagInfoTags_.begin(), ed = tagInfoTags_.end(); it != ed; ++it) {
        std::string label = it->label();
        std::string::size_type pos = label.find("TagInfos");
        if ((pos !=  std::string::npos) && (pos != label.length() - 8)) {
            label.erase(pos+8); // trim a tail after "TagInfos"
        }
        tagInfoLabels_.push_back(label);
    }
  }
  if (!addBTagInfo_) { addDiscriminators_ = false; addTagInfos_ = false; }
  // Check to see if the user wants to add user data
  if ( useUserData_ ) {
    userDataHelper_ = PATUserDataHelper<Jet>(iConfig.getParameter<edm::ParameterSet>("userData"), consumesCollector());
  }
  
  /////////////////////////////////////// 
  //TEMP small hack 
  addSecondaryVertexInfo_ = iConfig.getParameter<bool>( "addSecondaryVertexInfo" );
  svTagInfos_               = iConfig.getParameter<std::string>("svTagInfos");
  ipTagInfos_               = iConfig.getParameter<std::string>("ipTagInfos");  
  svComputer_               = iConfig.getParameter<std::string>("svComputer");
  computer = 0 ;  

  std::cout << "\n================================================" ;
  std::cout << "\n Setting for adding vertex information" ;
  std::cout << "\n addSecondaryVertexInfo: " << addSecondaryVertexInfo_ ;
  std::cout << "\n svTagInfos: " << svTagInfos_ ;
  std::cout << "\n ipTagInfos: " << ipTagInfos_ ;
  std::cout << "\n svComputer: " << svComputer_ << "\n";
  ////////////////////////////////////////

  // produces vector of jets
  produces<std::vector<Jet> >();
  produces<edm::OwnVector<reco::BaseTagInfo> > ("tagInfos");
}


PATJetUpdater::~PATJetUpdater() {

}


void PATJetUpdater::produce(edm::Event & iEvent, const edm::EventSetup & iSetup)
{
  // Get the vector of jets
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByToken(jetsToken_, jets);

  // read in the jet correction factors ValueMap
  std::vector<edm::ValueMap<JetCorrFactors> > jetCorrs;
  if (addJetCorrFactors_) {
    for ( size_t i = 0; i < jetCorrFactorsTokens_.size(); ++i ) {
      edm::Handle<edm::ValueMap<JetCorrFactors> > jetCorr;
      iEvent.getByToken(jetCorrFactorsTokens_[i], jetCorr);
      jetCorrs.push_back( *jetCorr );
    }
  }

  // Get the vector of jet tags with b-tagging info
  std::vector<edm::Handle<reco::JetFloatAssociation::Container> > jetDiscriminators;
  if (addBTagInfo_ && addDiscriminators_) {
    jetDiscriminators.resize(discriminatorTokens_.size());
    for (size_t i = 0; i < discriminatorTokens_.size(); ++i) {
        iEvent.getByToken(discriminatorTokens_[i], jetDiscriminators[i]);
    }
  }
  std::vector<edm::Handle<edm::View<reco::BaseTagInfo> > > jetTagInfos;
  if (addBTagInfo_ && addTagInfos_) {
    jetTagInfos.resize(tagInfoTokens_.size());
    for (size_t i = 0; i < tagInfoTokens_.size(); ++i) {
      iEvent.getByToken(tagInfoTokens_[i], jetTagInfos[i]);
    }
  }

  //////////////////////////////////////////////
  //TEMP small hack
  edm::ESHandle<JetTagComputer> computerHandle;
  iSetup.get<JetTagComputerRecord>().get(svComputer_.c_str(), computerHandle );
  computer = dynamic_cast<const GenericMVAJetTagComputer*>( computerHandle.product() );
  ///////////////////////////////////////////////
  
  ////////////////////////////////////////////////
  // loop over jets
  std::auto_ptr< std::vector<Jet> > patJets ( new std::vector<Jet>() );

  std::auto_ptr<edm::OwnVector<reco::BaseTagInfo> > tagInfosOut ( new edm::OwnVector<reco::BaseTagInfo>() );

  edm::RefProd<edm::OwnVector<reco::BaseTagInfo> > h_tagInfosOut = iEvent.getRefBeforePut<edm::OwnVector<reco::BaseTagInfo> > ( "tagInfos" );

  for (edm::View<reco::Jet>::const_iterator itJet = jets->begin(); itJet != jets->end(); itJet++) {

    // construct the Jet from the ref -> save ref to original object
    unsigned int idx = itJet - jets->begin();
    const edm::RefToBase<reco::Jet> jetRef = jets->refAt(idx);
    const edm::RefToBase<Jet> patJetRef(jetRef.castTo<JetRef>());
    Jet ajet( patJetRef );

    if (addJetCorrFactors_) {
      // undo previous jet energy corrections
      ajet.setP4(ajet.correctedP4(0));
      // clear previous JetCorrFactors
      ajet.jec_.clear();
      // add additional JetCorrs to the jet
      for ( unsigned int i=0; i<jetCorrFactorsTokens_.size(); ++i ) {
	const JetCorrFactors& jcf = jetCorrs[i][jetRef];
	// uncomment for debugging
	// jcf.print();
	ajet.addJECFactors(jcf);
      }
      std::vector<std::string> levels = jetCorrs[0][jetRef].correctionLabels();
      if(std::find(levels.begin(), levels.end(), "L2L3Residual")!=levels.end()){
	ajet.initializeJEC(jetCorrs[0][jetRef].jecLevel("L2L3Residual"));
      }
      else if(std::find(levels.begin(), levels.end(), "L3Absolute")!=levels.end()){
	ajet.initializeJEC(jetCorrs[0][jetRef].jecLevel("L3Absolute"));
      }
      else{
	ajet.initializeJEC(jetCorrs[0][jetRef].jecLevel("Uncorrected"));
	if(printWarning_){
	  edm::LogWarning("L3Absolute not found") << "L2L3Residual and L3Absolute are not part of the jetCorrFactors\n"
						  << "of module " <<  jetCorrs[0][jetRef].jecSet() << ". Jets will remain"
						  << " uncorrected."; printWarning_=false;
	}
      }
    }

    // add b-tag info if available & required
    if (addBTagInfo_) {
        if (addDiscriminators_) {
            for (size_t k=0; k<jetDiscriminators.size(); ++k) {
                float value = (*jetDiscriminators[k])[jetRef];
                ajet.addBDiscriminatorPair(std::make_pair(discriminatorLabels_[k], value));
            }
        }
        if (addTagInfos_) {
	  for (size_t k=0; k<jetTagInfos.size(); ++k) {
	    const edm::View<reco::BaseTagInfo> & taginfos = *jetTagInfos[k];
	    // This is not associative, so we have to search the jet
	    edm::Ptr<reco::BaseTagInfo> match;
	    // Try first by 'same index'
	    if ((idx < taginfos.size()) && (taginfos[idx].jet() == jetRef)) {
	      match = taginfos.ptrAt(idx);
	    } else {
	      // otherwise fail back to a simple search
	      for (edm::View<reco::BaseTagInfo>::const_iterator itTI = taginfos.begin(), edTI = taginfos.end(); itTI != edTI; ++itTI) {
		if (itTI->jet() == jetRef) { match = taginfos.ptrAt( itTI - taginfos.begin() ); break; }
	      }
	    }
	    if (match.isNonnull()) {
	      tagInfosOut->push_back( match->clone() );
	      // set the "forward" ptr to the thinned collection
	      edm::Ptr<reco::BaseTagInfo> tagInfoForwardPtr ( h_tagInfosOut.id(), &tagInfosOut->back(), tagInfosOut->size() - 1 );
	      // set the "backward" ptr to the original collection for association
	      edm::Ptr<reco::BaseTagInfo> tagInfoBackPtr ( match );
	      // make FwdPtr
	      TagInfoFwdPtrCollection::value_type tagInfoFwdPtr( tagInfoForwardPtr, tagInfoBackPtr ) ;
	      ajet.addTagInfo(tagInfoLabels_[k], tagInfoFwdPtr );
	    }
	  }
 
        } //if (addTagInfos_)
    } //if (addBTagInfo_)

    ////////////////////////////////////////////////////
    //TEMP small hack 
    int vtxCat(-10) ;
    int nVtx(-10) ;
    float vtxMass(-10) ;
    int vtxNTracks(-10) ;
    float vtxEnergyRatio(-10) ;
    float vtxJetDeltaR(-10) ;
    float flightDistance2dVal(-10) ;
    float flightDistance2dSig(-10) ;
    float flightDistance3dVal(-10) ;
    float flightDistance3dSig(-10) ;
    float trackJetPt(-10) ;
    if (addSecondaryVertexInfo_) {
      const reco::TemplatedSecondaryVertexTagInfo<reco::CandIPTagInfo,reco::VertexCompositePtrCandidate> *svTagInfo = ajet.tagInfoCandSecondaryVertex(svTagInfos_.c_str()); 
      const reco::CandIPTagInfo *ipTagInfo = ajet.tagInfoCandIP(ipTagInfos_.c_str());
      std::vector<const reco::BaseTagInfo*>  baseTagInfos;
      JetTagComputer::TagInfoHelper helper(baseTagInfos);
      baseTagInfos.push_back( ipTagInfo );
      baseTagInfos.push_back( svTagInfo );
      
      // TaggingVariables
      reco::TaggingVariableList vars = computer->taggingVariables(helper);

      vtxCat         = ( vars.checkTag(reco::btau::vertexCategory) ? vars.get(reco::btau::vertexCategory) : -10); //0: 0 has vertex, 1 pseudo vertex, 2 no reconstructed vertex
      nVtx  = ( vars.checkTag(reco::btau::jetNSecondaryVertices) ? vars.get(reco::btau::jetNSecondaryVertices) : -10 );
      vtxMass        = ( vars.checkTag(reco::btau::vertexMass) ? vars.get(reco::btau::vertexMass) : -10 );
      vtxNTracks     = ( vars.checkTag(reco::btau::vertexNTracks) ? vars.get(reco::btau::vertexNTracks) : -10 );
      vtxEnergyRatio = ( vars.checkTag(reco::btau::vertexEnergyRatio) ? vars.get(reco::btau::vertexEnergyRatio) : -10 );
      vtxJetDeltaR   = ( vars.checkTag(reco::btau::vertexJetDeltaR) ? vars.get(reco::btau::vertexJetDeltaR) : -10 );
      flightDistance2dVal         = ( vars.checkTag(reco::btau::flightDistance2dVal) ? vars.get(reco::btau::flightDistance2dVal) : -10 );
      flightDistance2dSig         = ( vars.checkTag(reco::btau::flightDistance2dSig) ? vars.get(reco::btau::flightDistance2dSig) : -10 );
      flightDistance3dVal         = ( vars.checkTag(reco::btau::flightDistance3dVal) ? vars.get(reco::btau::flightDistance3dVal) : -10 );
      flightDistance3dSig         = ( vars.checkTag(reco::btau::flightDistance3dSig) ? vars.get(reco::btau::flightDistance3dSig) : -10 );
      trackJetPt                  = ( vars.checkTag(reco::btau::trackJetPt) ? vars.get(reco::btau::trackJetPt) : -10 );
      //std::cout << "\n Vertex from CSVv2: " << vtxCat << "  " << nVtx << "  " << vtxMass << "  " << vtxNTracks << "  " << vtxEnergyRatio << "  " << vtxJetDeltaR ; 
    }
    //if (ajet.hasUserFloat("vtxMassCorr_IVF")) std::cout << "\n Before adding: " << ajet.userFloat("vtxMassCorr_IVF") ;
    ajet.addUserInt("vtxCat_IVF", vtxCat, true);
    ajet.addUserInt("nVtx_IVF", nVtx, true) ;
    ajet.addUserFloat("vtxMassCorr_IVF", vtxMass, true);
    ajet.addUserInt("vtxNTracks_IVF", vtxNTracks, true) ;
    ajet.addUserFloat("vtxEnergyRatio_IVF", vtxEnergyRatio, true) ;
    ajet.addUserFloat("vtxJetDeltaR_IVF", vtxJetDeltaR, true) ;
    ajet.addUserFloat("vtx2DVal_IVF", flightDistance2dVal, true) ;
    ajet.addUserFloat("vtx2DSig_IVF", flightDistance2dSig, true) ;
    ajet.addUserFloat("vtx3DVal_IVF", flightDistance3dVal, true) ;
    ajet.addUserFloat("vtx3DSig_IVF", flightDistance3dSig, true) ;
    ajet.addUserFloat("trackJetPt_IVF", trackJetPt, true) ;
    //std::cout << "\n After adding: " << ajet.userFloat("vtxMassCorr_IVF") ;
    ////////////////////////////////////////////////////////

    if ( useUserData_ ) {
      userDataHelper_.add( ajet, iEvent, iSetup );
    }

    // reassign the original object reference to preserve reference to the original jet the input PAT jet was derived from
    // (this needs to be done at the end since cloning the input PAT jet would interfere with adding UserData)
    ajet.refToOrig_ = patJetRef->originalObjectRef();

    patJets->push_back(ajet);
  }

  // sort jets in pt
  std::sort(patJets->begin(), patJets->end(), pTComparator_);

  // put genEvt  in Event
  iEvent.put(patJets);

  iEvent.put( tagInfosOut, "tagInfos" );

}

// ParameterSet description for module
void PATJetUpdater::fillDescriptions(edm::ConfigurationDescriptions & descriptions)
{
  edm::ParameterSetDescription iDesc;
  iDesc.setComment("PAT jet producer module");

  // input source
  iDesc.add<edm::InputTag>("jetSource", edm::InputTag("no default"))->setComment("input collection");

  // tag info
  iDesc.add<bool>("addTagInfos", true);
  std::vector<edm::InputTag> emptyVInputTags;
  iDesc.add<std::vector<edm::InputTag> >("tagInfoSources", emptyVInputTags);

  // jet energy corrections
  iDesc.add<bool>("addJetCorrFactors", true);
  iDesc.add<std::vector<edm::InputTag> >("jetCorrFactorsSource", emptyVInputTags);

  // btag discriminator tags
  iDesc.add<bool>("addBTagInfo",true);
  iDesc.add<bool>("addDiscriminators", true);
  iDesc.add<std::vector<edm::InputTag> >("discriminatorSources", emptyVInputTags);

  // Check to see if the user wants to add user data
  edm::ParameterSetDescription userDataPSet;
  PATUserDataHelper<Jet>::fillDescription(userDataPSet);
  iDesc.addOptional("userData", userDataPSet);
  
  ////////////////////////////////////////
  //TEMP small hacks
  iDesc.add<bool>("addSecondaryVertexInfo",true);
  iDesc.add<std::string>("svTagInfos","") ;
  iDesc.add<std::string>("ipTagInfos","") ;
  iDesc.add<std::string>("svComputer","") ;
  /////////////////////////////////////////
  
  descriptions.add("PATJetUpdater", iDesc);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATJetUpdater);
