// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of the HepBoostY class.
//

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include "CLHEP/Vector/defs.h"
#include "CLHEP/Vector/BoostY.h"
#include "CLHEP/Vector/Boost.h"
#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Vector/LorentzRotation.h"
#include "CLHEP/Vector/ZMxpv.h"

#include <cmath>
#include <iostream>

namespace CLHEP  {

// ----------  Constructors and Assignment:

HepBoostY & HepBoostY::set (double bbeta) {
  double b2 = bbeta*bbeta;
  if (b2 >= 1) {
    ZMthrowA (ZMxpvTachyonic(
    "Beta supplied to set HepBoostY represents speed >= c."));
    beta_  = 1.0 - 1.0E-8;              // NaN-proofing
    gamma_ = 1.0 / std::sqrt(1.0 - b2);
    return *this;
  }    
  beta_  = bbeta;
  gamma_ = 1.0 / std::sqrt(1.0 - b2);
  return *this;
}

// ----------  Accessors:

HepRep4x4 HepBoostY::rep4x4() const {
  double bg = beta_*gamma_;
  return HepRep4x4(   1,      0,    0,    0, 
		      0,    gamma_, 0,    bg,
                      0,      0,    1,    0,
		      0,     bg,    0,  gamma_ );
}

HepRep4x4Symmetric HepBoostY::rep4x4Symmetric() const {
  double bg = beta_*gamma_;
  return HepRep4x4Symmetric (   1,      0,    0,    0, 
		                      gamma_, 0,    bg,
                      		 	      1,    0,
		                                  gamma_ );
}

// ----------  Decomposition:

void HepBoostY::decompose (HepRotation & rotation, HepBoost & boost) const {
  HepAxisAngle vdelta = HepAxisAngle();
  rotation = HepRotation(vdelta);
  Hep3Vector bbeta = boostVector();
  boost = HepBoost(bbeta);
}

void HepBoostY::decompose (HepAxisAngle & rotation, Hep3Vector & boost) const {
  rotation = HepAxisAngle();
  boost = boostVector();
}

void HepBoostY::decompose (HepBoost & boost, HepRotation & rotation) const {
  HepAxisAngle vdelta = HepAxisAngle();
  rotation = HepRotation(vdelta);
  Hep3Vector bbeta = boostVector();
  boost = HepBoost(bbeta);
}

void HepBoostY::decompose (Hep3Vector & boost, HepAxisAngle & rotation) const {
  rotation = HepAxisAngle();
  boost = boostVector();
}

// ----------  Comparisons:

double HepBoostY::distance2( const HepBoost & b ) const {
  return b.distance2(*this);
}

double HepBoostY::distance2( const HepRotation & r ) const {
  double db2 = norm2();
  double dr2  = r.norm2();
  return (db2 + dr2);
}

double HepBoostY::distance2( const HepLorentzRotation & lt ) const {
  HepBoost b1;
  HepRotation r1;
  lt.decompose(b1,r1);
  double db2 = distance2(b1);
  double dr2  = r1.norm2();
  return (db2 + dr2);
}

bool HepBoostY::isNear (const HepRotation & r, double epsilon) const {
  double db2 = norm2();
  if (db2 > epsilon*epsilon) return false;
  double dr2  = r.norm2();
  return (db2+dr2 <= epsilon*epsilon);
}

bool HepBoostY::isNear ( const HepLorentzRotation & lt, 
					double epsilon  ) const {
  HepBoost b1;
  HepRotation r1;
  double db2 = distance2(b1);
  lt.decompose(b1,r1);
  if (db2 > epsilon*epsilon) return false;
  double dr2  = r1.norm2();
  return (db2 + dr2);
}

// ----------  Properties:

void HepBoostY::rectify() {
  // Assuming the representation of this is close to a true pure boost,
  // but may have drifted due to round-off error from many operations,
  // this forms an "exact" pure BoostY matrix for again.
  
  double b2 = beta_*beta_;
  if (b2 >= 1) {
    beta_ = 1.0 - 1.0e-8;		// Nan-proofing
    b2 = beta_*beta_;
  } 
  gamma_ = 1.0 / std::sqrt(1.0 - b2);
}

// ---------- Application:

// ---------- Operations in the group of 4-Rotations

HepBoostY HepBoostY::operator * (const HepBoostY & b) const {
  return HepBoostY ( (beta()+b.beta()) / (1+beta()*b.beta()) );
}
HepLorentzRotation HepBoostY::operator * (const HepBoost & b) const {
  HepLorentzRotation me (*this);
  return me*b;
}
HepLorentzRotation HepBoostY::operator * (const HepRotation & r) const {
  HepLorentzRotation me (*this);
  return me*r;
}
HepLorentzRotation HepBoostY::operator * (const HepLorentzRotation & lt) const {
  HepLorentzRotation me (*this);
  return me*lt;
}
 
// ---------- I/O
 
std::ostream & HepBoostY::print( std::ostream & os ) const {
  os << "Boost in Y direction (beta = " << beta_ 
			<< ", gamma = " << gamma_ << ") ";
  return os;
}

}  // namespace CLHEP
