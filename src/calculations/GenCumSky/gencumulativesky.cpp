// GenCumulativeSky was conceived, developed and validated by Darren Robinson and Andrew Stone for efficient solar irradiation modelling using RADIANCE
// When using GenCumulativeSky they would be pleased if you would ackowledge their work by referring to the following article: "Robinson, D., Stone, A., 
// Irradiation modeling made simple � the cumulative sky approach and its applications, Proc. PLEA 2004, Eindhoven 2004."


// gencumulativesky.cpp : Defines the entry point for the console application.

#include "cSkyVault.h"

int main(int argc, char* argv[])
{
	cSkyVault sky;
	double (*patchData)[5] = new double[145][5];
	int i,j, counter;

	double *CumSky;
	//double dx,dy,dz;
	//double alt,az,deltaalt,deltaaz;

	double rowdeltaaz[7]={12,12,15,15,20,30,60};
	//double rowdeltaaz[14]={6,6,6,6,7.5,7.5,7.5,7.5,10,10,15,15,30,30};
	int rowdeltaalt=12;

	char *filename=argv[argc-1];
	double hourshift=0;

	bool DoIlluminance=false;

	cSkyVault::eSunType SunType;
	cClimateFile::eClimateFileFormat ClimateFileFormat;

	bool DoDiffuse;

	if (!(argc>1))
	{
		// User didn't give any command line arguments 
		fprintf(stderr,"gencumulativesky: Error - invalid input parameters\n");
		goto USAGEERROR;
	}

	counter=1;

	// Set the default parameters
	SunType=cSkyVault::NO_SUN;
	DoDiffuse=true;
	sky.SetLatitude(51.7*M_PI/180);
	sky.SetLongitude(0*M_PI/180);
	ClimateFileFormat=cClimateFile::GLOBAL_DIFFUSE;

	while (counter<argc-1)
	{
		if (argv[counter][0]=='+' && argv[counter][1]=='s' && argv[counter][2]=='1')
		{
			SunType=cSkyVault::CUMULATIVE_SUN;
			counter++;
		}
		else if (argv[counter][0]=='+' && argv[counter][1]=='s' && argv[counter][2]=='2')
		{
			SunType=cSkyVault::MANY_SUNS;
			counter++;
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='d' )
		{
			DoDiffuse=false;
			counter++;
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='G' )
		{
			ClimateFileFormat=cClimateFile::GLOBAL_DIFFUSE;
			counter++;
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='B' )
		{
			ClimateFileFormat=cClimateFile::DIRECTHORIZONTAL_DIFFUSE;
			counter++;
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='l' )
		{
			DoIlluminance=true;
			counter++;
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='a')
		{
			if ((argc-counter)>2)
			{
				sky.SetLatitude(atof(argv[counter+1])*M_PI/180.);
				counter+=2;
			}
			else
			{
				goto USAGEERROR;
			}
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='h')
		{
			if ((argc-counter)>2)
			{
				// hourshift - used to alter the default time convention
				hourshift=atof(argv[counter+1]);
				counter+=2;
			}
			else
			{
				goto USAGEERROR;
			}
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='o' && (argc-counter)>2)
		{
			if ((argc-counter)>2)
			{
				sky.SetLongitude(atof(argv[counter+1])*M_PI/180.);
				counter+=2;
			}
			else
			{
				goto USAGEERROR;
			}
		}
		else if (argv[counter][0]=='-' && argv[counter][1]=='m' && (argc-counter)>2)
		{
			if ((argc-counter)>2)
			{
				sky.SetMeridian(atof(argv[counter+1])*M_PI/180.);
				counter+=2;
			}
			else
			{
				goto USAGEERROR;
			}
		}
		else
		{
			fprintf(stderr,"gencumulativesky: Error - invalid input parameter '%s'\n\n",argv[counter]);
			goto USAGEERROR;
		}
	}

	if (!sky.LoadClimateFile(filename, ClimateFileFormat))
	{
		fprintf(stderr,"gencumulativesky: Error reading climate file %s\n\n",filename);
		goto USAGEERROR;
	}

	sky.CalculateSky(SunType, DoDiffuse, DoIlluminance, hourshift);
		
	CumSky=sky.GetCumulativeSky();
	sky.GetPatchDetails(patchData);

	printf("{ This .cal file was generated automatically by gencumulativesky }\n");
	printf("{ ");
	for (j=0; j<argc; j++)
		printf("%s ",argv[j]);
	printf(" }\n\n");
	printf("skybright=");
	for (j=0; j<7; j++)
	{
		printf("row%d+",j);
	}
	printf("row7;\n\n");

	counter=0;
	for (j=0; j<7; j++)
	{
		// note first patch split into two parts - first part (> 0 deg) and last patch (<360)
		printf("row%d=if(and(alt-%d, %d-alt),select(floor(0.5+az/%5.2f)+1,\n",j,j*rowdeltaalt,(j+1)*rowdeltaalt,rowdeltaaz[j]);
		for (i=0+counter; i<counter+360/int(rowdeltaaz[j]); i++)
		{
			printf("\t%f,\n",CumSky[i]);
		}
		printf("\t%f),0);\n\n",CumSky[counter]);
		counter+=(int)(360/rowdeltaaz[j]);
	}	

	printf("row7=if(alt-84,%f,0);\n\n",CumSky[144]);

	printf("alt=asin(Dz)*180/PI;\n\n");

	printf("az=if(azi,azi,azi+360);\n");
	printf("azi=atan2(Dx,Dy)*180/PI;\n\n");

	return 0;

USAGEERROR:
	fprintf(stderr,"Usage: gencumulativesky [-d] [+s1|+s2] [-a latitude] [-o longitude] [-l] [-m standard meridian] [-h hourshift] [-G|-B] <climate file>\n");
	fprintf(stderr,"(Note: longitude +ve East of Greenwich)\n\n");
	fprintf(stderr,"\t-d\tIgnore diffuse irradiance\n");
	fprintf(stderr,"\t+s1\tUse \"smeared sun\" approach (default)\n");
	fprintf(stderr,"\t+s2\tUse \"binned sun\" approach\n");
	fprintf(stderr,"\t-l\tOutput luminance instead of radiance\n");
	fprintf(stderr,"\t-G\tFile format is col1=global irradiance (W/m2), col2=diffuse irradiance\n");
	fprintf(stderr,"\t-B\tFile format is col1=direct horizontal irradiance (W/m2), col2=diffuse irradiance\n\n");
	return -1;
}

