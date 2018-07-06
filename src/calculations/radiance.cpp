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
#include "../config_constants.h"
#include "../common/utilities/stringutils.h"
#include "../os_definitions.h"
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
    
    std::string mode = "";
    if (imm_irrad) {
        mode = " -I ";
    }
    else if (do_irradiance) {
        mode = " -i ";
    }
    
    std::string v = vMode ? " -V " : "";
    std::string ropts = options->getInlineVersion();
    
    std::string command = "rcontrib -h " + v + mode + ropts + " -e MF:"+ std::to_string(mf) + " -f reinhart.cal -b rbin -bn Nrbins -m " + std::string(modifier) + " " + octname + " > " + rgbfile ;
    // -faf
    
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
        
        if(result->nrows() != nsensors || result->ncols() != nbins){
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
                
                result->r()->setElement(nsensor,nbin,red);
                result->g()->setElement(nsensor,nbin,green);
                result->b()->setElement(nsensor,nbin,blue);
                
                //result->greenChannel())[nsensor]->at(nbin) = green;
                //result->blueChannel())[nsensor]->at(nbin) = blue;
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

bool rtrace(RTraceOptions * options, char * octname, bool do_irradiance, bool imm_irrad, std::string amb, std::vector<RAY> * rays, ColorMatrix * result)
{
    // Build the command
    std::string rgbfile = std::string(octname) +".rgb";
    
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
    FOPEN(resultFile, &rgbfile[0], "r");
    
    if (resultFile != NULL)
    {
        float r;
        float g;
        float b;
        
        size_t i = 0;
        while (FSCANF(resultFile, "%f %f %f", &r, &g, &b) != EOF)
        {
            result->r()->setElement(i,0,r);
            result->g()->setElement(i,0,g);
            result->b()->setElement(i,0,b);
            //rays->at(i).rcol[RED] = r;
            //rays->at(i).rcol[GRN] = g;
            //rays->at(i).rcol[BLU] = b;
            
            i++;
        }
        
        fclose(resultFile);
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


bool rtrace_i( RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays, ColorMatrix * result)
{
    return rtrace( options, octname, true, false, amb, rays, result);
}


bool rtrace_I( RTraceOptions * options, char * octname, std::string amb, std::vector<RAY> * rays, ColorMatrix * result)
{
    return rtrace(options, octname, false, true, amb, rays, result);
}


bool oconv(std::string octname, OconvOptions * options, RadExporter exporter)
{    
    std::string command = "oconv - > " + std::string(octname);
  	
    FILE *octree = POPEN(&command[0], "w");
            

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
      exporter.writeLayersInOneFile(octree, "black");
    }
    else {
      exporter.writeLayersInOneFile(octree, NULL);
    }
    
	PCLOSE(octree);

    return true;
}

bool genPerezSkyVector(int month, int day, float hour, float direct, float diffuse, float albedo, float latitude, float longitude, float standardMeridian, int skyMF, bool sunOnly, bool sharpSun, float rotate, ColorMatrix * skyVec)
{
    GenDayMtx g = GenDayMtx();
    
    double	rotation = rotate;		/* site rotation (degrees) */
    int	dir_is_horiz;		/* direct is meas. on horizontal? */
    float	*mtx_data = NULL;	/* our matrix data */
    //int	ntsteps = 1;		/* number of rows in matrix */
    int	step_alloc = 0;
    int	mo = month; int da = day;			/* month (1-12) and day (1-31) */
    double	hr = hour;			/* hour (local standard time) */
    double	dir = direct, dif=diffuse;		/* direct and diffuse values */
    
    g.s_latitude = latitude;
    g.s_longitude = longitude;
    g.s_meridian = standardMeridian;
    
    
    // Get options
    g.grefl[0] = g.grefl[1] = g.grefl[2] = albedo;
    g.rhsubdiv = skyMF;
    if(sunOnly){
        g.skycolor[0] = g.skycolor[1] = g.skycolor[2] = 0;
    }
    
    if(sharpSun){
        g.nsuns = 1;
        g.fixed_sun_sa = PI/360.0*0.533;
        g.fixed_sun_sa *= g.fixed_sun_sa*PI;
    }
    
    switch (g.input) {        /* translate units */
        case 1:
            g.input = 1;        /* radiometric quantities */
            dir_is_horiz = 0;    /* direct is perpendicular meas. */
            break;
        case 2:
            g.input = 1;        /* radiometric quantities */
            dir_is_horiz = 1;    /* solar measured horizontally */
            break;
        case 3:
            g.input = 2;        /* photometric quantities */
            dir_is_horiz = 0;    /* direct is perpendicular meas. */
            break;
        default:
            FATAL(x,"Impossible input format");
            return false;
    }
    
    /* START CALCULATION */
    
    g.rh_init();			/* initialize sky patches */

        /* convert quantities to radians */
    g.s_latitude = DegToRad(g.s_latitude);
    g.s_longitude = DegToRad(g.s_longitude);
    g.s_meridian = DegToRad(g.s_meridian);
    
    
    double		sda, sta;
                /* make space for next time step */
    if (1 > step_alloc) {
        step_alloc += (step_alloc>>1) + 8;
        mtx_data = g.resize_dmatrix(mtx_data, step_alloc, g.nskypatch);
    }
    if (dif <= 1e-4) { // it is night
        memset(mtx_data, 0, sizeof(float)*3*g.nskypatch);
    }else { // it is daytime
        /* compute solar position */
        g.julian_date = g.jdate(mo, da);
        sda = g.sdec(g.julian_date);
        sta = g.stadj(g.julian_date);
        g.altitude = g.salt(sda, hr+sta);
        g.azimuth = g.sazi(sda, hr+sta) + PI - DegToRad(rotation);
        /* convert measured values */
        if (dir_is_horiz && g.altitude > 0.)
            dir /= sin(g.altitude);
        if (g.input == 1) {
            g.dir_irrad = dir;
            g.diff_irrad = dif;
        } else /* input == 2 */ {
            g.dir_illum = dir;
            g.diff_illum = dif;
        }
        /* compute sky patch values */
        g.ComputeSky(mtx_data);
        g.AddDirect(mtx_data);
    }
    
    /* Translate values from mtx_data into ColorMatrix */
    size_t nBins = nReinhartBins(skyMF);
    size_t aux = 0;
    for(size_t bin=0; bin < nBins; bin++){
        skyVec->r()->setElement(bin,0,mtx_data[aux++]);
        skyVec->g()->setElement(bin,0,mtx_data[aux++]);
        skyVec->b()->setElement(bin,0,mtx_data[aux++]);
    }

    return true;
}


void interpolatedDCTimestep(int interp, GroundhogModel * model, const ColorMatrix * DC, bool sunOnly, bool sharpSun, ColorMatrix * result)
{
    
    // Get location info
    Location * location = model -> getLocation();
    float albedo = location->getAlbedo();
    float latitude = location->getLatitude();
    float longitude = location-> getLongitude();
    float meridian = location->getTimeZone()*(-15.0);
    double rotation = model -> getNorthCorrection();
    
    // Get sizes and resize
    const size_t nSensors = DC->nrows();
    const size_t nBins = DC->ncols();
    size_t nSamples = location->getWeatherSize();
    int mf = mfFromNBins((int)nBins);
    size_t nstep = 0;
    
    if (nSamples == 0)
        FATAL(m,"No Weather Data when CalculateDirectSunComponent");
    
    result->resize(nSensors,interp*nSamples);
    
    // Initialize Sky vector
    ColorMatrix skyVector = ColorMatrix(nBins,1);
    
    // Interpolate and multiply
    
    for(int timestep = 0 ; timestep < nSamples; timestep++ ){
        HourlyData now = HourlyData();
        float floatInter = (float)interp;
        float q;
        for(int i = 0; i < interp; i++){
            q = (float)i / floatInter;
            
            location->getInterpolatedData(timestep,q,&now);
            
            if(now.diffuse_horizontal > 1e-4){
                // Is day... calculate
                genPerezSkyVector(now.month, now.day, now.hour, now.direct_normal, now.diffuse_horizontal, albedo, latitude, longitude, meridian, mf, sunOnly, sharpSun, rotation, &skyVector);
                
                // Multiply
                DC->multiplyToColumn(&skyVector, nstep, result);
            } // No else... matrices come with zeroes
            
            nstep++;
        }
    }
}


void calcCBDMScore(int interp, GroundhogModel * model, int firstMonth, int lastMonth, double early, double late, double minLux, double maxLux, const Matrix * input, Matrix * result, std::function<double(double v, double min, double max)> scoreCalculator)
{
    
    // Get size
    size_t nsensors = input->nrows();
    
    // Make space for the final results results
    result->resize(nsensors,1);
    
    /* Calculate the score (in percentage of time, from 0 to 100) */
    
    // Get location and weather data size
    Location * location = model->getLocation();
    size_t nSamples = location->getWeatherSize();
    
    // Initialize variables
    float lux;
    size_t nWorkingTsteps = 0;
    size_t nstep=-1;
    HourlyData now = HourlyData();
    float floatInter = (float)interp;
    float q;
    
    // Iterate through the weather data
    for(int timestep = 0 ; timestep < nSamples; timestep++ ){
        
        // Interpolate between timesteps
        for(int i = 0; i < interp; i++){
            // Fraction between timesteps
            q = (float)i / floatInter;
            
            // Fill the HourlyData with interpolated information
            location->getInterpolatedData(timestep,q,&now);
            
            // Advance in time
            nstep++;
            
            // Check if this moment counts
            if(now.month < firstMonth || now.month > lastMonth)
                continue;
            
            if(now.hour < early || now.hour > late)
                continue;
            
            // Increase working timesteps, if it counts
            nWorkingTsteps++;
            
            // Iterate all sensors, increasing the score if needed
            for(int sensor = 0; sensor < nsensors; sensor++){
                lux = input->getElement(sensor,nstep);
                
                double score = scoreCalculator(lux, minLux, maxLux);
                
                result->setElement(sensor,0,result->getElement(sensor,0)+score);                
            }
        }
    }
    
    // Normalize by the total number of timestep
    float totalSteps = (float)nWorkingTsteps/100.0;
    
    for(size_t sensor = 0; sensor < nsensors; sensor++){
        result->setElement(sensor, 0, result->getElement(sensor,0)/totalSteps);
    }
}




