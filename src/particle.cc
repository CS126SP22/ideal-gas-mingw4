#include "particle.h"

namespace idealgas {

using glm::vec2;

Particle::Particle(const vec2 &position, const vec2 &velocity, double radius) {
this->position_ = position;
this->velocity_ = velocity;
this->radius_ = radius;

}
}  // namespace idealgas