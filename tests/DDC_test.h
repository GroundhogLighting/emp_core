

#include "CalculateDDCGlobalMatrix.h"


TEST(DDC_TEST,CalculateDDCGlobalMatrix){
    
    int mf = 1;
    
    // Build task manager
    TaskManager tm = TaskManager();
    
    // Build rays
    FVECT origin = {0,0,0};
    FVECT dir = {0,0,1};
    std::vector<RAY> rays = std::vector<RAY>(1);
    VCOPY(rays.at(0).rorg, origin);
    VCOPY(rays.at(0).rdir, dir);
    
    // Create model
    GroundhogModel model = GroundhogModel();
    
    // Create Task
    RTraceOptions options = RTraceOptions();
    options.setOption("ab",1);
    options.setOption("ad",10000);
    options.setOption("lw",0.000001);
    
    
    
    CalculateDDCGlobalMatrix * calcTask = new CalculateDDCGlobalMatrix(&model, &rays, mf, &options);
    
    // Add task and solve
    tm.addTask(calcTask);
    tm.solve(NULL);
    
    ColorMatrix * results = &(calcTask->result);
    
    results->redChannel()->print();
    
    // ASSES
    // Check ground
    ASSERT_EQ(0.0,(*results->redChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results->greenChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results->blueChannel())[0]->at(0));
    
    // Compare with approximated analytical solution
    double solidAngle;
    for(size_t i=1; i < nReinhartBins(mf); i++){
        Vector3D a = reinhartCenterDir(i, mf, &solidAngle);
        double altitude = asin(a.getZ());
        double v = sin(altitude)*solidAngle;
        
        std::cout << (*results->redChannel())[0]->at(i) << "," << v << std::endl;
        
        ASSERT_NEAR(v,(*results->redChannel())[0]->at(i),1e-2);
        ASSERT_NEAR(v,(*results->greenChannel())[0]->at(i),1e-2);
        ASSERT_NEAR(v,(*results->blueChannel())[0]->at(i),1e-2);
        
    }
}
