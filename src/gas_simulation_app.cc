#include "gas_simulation_app.h"
#include <iostream>

namespace idealgas {

IdealGasApp::IdealGasApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  container_.Display();
}

void IdealGasApp::keyDown( ci::app::KeyEvent event ) {
  char character = event.getChar();

  if (character == 'a') {
    std::cout << "Speed down." << std::endl;
    container_.ChangePlayspeed(-1);
  } else if (character == 'd') {
    std::cout << "Speed up." << std::endl;
    container_.ChangePlayspeed(1);
  } else if (character == 's') {
    std::cout << "Save state." << std::endl;
    container_.SaveStateSignal();
  } else if (character == 'l') {
    std::cout << "Load state." << std::endl;
    container_.LoadStateSignal();
  }

}

void IdealGasApp::mouseMove( ci::app::MouseEvent event ) {

  vec2 p = event.getPos();

  if (p.x > 100 && p.x < 340 && p.y > 650 && p.y < 780) {
    container_.SetHoverEffect(true, 0);
  } else if (p.x > 340 && p.x < 580 && p.y > 650 && p.y < 780) {
    container_.SetHoverEffect(true, 1);
  } else if (p.x > 580 && p.x < 820 && p.y > 650 && p.y < 780) {
    container_.SetHoverEffect(true, 2);
  } else {
    container_.SetHoverEffect(false, -1);
  }

}


void IdealGasApp::update() {
  container_.AdvanceOneFrame();
}

}  // namespace idealgas
