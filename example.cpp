#include "src/math/matrix/fwd.hpp"
#include <ayan/math/vec.hpp>
#include <ayan/math/mat.hpp>
#include <iostream>

int main() {
  using namespace ayan::math;

  Vec3f a{1.0f, 2.0f, 3.0f};
  Vec3f b{4.0f, 5.0f, 6.0f};
  Vec3f c = a + b;

  auto [x, y, z] = c; // checking structure binding

  std::cout << x << " " << y << " " << z << std::endl;

  Mat4i m = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    3, 1, 1, 1,
    8, 9, 1, 3
  };

  auto [s1, s2, s3, s4] = m.row<3>();
  std::cout << s1 << " " << s2 << " " << s3 << " " << s4 << std::endl;
}
