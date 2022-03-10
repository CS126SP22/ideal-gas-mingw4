#pragma once

#include <vector>

#include "cinder/gl/gl.h"
#include "particle.h"

using glm::vec2;
namespace idealgas {
    class Particle {
    public:
        /**
             * Constructs a particle with specified position, velocity, and radius.
             * @param position The current position of the particle.
             * @param velocity The current velocity of the particle.
             * @param radius The radius of the particle.
             */
        Particle(const vec2 &position, const vec2 &velocity, double radius);

        glm::vec2 position_;
        glm::vec2 velocity_;
        double radius_;
    };
}  // namespace idealgas
