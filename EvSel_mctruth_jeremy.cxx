#ifndef LARLITE_EVSEL_MCTRUTH_CXX
#define LARLITE_EVSEL_MCTRUTH_CXX

#include "EvSel_mctruth.h"

namespace larlite {

  bool EvSel_mctruth::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    loop_number = 1;
    candidates = 0;
    return true;
  }
  
  bool EvSel_mctruth::analyze(storage_manager* storage) {

    std::cout << "Event number: " << loop_number << std::endl;

    std::vector<mcshower> showers;
    std::vector<mctrack> tracks;
    std::vector<unsigned int> mothers;
    
    int p = 0;
    
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    
    if(!ev_mcs){
      std::cout << "MCShower pointer invalid! Exiting..." << std::endl;
      exit(1);
    }
    else if(!ev_mct){
      std::cout << "MCTrack pointer invalid! Exiting..." << std::endl;
      exit(1);
    }
    
    for(auto const& mcs : *ev_mcs){
      if(mcs.PdgCode() == 22 && mcs.MotherPdgCode() == 111){
        int seen = 0;
        for(auto const& mother: mothers){
          if(mcs.MotherTrackID() == mother) seen = 1;
        }
        if(seen == 0){
          mothers.push_back(mcs.MotherTrackID());
          showers.push_back(mcs);
          p++;
        }
      }
    }
    
    for(auto const& mct: *ev_mct){
      int pdg = mct.PdgCode();
      if(pdg == 211||pdg == -211){
        tracks.push_back(mct);
        p++;
      }
    }

    if(p > 1) select(showers,tracks,&candidates);

    std::cout << std::endl;
    loop_number++;
    return true;
  }

  bool EvSel_mctruth::finalize() {

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

    std::cout << "Number of candidate events in sample: " << candidates << std::endl;  
    return true;
  }

}
#endif
