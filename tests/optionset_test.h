/*  os_test.h */

#include "common/options/optionset.h"


TEST(OptionSetTests, io_options)
{
  // Add options
 
  OptionSet os = OptionSet();

  // Define variables
  int i1 = 1;
  double d1 = 1.264364326542;
  std::string s1 = "glare test string";

  // Write options
  ASSERT_NO_THROW(os.addOption("int option", i1));
  ASSERT_NO_THROW(os.addOption("double option", d1));
  ASSERT_NO_THROW(os.addOption("string option", s1));

  // Read options
  int i2 = os.getOption<int>("int option");
  double d2 = os.getOption<double>("double option");
  std::string s2 = os.getOption<std::string>("string option");

  // Compare
  ASSERT_EQ(i1, i2);
  ASSERT_EQ(d1, d2);
  ASSERT_EQ(s1, s2);
}


TEST(OptionSetTests, setOptions)
{
  OptionSet os = OptionSet();

  os.addOption("op", 1);

  ASSERT_EQ(1, os.getOption<int>("op"));

  // modify the option
  os.setOption("op", "auto");
  ASSERT_EQ(os.getOption<std::string>("op"), "auto");
}

TEST(OptionSetTests, compare_Optionsets)
{
  OptionSet os1 = OptionSet();
  OptionSet os2 = OptionSet();

  // Test empty comparison
  ASSERT_TRUE(os1.isEqual(&os2));

  // Compare same options
  os1.addOption("int 1", 1);
  os2.addOption("int 1", 1);
  os1.addOption("double 1", 1.123123);
  os2.addOption("double 1", 1.123123);
  os1.addOption("string 1", "string test");
  os2.addOption("string 1", "string test");
  ASSERT_TRUE(os1.isEqual(&os2));

  // Compare different options
  os1.setOption("int 1", 23);
  ASSERT_FALSE(os1.isEqual(&os2));

  // Test mixing types
  os1.setOption("int 1", "1");
  ASSERT_FALSE(os1.isEqual(&os2));

  // Add another option to os1
  os1.addOption("int 2", 1);
  ASSERT_FALSE(os1.isEqual(&os2));

}