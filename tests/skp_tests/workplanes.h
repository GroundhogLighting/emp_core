#pragma once

#include "./common.h"


TEST(SKPRead, small_square_WP)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("small_square_WP",version);
        
        // Only two layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,2);
        
        // Check names
        ASSERT_EQ(model.getLayerRef(0)->getName(), "Layer0");
        ASSERT_EQ(model.getLayerRef(1)->getName(), "Layer1");
        
        // Count components in the component
        size_t nComponents = model.getNumComponentDefinitions();
        ASSERT_EQ(nComponents,0);
        
        // Check number of workplanes
        ASSERT_EQ(1,model.getNumWorkplanes());
        
        // Check the Workplane
        Workplane * wp = model.getWorkplaneRef(0);
        std::string wpName = wp->getName();
        
        ASSERT_EQ(wpName, std::string("WP"));
        
        Loop * loop = wp->getPolygonRef(0)->getOuterLoopRef();
        
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
        
        
        // Check number of materials... No default material because no faces
        ASSERT_EQ(model.getNumMaterials(),0);
        
        // Check pixel size
        ASSERT_EQ(0.76,wp->getMaxArea());
    }    
    
}


TEST(SKPRead, workplane_group)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("workplane_group",version);
        
        // Only two layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,1);
        
        // Check names
        ASSERT_EQ(model.getLayerRef(0)->getName(), "Layer0");
        
        // CHeck geometry
        ASSERT_EQ(0,model.getLayerRef(0)->getObjectsRef()->size());
        
        // Count components in the component
        size_t nComponents = model.getNumComponentDefinitions();
        ASSERT_EQ(nComponents,0);
        
        // Check number of workplanes
        ASSERT_EQ(1,model.getNumWorkplanes());
        
        // Check the Workplane
        Workplane * wp = model.getWorkplaneRef(0);
        std::string wpName = wp->getName();
        ASSERT_EQ(wpName, std::string("WP"));
        
        ASSERT_EQ(3,wp->getNumPolygons());
        
        
        // Check number of materials... No default material because no faces
        ASSERT_EQ(model.getNumMaterials(),0);
    }
    
}



TEST(SKPRead, ugly_workplane)
{
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("ugly_workplane",version);
        
        // Only two layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,1);
        
        // Check names
        ASSERT_EQ(model.getLayerRef(0)->getName(), "Layer0");
        
        
        // Count components in the component
        size_t nComponents = model.getNumComponentDefinitions();
        ASSERT_EQ(nComponents,0);
        
        // Check number of workplanes
        ASSERT_EQ(1,model.getNumWorkplanes());
        
        // Check the Workplane
        Workplane * wp = model.getWorkplaneRef(0);
        std::string wpName = wp->getName();
        ASSERT_EQ(wpName, std::string("WP"));
        
        ASSERT_EQ(1,wp->getNumPolygons());
        
        Polygon3D * p = wp->getPolygonRef(0);
        ASSERT_FALSE(p->hasInnerLoops());
        
        ASSERT_EQ(6,p->getOuterLoopRef()->size());
        ASSERT_EQ(4,p->getOuterLoopRef()->realSize());
        
        
        // Check number of materials... only the default material
        ASSERT_EQ(model.getNumMaterials(),1);
    }
    
}
