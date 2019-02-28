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


// GenCumulativeSky was conceived, developed and validated by Darren Robinson and
// Andrew Stone for efficient solar irradiation modelling using RADIANCE

// When using GenCumulativeSky they would be pleased if you would ackowledge their work
// by referring to the following article: "Robinson, D., Stone, A., Irradiation modeling
//made simple � the cumulative sky approach and its applications, Proc. PLEA 2004, Eindhoven 2004."

// THIS IS BASICALLY A COPY/PASTE/MODIFY OF THE ORIGINAL gencumulativesku.cpp FILE.

#include "./gencumulativesky.h"

#include "./GenCumSky/cSkyVault.h"

void genCumulativeSky(EmpModel * model, bool DoIlluminance, bool DoDiffuse, std::string filename)
{
    double hourshift = 0;
    
    cSkyVault sky;
    double (*patchData)[5] = new double[145][5];
    int i,j, counter;
    
    double *CumSky;
    
    double rowdeltaaz[7]={12,12,15,15,20,30,60};
    int rowdeltaalt=12;
    
    /* TRANSFORMED INTO ARGUMENTS TO FUNCTION */
    //char * filename=argv[argc-1];
    //double hourshift=0;
    //bool DoIlluminance=false;
    //bool DoDiffuse;
    
    cSkyVault::eSunType SunType;
    cClimateFile::eClimateFileFormat ClimateFileFormat;
    

    /*
    if (!(argc>1))
    {
        // User didn't give any command line arguments
        fprintf(stderr,"gencumulativesky: Error - invalid input parameters\n");
        goto USAGEERROR;
    }
     */
    
    std::cerr << "EMP_CORE WARNING: gencumulativesky is not yet working... Line: " << __LINE__ <<  " File:" << __FILE__ << std::endl;
    
    // Get model location
    Location * location = model->getLocation();
    
    if(!location->hasWeather())
        throw "Your model requires weather data to calculate a cumulative sky";
    
    SunType=cSkyVault::NO_SUN;
    
    double latitude = location->getLatitude()*M_PI/180;
    sky.SetLatitude(latitude);
    
    // EMP convention is latitude West (Santiago, Chile is about 73)... gencumulativesky is the other way around
    double longitude = -location->getLongitude()*M_PI/180;
    sky.SetLongitude(longitude);
    
    double meridian = 15.0 * location->getTimeZone()*M_PI/180;
    sky.SetMeridian(meridian);
    
    ClimateFileFormat=cClimateFile::GLOBAL_DIFFUSE;
    //ClimateFileFormat=cClimateFile::DIRECTHORIZONTAL_DIFFUSE; // We will use this.
    
    
    sky.loadModelWeather(model, ClimateFileFormat);
    
    
    
    
    sky.CalculateSky(SunType, DoDiffuse, DoIlluminance, hourshift);
    
    CumSky=sky.GetCumulativeSky();
    sky.GetPatchDetails(patchData);
    
    // Open file
    // We use this method to keep consistency with the original C-like code
    
    FILE * calFile;
    calFile = fopen (&filename[0],"w");
    
    fprintf(calFile,"{ This .cal file was generated automatically by gencumulativesky within Emp_core }\n");
    fprintf(calFile,"{ ");
    //for (j=0; j<argc; j++)
    //    printf("%s ",argv[j]);
    fprintf(calFile," }\n\n");
    fprintf(calFile,"skybright=");
    for (j=0; j<7; j++)
    {
        fprintf(calFile,"row%d+",j);
    }
    fprintf(calFile,"row7;\n\n");
    
    counter=0;
    for (j=0; j<7; j++)
    {
        // note first patch split into two parts - first part (> 0 deg) and last patch (<360)
        fprintf(calFile,"row%d=if(and(alt-%d, %d-alt),select(floor(0.5+az/%5.2f)+1,\n",j,j*rowdeltaalt,(j+1)*rowdeltaalt,rowdeltaaz[j]);
        for (i=0+counter; i<counter+360/int(rowdeltaaz[j]); i++)
        {
            fprintf(calFile,"\t%f,\n",CumSky[i]);
        }
        fprintf(calFile,"\t%f),0);\n\n",CumSky[counter]);
        counter+=(int)(360/rowdeltaaz[j]);
    }
    
    fprintf(calFile,"row7=if(alt-84,%f,0);\n\n",CumSky[144]);
    
    fprintf(calFile,"alt=asin(Dz)*180/PI;\n\n");
    
    fprintf(calFile,"az=if(azi,azi,azi+360);\n");
    fprintf(calFile,"azi=atan2(Dx,Dy)*180/PI;\n\n");
    
    fclose(calFile);
}

