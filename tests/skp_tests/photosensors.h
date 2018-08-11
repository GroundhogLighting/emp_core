#pragma once

#include "./common.h"


TEST(SKPRead, photosensors)
{
    
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("photosensors",version);
        
        // No components
        ASSERT_EQ(model.getNumComponentDefinitions(),0);
        
        // Check number of materials
        ASSERT_EQ(model.getNumMaterials(),0);
        
        // Check number of photosensors
        ASSERT_EQ(model.countPhotosensors(),2);
        
        // Check for photosensor 1
        Photosensor * origin = model.getPhotosensorRef("Origin");
        Photosensor * oneM = model.getPhotosensorRef("1m");
        Photosensor * twoMEast = model.getPhotosensorRef("2m east");
        
        ASSERT_TRUE(origin != nullptr);
        ASSERT_TRUE(oneM != nullptr);
        ASSERT_TRUE(twoMEast != nullptr);
        
        // Check positions and directions
        Point3D originP = origin->getPosition();
        Vector3D originD = origin->getDirection();
        ASSERT_EQ(originP.getX(),0);
        ASSERT_EQ(originP.getY(),0);
        ASSERT_EQ(originP.getZ(),0);
        ASSERT_EQ(originD.getX(),0);
        ASSERT_EQ(originD.getY(),0);
        ASSERT_EQ(originD.getZ(),1);
        
        Point3D oneMP = oneM->getPosition();
        Vector3D oneMD = oneM->getDirection();
        ASSERT_EQ(oneMP.getX(),0);
        ASSERT_EQ(oneMP.getY(),0);
        ASSERT_EQ(oneMP.getZ(),1);
        ASSERT_EQ(oneMD.getX(),0);
        ASSERT_EQ(oneMD.getY(),0);
        ASSERT_EQ(oneMD.getZ(),1);
        
        Point3D twoMEastP = twoMEast->getPosition();
        Vector3D twoMEastD = twoMEast->getDirection();
        ASSERT_EQ(twoMEastP.getX(),0);
        ASSERT_EQ(twoMEastP.getY(),0);
        ASSERT_EQ(twoMEastP.getZ(),2);
        ASSERT_EQ(twoMEastD.getX(),1);
        ASSERT_EQ(twoMEastD.getY(),0);
        ASSERT_EQ(twoMEastD.getZ(),0);
        
        
    }
}


