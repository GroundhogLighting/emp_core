#include <stdio.h>
#include <stdlib.h>

class cClimateFile
{
public:
	enum eClimateFileFormat { GLOBAL_DIFFUSE, DIRECTHORIZONTAL_DIFFUSE };

	cClimateFile(void);
	~cClimateFile(void);

	// read in climate file
	// must specify the hour numbering convention -1 = hour beginning, 0 = centered, 1 = hour ending
	bool ReadClimateFile(char *FileName, int HourConvention, eClimateFileFormat ClimateFileFormat);

	// check that the currently stored data is valid
	bool ValidateData();

	// return direct radiation for a given day (Ibh=Igh-Idh)
	// day 1 = 1st jan, hour is 0-23
	double GetDirectRad(double hour, int day);
	double GetDiffuseRad(double hour, int day);
	double GetGlobalRad(double hour, int day);

private:
	// climate data
	double *m_ptIgh, *m_ptIdh;
	// number of data points (almost certainly = 24*365=8760)
	int m_NumPoints;

	FILE *LoadFile(char *fname);

};
