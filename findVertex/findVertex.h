/**
 * \file findVertex.h
 *
 * \ingroup findVertex
 * 
 * \brief Class def header for a class findVertex
 *
 * @author rsjones
 */

/** \addtogroup findVertex

    @{*/

#ifndef LARLITE_FINDVERTEX_H
#define LARLITE_FINDVERTEX_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class findVertex
     User custom analysis class made by SHELL_USER_NAME
   */
  class findVertex : public ana_base{
  
  public:

    /// Default constructor
    findVertex(){ _name="findVertex"; _fout=0;}

    /// Default destructor
    virtual ~findVertex(){}

    /** IMPLEMENT in findVertex.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in findVertex.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in findVertex.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

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
