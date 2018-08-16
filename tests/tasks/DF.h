
#include "../../include/emp_core.h"
#include "./common.h"

TEST(DaylightFactorTest, singleExteriorSensor)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create empty model
    EmpModel model = EmpModel();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 2);
    options.setOption("ad", 50000);
    
    // Create rays
    FVECT origin = {0,0,0};
    FVECT dir = {0,0,1};
    std::vector<RAY> rays = std::vector<RAY>(1);
    VCOPY(rays.at(0).rorg, origin);
    VCOPY(rays.at(0).rdir, dir);
    
    // Create Task
    CalculateDaylightFactor * task = new CalculateDaylightFactor(&model, &options, &rays);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    ASSERT_NEAR((task->result).getElement(0,0),100.0,0.5); // 0.5% error.
    
}

TEST(DaylightFactorTest, fullInterior_3_100)
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
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,3.0,100.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,0.0,0.5); // 0.5% error.
    
    CalculateDaylightFactor * t2 = static_cast<CalculateDaylightFactor * >(task->getDependencyRef(0));
    size_t nSensors = t2->result.nrows();
    for(size_t i=0; i<nSensors; i++){
        ASSERT_NEAR(t2->result.getElement(i,0),0.0,0.5); // 0.5% error.
    }
}


TEST(DaylightFactorTest, fullInterior_0_100)
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
    
    CalculateDaylightFactor * t2 = static_cast<CalculateDaylightFactor * >(task->getDependencyRef(0));
    size_t nSensors = t2->result.nrows();
    for(size_t i=0; i<nSensors; i++){
        ASSERT_NEAR(t2->result.getElement(i,0),0.0,0.5); // 0.5% error.
    }
}


TEST(DaylightFactorTest, halfInterior_0_100)
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


TEST(DaylightFactorTest, halfInterior_3_100)
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


TEST(DaylightFactorTest, mostlyInterior_3_100)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 1);
    options.setOption("ad", 8000);
    
    // Create model without exterior
    CREATE_MODEL(0.15f);
    
    // Create Task
    CheckDFCompliance * task = new CheckDFCompliance("checkDF",&model, &options, workplane,3.0,100.0);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    
    ASSERT_NEAR(task->compliance,15.0,0.5); // 0.5% error.
}


TEST(DaylightFactorTest, halfInterior_3_10)
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

