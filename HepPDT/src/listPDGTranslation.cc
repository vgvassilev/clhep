// $Id: listPDGTranslation.cc,v 1.1.4.2 2005/03/17 22:42:53 garren Exp $
// ----------------------------------------------------------------------
//
// listPDGTranslation.cc
// Author:  Lynn Garren
//
// list translations for various MonteCarlo input types
// DO NOT mix these functions with the addXXXParticles functions
// These functions will read a table file and write a translation list
// and are designed to be used when checking particle ID translations
//
// ----------------------------------------------------------------------


#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "CLHEP/HepPDT/defs.h"
#if HAVE_SSTREAM
#include <sstream>
#else
#include "CLHEP/HepPDT/StringStream.h"
#endif
#include "CLHEP/HepPDT/ParticleTranslation.hh"

namespace HepPDT {


/**
 * @author
 * @ingroup heppdt
 */

// --- free functions
//
void getPDGpid( std::vector<int> & idlist,  std::string & pdline );

ParticleTranslation parsePDGline( int oid,  std::string & pdline )
{
   double v, e1, e2, err;
   std::string name, ckey, charges, bigname;
   ckey = pdline.substr(0,1);
   name = charges = "";
   v = e1 = e2 = 0.0;
   std::istringstream val( pdline.substr(34,33).c_str() );
   val >> v >> e1 >> e2;
   err = sqrt( (e1*e1 + e2*e2)/2.0 );
   bigname = pdline.substr(68,21);
   int blank = bigname.find(" ");
   name = bigname.substr(0,blank);
   // really should strip out leading blanks 
   // unfortunately, the istrstream trick does not work with strings in KCC
   charges = bigname.substr(blank+1,20-blank);
   //std::cout << blank << " -- " << pdline.substr(68,21) << std::endl;
   //std::istringstream namelist( pdline.substr(68,21).c_str() );
   //namelist >> name >> charges;
   // std::cout << ckey << " " << tpd.tempID.pid() << " "  
   //      << v << " " << e1 << " " << e2 << " " << name << " " << charges << std::endl;

   // now add appropriate properties
   std::string mc = std::string("PDG table");
   // for our purposes, only use the mass lines
   if( ckey.find("M") == 0 ) {
      ParticleID pid( translatePDGtabletoPDT(oid) );
      return ParticleTranslation( pid, oid, name, mc );
   }
   return ParticleTranslation();
}

bool  listPDGTranslation    ( std::istream & pdfile, std::ostream & os )
{
  // mass and width lines can be in any order
  std::vector<int> idlist;
  std::string pdline;
  std::vector<ParticleTranslation> plist;
  // read and parse each line
  while( std::getline( pdfile, pdline) ) {
    getPDGpid( idlist, pdline );
    for( unsigned int i = 0; i < idlist.size(); ++i )
    {
        ParticleTranslation pt = parsePDGline( idlist[i], pdline ); 
	// test for empty ParticleTranslation
	if( pt.oid() != 0 ) {
	    plist.push_back( pt );
	    pt.write( os );
	}
    }
  }
  // iterate over the list for a more compact comparison listing
  os << std::endl;
  for ( unsigned int cit = 0; cit<plist.size(); ++cit ) {
       ParticleTranslation p = plist[cit];
       p.writeComparison( os );
  }
  return true;
}

}  // namespace HepPDT
