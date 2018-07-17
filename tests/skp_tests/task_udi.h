#pragma once

#include "./common.h"


TEST(SKPRead, task_udi)
{
    FOR_ALL_VERSIONS {
        
        ASSERT_LOAD_MODEL("task_udi",version);

        // Only one layer
        size_t nLayers = model.getNumLayers();
        ASSERT_EQ(nLayers,1);
        
        // Check names
        ASSERT_EQ(model.getLayerRef(0)->getName(), "Layer0");
        
        // Count components in the component
        size_t nComponents = model.getNumComponentDefinitions();
        ASSERT_EQ(nComponents,0);
        
        /* CHECK TASKS */
        // Check number of tasks
        size_t nTasks = model.countTasks();
        ASSERT_EQ(1,nTasks);
        
        // Check the kind and name of task
        const json * task = model.getTask(0);
        std::string taskName = (*task)["name"].get<std::string>();
        ASSERT_EQ(taskName,"UDI task");
        
        /* CHECK WORKPLANES */
        
        // Check number of workplanes
        ASSERT_EQ(2,model.getNumWorkplanes());
        
        // Check the Workplane with tasks
        std::string withName = "with task";
        Workplane * with = model.getWorkplaneByName(&withName); // Retrieve
        ASSERT_EQ( withName,with->getName()); // Check name
        const std::vector<std::string> * withTasks = with->getTasks(); // Get tasks
        ASSERT_EQ(1,withTasks->size()); // Count tasks
        const std::string udiTask = withTasks->at(0);
        ASSERT_EQ(udiTask,"UDI task");
        
        // Check the Workplane without tasks
        std::string withoutName = "without task";
        Workplane * without = model.getWorkplaneByName(&withoutName); // Retrieve
        ASSERT_EQ( withoutName,without->getName()); // Check name
        const std::vector<std::string> * withoutTasks = without->getTasks(); // Get tasks
        ASSERT_EQ(0,withoutTasks->size()); // Count tasks
        
        
    }
}
