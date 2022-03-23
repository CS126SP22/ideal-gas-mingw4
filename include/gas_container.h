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
   * Constructs a default gas container with random locations and speeds.
   */
  GasContainer();

  /**
   * Constructs a gas container with parameters specified.
   * @param points_color the color of points
   * @param points_position the position of points
   * @param points_size  the size of points
   * @param points_mass  the  mass of points
   * @param points_velocity the velocity of points
   * @param points_velocity_changed the velocity of points changed.
   * @param points_new_velocity  the new velocity of points
   * @param points_v_distributions  the speed distributions of points.
   */
  GasContainer(std::vector<std::string> points_color,
               std::vector<vec2> points_position,
               std::vector<float> points_size,
               std::vector<float> points_mass,
               std::vector<vec2> points_velocity,

               std::vector<bool> points_velocity_changed,
               std::vector<vec2> points_new_velocity,

               std::vector<std::vector<int>> points_v_distributions);


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
 * Changes the play speed for week 1 extra credit: allow user
 * to speed up/slow down the simulation  using keyboard or mouse input.
 * @param d When d is -1, the playspeed is slowed down by 0.9;
 *          when d is 1. the playspeed is speed up by 0.9.
 */
  void ChangePlayspeed(int d);

  void ComputeVelocityDistribution();

  //Methods for week 1 extra credit: allow a user to save a snapshot of the simulation's state to a file,
  //and load the snapshot from that file back into the simulation at a later time.
  void SaveState();
  void LoadState();

  void SaveStateSignal();
  void LoadStateSignal();

  //Setters for particles' positions, velocities, and new velocities.
  void SetPointPos(vec2 p, int i);
  void SetPointV(vec2 v, int i);
  void SetPointNewV(vec2 v, int i);

  //Getters for particles' positions, velocities, and enw velocities.
  vec2 GetPointPos(int i);
  vec2 GetPointV(int i);
  vec2 GetPointNewV(int i);

  //Setters for particles' sizes, number, and hovereffect.
  void SetPointSize(float s, int i);
  void SetPointMass(float m, int i);
  void SetPointNum(int n);
  void SetHoverEffect(bool flag, int type);

 private:

  //Original collision handler.
  void HandlingCollisionsAllPoints();
  //Collision handler for week 1 extra credit: having a O(nlogn) algorithm to check for collisions.
  void HandlingCollisionsAllPoints_Efficient();
  //A helper function for HandlingCollisionsAllPoints_Efficient().
  void HandlingCollision(int i, int j);

  //Variables for gas containers.
  int points_num = 300;
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
