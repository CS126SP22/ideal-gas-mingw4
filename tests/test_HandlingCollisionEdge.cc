#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

#define cmpf(A, B) ((fabs((A) - (B)) < 0.00001f))

TEST_CASE("HandlingCollisionEdge test") {
  GasContainer container;

  container.SetPointNum(1);

  container.SetPointPos(vec2(101, 101), 0);
  container.SetPointV(vec2(-1, -1), 0);
  container.SetPointSize(2.0, 0);

  container.AdvanceOneFrame();

  vec2 new_v_0 = container.GetPointV(0);

  REQUIRE(cmpf(new_v_0.x, 1.0f));
  REQUIRE(cmpf(new_v_0.y, 1.0f));


  container.SetPointPos(vec2(599, 399), 0);
  container.SetPointV(vec2(1, 1), 0);
  container.SetPointSize(2.0, 0);

  container.AdvanceOneFrame();

  new_v_0 = container.GetPointV(0);

  REQUIRE(cmpf(new_v_0.x, -1.0f));
  REQUIRE(cmpf(new_v_0.y, -1.0f));
}
