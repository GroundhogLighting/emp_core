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



#include <fstream>

#include "./radiance.h"

#include "config_constants.h"
#include "common/utilities/stringutils.h"
#include "os_definitions.h"


/*
extern "C" {

	static void
		rayirrad(			//compute irradiance rather than radiance
			RAY *r
		)
	{
		void(*old_revf)(RAY *) = r->revf;

		r->rot = 1e-5;			// pretend we hit surface 
		VSUM(r->rop, r->rorg, r->rdir, r->rot);
		r->ron[0] = -r->rdir[0];
		r->ron[1] = -r->rdir[1];
		r->ron[2] = -r->rdir[2];
		r->rod = 1.0;
		//compute result 
		r->revf = raytrace;
		(*ofun[Lamb.otype].funp)(&Lamb, r);
		r->revf = old_revf;
	}

}
*/

bool rtrace(Triangulation * t, RTraceOptions * options, char * octname, bool do_irradiance, bool imm_irrad, std::string amb, std::vector<RAY> * rays)
{
	
  // Build the command
  std::string rgbfile = octname + std::to_string(rand()) + std::string(".rgb");

  std::string mode = "";
  if (imm_irrad) {
    mode = " -I ";
  }
  else if (do_irradiance) {
    mode = " -i ";
  }
  std::string ropts = options->getInlineVersion();  

  std::string command = "rtrace -h" + mode + ropts + " -af " + amb + + " " + octname + " > " + rgbfile ; // -I - ab 2 octree.oct > A.txt";
  
  // Create the file
  FILE *rt = POPEN(&command[0], "w");

  // trace a ray for each triangle	
  size_t nTriangles = t->getNumTriangles(); 
  
  Vector3D normal = t->getPolygon()->getNormal();

  for (size_t i = 0; i < nTriangles; i++) {
    // Get Triangle and Ray
    Triangle * tr = t->getTriangleRef(i);
    
    // then process
    Point3D o = tr->getCenter();

    fprintf(rt, "%f %f %f %f %f %f\n", o.getX(),o.getY(), o.getZ(), normal.getX(), normal.getY(), normal.getZ() );    
  }
  PCLOSE(rt);

  // Read results back
  //FILE * result = fopen(&rgbfile[0], "r");
  FOPEN(result, &rgbfile[0], "r");

  if (result != NULL)
  {
    float r;
    float g;
    float b;

    size_t i = 0;
    while (FSCANF(result, "%f %f %f", &r, &g, &b) != EOF)
    {                
      rays->at(i).rcol[0] = r;
      rays->at(i).rcol[1] = g;
      rays->at(i++).rcol[2] = b;
    }

    fclose(result);
  }
  else {
    FATAL(err,"Unable to open RGB result file '" + rgbfile +"' while soliving RTraceTask");
    return false;
  }

  if (remove(&rgbfile[0])) {
    std::string msg = "Impossible to remove file" + rgbfile;
    FATAL(errms, msg);
  }

  /* 
    For now, to allow parallel processing, we will 
    call RTRACE from the command line
  */


  /* Direct C++ call... not yet supported */
  /*
  do_irrad = do_irradiance ? 1 : 0;
  ray_pnprocs = 1;


  // Expose the provided options
  options->exposeOptions();

  // set the ambient file
  ambfile = &amb[0];
    
  // save the parameters
  RAYPARAMS rp;
  ray_save(&rp);
	

  ray_init(octname);

  for (size_t i = 0; i < nTriangles; i++) {
      // Get Triangle and Ray
	  Triangle * tr = t->getTriangleRef(i);		
      RAY * ray = &(rays->at(i));

	  // then process
	  Point3D o = tr->getCenter();
		
      // set origin and direction
	  FVECT org = { o.getX(), o.getY(), o.getZ() };
	  FVECT dir = { 0, 0, 1 };
	  VCOPY(ray->rorg, org);
	  VCOPY(ray->rdir, dir);
	  ray->rmax = 0;

	  double dmax = 0;

      // calculate
	  rayorigin(ray, PRIMARY, NULL, NULL);
	  if (imm_irrad) {
		  VSUM(ray->rorg, org, dir, 1.1e-4);
		  ray->rdir[0] = -dir[0];
          ray->rdir[1] = -dir[1];
          ray->rdir[2] = -dir[2];
          ray->rmax = 0.0;
          ray->revf = rayirrad;
	  }
	  else {
		  VCOPY(ray->rorg, org);
		  VCOPY(ray->rdir, dir);
          ray->rmax = dmax;
	  }

      // increase one
	  //samplendx++;

      // copy value
	  rayvalue(ray);
               		
  }        
  ray_done(1);
  */

  return true;
}


bool rtrace_i(Triangulation * t, RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays)
{
	return rtrace(t, options, octname, true, false, amb, rays);
}


bool rtrace_I(Triangulation * t, RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays)
{
	return rtrace(t, options, octname, false, true, amb, rays);
}


bool oconv(std::string octname, OconvOptions * options, RadExporter exporter)
{
    std::string command = "oconv - > " + std::string(octname);
  	
    FILE *octree = POPEN(&command[0], "w");
    
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

