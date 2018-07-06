

#include "../../include/emp_core.h"
#include "./common.h"

#define ASSERT_CBDM_TASKS(ase,da,udi) \
    CheckASECompliance * aseTask = new CheckASECompliance("CheckASE", &model, &options, workplane, 6, minLux, early, late, firstMonth, lastMonth, minTime); \
    CheckUDICompliance * udiTask = new CheckUDICompliance("checkUDI", &model, &options, workplane, 6,1, minLux, maxLux, early, late, firstMonth, lastMonth, minTime); \
    CheckDACompliance * daTask = new CheckDACompliance("checkDA", &model, &options, workplane, 6,1, minLux, early, late, firstMonth, lastMonth, minTime); \
    tm.addTask(aseTask); \
    tm.addTask(daTask); \
    tm.addTask(udiTask); \
    tm.solve(); \
    ASSERT_NEAR(aseTask->getCompliance(),ase,0.5); \
    ASSERT_NEAR(udiTask->getCompliance(),udi,0.5); \
    ASSERT_NEAR(daTask->getCompliance(),da,0.5); \



TEST(CBDMTest, CBDMfullInterior)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    
    // Create model without exterior
    CREATE_MODEL(0);
    workplane->setMaxArea(40);
    
    // Create Task
    double minLux = 300;
    double maxLux = 3000;
    double early = 8;
    double late = 18;
    int firstMonth = 1;
    int lastMonth = 12;
    double minTime = 10; // Percent of time
    
    ASSERT_CBDM_TASKS(0,0,0);
    
    size_t nSensors = aseTask->getResult()->nrows();
    for(size_t i=0; i<nSensors; i++){
        ASSERT_NEAR(aseTask->getResult()->getElement(i,0),0.0,0.5); // 0.5% error.
        ASSERT_NEAR(daTask->getResult()->getElement(i,0),0.0,0.5); // 0.5% error.
        ASSERT_NEAR(udiTask->getResult()->getElement(i,0),0.0,0.5); // 0.5% error.
    }
}


TEST(CBDMTest, CBDMHalfExteriorDaytime)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    
    // Create model without exterior
    CREATE_MODEL(0.5);
    workplane->setMaxArea(40);
    
    // Create Task
    double minLux = 300;
    double maxLux = 3000;
    double early = 12;
    double late = 15;
    int firstMonth = 1;
    int lastMonth = 12;
    double minTime = 1; // Percent of time
    
    ASSERT_CBDM_TASKS(50,50,0);
    
}


TEST(CBDMTest, CBDMHalfExteriorNightTime)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    
    // Create model without exterior
    CREATE_MODEL(0.5);
    workplane->setMaxArea(40);
    
    // Create Task
    double minLux = 300;
    double maxLux = 3000;
    double early = 20;
    double late = 24;
    int firstMonth = 1;
    int lastMonth = 12;
    double minTime = 1; // Percent of time
    
    ASSERT_CBDM_TASKS(0.0,0.0,0);
    
}



