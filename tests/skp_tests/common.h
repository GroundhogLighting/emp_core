
#pragma once

#include "../include/emp_core.h"

#define RELATIVE_LOCATION "/Users/german/Documents/emp_core" // ../..

#define ASSERT_LOAD_MODEL(modelName,version) \
EmpModel model = EmpModel(); \
SKPreader reader = SKPreader(&model,false); \
std::string name=std::string(RELATIVE_LOCATION)+"/tests/skp_files/"+std::to_string(version)+"/"+std::string(modelName)+".skp"; \
ASSERT_NO_THROW(reader.parseSKPModel(name));

#define FOR_ALL_VERSIONS std::vector<int> skpVersions = {2017}; \
for(int version : skpVersions)

