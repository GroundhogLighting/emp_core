

#include "../../include/emp_core.h"
#include "./common.h"


TEST(ASETest, ASEfullInterior)
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
    double early = 8;
    double late = 18;
    int firstMonth = 1;
    int lastMonth = 12;
    double minTime = 10; // Percent of time
    CheckASECompliance * task = new CheckASECompliance("checkASE", &model, &options, workplane, 6, minLux, early, late, firstMonth, lastMonth, minTime);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    ASSERT_NEAR(task->compliance,0.0,0.5); // 0.5% error.
    
    size_t nSensors = task->result.nrows();
    for(size_t i=0; i<nSensors; i++){
        ASSERT_NEAR(task->result.getElement(i,0),0.0,0.5); // 0.5% error.
    }
}


TEST(ASETest, ASEHalfExterior)
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
    double early = 12;
    double late = 15;
    int firstMonth = 1;
    int lastMonth = 12;
    double minTime = 1; // Percent of time
    CheckASECompliance * task = new CheckASECompliance("checkASE", &model, &options, workplane, 6, minLux, early, late, firstMonth, lastMonth, minTime);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    ASSERT_NEAR(task->compliance,50.0,0.5); // 0.5% error.
    
}

/*
TEST(ASETest, fullInterior_0_100)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 1);
    options.setOption("ad", 8000);
    
    // Create model without exterior
    CREATE_MODEL(0);
    
    // Create Task
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,0.0,100.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,100.0,0.5); // 0.5% error.
    
    size_t nSensors = task->result.nrows();
    for(size_t i=0; i<nSensors; i++){
        ASSERT_NEAR(task->result.getElement(i,0),0.0,0.5); // 0.5% error.
    }
}


TEST(ASETest, halfInterior_0_100)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 1);
    options.setOption("ad", 8000);
    
    // Create model without exterior
    CREATE_MODEL(0.5);
    
    // Create Task
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,0.0,100.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,100.0,0.5); // 0.5% error.
    
}


TEST(ASETest, halfInterior_3_100)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 1);
    options.setOption("ad", 8000);
    
    // Create model without exterior
    CREATE_MODEL(0.5);
    
    // Create Task
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,3.0,100.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,50.0,0.5); // 0.5% error.
}


TEST(ASETest, mostlyInterior_3_100)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 1);
    options.setOption("ad", 8000);
    
    // Create model without exterior
    CREATE_MODEL(0.15);
    
    // Create Task
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,3.0,100.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,15.0,0.5); // 0.5% error.
}


TEST(ASETest, halfInterior_3_10)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 1);
    options.setOption("ad", 8000);
    
    // Create model without exterior
    CREATE_MODEL(0.5);
    
    // Create Task
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,3.0,10.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,0.0,0.5); // 0.5% error.
    
}
*/

