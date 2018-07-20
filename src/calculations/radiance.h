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

#pragma once

#include <stddef.h> // In macOS we need this because size_t does not work

#include "./reinhart.h"
#include "./color_matrix.h"
#include "./oconv_options.h"
#include "../writers/rad/radexporter.h"


/// Do not change this, please... it is required to compile Radiance in C++
#define FUN_ARGLIST ...


//#define RAD(v) (0.265*v[0]+0.67*v[1]+0.065*v[2])
//#define LIGHT(v) (47.435*v[0]+119.93*v[1]+11.635*v[2])


/* EMULATE RADIANCE'S RAY AND OTHERS... A SHORT VERSION OF THEM */
extern "C" {
    #define  RED        0
    #define  GRN        1
    #define  BLU        2

    #define  colval(col,pri)    ((col)[pri])
    #define  VCOPY(v1,v2)    ((v1)[0]=(v2)[0],(v1)[1]=(v2)[1],(v1)[2]=(v2)[2])

    typedef float COLORV;
    typedef COLORV  COLOR[3];    /* red, green, blue (or X,Y,Z) */

    #define RREAL double
    typedef RREAL  FVECT[3];

    /* Arrange so double's come first for optimal alignment */
    /* Pointers and long's come second for 64-bit mode */
    /* Int's next (unknown length), then floats, followed by short's & char's */
    typedef struct ray {
        COLOR    rcol;        /* returned radiance value */
        FVECT    rorg;        /* origin of ray */
        FVECT    rdir;        /* normalized direction of ray */
        
    }  RAY;
}



//! This function emulates the use of Radiance's RCONTRIB program
/*!
 
 @author German Molina
 @param[in] options The RTRACE options
 @param[in] octname The name of the octree to read
 @param[in] do_irrad The parameter that emulates the '-i' option
 @param[in] imm_irrad The parameter that emulates the '-I' option
 @param[in] rays
 @param[in] mf The Reinhart subdivition scheme to use
 @param[in] modifier The modifier passed to the -m option
 @param[in] vMode The -V option
 @param[out] result The result ColorMatrix
 @note Always enables the -V option
 */
bool rcontrib(RTraceOptions * options, char * octname, bool do_irradiance, bool imm_irrad, std::vector<RAY> * rays, int mf,const char * modifier, bool vMode, ColorMatrix * result);

//! This function emulates the use of Radiance's RTRACE program
/*!
 @author German Molina
 @param[in] options The RTRACE options
 @param[in] octname The name of the octree to read
 @param[in] do_irrad The parameter that emulates the '-i' option in RTRACE
 @param[in] imm_irrad The parameter that emulates the '-I' option in RTRACE
 @param[in] amb The name of the ambient file to use
 @param rays The place where the resulting rays will be stored
 */
bool rtrace(RTraceOptions * options, char * octname, bool do_irrad, bool imm_irrad, std::string amb, std::vector<RAY> * rays, ColorMatrix * result);


//! This function emulates the use of Radiance's RTRACE program with the -I option enabled
/*!
 @author German Molina
 @param[in] options The RTRACE options
 @param[in] octname The name of the octree to read
 @param[in] amb The name of the ambient file to use
 @param[out] rays The place where the resulting rays will be stored
 */
bool rtrace_I( RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays, ColorMatrix * result);


//! This function emulates the use of Radiance's RTRACE program with the -i option enabled
/*!
 @author German Molina
 @param[in] options The RTRACE options
 @param[in] octname The name of the octree to read
 @param[in] amb The name of the ambient file to use
 @param[out] rays The place where the resulting rays will be stored
 */
bool rtrace_i( RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays, ColorMatrix * result);


//! Creates an octree according to certain option
/*
@author German Molina
@param[in] octreeName The name of the octree to create
@param[in] options The OconvOptions set
@param[in] exporter The RadianceExporter that will write all the necessary geometry
@todo Lights on
@return success
*/
bool oconv(std::string octreeName, OconvOptions * options, RadExporter exporter);

//! Calculates a single sky vector according to the Perez model
/*!
 @author German Molina
 @return the patch that uses the sun
 @param month The month of the year
 @param day The day of the month
 @param hour The hour of the day
 @param direct The direct normal irradiance
 @param diffuse The diffuse horizontal irradiance
 @param albedo The albedo in the location
 @param latitude The latitude
 @param longitude The longitude
 @param standardMeridian The standard meridian
 @param skyMF The sky subdivition scheme
 @param sunOnly Option for avoiding the sky, calculating only the sun
 @param sharpSun An option to use the -5 option in gendaymtx
 @param rotation Rotate the sky (in degrees)
 @param[out] skyVec The resulting sky vector
 */
int genPerezSkyVector(int month, int day, float hour, float direct, float diffuse, float albedo, float latitude, float longitude, float standardMeridian, int skyMF, bool sunOnly, bool sharpSun, float rotation, ColorMatrix * skyVec);


void interpolatedDCTimestep(int interp, GroundhogModel * model, const ColorMatrix * DC, bool sunOnly, bool sharpSun, ColorMatrix * result);

void calcCBDMScore(int interp, GroundhogModel * model, int firstMonth, int lastMonth, double early, double late, double minLux, double maxLux, const Matrix * input, Matrix * result, std::function<double(double v, double min, double max)> scoreCalculator);
    


