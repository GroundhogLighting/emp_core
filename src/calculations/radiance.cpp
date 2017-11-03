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

#include "./radiance.h"

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

#include <iostream>
bool rtrace(Triangulation * t, RTraceOptions * options, std::string baseDir, std::string octname, bool do_irradiance, bool imm_irrad, std::string amb, std::vector<RAY> * rays)
{

	// Move to the base directory
	CHDIR(&baseDir[0]);


		

	if (do_irradiance) {
		do_irrad = 1; // +i
	}
	else {
		do_irrad = 0;
	}

	
	options->exposeOptions();
	ambfile = &amb[0];

	RAYPARAMS rp;
	ray_save(&rp);
	
	ray_init(&octname[0]);

	Triangle * tr;
	for (size_t i = 0; i < t->getNumTriangles(); i++) {

		// Check if triangle still exists
		tr = t->getTriangleRef(i);
		if (tr == NULL)
			continue;

		// then process
		Point3D o = tr->getCenter();
		
		FVECT org = { o.getX(), o.getY(), o.getZ() };
		FVECT dir = { 0, 0, 1 };
		VCOPY((*rays)[i].rorg, org);
		VCOPY((*rays)[i].rdir, dir);
		(*rays)[i].rmax = 0;

		double dmax = 0;

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

		samplendx++;
		rayvalue(&((*rays)[i]));
		
	}

	ray_done(1);


	
	// Return to the original
	CHDIR("..");
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