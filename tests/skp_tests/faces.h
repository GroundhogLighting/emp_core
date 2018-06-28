#pragma once

#include "./common.h"


TEST(SKPRead, small_square)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("small_square",version);
    
        // Only one layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,2);
    
        // Check names
        ASSERT_TRUE(*model.getLayerRef(0)->getName() == "Layer0");
        ASSERT_TRUE(*model.getLayerRef(1)->getName() == "Layer1");
    
        // Count components in the component
        size_t nComponents = model.getNumComponentDefinitions();
        ASSERT_EQ(nComponents,0);
    
        // Check the face
        Otype * o = model.getLayerRef(0)->getObjectRef(0);
        Loop * loop = static_cast<Face *>(o)->getOuterLoopRef();
        size_t nVertices = loop->size();
        ASSERT_EQ(nVertices,4);
    
        Point3D a = loop->getVertexRef(0);
        Point3D b = loop->getVertexRef(1);
        Point3D c = loop->getVertexRef(2);
        Point3D d = loop->getVertexRef(3);
    
        ASSERT_TRUE(a.isEqual(Point3D(0,0,0)) || a.isEqual(Point3D(0,1,0)) || a.isEqual(Point3D(1,0,0)) || a.isEqual(Point3D(1,1,0)));
        ASSERT_TRUE(b.isEqual(Point3D(0,0,0)) || b.isEqual(Point3D(0,1,0)) || b.isEqual(Point3D(1,0,0)) || b.isEqual(Point3D(1,1,0)));
        ASSERT_TRUE(c.isEqual(Point3D(0,0,0)) || c.isEqual(Point3D(0,1,0)) || c.isEqual(Point3D(1,0,0)) || c.isEqual(Point3D(1,1,0)));
        ASSERT_TRUE(d.isEqual(Point3D(0,0,0)) || d.isEqual(Point3D(0,1,0)) || d.isEqual(Point3D(1,0,0)) || d.isEqual(Point3D(1,1,0)));
    
    
        // Check number of materials... only Default one
        ASSERT_EQ(model.getNumMaterials(),1);
    }
    
}



TEST(SKPRead, face_with_holes)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("face_with_holes",version);
        
        // Only one layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,1);
        
        // Check names
        ASSERT_TRUE(*model.getLayerRef(0)->getName() == "Layer0");
        
        // Count components in the component
        size_t nComponents = model.getNumComponentDefinitions();
        ASSERT_EQ(nComponents,0);
        
        // Check the face
        Face * face = static_cast<Face *>(model.getLayerRef(0)->getObjectRef(0));
        ASSERT_TRUE(face->hasInnerLoops());
        
        ASSERT_EQ(face->polygon->countInnerLoops(),7);
        
        
        
        // Check number of materials... only Default one
        ASSERT_EQ(model.getNumMaterials(),1);
    }
    
}

