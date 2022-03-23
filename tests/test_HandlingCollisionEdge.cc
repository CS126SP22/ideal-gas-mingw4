#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

#define cmpf(A, B) ((fabs((A) - (B)) < 0.00001f))

TEST_CASE("HandlingCollisionEdge test") {
  GasContainer container;

  container.SetParticlesNum(1);
  container.SetBoundaryType(false);

  container.SetParticlesPos(vec2(101, 101), 0);
  container.SetParticlesVelocity(vec2(-1, -1), 0);
  container.SetParticlesSize(2.0, 0);

  container.AdvanceOneFrame();

  vec2 new_v_0 = container.GetParticlesVelocity(0);

  REQUIRE(cmpf(new_v_0.x, 1.0f));
  REQUIRE(cmpf(new_v_0.y, 1.0f));


  container.SetParticlesPos(vec2(799, 599), 0);
  container.SetParticlesVelocity(vec2(1, 1), 0);
  container.SetParticlesSize(2.0, 0);

  container.AdvanceOneFrame();

  new_v_0 = container.GetParticlesVelocity(0);

  REQUIRE(cmpf(new_v_0.x, -1.0f));
  REQUIRE(cmpf(new_v_0.y, -1.0f));
}
