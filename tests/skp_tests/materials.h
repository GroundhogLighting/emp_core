#pragma once

#include "./common.h"


TEST(SKPRead, default_materials)
{
    
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("default_materials",version);
                
        // Only one component
        ASSERT_EQ(model.getNumComponentDefinitions(),0);
        
        // Check number of materials
        ASSERT_EQ(model.getNumMaterials(),2);
        
        // Check number of window groups
        ASSERT_EQ(1,model.getNumWindowGroups());
        
        // Check the window Groups size
        WindowGroup * wg = model.getWindowGroupRef(0);
        ASSERT_EQ(wg->size(),1);
        
        // Check the material of the window
        Otype * win = wg->getWindowRef(0);
        Material * winMaterial = win->getMaterial();
        
        // Check class
        ASSERT_NE(dynamic_cast<Glass *>(winMaterial),nullptr);
        ASSERT_EQ(*(winMaterial->getName()),"Default-Glass");
        
        // Check the material of the window
        Otype * face = model.getLayerRef(0)->getObjectRef(0);
        Material * faceMaterial = face->getMaterial();
        
        // Check class
        ASSERT_NE(dynamic_cast<Plastic *>(faceMaterial),nullptr);
        ASSERT_EQ(*(faceMaterial->getName()),"Default-Material");
        
        
        
    }
}

