#include "common/geometry/matrix4x4.h"


TEST(Matrix4x4_TEST, BasicConstructor) {

  Matrix4x4 m = Matrix4x4();
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      double v = m.getElement(row, col);
      if (row == col) {
        ASSERT_EQ(1, v);
      }
      else {
        ASSERT_EQ(0,v);
      }
    }
  }
}

TEST(Matrix4x4_error, out_of_range) {
  Matrix4x4 m = Matrix4x4();
  EXPECT_ANY_THROW(m.getElement(0, 21));
  EXPECT_ANY_THROW(m.getElement(10, 1));
  EXPECT_ANY_THROW(m.getElement(10, 21));
  EXPECT_ANY_THROW(m.getElement(1, -2));
}

TEST(Matrix4x4_TEST, Multiplication) {

  Matrix4x4 a = Matrix4x4();
  Matrix4x4 b = Matrix4x4();

  // Change some elements
  b.setElement(1, 3, 1);
  b.setElement(1, 1, 3);
  b.setElement(3, 1, 23);

  // Compare
  Matrix4x4 * res = a*&b;

  
  for (int i = 0; i < 16; i++) {
    ASSERT_EQ(b.getElement(i), res->getElement(i));
  }

  delete res;

  // Change some elements in a
  a.setElement(3, 2, 14);
  a.setElement(3, 3, 14);
  a.setElement(0, 3, -23);

  res = a*&b;

  int i = 0;
  ASSERT_EQ(res->getElement(i++), 1);
  ASSERT_EQ(res->getElement(i++), -529);
  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), -23);


  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 3);
  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 1);

  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 1);
  ASSERT_EQ(res->getElement(i++), 0);


  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 322);
  ASSERT_EQ(res->getElement(i++), 14);
  ASSERT_EQ(res->getElement(i++), 14);
  delete res;

  // TEST MULTIPLY THIS
  a.multiplyThis(&b); 
  i = 0;
  res = &a; //lazy....
  ASSERT_EQ(res->getElement(i++), 1);
  ASSERT_EQ(res->getElement(i++), -529);
  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), -23);


  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 3);
  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 1);

  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 1);
  ASSERT_EQ(res->getElement(i++), 0);


  ASSERT_EQ(res->getElement(i++), 0);
  ASSERT_EQ(res->getElement(i++), 322);
  ASSERT_EQ(res->getElement(i++), 14);
  ASSERT_EQ(res->getElement(i++), 14);
}
