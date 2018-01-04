#include <cmath>

#include "../src/common/geometry/segment.h"
#include "calculations/reinhart.h"


TEST(ReinhartTest, rnaz) {
  size_t mf = 1;
  ASSERT_EQ(rnaz(0, mf), 30);
  ASSERT_EQ(rnaz(1, mf), 30);
  ASSERT_EQ(rnaz(2, mf), 24);
  ASSERT_EQ(rnaz(3, mf), 24);
  ASSERT_EQ(rnaz(4, mf), 18);
  mf = 3;
  ASSERT_EQ(rnaz(0, mf), 90);
  ASSERT_EQ(rnaz(1, mf), 90);
  ASSERT_EQ(rnaz(2, mf), 90);
  ASSERT_EQ(rnaz(3, mf), 90);
  ASSERT_EQ(rnaz(4, mf), 90);
  ASSERT_EQ(rnaz(6, mf), 72);
  ASSERT_EQ(rnaz(12, mf), 54);

}

TEST(ReinhartTest, raccum) {

  size_t mf = 1;

  ASSERT_EQ(raccum(0,mf), 0);
  ASSERT_EQ(raccum(1,mf), 30);
  ASSERT_EQ(raccum(2,mf), 60);
  ASSERT_EQ(raccum(3,mf), 84);
  ASSERT_EQ(raccum(4,mf), 108);
  ASSERT_EQ(raccum(5,mf), 126);
  ASSERT_EQ(raccum(6,mf), 138);

  mf = 2;

  ASSERT_EQ(raccum(0, mf), 0);
  ASSERT_EQ(raccum(1, mf), 60);
  ASSERT_EQ(raccum(2, mf), 120);
  ASSERT_EQ(raccum(3, mf), 180);
  ASSERT_EQ(raccum(4, mf), 240);
  ASSERT_EQ(raccum(5, mf), 288);
  ASSERT_EQ(raccum(6, mf), 336);
}

TEST(ReinhartTest, NRBins) {
  ASSERT_EQ(nReinhartBins(1), 146);
  ASSERT_EQ(nReinhartBins(2), 578);
  ASSERT_EQ(nReinhartBins(3), 1298);
  ASSERT_EQ(nReinhartBins(4), 2306);
  ASSERT_EQ(nReinhartBins(5), 3602);
  ASSERT_EQ(nReinhartBins(6), 5186);
  ASSERT_EQ(nReinhartBins(7), 7058);
  ASSERT_EQ(nReinhartBins(8), 9218);
  ASSERT_EQ(nReinhartBins(9), 11666);
  ASSERT_EQ(nReinhartBins(10), 14402);
}

TEST(ReinhartTest, binDir) {
  size_t mf = 1;

  Vector3D a = reinhartDir(1, mf);
  ASSERT_NEAR(a.getX(), 0.0, 1e-5);
  ASSERT_NEAR(a.getY(), 0.994522, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.104528, 1e-5);


  a = reinhartDir(14, mf);    
  ASSERT_NEAR(a.getX(), 0.404508, 1e-5);
  ASSERT_NEAR(a.getY(), -0.908541, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.104528, 1e-5);

  a = reinhartDir(62, mf);
  ASSERT_NEAR(a.getX(), 0.224144, 1e-5);
  ASSERT_NEAR(a.getY(), 0.836516, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.5, 1e-5);

  a = reinhartDir(107, mf);
  ASSERT_NEAR(a.getX(), -0.371572, 1e-5);
  ASSERT_NEAR(a.getY(), 0.643582, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.669131, 1e-5);

  mf = 3;

  a = reinhartDir(1, mf);
  ASSERT_NEAR(a.getX(), 0.0, 1e-5);
  ASSERT_NEAR(a.getY(), 0.999333, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.036522, 1e-5);


  a = reinhartDir(14, mf);    
  ASSERT_NEAR(a.getX(), 0.787485, 1e-5);
  ASSERT_NEAR(a.getY(), 0.615251, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.036522, 1e-5);

  
  a = reinhartDir(62, mf);
  ASSERT_NEAR(a.getX(), -0.898194, 1e-5);
  ASSERT_NEAR(a.getY(), -0.438079, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.036522, 1e-5);

  a = reinhartDir(107, mf);
  ASSERT_NEAR(a.getX(), 0.893402, 1e-5);
  ASSERT_NEAR(a.getY(), 0.435741, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.109371, 1e-5);
    
}