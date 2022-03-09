#include "gas_container.h"

namespace idealgas {

using glm::vec2;

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

}  // namespace idealgas
