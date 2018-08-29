#include "../include/emp_core.h"

TEST(StringTest, downCase)
{
    std::string s1 = "Hola que tal!";
    std::string s2 = downCase(s1);
    ASSERT_EQ("hola que tal!",s2);
    
    downCase(&s1);
    ASSERT_EQ("hola que tal!",s1);
}
