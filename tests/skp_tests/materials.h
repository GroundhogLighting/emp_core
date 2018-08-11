#pragma once

#include "./common.h"


TEST(SKPRead, default_materials)
{
    
    FOR_ALL_VERSIONS {
        ASSERT_LOAD_MODEL("default_materials",version);
                
        // No components
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
        const Material * winMaterial = win->getMaterial();
        
        // Check class
        ASSERT_NE(dynamic_cast<const Glass *>(winMaterial),nullptr);
        ASSERT_EQ(winMaterial->getName(),"Default Glass");
        
        // Check the material of the window
        const Otype * face = model.getLayerRef(0)->getObjectRef(0);
        const Material * faceMaterial = face->getMaterial();
        
        // Check class
        ASSERT_NE(dynamic_cast<const Plastic *>(faceMaterial),nullptr);
        ASSERT_EQ(faceMaterial->getName(),"Default Material");
        
        
        
    }
}

