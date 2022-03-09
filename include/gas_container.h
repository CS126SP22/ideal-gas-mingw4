#pragma once

#include "cinder/gl/gl.h"

using glm::vec2;

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
    public:
        /**
         * Constructs a new container with specified length and height.
         * The frame of the new container is initialized to 0.
         * @param length The specified length of the container.
         * @param height The specified height of the container.
         */
         GasContainer(double width, double height);

        /**
         * Displays the container walls and the current positions of the particles.
         */
         void Display() const;

        /**
         * Updates the positions and velocities of all particles (based on the rules
         * described in the assignment documentation).
         */
         void AdvanceOneFrame();

    private:
        size_t frame_;                  /*< Frame of the container */
        double width_;                  /*< Width of the container */
        double height_;                 /*< Height of the container */
};


}  // namespace idealgas
