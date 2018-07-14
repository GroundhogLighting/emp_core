#ifndef INCLUDE_EMP_CORE_API_H
#define INCLUDE_EMP_CORE_API_H




#include "../src/config_constants.h"
#include "../src/taskmanager/mutexes.h"



// Extern classes

#include "../src/taskmanager/taskmanager.h"
class TaskManager;

#include "../src/groundhogmodel/groundhogmodel.h"
class GroundhogModel;


#include "../src/groundhogmodel/src/rtraceoptions.h"
class RTraceOptions;

#include "../src/calculations/oconv_options.h"
class OconvOptions;

#include "../src/writers/rad/radexporter.h"
class RadExporter;

#include "../src/readers/SKPreader.h"
class SKPreader;

// Calculation Tasks
#include "../src/calculations/tasks/AddSkyToOctree.h"
class AddSkyToOctree;

#include "../src/calculations/tasks/4CM/Calculate4CMDirectSkyMatrix.h"
class Calculate4CMDirectSkyMatrix;

#include "../src/calculations/tasks/4CM/Calculate4CMNaiveDirectSkyMatrix.h"
class Calculate4CMNaiveDirectSkyMatrix;

#include "../src/calculations/tasks/DDC/CalculateDDCDirectSkyMatrix.h"
class CalculateDDCDirectSkyMatrix;

#include "../src/calculations/tasks/DDC/CalculateDDCDirectSunPatchComponent.h"
class CalculateDDCDirectSunPatchComponent;

#include "../src/calculations/tasks/DDC/CalculateDDCGlobalComponent.h"
class CalculateDDCGlobalComponent;

#include "../src/calculations/tasks/DDC/CalculateDDCGlobalMatrix.h"
class CalculateDDCGlobalMatrix;

#include "../src/calculations/tasks/DaylightFactor/CalculateDaylightFactor.h"
class CalculateDaylightFactor;

#include "../src/calculations/tasks/DirectSun/CalculateDirectSolarIlluminance.h"
class CalculateDirectSolarIlluminance;

#include "../src/calculations/tasks/DirectSun/CalculateDirectSunComponent.h"
class CalculateDirectSunComponent;

#include "../src/calculations/tasks/DirectSun/CalculateDirectSunMatrix.h"
class CalculateDirectSunMatrix;

#include "../src/calculations/tasks/4CM/Create4CMDirectSkyOctree.h"
class Create4CMDirectSkyOctree;

#include "../src/calculations/tasks/4CM/Create4CMNaiveDirectSkyOctree.h"
class Create4CMNaiveDirectSkyOctree;

#include "../src/calculations/tasks/DDC/CreateDDCDirectSkyOctree.h"
class CreateDDCDirectSkyOctree;

#include "../src/calculations/tasks/DDC/CreateDDCGlobalOctree.h"
class CreateDDCGlobalOctree;

#include "../src/calculations/tasks/DaylightFactor/CreateDaylightFactorOctree.h"
class CreateDaylightFactorOctree;

#include "../src/calculations/tasks/DirectSun/CreateDirectSunOctree.h"
class CreateDirectSunOctree;

#include "../src/calculations/tasks/OconvTask.h"
class OconvTask;

#include "../src/calculations/tasks/CalculateStaticIlluminance.h"
class CalculateStaticIlluminance;

#include "../src/calculations/tasks/TriangulateWorkplane.h"
class TriangulateWorkplane;

#include "../src/calculations/tasks/4CM/Calculate4CMGlobalIlluminance.h"
class Calculate4CMGlobalIlluminance;

#include "../src/calculations/tasks/DDC/CalculateDDCGlobalIlluminance.h"
class CalculateDDCGlobalIlluminance;

#include "../src/calculations/tasks/DaylightFactor/CheckDFCompliance.h"
class CheckDFCompliance;

#include "../src/calculations/tasks/DirectSun/CheckASECompliance.h"
class CheckASECompliance;

#include "../src/calculations/tasks/CheckDACompliance.h"
class CheckDACompliance;

#include "../src/calculations/tasks/CheckUDICompliance.h"
class CheckUDICompliance;

#include "../src/calculations/tasks/CheckLUXCompliance.h"
class CheckLUXCompliance;

// RadExporter Tasks
#include "../src/writers/rad/tasks/WriteComponentDefinitions.h"
class WriteComponentDefinitions;

#include "../src/writers/rad/tasks/WriteCurrentSky.h"
class WriteCurrentSky;

#include "../src/writers/rad/tasks/WriteCurrentWeather.h"
class WriteCurrentWeather;

#include "../src/writers/rad/tasks/WriteLayers.h"
class WriteLayers;

#include "../src/writers/rad/tasks/WriteMaterials.h"
class WriteMaterials;

#include "../src/writers/rad/tasks/WriteModelInfo.h"
class WriteModelInfo;

#include "../src/writers/rad/tasks/WritePhotosensors.h"
class WritePhotosensors;

#include "../src/writers/rad/tasks/WriteRadianceRifFile.h"
class WriteRadianceRifFile;

#include "../src/writers/rad/tasks/WriteRadianceSceneFile.h"
class WriteRadianceSceneFile;

#include "../src/writers/rad/tasks/WriteViews.h"
class WriteViews;

#include "../src/writers/rad/tasks/WriteWorkplane.h"
class WriteWorkplane;


// String utils methods
bool stringInclude(std::string word, std::string substring);
bool stringInclude(const char * word, std::string substring);
bool stringInclude(const char * word, const char * substring);
bool stringIncludeAny(std::string word, const char ** substringArray, int nItems);
bool stringIncludeAny(const char * word, const char ** substringArray, int nItems);
void utf8toASCII(char * input, size_t inputLength, char * output, size_t * outputLength);
void fixString(char * s, size_t stringLength);
std::string size_tToString(size_t sz);
void tokenize(std::string * s, std::vector<std::string> * v);
bool is_number(const std::string& s);

// Basic IO
void warn(const char * message);
void fatal(const char * message, int ln, const char * file);
void inform(const char * message, bool verbose);
void warnNoMaterial(const char * type , char * name);

// Basic File functionality
bool createdir(std::string dirname);
bool fexists(std::string filename);
bool dexist(std::string dirname);
bool isDir(std::string dirname);


#endif
