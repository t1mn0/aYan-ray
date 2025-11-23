#include <ayan/math/vec.hpp>
#include <iostream>

int main() {
  using namespace ayan::math;

  Vec3f a{1.0f, 2.0f, 3.0f};
  Vec3f b{4.0f, 5.0f, 6.0f};
  Vec3f c = a + b;

  auto [x, y, z] = c; // checking structure binding

  std::cout << x << " " << y << " " << z << std::endl;
}
