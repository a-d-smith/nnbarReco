/**
 * \file vertexFinder.h
 *
 * \ingroup findVertex
 * 
 * \brief Class def header for a class vertexFinder
 *
 * @author rsjones/asmith
 */

/** \addtogroup findVertex

    @{*/

#ifndef LARLITE_VERTEXFINDER_H
#define LARLITE_VERTEXFINDER_H

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
     \class vertexFinder
     User custom analysis class made by SHELL_USER_NAME
   */
  class vertexFinder : public ana_base{
  
  public:

    /// Default constructor
    vertexFinder(){ _name="vertexFinder"; _fout=0;}

    /// Default destructor
    virtual ~vertexFinder(){}

    /** IMPLEMENT in vertexFinder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in vertexFinder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in vertexFinder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    std::vector<TVector3> vertices;
    std::vector<std::vector<int>> trackInVertex;
    std::vector<TVector3> diffVect;
    double tol = 1;
    int trackIndex;

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
