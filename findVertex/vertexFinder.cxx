#ifndef LARLITE_VERTEXFINDER_CXX
#define LARLITE_VERTEXFINDER_CXX

#include "vertexFinder.h"
#include "TVector3.h"
#include <vector>
#include <fstream>

namespace larlite {

  bool vertexFinder::initialize() {
    return true;
  }
  
  bool vertexFinder::analyze(storage_manager* storage) {
  
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

    // Open the output file for writing
    std::stringstream strstrm;
    strstrm << "dat/trackAndShowerData" << evIndex << ".txt";
    std::string fileName = strstrm.str();
    char filePath[1024];
    strcpy(filePath, fileName.c_str());
    std::ofstream file(filePath);

    // Clear the vectors from a previous event
    vertices.clear();		// Vector of vertices 
    idInVertex.clear();		// id of particles in a given vertex id = idInVertex[vertexId][track/showerIndex]
    typeInVertex.clear();	// Type: Shower (s) or Track (t)
    allStartPositions.clear();  // Saving all of the start positions in a 2D array

    trackIndex = 0;

    // Loop over the tracks
    for(auto const& mct : *ev_mct){
	// Get the start point of the current track
	TVector3 startPoint(mct.Start().X(), mct.Start().Y(), mct.Start().Z());
	// If this is the first track treat it as a vertex
	if (trackIndex == 0){
		vertices.push_back(startPoint);
		
		std::vector<int> idVector;
		idVector.push_back(trackIndex);
		idInVertex.push_back(idVector);
		
		std::vector<TVector3> startVector;
		startVector.push_back(startPoint);
		allStartPositions.push_back(startVector);

		std::vector<char> typeVector;
		typeVector.push_back('t');
		typeInVertex.push_back(typeVector);
	}
	else{
		bool foundVertex = false;
		// Loop over all vertices found
		for (unsigned int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++){
			TVector3 diff;
			diff = startPoint - vertices[vertexIndex];
			// If the track is within the tolerance to an existing vertex 
			if (diff.Mag() <= tol){
				// Add it to the list of tracks and shower within that vertex
				double N = idInVertex[vertexIndex].size();
				vertices[vertexIndex] = pow((N + 1),-1)*((N * vertices[vertexIndex]) + startPoint);
				idInVertex[vertexIndex].push_back(trackIndex);
				allStartPositions[vertexIndex].push_back(startPoint);
				typeInVertex[vertexIndex].push_back('t');

				foundVertex = true;
				break;
			}
		}
		// If the track is not already in a vertex, then make a new vertex for it.
		if (!foundVertex){
			vertices.push_back(startPoint);

			std::vector<int> idVector;
			idVector.push_back(trackIndex);
			idInVertex.push_back(idVector);

			std::vector<TVector3> startVector;
			startVector.push_back(startPoint);
			allStartPositions.push_back(startVector);

			std::vector<char> typeVector;
			typeVector.push_back('t');
			typeInVertex.push_back(typeVector);
		}
	}
	
	trackIndex++;
    }

    // Repeat for showers.

    showerIndex = 0;
    // Loop over the showers
    for(auto const& mcs : *ev_mcs){
	TVector3 startPoint(mcs.Start().X(), mcs.Start().Y(), mcs.Start().Z());

	if (trackIndex == 0 && showerIndex == 0){
		vertices.push_back(startPoint);

		std::vector<int> idVector;
		idVector.push_back(showerIndex);
		idInVertex.push_back(idVector);

		std::vector<TVector3> startVector;
		startVector.push_back(startPoint);
		allStartPositions.push_back(startVector);

		std::vector<char> typeVector;
		typeVector.push_back('s');
		typeInVertex.push_back(typeVector);
	}
	else{
		bool foundVertex = false;
		// Loop over all vertices found
		for (unsigned int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++){
			TVector3 diff;
			diff = startPoint - vertices[vertexIndex];

			if (diff.Mag() <= tol){
				double N = idInVertex[vertexIndex].size();
				vertices[vertexIndex] = pow((N + 1),-1)*((N * vertices[vertexIndex]) + startPoint);
				idInVertex[vertexIndex].push_back(showerIndex);
				allStartPositions[vertexIndex].push_back(startPoint);
				typeInVertex[vertexIndex].push_back('s');
				foundVertex = true;
				break;
			}
		}
		if (!foundVertex){
			vertices.push_back(startPoint);

			std::vector<int> idVector;
			idVector.push_back(showerIndex);
			idInVertex.push_back(idVector);

			std::vector<TVector3> startVector;
			startVector.push_back(startPoint);
			allStartPositions.push_back(startVector);

			std::vector<char> typeVector;
			typeVector.push_back('s');
			typeInVertex.push_back(typeVector);
		}
	}
	
	showerIndex++;
    }

    std::cout << "Total Tracks  : " << trackIndex << std::endl;
    std::cout << "Total Showers : " << showerIndex << std::endl;
    

    if ( file.is_open() ){
	file << "e, " << evIndex << std::endl;
    	for (unsigned int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++){
		file << "v, " << vertexIndex << ", ";
		file << vertices[vertexIndex].x() << ", ";
		file << vertices[vertexIndex].y() << ", ";
		file << vertices[vertexIndex].z() << std::endl;
		for (unsigned int idIndex=0; idIndex<idInVertex[vertexIndex].size(); idIndex++){
			file << typeInVertex[vertexIndex][idIndex] << ", " << idInVertex[vertexIndex][idIndex] << ", ";
			file << allStartPositions[vertexIndex][idIndex].x() << ", ";
			file << allStartPositions[vertexIndex][idIndex].y() << ", ";
			file << allStartPositions[vertexIndex][idIndex].z() << std::endl;
		}
	}
    } 

    file.close();

    return true;
  }

  bool vertexFinder::finalize() {
    std::cout << "ALL IS WELL! PROCEED :)" << std::endl;
    return true;
  }

}
#endif
