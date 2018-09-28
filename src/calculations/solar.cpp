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


#include "./solar.h"
#include <math.h>
#include "../common/utilities/io.h"

#define PI 3.141592654
#define MAX_DEC 0.4092797096461111 // 23.45 degrees in Radians
#define MIN_DEC -MAX_DEC



/* THESE FUNCTION COME FROM RADIANCE */

/* Julian date (days into year) */
int jdate(int month,int day)
{
    static short  mo_da[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
    
    return(mo_da[month-1] + day);
}

/* solar time adjustment from Julian date */
double stadj(int  jd,double s_longitude,double s_meridian)
{
    return( 0.170 * sin( (4*PI/373) * (jd - 80) ) -
           0.129 * sin( (2*PI/355) * (jd - 8) ) +
           12 * (s_meridian - s_longitude) / PI );
}

/* solar declination angle from Julian date */
double sdec(int  jd)
{
    return( 0.4093 * sin( (2*PI/368) * (jd - 81) ) );
}

/* solar altitude from solar declination and solar time */
double salt(double sd,double st,double s_latitude)
{
    return( asin( sin(s_latitude) * sin(sd) -
                 cos(s_latitude) * cos(sd) * cos(st*(PI/12)) ) );
}


double
sazi(    /* solar azimuth from solar declination and solar time */
     double sd,
     double st,
     double s_latitude
     )
{
    return( -atan2( cos(sd)*sin(st*(PI/12)),
                   -cos(s_latitude)*sin(sd) -
                   sin(s_latitude)*cos(sd)*cos(st*(PI/12)) ) );
}

/* END OF RADIANCE */
 


bool isInSolarTrajectory(const Vector3D dir, const double lat, const int mf)
{

    // TEST ALL POSSIBLE COMBINATIONS
    const double phi = DegToRad(lat); 
    
    // maximum difference, given by the mg
    const double alpha = 0.5 * PI / (mf * 7 + 0.5);
    //const double tstep = 10/60.0; //  minute?
    const double tstep = 2.0*alpha/0.261799;

    double dec, alt, azi, cosDifference, difference;
    
    
    
    for(int day = 1; day <= 365; day++){
        
        // Get declination for the day
        dec = sdec(day);
        
        // Calculate sunrise from eq. 1.6.11 of Duffy and Beckmann
        const double ws = fabs(acos(-tan(phi)*tan(dec)));
        const double halfDay = ws/0.261799; // divide per radians/hour (15 degrees per hour)
        const double sunrise = 12.0 - halfDay;
        const double sunset = 12.0 + halfDay;
        
        for(double solarTime = sunrise; solarTime <= sunset; solarTime+= tstep){
            // Calculate position of the sun
            alt = salt(dec,solarTime,phi);
            azi = sazi( dec, solarTime,phi) + PI;
            
            
            double v1 = cos(alt);
            double v0 = (v1*sin(azi));
            v1 *= cos(azi);
            double v2 = (sin(alt));
            Vector3D sunDir = Vector3D(v0,v1,v2);
            
            // cosine of the angle
            cosDifference = sunDir*dir;
            difference = acos(cosDifference);
            
            //check
            if(difference <= alpha)
                return true;
        
        }// end of iterate the day
        
    }// end of iterate year
    return false;
    
  
}
