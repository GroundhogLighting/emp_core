/*****************************************************************************
Emp

Copyright (C) 2017  German Molina (germolinal@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/
#include <cmath>
#include "./reinhart.h"

const size_t tnaz[7] = { 30, 30, 24, 24, 18, 12, 6 };

size_t rnaz(size_t r, size_t MF)
{
  if (r > 7 * MF - 0.5) {
    return 1;
  }
  else {
    int i = (int)floor((r + 0.5f) / MF);
    return MF*tnaz[i];
  }
}

size_t raccum(size_t r, size_t MF)
{
  if (r - 0.5 > 0) {
    return rnaz(r - 1, MF) + raccum(r-1, MF);
  }
  else {
    return 0;
  }
}

size_t nReinhartBins(int MF)
{
  return 1+raccum(7*MF+1, MF);
}

size_t mfFromNBins(const int nbins)
{
    size_t mf = 1;
    size_t currentNBins = nReinhartBins(mf);
    
    while( nbins >= currentNBins ){
        if(nbins == currentNBins)
            return mf;
        
        currentNBins = nReinhartBins(++mf);
        
    }
    return -1;
}

size_t Rfindrow(size_t r, size_t rem, size_t MF)
{
  size_t rnazr = rnaz(r, MF);
  int aux = (int)(rem - rnazr);
  if (aux > 0.5) {
    return Rfindrow(r + 1, rem - rnazr, MF);
  }
  else {
    return r;
  }
}

size_t Rfindrow(size_t rem, size_t MF)
{
    return  Rfindrow(0, rem, MF);
}


Vector3D reinhartCenterDir(size_t nbin, size_t MF)
{
  return reinhartCenterDir(nbin, MF, NULL);
}

double reinhartSolidAngle(size_t nbin, size_t MF)
{
  double ret;
  reinhartCenterDir(nbin, MF, &ret);
  return ret;
}

Vector3D reinhartCenterDir(size_t nbin, size_t MF, double * solidAngle)
{
    return reinhartDir(nbin, MF, 0.5, 0.5, solidAngle) ;
}

Vector3D reinhartDir(size_t nbin, size_t MF,  const double x1, const double x2)
{
    return reinhartDir(nbin, MF, x1, x2, NULL);
}

Vector3D reinhartDir(size_t nbin, size_t MF,  const double x1, const double x2, double * solidAngle)
{
  const double PI = 3.141592654;
  const double alpha = 90.0 / (MF * 7 + 0.5);  
  const double RAH = alpha *PI / 180.0;
  const size_t RowMax = 7 * MF + 1;
  const size_t Rmax = raccum(RowMax,MF);
  
  size_t Rrow; 
  if (nbin - (Rmax - .5) > 0) {
    Rrow = RowMax - 1;
  }
  else {
    Rrow = Rfindrow(0, nbin, MF);
  }


  // Find Ralt
  double Ralt;
  if (nbin - 0.5 > 0) {
    Ralt = (Rrow + x1)*RAH;
  }else {
    Ralt = asin(-x1);
  }

  size_t nBins = rnaz(Rrow, MF);

  // Find Razi
  const size_t Rcol = nbin - raccum(Rrow,MF) -1 ;
  double Razi_width = 2 * PI / nBins;

  double Razi;
  if (nbin > 0) {
    Razi = (Rcol + x2 - .5)*Razi_width;
  }
  else {
    Razi = 2 * PI*x2;
  }

  // Compute
  double cos_ralt = cos(Ralt);
  double dx = sin(Razi)*cos_ralt;
  double dy = cos(Razi)*cos_ralt;
  double dz = sin(Ralt);

  if (solidAngle != NULL) {
    if (Rrow == (RowMax-1)) {
      // if polar cap
      *solidAngle = coneSolidAngle(RAH/2.0);
    }
    else {
      *solidAngle = 2.0 * PI * (sin(Ralt + RAH/2.0)-sin(Ralt - RAH/2.0)) / (double)nBins;
    }
  }

  return Vector3D(dx, dy, dz);
}


double coneSolidAngle(double angle)
{
  return 2.0 * 3.141592654 * (1 - cos(angle));
}
