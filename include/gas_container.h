#pragma once

#include "cinder/gl/gl.h"
#include <vector>
#include <string>

using glm::vec2;

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:
  /**
   * TODO: Add more parameters to this constructor, and add documentation.
   */
  GasContainer();

  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;

  /**
   * Updates the positions and velocities of all particles (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();

  void ChangePlayspeed(int d);

  void ComputeVelocityDistribution();

  void SaveState();
  void LoadState();

  void SaveStateSignal();
  void LoadStateSignal();

  void SetPointPos(vec2 p, int i);
  void SetPointV(vec2 v, int i);
  void SetPointNewV(vec2 v, int i);

  vec2 GetPointPos(int i);
  vec2 GetPointV(int i);
  vec2 GetPointNewV(int i);

  void SetPointSize(float s, int i);
  void SetPointNum(int n);

  void SetHoverEffect(bool flag, int type);

 private:
  /**
   * This variable is just for the purposes of demonstrating how to make a shape move
   * across a screen. Please remove it once you start working on your code.
   */

  void HandlingCollisionsAllPoints();
  void HandlingCollisionsAllPoints_Efficient();
  void HandlingCollision(int i, int j);

  int points_num = 1000;

  // std::string points_color = "orange";
  std::vector<std::string> points_color;
  std::vector<vec2> points_position;
  std::vector<float> points_size;
  std::vector<float> points_mass;
  std::vector<vec2> points_velocity;

  std::vector<bool> points_velocity_changed;
  std::vector<vec2> points_new_velocity;

  std::vector<std::vector<int>> points_v_distributions;

  bool is_circle = true;

  vec2 box_top_left = vec2(100, 100);
  // vec2 box_bottom_right = vec2(600, 400);
  vec2 box_bottom_right = vec2(800, 600);

  vec2 circle_center = vec2(450, 350);
  float circle_r = 300;

  float play_speed = 1.0f;
  float play_speed_signal = 1.0f;
  float default_point_size = 2.5f;

  bool save_state_flag = false;
  bool load_state_flag = false;

  bool hover_effect_flag = false;
  int hover_effect_type = -1;
};

}  // namespace idealgas
