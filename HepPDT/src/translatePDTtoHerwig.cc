// $Id: translatePDTtoHerwig.cc,v 1.2 2004/04/14 23:56:28 garren Exp $
// ------------------------------------
//
// translatePDTtoHerwig.cc
// ..      PDT numbering scheme to HERWIG numbering scheme
//
// ------------------------------------

#include <iostream>

#include "CLHEP/HepPDT/ParticleID.hh"

namespace HepPDT {
      
int translatePDTtoHerwig( const int id )
{

// .. ITABJ(I) converts miscellaneous HERWIG particle ID's to a standard scheme
      static int itabj[99] = {1,2,3,4,5,6,7,8,0,0,
                 11,12,13,14,15,16,0,0,0,0,
                 21,22,23,24,25,0,0,0,0,0,
                 0,32,0,0,35,36,37,0,39,0,
                 0,0,0,0,0,0,0,0,0,0,
                 26,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,
                 81,82,83,84,85,86,87,88,89,90,
                 91,92,93,94,95,96,97,0,0};
    static  int noant[7] = {-21,-22,-23,-25,-51,-35,-36};
    int i;

int   hwtran=id;
int   ida=abs(id);
int   j1=ida%10;
int   i1=(ida/10)%10;
int   i2=(ida/100)%10;
int   i3=(ida/1000)%10;
int   k99 = (ida/100000)%100;
int   ksusy = (ida/1000000)%10;
int   kqn=(ida/1000000000)%10;

      if(ida == 0){
// ..       std::cout << " HWTRAN 1: particle ID is zero" << std::endl;
      } else if(kqn == 1) {
// .. ions not recognized
        hwtran=0;
        std::cout << " HWTRAN 3: nonallowed ion" << std::endl;
      } else if(ida < 100){
// ..Higgs, etc.
        hwtran=itabj[ida-1];
// ..check for illegal antiparticles
        if(id < 0){
          hwtran=-itabj[ida-1];
          if(hwtran >= -99 && hwtran <= -81) { hwtran=0; }
          for( i=0; i<7; ++i ) {
            if(hwtran == noant[i]) { hwtran=0; }
          }
        }
      } else if(k99 == 99 ) {
// .. special case remnant particles - used internally
        hwtran = 0;
        if(id == 9920022) { hwtran=98; }
        if(id == 9922212) { hwtran=99; }
        if(id == -9922212) { hwtran=-99; }
      } else if(ksusy == 1 || ksusy == 2) {
// .. SUSY
      } else if(i1 != 0 && i3 != 0 && j1 == 2){
// ..spin 1/2 baryons
      } else if(i1 != 0 && i3 != 0 && j1 == 4){
// ..spin 3/2 baryons
      } else if(i1 != 0 && i2 != 0 && i3 == 0){
// ..mesons 
// ..check for illegal antiparticles
        if(i1 == i2 && id < 0) { hwtran=0; }
      } else if(i2 != 0 && i3 != 0 && i1 == 0){
// ..diquarks
      } else {
// ..undefined
        hwtran=0;
      }
// ..check for illegal anti KS, KL
      if(id == -130 || id == -310) { hwtran=0; }

      if(hwtran == 0 && ida != 0){
        std::cout << "  HWTRAN 3: PDT particle " << id
                  << " translates to zero" << std::endl;
      }
      return hwtran;
}

}	// HepPDT
