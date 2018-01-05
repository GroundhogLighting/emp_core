
#include <stdio.h>
#include <fstream>

#include "calculations/radiance.h"
#include "os_definitions.h"

TEST(RadianceTest, calcDF)
{
    // Write an octree

    std::string octname = "./test_outputs/octree.oct";
    std::string command = "oconv - > " + octname + " 2> " + octname + ".err";


    FILE *octree = POPEN(&command[0], "w");
    fprintf(octree, "!gensky -ang 45 40 -c -B %f -g 0.2\n",100.0/179.0);
    fprintf(octree, RADIANCE_SKY_COMPLEMENT);
    PCLOSE(octree);

    /* RAY TRACE */

    // Build a pseudo-workplane
    Polygon3D p = Polygon3D();
    p.setNormal(Vector3D(0, 0, 1));
    Loop * l = p.getOuterLoopRef();
    l->addVertex(new Point3D(-1, -1, 1));
    l->addVertex(new Point3D(1, -1, 1));
    l->addVertex(new Point3D(1, 1, 1));
    l->addVertex(new Point3D(-1, 1, 1));
    Triangulation t = Triangulation(&p);
    
    // Mesh
    t.mesh(0.05, 1.3);
    t.purge();

    // Create options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 2);
    options.setOption("ad", 7000);

    size_t nTriangles = t.getNumTriangles();
    std::vector<RAY> rays = std::vector<RAY>(nTriangles);
   
    std::string amb = "./test_outputs/aa.amb";
    
    rtrace_I(&t, &options, &octname[0], &amb[0], &rays);
    remove(&amb[0]);

    // Use results
    for(auto ray:rays){
      ASSERT_GT(0.1, std::abs(LIGHT(ray.rcol) - 100.0));
    }
    remove(&amb[0]);


}

