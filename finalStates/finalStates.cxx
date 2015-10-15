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
    int gammaIndex = -1; 

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
		if (pdg == 22){
			gammaIndex = pdgFound.size() - 1; 
		}
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
	
    // Check if we have at least 2 photon tracks
    // Make sure they don't come from the primary vertex
    // Find pairs of photons that come from the same start position.
    if (gammaIndex != -1){
    	if ((pdgCount[gammaIndex]-numPrimary[gammaIndex]) >= 2){
    		for(auto const& mcs : *ev_mcs){
			TVector3 startPoint(mcs.Start().X(), mcs.Start().Y(), mcs.Start().Z());
			TVector3 diff = origin-startPoint;
			double dist = diff.Mag();

			TVector3 mom(mcs.Start().Px(), mcs.Start().Py(), mcs.Start().Pz());

			if (dist > tol){
				photonStart.push_back(startPoint);
				photonMom.push_back(mom);
			}
		}
		for(unsigned int i = 0; i < photonStart.size(); i++){
			for(unsigned int j = 0; j < photonStart.size(); j++){
				if (i > j){
					TVector3 diff = photonStart[i]-photonStart[j];
					double dist = diff.Mag();
					if (dist <= tol){
						std::vector<unsigned int> thisPair;
						thisPair.push_back(i);
						thisPair.push_back(j);	
						pairIndices.push_back(thisPair);
					}
				}
			}
		}
		// Up until now, this has been general, but when running it over the nnbar
		// file Jeremy gave us there was one event with 4 gammas which all came
		// from the same start point and the rest were all gamma pairs
		// To save time, the remaining analysis only looks at events with single numbers
		// of gamma pairs.
		// For other files, may need to look more closely at > 2 gammas

				
    		// Try and project back the pairs to the origin and infer the production of a pi0 from this
		if (pairIndices.size() == 1){ // This is where we are becoming specific
			// Calculate the vector sum of the pair's momentum and use this as a direction to project back to the origin
			TVector3 P = photonMom[pairIndices[0][0]] + photonMom[pairIndices[0][1]];
			TVector3 Phat = pow(P.Mag(),-1)*P;

			// Assume we have some percentage error on the total momentum, pErr.
			// This could point in any direction so choose it to be 90 deg to the direction of P as 
			// a worst case scenario. 
			double pErr = P.Mag()*0.5; // 40% 

			// Now use the direction of P to project a cone (with an opening angle defined by the pErr) and
			// see if the origin lies within the volume of the cone.
			
			// After a lot of geometry, we derived a condition for the origin to be in this cone. (We are also re-defining
			// some things so it's consistent with the notation we used to derive the condition)
			TVector3 Q = photonStart[pairIndices[0][0]];
			TVector3 C = origin;
			TVector3 QmC = Q - C;
			TVector3 S = QmC - ( QmC.Dot(Phat) )*Phat;
			
			if (asin(S.Mag()/QmC.Mag()) <= atan(pErr/P.Mag())){
				std::cout << "THEY PROJECT TO THE ORIGIN!" << std::endl;
			}
			else{
				std::cout << "NO PROJECTION TO ORIGIN" << std::endl;
			}
			std::cin.get();
				
		}
    	}
    }

    pdgFound.clear();
    pdgCount.clear();
    numPrimary.clear();
    photonStart.clear();
    pairIndices.clear();
    photonMom.clear();
    

    return true;
  }

  bool finalStates::finalize() {
    return true;
  }

}
#endif
