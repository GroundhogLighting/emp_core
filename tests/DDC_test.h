
#include "../include/emp_core.h"
#include <algorithm>


TEST(DDC_TEST,Empty_CalculateDDCGlobalMatrix){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
        
    // Create Task
    CalculateDDCGlobalMatrix * calcTask = new CalculateDDCGlobalMatrix(&emptyModel, &rays, skyMF, &options);
    
    // Add task and solve
    tm.addTask(calcTask);
    
    tm.solve();
    
    
    ColorMatrix * results = calcTask->getResult();
        
    // ASSES
    // Check ground
    ASSERT_EQ(0.0,results->redChannel()->getElement(0,0));
    ASSERT_EQ(0.0,results->greenChannel()->getElement(0,0));
    ASSERT_EQ(0.0,results->blueChannel()->getElement(0,0));
    
    // Compare with approximated analytical solution
    double solidAngle;
    for(size_t i=1; i < nReinhartBins(skyMF); i++){
        Vector3D a = reinhartCenterDir(i, skyMF, &solidAngle);
        double altitude = asin(a.getZ());
        double v = sin(altitude)*solidAngle;                
        
        ASSERT_NEAR(v,results->redChannel()->getElement(0,i),1e-2);
        ASSERT_NEAR(v,results->greenChannel()->getElement(0,i),1e-2);
        ASSERT_NEAR(v,results->blueChannel()->getElement(0,i),1e-2);
        
    }
}


TEST(DDC_TEST,Empty_CalculateDDCDirectSkyMatrix){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create Task
    CalculateDDCDirectSkyMatrix * calcTask = new CalculateDDCDirectSkyMatrix(&emptyModel, &rays, skyMF, &options);
    
    // Add task and solve
    tm.addTask(calcTask);
    tm.solve(NULL);
    
    ColorMatrix * results = &(calcTask->result);
    
    
    // ASSES
    // Check ground
    ASSERT_EQ(0.0,results->redChannel()->getElement(0,0));
    ASSERT_EQ(0.0,results->greenChannel()->getElement(0,0));
    ASSERT_EQ(0.0,results->blueChannel()->getElement(0,0));
    
    // Compare with approximated analytical solution
    double solidAngle;
    for(size_t i=1; i < nReinhartBins(skyMF); i++){
        Vector3D a = reinhartCenterDir(i, skyMF, &solidAngle);
        double altitude = asin(a.getZ());
        double v = sin(altitude)*solidAngle;
        
        ASSERT_NEAR(v,results->redChannel()->getElement(0,i),1e-2);
        ASSERT_NEAR(v,results->greenChannel()->getElement(0,i),1e-2);
        ASSERT_NEAR(v,results->blueChannel()->getElement(0,i),1e-2);
    }
}


TEST(DDC_TEST, Empty_global_DDC_vs_reference){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
   
    // Create DDC Task
    CalculateDDCGlobalComponent * task = new CalculateDDCGlobalComponent(&emptyModel, &rays, skyMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = emptyReference[i][0];
        
        //std::cout << value << "," << reference << std::endl;
        ASSERT_NEAR(value,reference,max(reference * 0.05, 5.0));
    }
}


TEST(DDC_TEST, Simple_global_DDC_vs_reference){
#include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCGlobalComponent * task = new CalculateDDCGlobalComponent(&simpleModel, &rays, skyMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    
    // Compare to reference Solution
    double msd = 0;
    int count = 0;
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = simpleReference[i][0];
        //std::cout << value << "," << reference << std::endl;
        if(reference > 1e-3){
            double v = ((value - reference)/reference);
            msd += (v*v);
            count++;
        }
        ASSERT_NEAR(value,reference,max(reference * 0.05, 5.0));
    }
    ASSERT_TRUE( std::sqrt(msd/(double)count) < 0.05);
}


TEST(DDC_TEST, Empty_directSunPatch_DDC_vs_reference){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCDirectSunPatchComponent * task = new CalculateDDCDirectSunPatchComponent(&emptyModel, &rays, skyMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = emptyReference[i][1];
        //std::cout << value << " " << reference << std::endl;
        ASSERT_NEAR(value,reference,0.6*reference);
    }
}


TEST(DDC_TEST, Simple_directSunPatch_DDC_vs_reference){
#include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCDirectSunPatchComponent * task = new CalculateDDCDirectSunPatchComponent(&simpleModel, &rays, skyMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    double msd = 0;
    int count = 0;
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = simpleReference[i][1];
        if(reference > 1e-3){
            double v = ((value - reference)/reference);
            msd += (v*v);
            count++;
        }
        ASSERT_NEAR(value,reference, max(reference * 0.05, 5.0) );
    }
    ASSERT_TRUE( std::sqrt(msd/(double)count) < 0.05);
    
}



TEST(DDC_TEST, Empty_directSharpSun_DDC_vs_reference){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDirectSunComponent * task = new CalculateDirectSunComponent(&emptyModel, &rays, sunMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = emptyReference[i][2];
        
        ASSERT_NEAR(value,reference,0.03*reference);
    }
}


TEST(DDC_TEST, Simple_directSharpSun_DDC_vs_reference){
#include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDirectSunComponent * task = new CalculateDirectSunComponent(&simpleModel, &rays, sunMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    double msd = 0;
    int count = 0;
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = simpleReference[i][2];
        if(reference > 1e-3){
            double v = ((value - reference)/reference);
            msd += (v*v);
            count++;
        }
        ASSERT_NEAR(value,reference,max(reference * 0.05, 5.0));
    }
    ASSERT_TRUE( std::sqrt(msd/(double)count) < 0.05);
    
}


TEST(DDC_TEST,Empty_full_DDC_vs_RTRACE){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCGlobalIlluminance * task = new CalculateDDCGlobalIlluminance(&emptyModel, &rays, sunMF, skyMF, &options,1);
    
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = task->getResult()->getElement(0,i);
        double reference = 179.0*emptyReference[i][3];
        //std::cout << value << " " << reference << std::endl;
        ASSERT_NEAR(value,reference,reference*0.035);
    }
    
}


TEST(DDC_TEST,Simple_full_DDC_vs_RTRACE){
#include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCGlobalIlluminance * task = new CalculateDDCGlobalIlluminance(&simpleModel, &rays, sunMF, skyMF, &options,1);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    
    // Compare to reference Solution
    double msd = 0;
    int count = 0;
    for(int i=0; i<48; i++){
        double value = task->getResult()->getElement(0,i);
        double reference = 179.0*simpleReference[i][3];
        if(reference > 1e-3){
            double v = ((value - reference)/reference);
            msd += (v*v);
            count++;
        }
        ASSERT_NEAR(value,reference,max(reference * 0.05, 5.0));
    }
    ASSERT_TRUE( std::sqrt(msd/(double)count) < 0.05);
    
}

