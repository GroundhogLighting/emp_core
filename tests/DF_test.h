
#include "../include/emp_core.h"
//#include "calculations/tasks/CalculateDaylightFactor.h"

TEST(DaylightFactorTest, calculate)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create empty model
    GroundhogModel model = GroundhogModel();
    
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
    
    ASSERT_NEAR(task->result.redChannel()->getElement(0,0),100.0,0.5); // 0.5% error.
    
}
