#include "gas_simulation_app.h"

namespace idealgas {

IdealGasApp::IdealGasApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  container_.Display();
}

void IdealGasApp::update() {
    for (std::vector<Particle>::iterator p1 = container_.particles_.begin(); p1 != container_.particles_.end(); ++p1) {
        //Movement before collision.
        p1->position_ += p1->velocity_;

        //Check and handle collision with walls.

        //Check collision with left wall
        if (p1->position_.x - p1->radius_ <= 0 && p1->velocity_.x <0) {
            p1->velocity_.x = -p1->velocity_.x;
        }
        //Check collision with right wall
        if (p1->position_.x + p1->radius_ >= container_.getWidth() && p1->velocity_.x >0) {
            p1->velocity_.x = -p1->velocity_.x;
        }
        //Check collision with bottom wall
        if (p1->position_.y - p1->radius_ <= 0 && p1->velocity_.y < 0) {
            p1->velocity_.y = -p1->velocity_.x;
        }
        //Check collision with top wall
        if (p1->position_.y + p1->radius_ >= container_.getHeight() && p1->velocity_.y > 0) {
            p1->velocity_.y = -p1->velocity_.y;
        }

        //Check and handle collision with other particles
        for (std::vector<Particle>::iterator p2 = container_.particles_.begin(); p2 != p1; ++p2) {
            if (glm::distance(p1->position_, p2->position_) < p1->radius_ + p2->radius_
                && glm::dot((p1->velocity_ - p2->velocity_), (p1->position_ - p2->position_)) < 0) {
                vec2 p1_velocity = p1->velocity_ - glm::dot((p1->velocity_ - p2->velocity_), (p1->position_ - p2->position_))
                                                           / glm::length2(p1->position_ - p2->position_)
                                                           * (p1->position_ - p2->position_);
                vec2 p2_velocity = p2->velocity_ - glm::dot((p2->velocity_ - p1->velocity_), (p2->position_ - p1->position_))
                                                           / glm::length2(p2->position_ - p1->position_)
                                                           * (p2->position_ - p1->position_);
                p1->velocity_ = p1_velocity;
                p2->velocity_ = p2_velocity;
            }

        }

    }
  container_.AdvanceOneFrame();
}

}  // namespace idealgas
