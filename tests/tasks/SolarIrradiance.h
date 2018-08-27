
#include "../../include/emp_core.h"
#include "./common.h"

TEST(SolarIrradiadiation, singleExteriorSensor)
{
    // Create Task Manager
    TaskManager tm = TaskManager();
    
    // Create empty model
    EmpModel model = EmpModel();
    
    // Needs a weather
    model.getLocation()->fillWeatherFromEPWFile("../../tests/weather/Oslo.epw");
    
    // Create Options
    RTraceOptions options = RTraceOptions();
    options.setOption("ab", 2);
    options.setOption("ad", 50000);
    options.setOption("aa", 0.1);
    options.setOption("lw", 0.00001);
    
    // Create rays
    FVECT origin = {0,0,0};
    FVECT dir = {0,0,1};
    std::vector<RAY> rays = std::vector<RAY>(1);
    VCOPY(rays.at(0).rorg, origin);
    VCOPY(rays.at(0).rdir, dir);
    
    // Create Task
    CalculateSolarIrradiation * task = new CalculateSolarIrradiation(&model, &options, &rays);
    
    // Add and solve
    tm.addTask(task);
    tm.solve();
    
    ASSERT_NEAR((task->result).getElement(0,0),807407,10); // 0.5% error.
    
}
