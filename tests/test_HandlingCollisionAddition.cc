#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

#define cmpf(A, B) ((fabs((A) - (B)) < 0.00001f))

TEST_CASE("HandlingCollision additional test") {
  GasContainer container;

  container.SetParticlesNum(2);

  container.SetParticlesPos(vec2(120, 120), 0);
  container.SetParticlesVelocity(vec2(0.1, 0.0), 0);
  container.SetParticlesSize(1.0, 0);

  container.SetParticlesPos(vec2(121, 121), 1);
  container.SetParticlesVelocity(vec2(0.1, 0.0), 1);
  container.SetParticlesSize(1.0, 1);

  container.AdvanceOneFrame();

  vec2 new_v_0 = container.GetParticlesVelocity(0);
  vec2 new_v_1 = container.GetParticlesVelocity(1);


  REQUIRE(cmpf(new_v_0.x, 0.1f));
  REQUIRE(cmpf(new_v_0.y, 0.0f));
  REQUIRE(cmpf(new_v_1.x, 0.1f));
  REQUIRE(cmpf(new_v_1.y, 0.0f));
}
