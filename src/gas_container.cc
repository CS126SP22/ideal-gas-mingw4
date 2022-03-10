#include "gas_container.h"
#include "particle.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer() {
    frame_ = 0;
    this->width_ = 200;
    this->height_ = 100;
    this->particles_ = std::vector<Particle>();
}

GasContainer::GasContainer(double width, double height) {
    frame_ = 0;
    this->width_ = width;
    this->height_ = height;
}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  ci::gl::color(ci::Color("orange"));
  ci::gl::drawSolidCircle(vec2(frame_, 200), 10);
  ci::gl::color(ci::Color("white"));
  ci::gl::drawStrokedRect(ci::Rectf(vec2(100, 100), vec2(600, 400)));
}

size_t GasContainer::getFrame() {
    return this->frame_;
}

double GasContainer::getHeight() {
    return this->height_;
}

double GasContainer::getWidth() {
    return this->width_;
}

void GasContainer::AdvanceOneFrame() {
  ++frame_;
}
void GasContainer::Calculate() {
    for (std::vector<Particle>::iterator p1 = particles_.begin(); p1 != particles_.end(); ++p1) {
        //Movement before collision.
        p1->position_ += p1->velocity_;

        //Check and handle collision with walls.

        //Check collision with left wall
        if (p1->position_.x - p1->radius_ <= 0 && p1->velocity_.x <0) {
            p1->velocity_.x = -p1->velocity_.x;
        }
        //Check collision with right wall
        if (p1->position_.x + p1->radius_ >= this->getWidth() && p1->velocity_.x >0) {
            p1->velocity_.x = -p1->velocity_.x;
        }
        //Check collision with bottom wall
        if (p1->position_.y - p1->radius_ <= 0 && p1->velocity_.y < 0) {
            p1->velocity_.y = -p1->velocity_.x;
        }
        //Check collision with top wall
        if (p1->position_.y + p1->radius_ >= this->getHeight() && p1->velocity_.y > 0) {
            p1->velocity_.y = -p1->velocity_.y;
        }

        //Check and handle collision with other particles
        for (std::vector<Particle>::iterator p2 = particles_.begin(); p2 != p1; ++p2) {

        }

    }
}

}  // namespace idealgas
