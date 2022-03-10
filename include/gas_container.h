#pragma once

#include <vector>

#include "cinder/gl/gl.h"
#include "cinder"
#include "particle.h"

using glm::vec2;

namespace idealgas {

    /**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
    class GasContainer {
    public:
        std::vector<Particle> particles_;   /*< Vector of particles */

        ci::Rectf rectangle;
        /**
         * Constructs a defualt new container with width of 200 and height of 100.
         */
        GasContainer();

        /**
         * Constructs a new container with specified length and height.
         * The frame of the new container is initialized to 0.
         * @param length The specified length of the container.
         * @param height The specified height of the container.
         */
        GasContainer(double width, double height);

        /**
          * Gets the frame of the container.
          * @return The frame of the container.
          */
        size_t getFrame();

        /**
          * Gets the width of the container.
          * @return The width of the container.
          */
        double getWidth();

        /**
          * Gets the height of the container.
          * @return The height of the container.
          */
        double getHeight();

        /**
         * Displays the container walls and the current positions of the particles.
         */
        void Display() const;

        /**
         * Updates the positions and velocities of all particles (based on the rules
         * described in the assignment documentation).
         */
        void AdvanceOneFrame();

        /**
          * Add a particle to the gas container.
          * @param particle The particle to add to the gas container.
          */
        void AddParticle(Particle const &particle);

        /**
          * Calculates the alternation of velocities and positions of all particles prior to frame advance.
          */
        void Calculate();

    private:
        size_t frame_;                      /*< Frame of the container */
        double width_;                      /*< Width of the container */
        double height_;                     /*< Height of the container */

    };



}  // namespace idealgas
