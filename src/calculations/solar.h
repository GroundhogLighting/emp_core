/*****************************************************************************
 Emp
 
 Copyright (C) 2018  German Molina (germolinal@gmail.com)
 
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

#pragma once

#include "../common/geometry/vector.h"



/* THESE FUNCTION COME FROM RADIANCE */


/* Degrees into radians */
#define DegToRad(deg)    ((deg)*(PI/180.))

/* Radiuans into degrees */
#define RadToDeg(rad)    ((rad)*(180./PI))

#ifndef PI
#define PI 3.141592654
#endif
/* Julian date (days into year) */
int jdate(int month,int day);

/* solar time adjustment from Julian date */
double stadj(int  jd,double s_longitude,double s_meridian);

/* solar declination angle from Julian date */
double sdec(int  jd);

/* solar altitude from solar declination and solar time */
double salt(double sd,double st,double s_latitude);

/* solar azimuth from solar declination and solar time */
double sazi( double sd, double st,double s_latitude);

/* END OF RADIANCE */


bool isInSolarTrajectory(const Vector3D dir, const double lat, const int mf);
