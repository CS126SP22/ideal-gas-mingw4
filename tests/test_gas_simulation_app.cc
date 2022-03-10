#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

TEST_CASE("Constructor Test") {
    SECTION("Frame is correctly initialized.") {
        REQUIRE(GasContainer(200, 100).getFrame() == 0);
    }
    SECTION("Width is correctly initialized.") {
        REQUIRE(GasContainer(300, 200).getWidth() == 300);
    }
    SECTION("Height is correctly initialized.") {
        REQUIRE(GasContainer(200, 100).getHeight() == 100);
    }
}

TEST_CASE("AdvanceOneFrame Test") {
    GasContainer container(200, 100);
    container.AdvanceOneFrame();
    REQUIRE(container.getFrame() == 1);
}