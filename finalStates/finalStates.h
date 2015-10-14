/**
 * \file finalStates.h
 *
 * \ingroup finalStates
 * 
 * \brief Class def header for a class finalStates
 *
 * @author rsjones
 */

/** \addtogroup finalStates

    @{*/

#ifndef LARLITE_FINALSTATES_H
#define LARLITE_FINALSTATES_H

#include "Analysis/ana_base.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include <iomanip>
#include <vector>
#include "TROOT.h"
#include "TGraph.h"
#include "TVector3.h"


namespace larlite {
  /**
     \class finalStates
     User custom analysis class made by SHELL_USER_NAME
   */
  class finalStates : public ana_base{
  
  public:

    /// Default constructor
    finalStates(){ _name="finalStates"; _fout=0;}

    /// Default destructor
    virtual ~finalStates(){}

    /** IMPLEMENT in finalStates.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in finalStates.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in finalStates.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    int evIndex = 0;
    int pdg;
    std::vector<double> pdgFound;
    std::vector<double> pdgCount;
    std::vector<int> numPrimary;
    bool found;

  protected:
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
