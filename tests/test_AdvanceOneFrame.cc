#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

#define cmpf(A, B) ((fabs((A) - (B)) < 0.00001f))

TEST_CASE("AdvanceOneFrame test") {
  GasContainer container;

  container.SetParticlesNum(2);

  container.SetParticlesPos(vec2(419.9, 420), 0);
  container.SetParticlesVelocity(vec2(0.1, 0), 0);
  container.SetParticlesSize(0.1, 0);

  container.SetParticlesPos(vec2(421.5, 421.4), 1);
  container.SetParticlesVelocity(vec2(-0.1, 0), 1);
  container.SetParticlesSize(0.1, 1);

  container.AdvanceOneFrame();

  vec2 new_pos_0 = container.GetParticlesPos(0);
  vec2 new_v_0 = container.GetParticlesVelocity(0);
  vec2 new_pos_1 = container.GetParticlesPos(1);
  vec2 new_v_1 = container.GetParticlesVelocity(1);

  REQUIRE(cmpf(new_pos_0.x, 420.0f));
  REQUIRE(cmpf(new_pos_0.y, 420.0f));
  REQUIRE(cmpf(new_pos_1.x, 421.4f));
  REQUIRE(cmpf(new_pos_1.y, 421.4f));

  REQUIRE(cmpf(new_v_0.x, 0.1f));
  REQUIRE(cmpf(new_v_0.y, 0.0f));
  REQUIRE(cmpf(new_v_1.x, -0.1f));
  REQUIRE(cmpf(new_v_1.y, 0.0f));
}
