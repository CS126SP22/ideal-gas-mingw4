#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "gas_container.h"

namespace idealgas {

/**
 * An app for visualizing the behavior of an ideal gas.
 */
class IdealGasApp : public ci::app::App {
 public:
  IdealGasApp();

  void draw() override;
  void update() override;
  void keyDown( ci::app::KeyEvent event ) override;
  void mouseMove( ci::app::MouseEvent event ) override;

  const int kWindowSize = 900;
  const int kMargin = 100;

 private:
  GasContainer container_;
};

}  // namespace idealgas
