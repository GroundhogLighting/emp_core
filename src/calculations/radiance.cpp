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
#include "./gendaymtx.h"


/*
 THIS IS USED WHEN RTRACE IS CALLED INTERNALLY
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

#include "reinhart.h"

bool rcontrib(RTraceOptions * options, char * octname, bool do_irradiance, bool imm_irrad, std::vector<RAY> * rays, int mf,const char * modifier, bool vMode, ColorMatrix * result)
{
    // Build the command
    std::string rgbfile = octname + std::to_string(rand()) + std::string(".mtx");
    
    std::string mode;
    if (imm_irrad) {
        mode = " -I ";
    }
    else if (do_irradiance) {
        mode = " -i ";
    }
    
    std::string v = vMode ? " -V " : "";
    std::string ropts = options->getInlineVersion();
    
    std::string command = "rcontrib -h " + v + mode + ropts + " -e MF:"+ std::to_string(mf) + " -f reinhart.cal -b rbin -bn Nrbins -m " + std::string(modifier) + " " + octname + " > " + rgbfile ;
    
    // Create the file
    FILE *rt = POPEN(&command[0], "w");
    
    
    for(auto ray : *rays){
        fprintf(rt, "%f %f %f %f %f %f\n", ray.rorg[0], ray.rorg[1], ray.rorg[2], ray.rdir[0], ray.rdir[1], ray.rdir[2]);
    }
    
    PCLOSE(rt);
    
    std::ifstream in;
    in.open(rgbfile);
    std::string line;
    if(in.is_open())
    {
        size_t nsensors = rays->size();
        size_t nsensor = 0;
        size_t nbins = nReinhartBins(mf);
        
        if(result->ncols() != nsensors || result->nrows() != nbins){
            WARN(msg,"Inconsistent size of result matrix when RCONTRIB... resizing");
            result->resize(nsensors,nbins);
        }
        
        std::string rval;
        float red, green, blue;
        
        while(std::getline(in, line)) //get 1 row as a string
        {
            std::istringstream iss(line); //put line into stringstream
            for(size_t nbin = 0; nbin < nbins; nbin++){
                
                // Red
                iss >> rval;
                red = stof(rval);
                
                // Green
                iss >> rval;
                green = stof(rval);
                
                // Blue
                iss >> rval;
                blue = stof(rval);
                
                (*result->redChannel())[nsensor]->at(nbin) = red;
                (*result->greenChannel())[nsensor]->at(nbin) = green;
                (*result->blueChannel())[nsensor]->at(nbin) = blue;
            }
        
            nsensor ++;
        }
    }
    
    
    if (remove(&rgbfile[0])) {
        std::string msg = "Impossible to remove file" + rgbfile;
        FATAL(errms, msg);
    }
    
    return true;
}

bool rtrace(RTraceOptions * options, char * octname, bool do_irradiance, bool imm_irrad, std::string amb, std::vector<RAY> * rays)
{
    // Build the command
    std::string rgbfile = octname + std::to_string(rand()) + std::string(".rgb");
    
    std::string mode;
    if (imm_irrad) {
        mode = " -I ";
    }
    else if (do_irradiance) {
        mode = " -i ";
    }
    std::string ropts = options->getInlineVersion();
    
    std::string command = "rtrace -h" + mode + ropts + " -af " + amb + " " + octname + " > " + rgbfile ;
    
    
    // Create the file
    FILE *rt = POPEN(&command[0], "w");
    
    
    for(auto ray : *rays){
        fprintf(rt, "%f %f %f %f %f %f\n", ray.rorg[0], ray.rorg[1], ray.rorg[2], ray.rdir[0], ray.rdir[1], ray.rdir[2]);
    }
        
    PCLOSE(rt);
    
    // Read results back
    FOPEN(result, &rgbfile[0], "r");
    
    if (result != NULL)
    {
        float r;
        float g;
        float b;
        
        size_t i = 0;
        while (FSCANF(result, "%f %f %f", &r, &g, &b) != EOF)
        {
            rays->at(i).rcol[RED] = r;
            rays->at(i).rcol[GRN] = g;
            rays->at(i).rcol[BLU] = b;
            i++;
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
     THIS IS USED WHEN RTRACE IS CALLED INTERNALLY
     
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


bool rtrace_i( RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays)
{
    return rtrace( options, octname, true, false, amb, rays);
}


bool rtrace_I( RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays)
{
    return rtrace(options, octname, false, true, amb, rays);
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

bool genPerezSkyVector(int month, int day, float hour, float direct, float diffuse, float albedo, float latitude, float longitude, float standardMeridian, int skyMF, bool sunOnly, bool sharpSun, float rotate, ColorMatrix * skyVec)
{
    double	rotation = rotate;		/* site rotation (degrees) */
    int	dir_is_horiz;		/* direct is meas. on horizontal? */
    float	*mtx_data = NULL;	/* our matrix data */
    int	ntsteps = 0;		/* number of rows in matrix */
    int	step_alloc = 0;
    int	mo = month; int da = day;			/* month (1-12) and day (1-31) */
    double	hr = hour;			/* hour (local standard time) */
    double	dir = direct, dif=diffuse;		/* direct and diffuse values */
    int	mtx_offset;
    
    s_latitude = latitude;
    s_longitude = longitude;
    s_meridian = standardMeridian;    
    
    
    // Get options
    grefl[0] = grefl[1] = grefl[2] = albedo;
    rhsubdiv = skyMF;
    if(sunOnly){
        skycolor[0] = skycolor[1] = skycolor[0] = 0;
    }
    
    if(sharpSun){
        nsuns = 1;
        fixed_sun_sa = PI/360.*0.533;
        fixed_sun_sa *= fixed_sun_sa*PI;
    }
    
    switch (input) {        /* translate units */
        case 1:
            input = 1;        /* radiometric quantities */
            dir_is_horiz = 0;    /* direct is perpendicular meas. */
            break;
        case 2:
            input = 1;        /* radiometric quantities */
            dir_is_horiz = 1;    /* solar measured horizontally */
            break;
        case 3:
            input = 2;        /* photometric quantities */
            dir_is_horiz = 0;    /* direct is perpendicular meas. */
            break;
        default:
            FATAL(x,"Impossible input format");
            return false;
    }
    
    /* START CALCULATION */
    
    rh_init();			/* initialize sky patches */

        /* convert quantities to radians */
    s_latitude = DegToRad(s_latitude);
    s_longitude = DegToRad(s_longitude);
    s_meridian = DegToRad(s_meridian);
    
    
    double		sda, sta;
                /* make space for next time step */
    mtx_offset = 3*nskypatch*ntsteps++;
    if (ntsteps > step_alloc) {
        step_alloc += (step_alloc>>1) + ntsteps + 7;
        mtx_data = resize_dmatrix(mtx_data, step_alloc, nskypatch);
    }
    if (dif <= 1e-4) { // it is night
        memset(mtx_data+mtx_offset, 0, sizeof(float)*3*nskypatch);
    }else { // it is daytime
        /* compute solar position */
        julian_date = jdate(mo, da);
        sda = sdec(julian_date);
        sta = stadj(julian_date);
        altitude = salt(sda, hr+sta);
        azimuth = sazi(sda, hr+sta) + PI - DegToRad(rotation);
        /* convert measured values */
        if (dir_is_horiz && altitude > 0.)
            dir /= sin(altitude);
        if (input == 1) {
            dir_irrad = dir;
            diff_irrad = dif;
        } else /* input == 2 */ {
            dir_illum = dir;
            diff_illum = dif;
        }
        /* compute sky patch values */
        ComputeSky(mtx_data+mtx_offset);
        AddDirect(mtx_data+mtx_offset);        
    }
    
    
    /* Translate values from mtx_data into ColorMatrix */
    size_t nBins = nReinhartBins(skyMF);
    size_t aux = 0;
    for(size_t bin=0; bin < nBins; bin++){
        skyVec->redChannel()->setElement(bin,0,mtx_data[aux++]);
        skyVec->greenChannel()->setElement(bin,0,mtx_data[aux++]);
        skyVec->blueChannel()->setElement(bin,0,mtx_data[aux++]);
    }

    return true;
}



