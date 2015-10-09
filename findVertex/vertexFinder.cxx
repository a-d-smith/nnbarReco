#ifndef LARLITE_VERTEXFINDER_CXX
#define LARLITE_VERTEXFINDER_CXX

#include "vertexFinder.h"
#include "TVector3.h"
#include <vector>

namespace larlite {

  bool vertexFinder::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    return true;
  }
  
  bool vertexFinder::analyze(storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //
  
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

    diffVect.clear();
    vertices.clear();
    trackInVertex.clear();
    trackIndex = 0;

    // Loop over the tracks
    for(auto const& mct : *ev_mct){
	TVector3 startPoint(mct.Start().X(), mct.Start().Y(), mct.Start().Z());

	if (trackIndex == 0){
		vertices.push_back(startPoint);

		std::vector<int> blankVector;
		blankVector.push_back(trackIndex);
		trackInVertex.push_back(blankVector);
	}
	else{
		bool foundVertex = false;
		// Loop over all vertices found
		for (unsigned int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++){
			TVector3 diff;
			diff = startPoint - vertices[vertexIndex];
			diffVect.push_back(diff);				

			if (diff.Mag() <= tol){
				trackInVertex[vertexIndex].push_back(trackIndex);
				foundVertex = true;
				break;
			}
		}
		if (!foundVertex){
			vertices.push_back(startPoint);

			std::vector<int> blankVector;
			blankVector.push_back(trackIndex);
			trackInVertex.push_back(blankVector);
		}
	}
	
	trackIndex++;
    }

    std::cout << "TOTAL TRACKS " << trackIndex << std::endl;

    for (unsigned int vertexIndex=0; vertexIndex<vertices.size(); vertexIndex++){
	std::cout << "(x, y, z)  =  (";
	std::cout << std::setw(10) << std::setprecision(3) << vertices[vertexIndex].x() << ", ";
	std::cout << std::setw(10) << std::setprecision(3) << vertices[vertexIndex].y() << ", ";
	std::cout << std::setw(10) << std::setprecision(3) << vertices[vertexIndex].z() << ") " << std::endl;
    	for (unsigned int trackIndex=0; trackIndex<trackInVertex[vertexIndex].size(); trackIndex++){
		std::cout << std::setw(3) << vertexIndex << " | " << std::setw(3) << trackInVertex[vertexIndex][trackIndex] << std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
    }
    std::cout << std::endl << std::endl;
    /*
    TGraph *graph = new  TGraph(i,X,Y);
    graph->Draw();
 
    std::cin.get();*/

    return true;
  }

  bool vertexFinder::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

}
#endif
