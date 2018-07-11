#pragma once

#include "./common.h"


TEST(SKPRead, empty)
{
    FOR_ALL_VERSIONS{
        ASSERT_LOAD_MODEL("empty",version);
        
        // Only one layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,1);
        
        // Only one component
        ASSERT_EQ(model.getNumComponentDefinitions(),1);
        
        // Count faces in the component
        size_t nFaces = model.getComponentDefinitionRef(0)->getObjectsRef()->size();
        ASSERT_EQ(nFaces,32);
    }
    
    
    
}


TEST(SKPRead, deleted)
{
    FOR_ALL_VERSIONS{        
        ASSERT_LOAD_MODEL("deleted",version);
    
        // Only one layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,1);
        
        // Count faces in the component
        size_t nFaces = model.getLayerRef(0)->getObjectsRef()->size();
        ASSERT_EQ(nFaces,5);
    }
}

