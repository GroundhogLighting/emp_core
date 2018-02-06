
#include "../src/common/geometry/segment.h"
#include "calculations/reinhart.h"

#define PI 3.14159265359
#define RADIANS(x) (x*PI/180.0)

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

TEST(ReinhartTest, binCenterDir) {
  size_t mf = 1;

  Vector3D a = reinhartCenterDir(1, mf);
  ASSERT_NEAR(a.getX(), 0.0, 1e-5);
  ASSERT_NEAR(a.getY(), 0.994522, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.104528, 1e-5);


  a = reinhartCenterDir(14, mf);
  ASSERT_NEAR(a.getX(), 0.404508, 1e-5);
  ASSERT_NEAR(a.getY(), -0.908541, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.104528, 1e-5);

  a = reinhartCenterDir(62, mf);
  ASSERT_NEAR(a.getX(), 0.224144, 1e-5);
  ASSERT_NEAR(a.getY(), 0.836516, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.5, 1e-5);

  a = reinhartCenterDir(107, mf);
  ASSERT_NEAR(a.getX(), -0.371572, 1e-5);
  ASSERT_NEAR(a.getY(), 0.643582, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.669131, 1e-5);

  mf = 3;

  a = reinhartCenterDir(1, mf);
  ASSERT_NEAR(a.getX(), 0.0, 1e-5);
  ASSERT_NEAR(a.getY(), 0.999333, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.036522, 1e-5);


  a = reinhartCenterDir(14, mf);
  ASSERT_NEAR(a.getX(), 0.787485, 1e-5);
  ASSERT_NEAR(a.getY(), 0.615251, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.036522, 1e-5);

  
  a = reinhartCenterDir(62, mf);
  ASSERT_NEAR(a.getX(), -0.898194, 1e-5);
  ASSERT_NEAR(a.getY(), -0.438079, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.036522, 1e-5);

  a = reinhartCenterDir(107, mf);
  ASSERT_NEAR(a.getX(), 0.893402, 1e-5);
  ASSERT_NEAR(a.getY(), 0.435741, 1e-5);
  ASSERT_NEAR(a.getZ(), 0.109371, 1e-5);
    
}


TEST(ReinhartTest, binDir) {
    size_t mf = 1;
    Vector3D a = reinhartDir(1, 1, 0, 0.5); // Patch in first row
    ASSERT_NEAR(a.getX(), 0.0, 1e-2);
    ASSERT_NEAR(a.getY(), 1, 1e-2);
    ASSERT_NEAR(a.getZ(), 0, 1e-2);
    
    a = reinhartDir(1, mf, 1, 0.5); // Patch in first row
    ASSERT_NEAR(a.getZ(), sin(RADIANS(12.0)) , 1e-2);
    
    mf = 2;
    a = reinhartDir(1, 1, 0, 0.5); // Patch in first row
    ASSERT_NEAR(a.getX(), 0.0, 1e-2);
    ASSERT_NEAR(a.getY(), 1, 1e-2);
    ASSERT_NEAR(a.getZ(), 0, 1e-2);
    
    a = reinhartDir(1, mf, 1, 0.5); // Patch in first row
    ASSERT_NEAR(a.getZ(), sin(RADIANS(6.0)) , 1e-2);
    
}

TEST(ReinhartTest, binSolidAngle) {

  size_t mf = 1;
  size_t bin = 1;
  size_t acc = 0;

  // First row
  acc += 30;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0435, 1e-4);
  }

  // Second row
  acc += 30;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0416, 1e-4);
  }

  // third row
  acc += 24;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0474, 1e-4);
  }

  // 4th row
  acc += 24;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0407, 1e-4);
  }

  // 5th row
  acc += 18;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0429, 1e-4);
  }

  // 6th row
  acc += 12;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0445, 1e-4);
  }

  // 7th row
  acc += 6;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0455, 1e-4);
  }

  // 8th: Polar Cap
  acc += 1;
  while (bin <= acc) {
    ASSERT_NEAR(reinhartSolidAngle(bin++, mf), 0.0344, 1e-4);
  }

}

TEST(ReinhartTest, Rfindrow){
    const size_t tnaz[7] = { 30, 30, 24, 24, 18, 12, 6 };
    size_t npatch = 0;
    for(size_t nrow = 0; nrow < 7; nrow++){
        for(size_t i = 1; i<= tnaz[nrow]; i++){            
            ASSERT_EQ(Rfindrow(npatch+1,1),nrow);
            npatch++;
        }
    }
    // Polar Cap
    ASSERT_EQ(Rfindrow(0,npatch+1,1),7);
    
}

