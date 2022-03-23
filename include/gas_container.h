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
   * @param particles_color_ the color of points
   * @param particles_positions_ the position of points
   * @param particles_radii_  the size of points
   * @param particles_masses_  the  mass of points
   * @param particles_velocities_ the velocity of points
   * @param particles_velocity_changes_ the velocity of points changed.
   * @param particles_new_velocities_  the new velocity of points
   * @param particles_v_distributions_  the speed distributions of points.
   */
  GasContainer(std::vector<std::string> particles_color_,
               std::vector<vec2> particles_positions_,
               std::vector<float> particles_radii_,
               std::vector<float> particles_masses_,
               std::vector<vec2> particles_velocities_,

               std::vector<bool> particles_velocity_changes_,
               std::vector<vec2> particles_new_velocities_,

               std::vector<std::vector<int>> particles_v_distributions_);


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
  void SetParticlesPos(vec2 p, int i);
  void SetParticlesVelocity(vec2 v, int i);
  void SetParticlesNewVelocity(vec2 v, int i);

  //Getters for particles' positions, velocities, and enw velocities.
  vec2 GetParticlesPos(int i);
  vec2 GetParticlesVelocity(int i);
  vec2 GetParticlesNewVelocity(int i);

  //Setters for particles' sizes, number, and hovereffect.
  void SetParticlesSize(float s, int i);
  void SetParticlesMass(float m, int i);
  void SetParticlesNum(int n);
  void SetHoverEffect(bool flag, int type);

  // Setters for boundary type.
  void SetBoundaryType(bool _is_circle);

 private:

  //Original collision handler.
  void HandlingCollisionsAllPoints();
  //Collision handler for week 1 extra credit: having a O(nlogn) algorithm to check for collisions.
  void HandlingCollisionsAllPoints_Efficient();
  //A helper function for HandlingCollisionsAllPoints_Efficient().
  void HandlingCollision(int i, int j);

  //Variables for gas containers.
  int particles_num_ = 300;
  std::vector<std::string> particles_color_;
  std::vector<vec2> particles_positions_;
  std::vector<float> particles_radii_;
  std::vector<float> particles_masses_;
  std::vector<vec2> particles_velocities_;

  std::vector<bool> particles_velocity_changes_;
  std::vector<vec2> particles_new_velocities_;

  std::vector<std::vector<int>> particles_v_distributions_;

  bool is_circle_ = true;

  vec2 box_top_left_ = vec2(100, 100);
  vec2 box_bottom_right_ = vec2(800, 600);

  vec2 circle_center_ = vec2(450, 350);
  float circle_radius_ = 300;

  float play_speed_ = 1.0f;
  float play_speed_signal_ = 1.0f;
  float default_particles_radii = 2.5f;

  bool save_state_flag_ = false;
  bool load_state_flag_ = false;

  bool hover_effect_flag_ = false;
  int hover_effect_type_ = -1;
};

}  // namespace idealgas
