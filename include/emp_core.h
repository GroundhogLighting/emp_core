#ifndef INCLUDE_EMP_CORE_H
#define INCLUDE_EMP_CORE_H

#include "../src/groundhogmodel/groundhogmodel.h"
#include "../src/calculations/oconv_options.h"
#include "../src/writers/rad/radexporter.h"
#include "../src/common/taskmanager/taskmanager.h"
#include "../src/config_constants.h"
#include "../src/common/taskmanager/mutexes.h"


// Extern classes
class GroundhogModel;
class TaskManager;
class RTraceOptions;
class OconvOptions;
class RadExporter;

// Calculation Tasks
#include "../src/calculations/tasks/AddSkyToOctree.h"
class AddSkyToOctree;

#include "../src/calculations/tasks/Calculate4CMDirectSkyMatrix.h"
class Calculate4CMDirectSkyMatrix;

#include "../src/calculations/tasks/Calculate4CMNaiveDirectSkyMatrix.h"
class Calculate4CMNaiveDirectSkyMatrix;

#include "../src/calculations/tasks/CalculateDDCDirectSkyMatrix.h"
class CalculateDDCDirectSkyMatrix;

#include "../src/calculations/tasks/CalculateDDCDirectSunPatchIlluminance.h"
class CalculateDDCDirectSunPatchIlluminance;

#include "../src/calculations/tasks/CalculateDDCGlobalIlluminance.h"
class CalculateDDCGlobalIlluminance;

#include "../src/calculations/tasks/CalculateDDCGlobalMatrix.h"
class CalculateDDCGlobalMatrix;

#include "../src/calculations/tasks/CalculateDaylightFactor.h"
class CalculateDaylightFactor;

#include "../src/calculations/tasks/CalculateDirectSunIlluminance.h"
class CalculateDirectSunIlluminance;

#include "../src/calculations/tasks/CalculateDirectSunMatrix.h"
class CalculateDirectSunMatrix;

#include "../src/calculations/tasks/Create4CMDirectSkyOctree.h"
class Create4CMDirectSkyOctree;

#include "../src/calculations/tasks/Create4CMNaiveDirectSkyOctree.h"
class Create4CMNaiveDirectSkyOctree;

#include "../src/calculations/tasks/CreateDDCDirectSkyOctree.h"
class CreateDDCDirectSkyOctree;

#include "../src/calculations/tasks/CreateDDCGlobalOctree.h"
class CreateDDCGlobalOctree;

#include "../src/calculations/tasks/CreateDaylightFactorOctree.h"
class CreateDaylightFactorOctree;

#include "../src/calculations/tasks/CreateDirectSunOctree.h"
class CreateDirectSunOctree;

#include "../src/calculations/tasks/OconvTask.h"
class OconvTask;

#include "../src/calculations/tasks/RTraceTask.h"
class RTraceTask;

#include "../src/calculations/tasks/TriangulateWorkplane.h"
class TriangulateWorkplane;

#include "../src/calculations/tasks/do4CM.h"
class do4CM;

#include "../src/calculations/tasks/doDDC.h"
class doDDC;

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
