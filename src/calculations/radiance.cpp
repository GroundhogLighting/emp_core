/*****************************************************************************
Glare

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

#include <fstream>

#include "./radiance.h"

#include "config_constants.h"
#include "common/utilities/stringutils.h"
#include "os_definitions.h"

extern "C" {

	static void
		rayirrad(			/* compute irradiance rather than radiance */
			RAY *r
		)
	{
		void(*old_revf)(RAY *) = r->revf;

		r->rot = 1e-5;			/* pretend we hit surface */
		VSUM(r->rop, r->rorg, r->rdir, r->rot);
		r->ron[0] = -r->rdir[0];
		r->ron[1] = -r->rdir[1];
		r->ron[2] = -r->rdir[2];
		r->rod = 1.0;
		/* compute result */
		r->revf = raytrace;
		(*ofun[Lamb.otype].funp)(&Lamb, r);
		r->revf = old_revf;
	}

}

bool rtrace(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, bool do_irradiance, bool imm_irrad, std::string amb, std::vector<RAY> * rays)
{
    do_irrad = do_irradiance ? 1 : 0;
	
    // Expose the provided options
	options->exposeOptions();

    // set the ambient file
	ambfile = &amb[0];

    // save the parameters
	RAYPARAMS rp;
	ray_save(&rp);
	
    // set up Radiance
	ray_init(&octname[0]);

    // trace a ray for each triangle
	Triangle * tr;
	for (size_t i = 0; i < t->getNumTriangles(); i++) {

		// Check if triangle still exists
		tr = t->getTriangleRef(i);
		if (tr == NULL)
			continue;

		// then process
		Point3D o = tr->getCenter();
		
        // set origin and direction
		FVECT org = { o.getX(), o.getY(), o.getZ() };
		FVECT dir = { 0, 0, 1 };
		VCOPY((*rays)[i].rorg, org);
		VCOPY((*rays)[i].rdir, dir);
		(*rays)[i].rmax = 0;

		double dmax = 0;

        // calculate
		rayorigin(&(*rays)[i], PRIMARY, NULL, NULL);
		if (imm_irrad) {
			VSUM((*rays)[i].rorg, org, dir, 1.1e-4);
			(*rays)[i].rdir[0] = -dir[0];
			(*rays)[i].rdir[1] = -dir[1];
			(*rays)[i].rdir[2] = -dir[2];
			(*rays)[i].rmax = 0.0;
			(*rays)[i].revf = rayirrad;
		}
		else {
			VCOPY((*rays)[i].rorg, org);
			VCOPY((*rays)[i].rdir, dir);
			(*rays)[i].rmax = dmax;
		}

        // increase one
		samplendx++;

        // copy value
		rayvalue(&((*rays)[i]));
		
	}

	ray_done(1);
	
	return true;
}


bool rtrace_i(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, std::string amb, std::vector<RAY> * rays)
{
	return rtrace(t, options, baseDir, octname, true, false,amb,rays);
}


bool rtrace_I(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, std::string amb, std::vector<RAY> * rays)
{
	return rtrace(t, options, baseDir, octname, false, true, amb, rays);
}


bool oconv(std::string octname, OconvOptions * options, RadExporter exporter)
{
    std::string command = "oconv - > " + octname + " 2> "+octname+".err";
  	
    FILE *octree = POPEN(&command[0], "w");
    //FOPEN(octree, octname.c_str(), "w");

    // check sky
    if (options->getOption<bool>(OCONV_INCLUDE_SKY)) {
      std::string sky = options->getOption<std::string>(OCONV_SKY);
      if (sky == OCONV_USE_CURRENT_SKY) {
        exporter.writeSky(octree);
      }
      else {
        fprintf(octree, "!%s\n", &sky[0]);
      }
    }

    // Add all the materials
    bool blackGeometry = options->getOption<bool>(OCONV_USE_BLACK_GEOMETRY);
    if (blackGeometry) {      
      fprintf(octree, "void plastic black 0 0 5 0 0 0 0 0 \n\n");      
    }
    exporter.writeMaterials(octree);
    
    // Check windows
    if (options->getOption<bool>(OCONV_INCLUDE_WINDOWS)) {
      exporter.writeWindows(octree);
    }

    // check lights
    if (options->getOption<bool>(OCONV_LIGHTS_ON)) {
      std::cerr << "OCONVing lights is still not supported\n";
    }


    // Add the geometry
    if (blackGeometry) {
      exporter.writeLayers(octree, "black");
    }
    else {
      exporter.writeLayers(octree, NULL);
    }



    
	PCLOSE(octree);

    return true;
}