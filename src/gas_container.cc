#include "gas_container.h"
#include <random>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
// #include <jsoncpp/json/json.h>
#include <numeric>
#include <algorithm>

namespace idealgas {

using glm::vec2;

std::vector<size_t> sort_indexes(const std::vector<vec2> &v) {

  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  // using std::stable_sort instead of std::sort
  // to avoid unnecessary index re-orderings
  // when v contains elements of equal values 
  std::stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1].x < v[i2].x;});

  return idx;
}


GasContainer::GasContainer() {

  std::srand( ( unsigned int )std::time( nullptr ) );

  for (int i = 0; i < particles_num_; ++i) {
    //Create particles
    if (is_circle_) {
      //Vector that records the top-left coordinates of the container.
      vec2 tl = circle_center_ - vec2(circle_radius_, circle_radius_);
      //Vector that records the bottom-right coordinates of the container.
      vec2 br = circle_center_ + vec2(circle_radius_, circle_radius_);

      while (true) {
        float x = tl.x + std::rand() % ( (int) (br.x - 1 - tl.x) );
        float y = tl.y + std::rand() % ( (int) (br.y - 1 - tl.y) );
        if (glm::length(vec2(x, y) - circle_center_) < circle_radius_ * 0.5) {
          particles_positions_.push_back(vec2(x, y));
          break;
        }
      }

    } else {
      particles_positions_.push_back(vec2(
        box_top_left_.x + std::rand() % ( (int) (box_bottom_right_.x - 1 - box_top_left_.x) ),
        box_top_left_.y + std::rand() % ( (int) (box_bottom_right_.y - 1 - box_top_left_.y) )
      ));
    }


    float vx = -3.0 + std::rand() % ( 7 );
    float vy = -3.0 + std::rand() % ( 7 );
    particles_velocities_.push_back(vec2( vx / 3.0, vy / 3.0 ));

    int __p = std::rand();

    if (__p % 3 == 0) {
      particles_color_.push_back(std::string("orange"));
      particles_radii_.push_back(default_particles_radii);
      particles_masses_.push_back(1);
    } else if (__p % 3 == 1) {
      particles_color_.push_back(std::string("cyan"));
      particles_radii_.push_back(default_particles_radii * 1.1);
      particles_masses_.push_back(2);
    } else {
      particles_color_.push_back(std::string("pink"));
      particles_radii_.push_back(default_particles_radii * 2.0);
      particles_masses_.push_back(4);
    }

    particles_velocity_changes_.push_back(false);
    particles_new_velocities_.push_back(vec2( 0, 0 ));
  }


  for (int s = 0; s < 3; s ++) {
    std::vector<int> d;
    for (int i = 0; i < 25; ++i) {
      d.push_back(0);
    }
    particles_v_distributions_.push_back(d);
  }

}

GasContainer::GasContainer(std::vector<std::string> particles_color_,
                           std::vector<vec2> particles_positions_,
                           std::vector<float> particles_radii_,
                           std::vector<float> particles_masses_,
                           std::vector<vec2> particles_velocities_,
                           std::vector<bool> particles_velocity_changes_,
                           std::vector<vec2> particles_new_velocities_,
                           std::vector<std::vector<int>> particles_v_distributions_) {
    this->particles_color_ = particles_color_;
    this->particles_positions_ = particles_positions_;
    this->particles_radii_ = particles_radii_;
    this->particles_masses_ = particles_masses_;
    this->particles_velocities_ = particles_velocities_;
    this->particles_velocity_changes_ = particles_velocity_changes_;
    this->particles_new_velocities_ = particles_velocities_;
    this->particles_v_distributions_ = particles_v_distributions_;
}

void GasContainer::Display() const {

  for (int i = 0; i < particles_num_; ++i) {
    ci::gl::color(ci::Color(particles_color_[i].c_str()));
    ci::gl::drawSolidCircle(particles_positions_[i], particles_radii_[i]);
  }

  //Display container depends on its shape.
  if (is_circle_) {
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedCircle(circle_center_, circle_radius_);
  } else {
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedRect(ci::Rectf(box_top_left_, box_bottom_right_));
  }

  float bottom = 780;
  float left = 100;
  for (int type = 0; type < 3; type++) {
    for (unsigned i = 0; i < particles_v_distributions_[type].size(); ++i) {

      if (type == 0) ci::gl::color(ci::Color("orange"));
      else if (type == 1) ci::gl::color(ci::Color("cyan"));
      else ci::gl::color(ci::Color("pink"));

      int c = particles_v_distributions_[type][i];
      ci::gl::drawSolidRect(ci::Rectf(vec2(left+i*10, bottom-c*1.2), vec2(left+i*10+5, bottom)));
    }
    left += 260;
  }


  //Section for week 2 extra credit: Maker the histogram interactive (e.g. add hover effects)
  if (hover_effect_flag_) {
    int type = hover_effect_type_;

    ci::gl::color(ci::Color("black"));
    ci::gl::drawSolidRect(ci::Rectf(vec2(300, 250), vec2(600, 450)));

    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedRect(ci::Rectf(vec2(300, 250), vec2(600, 450)));

    if (type == 0) ci::gl::color(ci::Color("orange"));
    else if (type == 1) ci::gl::color(ci::Color("cyan"));
    else ci::gl::color(ci::Color("pink"));


    for (unsigned i = 0; i < particles_v_distributions_[type].size(); ++i) {
      int c = particles_v_distributions_[type][i];
      ci::gl::drawSolidRect(ci::Rectf(vec2(310.0f+i*10, 440.0f-c*2.0f), vec2(310.0f+i*10+8, 440.0f)));
    }

  }


}

//
void GasContainer::HandlingCollisionsAllPoints() {

  for (int i = 0; i < particles_num_; ++i) {
    for (int j = 0; j < particles_num_; ++ j) {
      if (i > j)
        HandlingCollision(i, j);
    }
  }

  for (int i = 0; i < particles_num_; ++i) {
    if (particles_velocity_changes_[i]) {
      particles_velocities_[i] = particles_new_velocities_[i];
    }
  }
}


void GasContainer::HandlingCollisionsAllPoints_Efficient() {

  int max_point_size = 0;
  for (int i = 0; i < particles_num_; ++i ) {
    max_point_size = max_point_size < particles_radii_[i] ? particles_radii_[i] : max_point_size;
  }

  std::vector<vec2> new_particles_positions_;

  for (int i = 0; i < particles_num_; ++i) {
    //The play_speed_ parameter is for week1 extra credit: speed down or slow down the simulation.
    new_particles_positions_.push_back(particles_positions_[i] + particles_velocities_[i] * play_speed_);
  }

  std::vector<size_t> points_sorted_idx = sort_indexes(new_particles_positions_);

  for (int k = 0; k < particles_num_; ++k) {
    int i = points_sorted_idx[k];
    for (int w = k+1; w < particles_num_; ++w) {
      int j = points_sorted_idx[w];
      if (std::abs(new_particles_positions_[j].x - new_particles_positions_[i].x) < max_point_size * 3) {
        HandlingCollision(i, j);
      } else {
        break;
      }
    }
  }

  for (int i = 0; i < particles_num_; ++i) {
    if (particles_velocity_changes_[i]) {
      particles_velocities_[i] = particles_new_velocities_[i];
    }
  }

}


void GasContainer::HandlingCollision(int i, int j) {
  vec2 pos_i = particles_positions_[i];
  vec2 pos_j = particles_positions_[j];
  vec2 v_i = particles_velocities_[i];
  vec2 v_j = particles_velocities_[j];
  float s_i = particles_radii_[i];
  float s_j = particles_radii_[j];
  float m_i = particles_masses_[i];
  float m_j = particles_masses_[j];

  vec2 new_pos_i = pos_i + v_i * play_speed_;
  vec2 new_pos_j = pos_j + v_j * play_speed_;

  if ( glm::dot (new_pos_i - new_pos_j, v_i - v_j) > 0 )
    return;

  if (glm::length(new_pos_i - new_pos_j) <= s_i + s_j) {

    vec2 new_v_i = v_i - (2.0f * m_j / (m_i + m_j)  ) * glm::dot(v_i - v_j, pos_i - pos_j) * (pos_i - pos_j) / (glm::length(pos_i - pos_j) * glm::length(pos_i - pos_j));
    vec2 new_v_j = v_j - (2.0f * m_i / (m_i + m_j)  ) * glm::dot(v_j - v_i, pos_j - pos_i) * (pos_j - pos_i) / (glm::length(pos_j - pos_i) * glm::length(pos_j - pos_i));

    particles_new_velocities_[i] = new_v_i;
    particles_new_velocities_[j] = new_v_j;

    particles_velocity_changes_[i] = true;
    particles_velocity_changes_[j] = true;

  }
}

void GasContainer::AdvanceOneFrame() {

  if (play_speed_signal_ != play_speed_) {
    play_speed_ = play_speed_signal_;
  }

  // HandlingCollisionsAllPoints();
  HandlingCollisionsAllPoints_Efficient();

  if (is_circle_) {

    for (int i = 0; i < particles_num_; ++i) {
      vec2 new_pos = particles_positions_[i] + particles_velocities_[i] * play_speed_;
      vec2 new_v = particles_velocities_[i];
      float p_size = particles_radii_[i];

      // collision!!!
      if (glm::length(new_pos - circle_center_) >= circle_radius_-p_size) {
        // where collision
        float t = 0.0f;
        vec2 x = particles_positions_[i] - circle_center_;
        float A = glm::dot(new_v, new_v);
        float B = 2.0f * glm::dot(new_v, x);
        float C = glm::dot(x, x) - (circle_radius_-p_size) * (circle_radius_-p_size);
        t = (glm::sqrt(B*B - 4.0f*A*C) - B) / (2.0f * C);

        new_pos = particles_positions_[i] + particles_velocities_[i] * play_speed_ * t;

        vec2 N = circle_center_ - new_pos;
        new_v = glm::reflect(particles_velocities_[i], N);
        new_v = glm::length(particles_velocities_[i]) * glm::normalize(new_v);

      }

      particles_positions_[i] = new_pos;
      particles_velocities_[i] = new_v;

    }

  } else {

    for (int i = 0; i < particles_num_; ++i) {
      // particles_positions_[i] = particles_positions_[i] + particles_velocities_[i];
      vec2 new_pos = particles_positions_[i] + particles_velocities_[i] * play_speed_;
      vec2 new_v = particles_velocities_[i];
      float p_size = particles_radii_[i];

      if (new_pos.x - p_size < box_top_left_.x && new_v.x < 0) {
        new_pos.x = box_top_left_.x + p_size;
        new_v.x = -new_v.x;
      } else if (new_pos.x + p_size > box_bottom_right_.x && new_v.x > 0) {
        new_pos.x = box_bottom_right_.x - p_size;
        new_v.x = -new_v.x;
      }

      if (new_pos.y - p_size < box_top_left_.y && new_v.y < 0) {
        new_pos.y = box_top_left_.y + p_size;
        new_v.y = -new_v.y;
      } else if (new_pos.y + p_size > box_bottom_right_.y && new_v.y > 0) {
        new_pos.y = box_bottom_right_.y - p_size;
        new_v.y = -new_v.y;
      }

      particles_positions_[i] = new_pos;
      particles_velocities_[i] = new_v;
    }
  }

  for (int i = 0; i < particles_num_; ++i) {
    particles_velocity_changes_[i] = false;
  }

  if (save_state_flag_) {
    SaveState();
    save_state_flag_ = false;
  }

  if (load_state_flag_) {
    LoadState();
    load_state_flag_ = false;
  }

  ComputeVelocityDistribution();
}

void GasContainer::ChangePlayspeed (int d) {
  if (d == -1) {
    play_speed_signal_ *= 0.9;
  } else if (d == 1) {
    play_speed_signal_ /= 0.9;
  }
}


void GasContainer::SaveState () {
  std::ofstream ofs("state.bin");

  ofs << particles_num_ << std::endl;

  for (int i = 0; i < particles_num_; ++i) {
    ofs << particles_radii_[i] << std::endl;
  }

  for (int i = 0; i < particles_num_; ++i) {
    ofs << particles_masses_[i] << std::endl;
  }

  for (int i = 0; i < particles_num_; ++i) {
    ofs << particles_color_[i] << std::endl;
  }

  for (int i = 0; i < particles_num_; ++i) {
    ofs << particles_positions_[i].x << " " << particles_positions_[i].y << std::endl;
  }

  for (int i = 0; i < particles_num_; ++i) {
    ofs << particles_velocities_[i].x << " " << particles_velocities_[i].y << std::endl;
  }

  ofs.close();
}

void GasContainer::LoadState () {

  std::ifstream ifs("state.bin");

  ifs >> particles_num_;

  for (int i = 0; i < particles_num_; ++i) {
    float s;
    ifs >> s;
    particles_radii_[i] = s;
  }

  for (int i = 0; i < particles_num_; ++i) {
    float m;
    ifs >> m;
    particles_masses_[i] = m;
  }

  for (int i = 0; i < particles_num_; ++i) {
    ifs >> particles_color_[i];
  }

  for (int i = 0; i < particles_num_; ++i) {
    float x, y;
    ifs >> x >> y;
    particles_positions_[i] = vec2(x, y);
  }

  for (int i = 0; i < particles_num_; ++i) {
    float x, y;
    ifs >> x >> y;
    particles_velocities_[i] = vec2(x, y);
  }

  ifs.close();

}

void GasContainer::SaveStateSignal () {
  save_state_flag_ = true;
}

void GasContainer::LoadStateSignal () {
  load_state_flag_ = true;
}


void GasContainer::SetPointSize (float s, int i) {
  particles_radii_[i] = s;
}

void GasContainer::SetPointMass(float m, int i) {
    particles_masses_[i] = m;
}

void GasContainer::SetPointNum (int n) {
  particles_num_ = n;
}


void GasContainer::SetPointPos (vec2 p, int i) {
  particles_positions_[i] = p;
}

void GasContainer::SetPointV (vec2 p, int i) {
  particles_velocities_[i] = p;
}

void GasContainer::SetPointNewV (vec2 p, int i) {
  particles_new_velocities_[i] = p;
}

vec2 GasContainer::GetPointPos(int i){
  return particles_positions_[i];
}

vec2 GasContainer::GetPointV(int i){
  return particles_velocities_[i];
}

vec2 GasContainer::GetPointNewV(int i){
  return particles_new_velocities_[i];
}


void GasContainer::SetHoverEffect(bool flag, int type) {
  hover_effect_flag_ = flag;
  hover_effect_type_ = type;
}


void GasContainer::ComputeVelocityDistribution() {

  int N = particles_v_distributions_[0].size();
  float max_v = 4.0f;    // min_v = 0;
  float intv = (max_v - 0.0f) / N;

  // int count[N] = {0};
  int all_count[3] = {0};
  for (int i = 0; i < N; i++) {
    for (int type = 0; type < 3; type++) {
      particles_v_distributions_[type][i] = 0;
    }
  }

  for (int i = 0; i < particles_num_; ++i) {

    float vi = glm::length(particles_velocities_[i]);

    for (int j = 0; j < N; ++j) {
      int type = -1;
      if (particles_masses_[i] == 1)
        type = 0;
      else if (particles_masses_[i] == 2)
        type = 1;
      else
        type = 2;

      if (vi >= intv * j && vi <= intv * (j+1.0)) {
        all_count[type] += 1;
        particles_v_distributions_[type][j] += 1;
        break;
      }
    }
  }
}

}  // namespace idealgas
