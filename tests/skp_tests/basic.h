#pragma once

#include "./common.h"


TEST(SKPRead, empty)
{
    int version = 2017;
    ASSERT_LOAD_MODEL("empty",version);
    
    // Only two layer
    size_t nLayers = model.getNumLayers();
    ASSERT_EQ(nLayers,1);
    
    // Count faces in the component
    size_t nFaces = model.getComponentDefinitionRef(0)->getObjectsRef()->size();
    ASSERT_EQ(nFaces,32);
}


TEST(SKPRead, deleted)
{
    int version = 2017;
    ASSERT_LOAD_MODEL("deleted",version);
    
    // Only two layer
    size_t nLayers = model.getNumLayers();
    ASSERT_EQ(nLayers,1);
    
    // Count faces in the component
    size_t nFaces = model.getLayerRef(0)->getObjectsRef()->size();
    ASSERT_EQ(nFaces,5);
}

