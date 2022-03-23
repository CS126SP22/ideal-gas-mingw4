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

  for (int i = 0; i < points_num; ++i) {
    //Create particles
    if (is_circle) {
      //Vector that records the top-left coordinates of the container.
      vec2 tl = circle_center - vec2(circle_r, circle_r);
      //Vector that records the bottom-right coordinates of the container.
      vec2 br = circle_center + vec2(circle_r, circle_r);

      while (true) {
        float x = tl.x + std::rand() % ( (int) (br.x - 1 - tl.x) );
        float y = tl.y + std::rand() % ( (int) (br.y - 1 - tl.y) );
        if (glm::length(vec2(x, y) - circle_center) < circle_r * 0.5) {
          points_position.push_back(vec2(x, y));
          break;
        }
      }

    } else {
      points_position.push_back(vec2(
        box_top_left.x + std::rand() % ( (int) (box_bottom_right.x - 1 - box_top_left.x) ),
        box_top_left.y + std::rand() % ( (int) (box_bottom_right.y - 1 - box_top_left.y) )
      ));
    }


    float vx = -3.0 + std::rand() % ( 7 );
    float vy = -3.0 + std::rand() % ( 7 );
    points_velocity.push_back(vec2( vx / 3.0, vy / 3.0 ));

    int __p = std::rand();

    if (__p % 3 == 0) {
      points_color.push_back(std::string("orange"));
      points_size.push_back(default_point_size);
      points_mass.push_back(1);
    } else if (__p % 3 == 1) {
      points_color.push_back(std::string("cyan"));
      points_size.push_back(default_point_size * 1.1);
      points_mass.push_back(2);
    } else {
      points_color.push_back(std::string("pink"));
      points_size.push_back(default_point_size * 2.0);
      points_mass.push_back(4);
    }

    points_velocity_changed.push_back(false);
    points_new_velocity.push_back(vec2( 0, 0 ));
  }


  for (int s = 0; s < 3; s ++) {
    std::vector<int> d;
    for (int i = 0; i < 25; ++i) {
      d.push_back(0);
    }
    points_v_distributions.push_back(d);
  }

}

GasContainer::GasContainer(std::vector<std::string> points_color,
                           std::vector<vec2> points_position,
                           std::vector<float> points_size,
                           std::vector<float> points_mass,
                           std::vector<vec2> points_velocity,
                           std::vector<bool> points_velocity_changed,
                           std::vector<vec2> points_new_velocity,
                           std::vector<std::vector<int>> points_v_distributions) {
    this->points_color = points_color;
    this->points_position = points_position;
    this->points_size = points_size;
    this->points_mass = points_mass;
    this->points_velocity = points_velocity;
    this->points_velocity_changed = points_velocity_changed;
    this->points_new_velocity = points_velocity;
    this->points_v_distributions = points_v_distributions;
}

void GasContainer::Display() const {

  for (int i = 0; i < points_num; ++i) {
    ci::gl::color(ci::Color(points_color[i].c_str()));
    ci::gl::drawSolidCircle(points_position[i], points_size[i]);
  }

  //Display container depends on its shape.
  if (is_circle) {
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedCircle(circle_center, circle_r);
  } else {
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedRect(ci::Rectf(box_top_left, box_bottom_right));
  }

  float bottom = 780;
  float left = 100;
  for (int type = 0; type < 3; type++) {
    for (unsigned i = 0; i < points_v_distributions[type].size(); ++i) {

      if (type == 0) ci::gl::color(ci::Color("orange"));
      else if (type == 1) ci::gl::color(ci::Color("cyan"));
      else ci::gl::color(ci::Color("pink"));

      int c = points_v_distributions[type][i];
      ci::gl::drawSolidRect(ci::Rectf(vec2(left+i*10, bottom-c*1.2), vec2(left+i*10+5, bottom)));
    }
    left += 260;
  }


  //Section for week 2 extra credit: Maker the histogram interactive (e.g. add hover effects)
  if (hover_effect_flag) {
    int type = hover_effect_type;

    ci::gl::color(ci::Color("black"));
    ci::gl::drawSolidRect(ci::Rectf(vec2(300, 250), vec2(600, 450)));

    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedRect(ci::Rectf(vec2(300, 250), vec2(600, 450)));

    if (type == 0) ci::gl::color(ci::Color("orange"));
    else if (type == 1) ci::gl::color(ci::Color("cyan"));
    else ci::gl::color(ci::Color("pink"));


    for (unsigned i = 0; i < points_v_distributions[type].size(); ++i) {
      int c = points_v_distributions[type][i];
      ci::gl::drawSolidRect(ci::Rectf(vec2(310.0f+i*10, 440.0f-c*2.0f), vec2(310.0f+i*10+8, 440.0f)));
    }

  }


}

//
void GasContainer::HandlingCollisionsAllPoints() {

  for (int i = 0; i < points_num; ++i) {
    for (int j = 0; j < points_num; ++ j) {
      if (i > j)
        HandlingCollision(i, j);
    }
  }

  for (int i = 0; i < points_num; ++i) {
    if (points_velocity_changed[i]) {
      points_velocity[i] = points_new_velocity[i];
    }
  }
}


void GasContainer::HandlingCollisionsAllPoints_Efficient() {

  int max_point_size = 0;
  for (int i = 0; i < points_num; ++i ) {
    max_point_size = max_point_size < points_size[i] ? points_size[i] : max_point_size;
  }

  std::vector<vec2> new_points_position;

  for (int i = 0; i < points_num; ++i) {
    //The play_speed parameter is for week1 extra credit: speed down or slow down the simulation.
    new_points_position.push_back(points_position[i] + points_velocity[i] * play_speed);
  }

  std::vector<size_t> points_sorted_idx = sort_indexes(new_points_position);

  for (int k = 0; k < points_num; ++k) {
    int i = points_sorted_idx[k];
    for (int w = k+1; w < points_num; ++w) {
      int j = points_sorted_idx[w];
      if (std::abs(new_points_position[j].x - new_points_position[i].x) < max_point_size * 3) {
        HandlingCollision(i, j);
      } else {
        break;
      }
    }
  }

  for (int i = 0; i < points_num; ++i) {
    if (points_velocity_changed[i]) {
      points_velocity[i] = points_new_velocity[i];
    }
  }

}


void GasContainer::HandlingCollision(int i, int j) {
  vec2 pos_i = points_position[i];
  vec2 pos_j = points_position[j];
  vec2 v_i = points_velocity[i];
  vec2 v_j = points_velocity[j];
  float s_i = points_size[i];
  float s_j = points_size[j];
  float m_i = points_mass[i];
  float m_j = points_mass[j];

  vec2 new_pos_i = pos_i + v_i * play_speed;
  vec2 new_pos_j = pos_j + v_j * play_speed;

  if ( glm::dot (new_pos_i - new_pos_j, v_i - v_j) > 0 )
    return;

  if (glm::length(new_pos_i - new_pos_j) <= s_i + s_j) {

    vec2 new_v_i = v_i - (2.0f * m_j / (m_i + m_j)  ) * glm::dot(v_i - v_j, pos_i - pos_j) * (pos_i - pos_j) / (glm::length(pos_i - pos_j) * glm::length(pos_i - pos_j));
    vec2 new_v_j = v_j - (2.0f * m_i / (m_i + m_j)  ) * glm::dot(v_j - v_i, pos_j - pos_i) * (pos_j - pos_i) / (glm::length(pos_j - pos_i) * glm::length(pos_j - pos_i));

    points_new_velocity[i] = new_v_i;
    points_new_velocity[j] = new_v_j;

    points_velocity_changed[i] = true;
    points_velocity_changed[j] = true;

  }
}

void GasContainer::AdvanceOneFrame() {

  if (play_speed_signal != play_speed) {
    play_speed = play_speed_signal;
  }

  // HandlingCollisionsAllPoints();
  HandlingCollisionsAllPoints_Efficient();

  if (is_circle) {

    for (int i = 0; i < points_num; ++i) {
      vec2 new_pos = points_position[i] + points_velocity[i] * play_speed;
      vec2 new_v = points_velocity[i];
      float p_size = points_size[i];

      // collision!!!
      if (glm::length(new_pos - circle_center) >= circle_r-p_size) {
        // where collision
        float t = 0.0f;
        vec2 x = points_position[i] - circle_center;
        float A = glm::dot(new_v, new_v);
        float B = 2.0f * glm::dot(new_v, x);
        float C = glm::dot(x, x) - (circle_r-p_size) * (circle_r-p_size);
        t = (glm::sqrt(B*B - 4.0f*A*C) - B) / (2.0f * C);

        new_pos = points_position[i] + points_velocity[i] * play_speed * t;

        vec2 N = circle_center - new_pos;
        new_v = glm::reflect(points_velocity[i], N);
        new_v = glm::length(points_velocity[i]) * glm::normalize(new_v);

      }

      points_position[i] = new_pos;
      points_velocity[i] = new_v;

    }

  } else {

    for (int i = 0; i < points_num; ++i) {
      // points_position[i] = points_position[i] + points_velocity[i];
      vec2 new_pos = points_position[i] + points_velocity[i] * play_speed;
      vec2 new_v = points_velocity[i];
      float p_size = points_size[i];

      if (new_pos.x - p_size < box_top_left.x && new_v.x < 0) {
        new_pos.x = box_top_left.x + p_size;
        new_v.x = -new_v.x;
      } else if (new_pos.x + p_size > box_bottom_right.x && new_v.x > 0) {
        new_pos.x = box_bottom_right.x - p_size;
        new_v.x = -new_v.x;
      }

      if (new_pos.y - p_size < box_top_left.y && new_v.y < 0) {
        new_pos.y = box_top_left.y + p_size;
        new_v.y = -new_v.y;
      } else if (new_pos.y + p_size > box_bottom_right.y && new_v.y > 0) {
        new_pos.y = box_bottom_right.y - p_size;
        new_v.y = -new_v.y;
      }

      points_position[i] = new_pos;
      points_velocity[i] = new_v;
    }
  }

  for (int i = 0; i < points_num; ++i) {
    points_velocity_changed[i] = false;
  }

  if (save_state_flag) {
    SaveState();
    save_state_flag = false;
  }

  if (load_state_flag) {
    LoadState();
    load_state_flag = false;
  }

  ComputeVelocityDistribution();
}

void GasContainer::ChangePlayspeed (int d) {
  if (d == -1) {
    play_speed_signal *= 0.9;
  } else if (d == 1) {
    play_speed_signal /= 0.9;
  }
}


void GasContainer::SaveState () {
  std::ofstream ofs("state.bin");

  ofs << points_num << std::endl;

  for (int i = 0; i < points_num; ++i) {
    ofs << points_size[i] << std::endl;
  }

  for (int i = 0; i < points_num; ++i) {
    ofs << points_mass[i] << std::endl;
  }

  for (int i = 0; i < points_num; ++i) {
    ofs << points_color[i] << std::endl;
  }

  for (int i = 0; i < points_num; ++i) {
    ofs << points_position[i].x << " " << points_position[i].y << std::endl;
  }

  for (int i = 0; i < points_num; ++i) {
    ofs << points_velocity[i].x << " " << points_velocity[i].y << std::endl;
  }

  ofs.close();
}

void GasContainer::LoadState () {

  std::ifstream ifs("state.bin");

  ifs >> points_num;

  for (int i = 0; i < points_num; ++i) {
    float s;
    ifs >> s;
    points_size[i] = s;
  }

  for (int i = 0; i < points_num; ++i) {
    float m;
    ifs >> m;
    points_mass[i] = m;
  }

  for (int i = 0; i < points_num; ++i) {
    ifs >> points_color[i];
  }

  for (int i = 0; i < points_num; ++i) {
    float x, y;
    ifs >> x >> y;
    points_position[i] = vec2(x, y);
  }

  for (int i = 0; i < points_num; ++i) {
    float x, y;
    ifs >> x >> y;
    points_velocity[i] = vec2(x, y);
  }

  ifs.close();

}

void GasContainer::SaveStateSignal () {
  save_state_flag = true;
}

void GasContainer::LoadStateSignal () {
  load_state_flag = true;
}


void GasContainer::SetPointSize (float s, int i) {
  points_size[i] = s;
}

void GasContainer::SetPointMass(float m, int i) {
    points_mass[i] = m;
}

void GasContainer::SetPointNum (int n) {
  points_num = n;
}


void GasContainer::SetPointPos (vec2 p, int i) {
  points_position[i] = p;
}

void GasContainer::SetPointV (vec2 p, int i) {
  points_velocity[i] = p;
}

void GasContainer::SetPointNewV (vec2 p, int i) {
  points_new_velocity[i] = p;
}

vec2 GasContainer::GetPointPos(int i){
  return points_position[i];
}

vec2 GasContainer::GetPointV(int i){
  return points_velocity[i];
}

vec2 GasContainer::GetPointNewV(int i){
  return points_new_velocity[i];
}


void GasContainer::SetHoverEffect(bool flag, int type) {
  hover_effect_flag = flag;
  hover_effect_type = type;
}


void GasContainer::ComputeVelocityDistribution() {

  int N = points_v_distributions[0].size();
  float max_v = 4.0f;    // min_v = 0;
  float intv = (max_v - 0.0f) / N;

  // int count[N] = {0};
  int all_count[3] = {0};
  for (int i = 0; i < N; i++) {
    for (int type = 0; type < 3; type++) {
      points_v_distributions[type][i] = 0;
    }
  }

  for (int i = 0; i < points_num; ++i) {

    float vi = glm::length(points_velocity[i]);

    for (int j = 0; j < N; ++j) {
      int type = -1;
      if (points_mass[i] == 1)
        type = 0;
      else if (points_mass[i] == 2)
        type = 1;
      else
        type = 2;

      if (vi >= intv * j && vi <= intv * (j+1.0)) {
        all_count[type] += 1;
        points_v_distributions[type][j] += 1;
        break;
      }
    }
  }
}

}  // namespace idealgas
