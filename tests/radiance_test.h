
#include <stdio.h>
#include <fstream>

#include "calculations/radiance.h"
#include "calculations/tasks.h"
#include "os_definitions.h"
#include "groundhogmodel.h"

TEST(RTraceTest, calcDF)
{
    // Write an octree

    std::string octname = "./test_outputs/octree.oct";
    std::string command = "oconv - > " + octname + " 2> " + octname + ".err";


    FILE *octree = POPEN(&command[0], "w");
    fprintf(octree, "!gensky -ang 45 40 -c -B %f -g 0.2\n",100.0/179.0);
    fprintf(octree, RADIANCE_SKY_COMPLEMENT);
    PCLOSE(octree);

    /* RAY TRACE */

    FVECT origin = {0,0,0};
    FVECT dir = {0,0,1};
    std::vector<RAY> rays = std::vector<RAY>(1);
    
    VCOPY(rays.at(0).rorg, origin);
    VCOPY(rays.at(0).rdir, dir);

    // Create options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 2);
    options.setOption("ad", 7000);

   
    std::string amb = "./test_outputs/aa.amb";
    
    rtrace_I(&options, &octname[0], &amb[0], &rays);

    remove(&amb[0]);
    ASSERT_NEAR(LIGHT(rays.at(0).rcol),100.0,0.1);
    
}



TEST(FourCM, UnitDirectSkyLightSources)
{
    TaskManager tm = TaskManager();
    GroundhogModel model = GroundhogModel();
    
    Create4CMDirectSkyOctree * task = new Create4CMDirectSkyOctree(&model);
    tm.addTask(task);
    
    tm.solve(NULL);
    
    std::string octname = task->octreeName;
    
    /* RAY TRACE */
    
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
    
    
    std::string amb = "./test_outputs/aa.amb";
    
    rtrace_I(&options, &octname[0], &amb[0], &rays);
    
    remove(&amb[0]);
    
    ASSERT_NEAR(RAD(rays.at(0).rcol),PI,1e-3);
}

TEST(FourCM, DirectSkyMatrix)
{
    
    GroundhogModel model = GroundhogModel();
    
    Create4CMDirectSkyOctree task =  Create4CMDirectSkyOctree(&model);
    task.solve();
    
    std::string octname = task.octreeName;
    
    /* RAY TRACE */
    
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

/*
#include <chrono>


#define RANDOM(x) ((double)(rand()%100)/100.0)*x



TEST(FourCM_SERIAL_BENCHMARK, 4CMvsDDC)
{
    
    int sunMF = 6;
    int skyMF = 1;
    size_t nSkyBins = nReinhartBins(skyMF);
    
    RTraceOptions directOptions = RTraceOptions();
    directOptions.setOption("ab", 0);
    directOptions.setOption("dc", 1);
    directOptions.setOption("dt", 0);
    directOptions.setOption("lw",1e-7);
    directOptions.setOption("lr",5);
    
    RTraceOptions globalOptions = RTraceOptions();
    globalOptions.setOption("ab",5);
    globalOptions.setOption("lw",1e-7);
    globalOptions.setOption("lr",5);
    
    // Build a GroundhogModel
    GroundhogModel model = GroundhogModel();
    std::string layerName = "Layer 1";
    model.addLayer(&layerName);
    
    Material * material = model.addDefaultMaterial();
    
    
    for(size_t nsensors = 1; nsensors < 10000; nsensors *= 10){
        for(size_t nfaces = 1; nfaces < 10000; nfaces *= 10){
            TaskManager * tm = new TaskManager();
            
            for(size_t i=0; i<nfaces; i++){
                Polygon3D * p = new Polygon3D();
                Loop * outerLoop = p->getOuterLoopRef();
                outerLoop->addVertex(new Point3D(RANDOM(20.0),RANDOM(20.0),RANDOM(20.0)));
                outerLoop->addVertex(new Point3D(RANDOM(20.0),RANDOM(20.0),RANDOM(20.0)));
                outerLoop->addVertex(new Point3D(RANDOM(20.0),RANDOM(20.0),RANDOM(20.0)));
                std::string faceName = "face";
                Face * face = new Face(&faceName);
                face->setPolygon(p);
                face->setMaterial(material);
                model.addObjectToLayer(&layerName,face);
            }
            
            
            // Set sensors
            std::vector<RAY> rays = std::vector<RAY>(nsensors);
            for(size_t i=0; i<nsensors; i++){
                FVECT origin = {static_cast<float>(RANDOM(20.0)),static_cast<float>(RANDOM(20.0)),0.1};
                FVECT dir = {0,0,1};
                VCOPY(rays.at(0).rorg, origin);
                VCOPY(rays.at(0).rdir, dir);
            }
            
            
            // ColorMatrix
            ColorMatrix results = ColorMatrix(2,1);
            
            // OCONV
            // Black octree
            RadExporter exporter = RadExporter(&model);
            std::string blackOctreeName = "Oconv1.1.0.0.current.oct";
            OconvOptions blackOconvOptions = OconvOptions();
            blackOconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
            blackOconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);
            blackOconvOptions.setOption(OCONV_SKY, "current");
            blackOconvOptions.setOption(OCONV_INCLUDE_SKY, false);
            blackOconvOptions.setOption(OCONV_LIGHTS_ON, false);
            oconv(&blackOctreeName[0], &blackOconvOptions, exporter);
            
            // Direct sky octree
            CreateDirectSkyOctree directSkyOctreeTask = CreateDirectSkyOctree(&model);
            directSkyOctreeTask.solve();
            
            CreateDirectSunOctree directSunOctreeTask = CreateDirectSunOctree(&model, sunMF);
            directSunOctreeTask.solve();
            
            CreateNaiveDirectSkyOctree naiveDirectSkyOctreeTask = CreateNaiveDirectSkyOctree(&model, skyMF);
            naiveDirectSkyOctreeTask.solve();
            
            
            /// BEGIN TIMING 4CM
            auto begin4CM = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time;
            
            std::string octname = directSkyOctreeTask.octreeName;
            rcontrib(&directOptions, &octname[0], false, true, &rays, skyMF, "element", true, &results);
            auto endDirectSky = std::chrono::high_resolution_clock::now();
            time = endDirectSky - begin4CM;
            std::cerr << " .... Direct sky in " << time.count() << std::endl;
            
            octname = directSunOctreeTask.octreeName;
            rcontrib(&directOptions, &octname[0], false, true, &rays, sunMF, "solar", true, &results);
            auto endDirectSun = std::chrono::high_resolution_clock::now();
            time = endDirectSun - endDirectSky;
            std::cerr << " .... Direct sun in " << time.count() << std::endl;
            
            
            octname = naiveDirectSkyOctreeTask.octreeName;
            rcontrib(&directOptions, &octname[0], false, true, &rays, sunMF, "element", true, &results);
            auto endNaiveDirectSky = std::chrono::high_resolution_clock::now();
            time = endNaiveDirectSky - endDirectSun;
            std::cerr << " .... Naive Direct sky in " << time.count() << std::endl;
            
            // Create direct + indirect -- Original
            //RadExporter exporter = RadExporter(&model);
            std::string octreeName = "full_model.oct";
            OconvOptions oconvOptions = OconvOptions();
            oconv(&octreeName[0], &oconvOptions, exporter);
            Vector3D dir = Vector3D (1,1,1);
            for(int i=0; i<nSkyBins; i++){
                continue;
                std::string thisOctName = std::to_string(i)+"_" + octreeName;
                std::string command = "oconv -i " + octreeName + " - > " + thisOctName;
                std::string amb = std::to_string(i)+".amb";
                
                dir = reinhartCenterDir(i,skyMF);
                FILE *octree = POPEN(&command[0], "w");
                fprintf(octree, "void light element 0 0 3 1 1 1\n");
                fprintf(octree, "element source lightsource 0 0 4 %f %f %f 11.000\n",dir.getX(),dir.getY(),dir.getZ());
                PCLOSE(octree);
                //rtrace_I(&globalOptions, &thisOctName[0], &amb[0], &rays);
                remove(&amb[0]);
                remove(&thisOctName[0]);
            }
            
            // Create direct + indirect -- RCONTRIB
            //RadExporter exporter = RadExporter(&model);
            //std::string octreeName = "full_model.oct";
            //OconvOptions oconvOptions = OconvOptions();
            //oconv(&octreeName[0], &oconvOptions, exporter);
            //Vector3D dir = Vector3D (1,1,1);
            std::string thisOctName = "ALL_BINS_" + octreeName;
            std::string command = "oconv -i " + octreeName + " - > " + thisOctName;
            FILE *octree = POPEN(&command[0], "w");
            for(int i=0; i<nSkyBins; i++){
                std::string amb = std::to_string(i)+".amb";
                dir = reinhartCenterDir(i,skyMF);
                fprintf(octree, "void light element 0 0 3 1 1 1\n");
                fprintf(octree, "element source lightsource 0 0 4 %f %f %f 11.000\n",dir.getX(),dir.getY(),dir.getZ());
                
            }
            PCLOSE(octree);
            rcontrib(&globalOptions, &thisOctName[0], false, true, &rays, skyMF, "element", true, &results);
            
            // add ground
            
            
            auto end4CM = std::chrono::high_resolution_clock::now();
            time = end4CM - endNaiveDirectSky;
            std::cerr << " .... Direct + indirect sky in " << time.count() << std::endl;
            
            
            // DIRECT + DAYLIGHT COEFFICIENTS
            
            RTraceOptions globalDCOptions = RTraceOptions();
            globalDCOptions.setOption("ab",7);
            globalDCOptions.setOption("ad",7400);
            //globalDCOptions.setOption("aa",0.2);
            globalDCOptions.setOption("lw",1e-7);
            globalDCOptions.setOption("lr",5);
            
            // Global illuminance
            auto startDC = std::chrono::high_resolution_clock::now();
            
            // OCONV
            // Full scene octree
            std::string dcOctreeName = "dc_full_model.oct";
            OconvOptions dcOconvOptions = OconvOptions();
            oconv(&dcOctreeName[0], &dcOconvOptions, exporter);
            
            // Black octree
            std::string dcBlackOctreeName = "black_model.oct";
            OconvOptions dcBlackOconvOptions = OconvOptions();
            dcBlackOconvOptions.setOption(OCONV_INCLUDE_WINDOWS, true);
            dcBlackOconvOptions.setOption(OCONV_USE_BLACK_GEOMETRY, true);
            dcBlackOconvOptions.setOption(OCONV_SKY, "current");
            dcBlackOconvOptions.setOption(OCONV_INCLUDE_SKY, false);
            dcBlackOconvOptions.setOption(OCONV_LIGHTS_ON, false);
            oconv(&dcBlackOctreeName[0], &dcBlackOconvOptions, exporter);
            
            thisOctName = "GLOBAL_DC_" + dcOctreeName;
            command = "oconv -i " + dcOctreeName + " - > " + thisOctName;
            octree = POPEN(&command[0], "w");
            fprintf(octree, "void glow ground_glow 0 0 4 1 1 1 0\n");
            fprintf(octree, "ground_glow source ground 0 0 4 0 0 1 360\n");
            //fprintf(octree, "element source lightsource 0 0 4 %f %f %f 11.000\n",dir.getX(),dir.getY(),dir.getZ());
            PCLOSE(octree);
            rcontrib(&globalDCOptions, &thisOctName[0], false, true, &rays, skyMF, "ground_glow", true, &results);
            
            // Direct sun
            octname = directSunOctreeTask.octreeName;
            rcontrib(&directOptions, &octname[0], false, true, &rays, sunMF, "solar", true, &results);
            
            // Direct sky
            thisOctName = "DIRECT_SKY_DC_" + dcBlackOctreeName;
            command = "oconv -i " + dcBlackOctreeName + " - > " + thisOctName;
            octree = POPEN(&command[0], "w");
            fprintf(octree, "void glow ground_glow 0 0 4 1 1 1 0\n");
            fprintf(octree, "ground_glow source ground 0 0 4 0 0 1 360\n");
            //fprintf(octree, "element source lightsource 0 0 4 %f %f %f 11.000\n",dir.getX(),dir.getY(),dir.getZ());
            PCLOSE(octree);
            rcontrib(&globalDCOptions, &thisOctName[0], false, true, &rays, skyMF, "ground_glow", true, &results);
            
            
            
            auto endDC = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> dcTime = endDC - startDC;
            std::chrono::duration<double> FourCMtime = end4CM - begin4CM;
            
            delete tm;
            remove(&thisOctName[0]);
            std::cout << nfaces << "," << nsensors << "," << FourCMtime.count() << "," << dcTime.count() << std::endl;
            std::cerr << nfaces << "," << nsensors << "," << FourCMtime.count() << "," << dcTime.count() << std::endl;
        }
    }
   
    
    
}
 
 */


