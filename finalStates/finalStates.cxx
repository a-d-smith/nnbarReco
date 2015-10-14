#ifndef LARLITE_VERTEXFINDER_CXX
#define LARLITE_VERTEXFINDER_CXX

#include "finalStates.h"
#include "TVector3.h"
#include <vector>
#include <fstream>

namespace larlite {

  bool finalStates::initialize() {
    return true;
  }
  
  bool finalStates::analyze(storage_manager* storage) {
  
    evIndex++;   

    // Load in the shower and track data
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");

    // Ensure these pointers are valid
    if(!ev_mcs){
      std::cout << "MCShower pointer invalid! Exiting..." << std::endl;
      exit(1);
    }
    else if(!ev_mct){
      std::cout << "MCTrack pointer invalid! Exiting..." << std::endl;
      exit(1);
    }

    std::cout << std::endl;
    std::cout << "--------------- " << "Event " << std::setw(3) << evIndex << " ---------------" << std::endl;
    std::cout << "- " << "Tracks --" << "------------------------------" << std::endl;

    // We loop over all of the tracks and showers and count the distibution of particles within each event.
    // Jeremy produced these nnbar events to all happen at the centre of the detector, this appears to be (128, 0, 518).
    // We measure the distance between the start points of the tracks and showers and this origin to determine those which
    // were involved in the primary interaction (to a given tolerance).
    
    TVector3 origin(128, 0, 518);
    double tol = 1;
    // Loop over the tracks
    for(auto const& mct : *ev_mct){
	pdg = mct.PdgCode();
	TVector3 startPoint(mct.Start().X(), mct.Start().Y(), mct.Start().Z());
	TVector3 diff = origin-startPoint;
	double dist = diff.Mag();

	found = false;
	for (unsigned int i=0; i<pdgFound.size(); i++){
		if (pdg == pdgFound[i]) {
			found = true;
			pdgCount[i]++;
			if (dist <= tol){
				numPrimary[i]++;
			}
			break;
		}
	}
	if (!found){
		pdgFound.push_back(pdg);
		pdgCount.push_back(1);
		if (dist <= tol){
			numPrimary.push_back(1);
		}
		else{
			numPrimary.push_back(0);
		}
	}
    }
    std::cout << "---------- " << "  PDG" << "  :  " << "COUNT      " << "---------" << std::endl;
    for (unsigned int i=0; i<pdgFound.size(); i++){
	    std::cout << "---------- " << std::setw(5) << pdgFound[i] << "  :  " << std::setw(3) << numPrimary[i] << "/" << std::setw(3) << pdgCount[i] << "    ---------" << std::endl;
    }
    pdgFound.clear();
    pdgCount.clear();
    numPrimary.clear();

    std::cout << "- " << "Showers -" << "------------------------------" << std::endl;

    // Loop over the showers
    for(auto const& mcs : *ev_mcs){
	TVector3 startPoint(mcs.Start().X(), mcs.Start().Y(), mcs.Start().Z());
	TVector3 diff = origin-startPoint;
	double dist = diff.Mag();
	pdg = mcs.PdgCode();
	found = false;
	for (unsigned int i=0; i<pdgFound.size(); i++){
		if (pdg == pdgFound[i]) {
			found = true;
			pdgCount[i]++;
			if (dist <= tol){
				numPrimary[i]++;
			}
			break;
		}
	}
	if (!found){
		pdgFound.push_back(pdg);
		pdgCount.push_back(1);
		if (dist <= tol){
			numPrimary.push_back(1);
		}
		else{
			numPrimary.push_back(0);
		}
	}
    }

    std::cout << "---------- " << "  PDG" << "  :  " << "COUNT     " << "---------" << std::endl;
    for (unsigned int i=0; i<pdgFound.size(); i++){
	    std::cout << "---------- " << std::setw(5) << pdgFound[i] << "  :  " << std::setw(3) << numPrimary[i] << "/" << std::setw(3) << pdgCount[i] << "    ---------" << std::endl;
    }
    pdgFound.clear();
    pdgCount.clear();
    numPrimary.clear();

    return true;
  }

  bool finalStates::finalize() {
    return true;
  }

}
#endif
