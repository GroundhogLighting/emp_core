
#include "../include/emp_core.h"


TEST(GenCumulativeSkyTest, test1) {
    

    EmpModel model = EmpModel();
    Location * l = model.getLocation();
    l->fillWeatherFromEPWFile("../../tests/weather/Santiago.epw");
    genCumulativeSky(&model,false,true);
    
	ASSERT_TRUE(false);

}
