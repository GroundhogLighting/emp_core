

#include "calculations/tasks/doDDC.h"


TEST(DDC_TEST,Empty_CalculateDDCGlobalMatrix){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
        
    // Create Task
    CalculateDDCGlobalMatrix * calcTask = new CalculateDDCGlobalMatrix(&emptyModel, &rays, skyMF, &options);
    
    // Add task and solve
    tm.addTask(calcTask);
    tm.solve(NULL);
    
    ColorMatrix * results = &(calcTask->result);
        
    // ASSES
    // Check ground
    ASSERT_EQ(0.0,(*results->redChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results->greenChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results->blueChannel())[0]->at(0));
    
    // Compare with approximated analytical solution
    double solidAngle;
    for(size_t i=1; i < nReinhartBins(skyMF); i++){
        Vector3D a = reinhartCenterDir(i, skyMF, &solidAngle);
        double altitude = asin(a.getZ());
        double v = sin(altitude)*solidAngle;                
        
        ASSERT_NEAR(v,(*results->redChannel())[0]->at(i),1e-2);
        ASSERT_NEAR(v,(*results->greenChannel())[0]->at(i),1e-2);
        ASSERT_NEAR(v,(*results->blueChannel())[0]->at(i),1e-2);
        
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
    ASSERT_EQ(0.0,(*results->redChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results->greenChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results->blueChannel())[0]->at(0));
    
    // Compare with approximated analytical solution
    double solidAngle;
    for(size_t i=1; i < nReinhartBins(skyMF); i++){
        Vector3D a = reinhartCenterDir(i, skyMF, &solidAngle);
        double altitude = asin(a.getZ());
        double v = sin(altitude)*solidAngle;
        
        ASSERT_NEAR(v,(*results->redChannel())[0]->at(i),1e-2);
        ASSERT_NEAR(v,(*results->greenChannel())[0]->at(i),1e-2);
        ASSERT_NEAR(v,(*results->blueChannel())[0]->at(i),1e-2);        
    }
}


TEST(DDC_TEST, Empty_global_DDC_vs_reference){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
   
    // Create DDC Task
    CalculateDDCGlobalIlluminance * task = new CalculateDDCGlobalIlluminance(&emptyModel, &rays, skyMF, &options);
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
        //std::cout << value << " " << reference << std::endl;
        ASSERT_NEAR(value,reference,reference * 0.05);
    }
}


TEST(DDC_TEST, Simple_global_DDC_vs_reference){
#include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCGlobalIlluminance * task = new CalculateDDCGlobalIlluminance(&simpleModel, &rays, skyMF, &options);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = simpleReference[i][0];
        //std::cout << value << " " << reference << std::endl;
        ASSERT_NEAR(value,reference,reference * 0.05);
    }
}


TEST(DDC_TEST, Empty_directSunPatch_DDC_vs_reference){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDDCDirectSunPatchIlluminance * task = new CalculateDDCDirectSunPatchIlluminance(&emptyModel, &rays, skyMF, &options);
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
    CalculateDDCDirectSunPatchIlluminance * task = new CalculateDDCDirectSunPatchIlluminance(&simpleModel, &rays, skyMF, &options);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = simpleReference[i][1];
        //std::cout << value << " " << reference << std::endl;
        ASSERT_NEAR(value,reference,0.05*reference);
    }
}



TEST(DDC_TEST, Empty_directSharpSun_DDC_vs_reference){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    CalculateDirectSunIlluminance * task = new CalculateDirectSunIlluminance(&emptyModel, &rays, sunMF, &options);
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
    CalculateDirectSunIlluminance * task = new CalculateDirectSunIlluminance(&simpleModel, &rays, sunMF, &options);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Calculate Irradiance
    Matrix irradiance = Matrix(rays.size(),48);
    task->result.calcIrradiance(&irradiance);
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = irradiance.getElement(0,i);
        double reference = simpleReference[i][2];
        
        ASSERT_NEAR(value,reference,0.03*reference);
    }
}


TEST(DDC_TEST,Empty_full_DDC_vs_RTRACE){
    #include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    doDDC * task = new doDDC(&emptyModel, &rays, sunMF, skyMF, &options);
    
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    // Compare to reference Solution
    for(int i=0; i<48; i++){
        double value = task->result.getElement(0,i);
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
    doDDC * task = new doDDC(&simpleModel, &rays, sunMF, skyMF, &options);
    tm.addTask(task);
    
    // Solve
    tm.solve();
    
    
    
    // Get matrices
    int k=0;
    ColorMatrix * global = &(static_cast<CalculateDDCGlobalIlluminance *>(task->getDependencyRef(k++))->result);
    ColorMatrix * directSunPatch = &(static_cast<CalculateDDCDirectSunPatchIlluminance *>(task->getDependencyRef(k++))->result);
    ColorMatrix * directSun = &(static_cast<CalculateDirectSunIlluminance *>(task->getDependencyRef(k++))->result);
    
    // Calculate
    Matrix * globalRed =   global->redChannel();
    Matrix * directSunPatchRed =   directSunPatch->redChannel();
    Matrix * directSunRed =   directSun->redChannel();
    
    // Compare to reference Solution
    for(int i=0; i < 48; i++){
        double value = task->result.getElement(0,i);
        double reference = 179.0*simpleReference[i][3];
        
        ASSERT_NEAR(value,reference,reference*0.055);
        ASSERT_NEAR(globalRed->getElement(0,i)        ,simpleReference[i][0] ,simpleReference[i][0]*0.055);
        ASSERT_NEAR(directSunPatchRed->getElement(0,i),simpleReference[i][1] ,simpleReference[i][1]*0.055);
        ASSERT_NEAR(directSunRed->getElement(0,i)     ,simpleReference[i][2] ,simpleReference[i][2]*0.055);           
    }
}



TEST(DDC_TEST,TEST_BENCHMARK){
#include "./RTrace_reference.h"
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Create DDC Task
    doDDC * task = new doDDC(&simpleModel, &rays, sunMF, skyMF, &options);
    tm.addTask(task);
    
    //
    RTraceOptions rtraceoptions = RTraceOptions();
    rtraceoptions.setOption("ab",5);
    rtraceoptions.setOption("ad",5000);
    rtraceoptions.setOption("lw",0.000001);

    
    OconvOptions ops = OconvOptions();
    std::vector<RTraceTask *> rtraces = std::vector<RTraceTask *>();
    // Compare to reference Solution
    for(int i=0; i < 48; i++){
        
        std::string month = std::to_string(wea[i][0]);
        std::string day = std::to_string(wea[i][1]);
        std::string hour = std::to_string(wea[i][2]);
        std::string dir = std::to_string(wea[i][3]);
        std::string dif = std::to_string(wea[i][4]);
        std::string lat = std::to_string(latitude);
        std::string lon = std::to_string(longitude);
        std::string meridian = std::to_string(-15*time_zone);
        
        std::string sky = "gendaylit "+month+" "+day+" "+hour+" -W "+dir+" "+dif+" -a "+lat+" -o "+lon+" -m "+meridian + " -i 60 ";
        RTraceTask * localTask = new RTraceTask(&simpleModel, &rtraceoptions, &rays , &ops, sky);
        rtraces.push_back(localTask);
        
        // Skip night
        if(wea[i][4] < 1e-4){
            localTask->result.redChannel()  ->resize(1,1);
            localTask->result.greenChannel()->resize(1,1);
            localTask->result.blueChannel() ->resize(1,1);
            
            localTask->result.redChannel()->setElement(0,0,0);
            localTask->result.greenChannel()->setElement(0,0,0);
            localTask->result.blueChannel()->setElement(0,0,0);
            
            continue;
        }
        
        tm.addTask(localTask);
        
    }
    
    
    // Solve
    tm.solve();
    
    
    
    // Get matrices
    int k=0;
    ColorMatrix * global = &(static_cast<CalculateDDCGlobalIlluminance *>(task->getDependencyRef(k++))->result);
    ColorMatrix * directSunPatch = &(static_cast<CalculateDDCDirectSunPatchIlluminance *>(task->getDependencyRef(k++))->result);
    ColorMatrix * directSun = &(static_cast<CalculateDirectSunIlluminance *>(task->getDependencyRef(k++))->result);
    
    // Calculate
    Matrix * globalRed =   global->redChannel();
    Matrix * directSunPatchRed =   directSunPatch->redChannel();
    Matrix * directSunRed =   directSun->redChannel();
    
    
    // Compare to reference Solution
    for(int i=0; i < 48; i++){
        
        double r = rtraces.at(i)->result.redChannel()->  getElement(0,0);
        double g = rtraces.at(i)->result.greenChannel()->getElement(0,0);
        double b = rtraces.at(i)->result.blueChannel()-> getElement(0,0);
        
        double value = task->result.getElement(0,i);
        double reference = 179.0*simpleReference[i][3];
        double rtrace = 47.5*r + 119.95*g + 11.60*b;
        std::cout << reference << " " << rtrace << std::endl;
        /*
        if(reference > 1e-4){
            ASSERT_NEAR(value,reference,reference*0.055);
            ASSERT_NEAR(rtrace,reference,reference*0.055);
            ASSERT_NEAR(globalRed->getElement(0,i)        ,simpleReference[i][0] ,simpleReference[i][0]*0.055);
            ASSERT_NEAR(directSunPatchRed->getElement(0,i),simpleReference[i][1] ,simpleReference[i][1]*0.055);
            ASSERT_NEAR(directSunRed->getElement(0,i)     ,simpleReference[i][2] ,simpleReference[i][2]*0.055);
        }
         */
        
    }
    
    
    
    
    
}

