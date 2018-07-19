

TEST(FourCM, UnitDirectSkyLightSources)
{
    TaskManager tm = TaskManager();
    GroundhogModel model = GroundhogModel();
    
    Location * location = model.getLocation();
    location->setLatitude(33.0);
    
    CreateDirectSunOctree * task = new CreateDirectSunOctree(&model,6);
    tm.addTask(task);
    
    tm.solve();
    
    std::string octname = task->octreeName;
    
    //RAY TRACE
    
    FVECT origin = {0,0,0};
    FVECT dir = {0,0,1};
    std::vector<RAY> rays = std::vector<RAY>(1);
    
    VCOPY(rays.at(0).rorg, origin);
    VCOPY(rays.at(0).rdir, dir);
    
    // Create options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 0);
    options.setOption("dc", 1);
    options.setOption("dt", 0);
    
    
    //std::string amb = "./test_outputs/aa.amb";
    //ColorMatrix result = ColorMatrix(1,1);
    //rtrace_I(&options, &octname[0], &amb[0], &rays,&result);
    
    //remove(&amb[0]);
    
    //ASSERT_NEAR(result.redChannel()->getElement(0,0),PI,1e-3);
}

/*

TEST(FourCM, DirectSkyMatrix)
{
    
    GroundhogModel model = GroundhogModel();
    
    Create4CMDirectSkyOctree task =  Create4CMDirectSkyOctree(&model);
    task.solve();
    
    std::string octname = task.octreeName;
    
    // RAY TRACE
    
    FVECT origin = {0,0,0};
    FVECT dir = {0,0,1};
    std::vector<RAY> rays = std::vector<RAY>(1);
    
    VCOPY(rays.at(0).rorg, origin);
    VCOPY(rays.at(0).rdir, dir);
    
    // Create options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 0);
    options.setOption("dc", 1);
    options.setOption("dt", 0);
    
    const int mf = 1;
    
    // Calculate
    ColorMatrix results = ColorMatrix(1,nReinhartBins(mf));
    rcontrib(&options, &octname[0], false, true, &rays, mf, "element", true, &results);
    
    // Check ground
    ASSERT_EQ(0.0,(*results.redChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results.greenChannel())[0]->at(0));
    ASSERT_EQ(0.0,(*results.blueChannel())[0]->at(0));
    
    // Compare with approximated analytical solution
    double solidAngle;
    for(size_t i=1; i < nReinhartBins(mf); i++){
        Vector3D a = reinhartCenterDir(i, mf, &solidAngle);
        double altitude = asin(a.getZ());
        double v = sin(altitude)*solidAngle;
        
        double maxError = 1.4; // percent
        maxError /= 100.0;
        
        ASSERT_NEAR(v,(*results.redChannel())[0]->at(i),maxError*v);
        ASSERT_NEAR(v,(*results.greenChannel())[0]->at(i),maxError*v);
        ASSERT_NEAR(v,(*results.blueChannel())[0]->at(i),maxError*v);
        
    }
}
*/
