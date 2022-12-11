#ifndef ENGINE_STRUCT_H
#define ENGINE_STRUCT_H

#include "units.h"
#include <string>
#include <vector>

struct Engine {
  int rotors = 2;
  double stroke = 3 * units.getUnit["cm"];
  double radius = 10 * units.getUnit["cm"];
  double depth = 7 * units.getUnit["cm"];
  double intake_port_start = 130 * units.getUnit["deg"];
  double intake_port_end = 150 * units.getUnit["deg"];
  double exhaust_port_start = 220 * units.getUnit["deg"];
  double exhaust_port_end = 240 * units.getUnit["deg"];
  double rotor_mass = 5000 * units.getUnit["g"];
  double redline = 9500 * units.getUnit["rpm"];
  double rev_limit = 9500 * units.getUnit["rpm"];
  double depression_volume = 38 * units.getUnit["cc"];
  int simulation_frequency = 15000 * units.getUnit["hz"];
  double maxCFM;
  double displacement;
  std::string name;
  std::string node;
  std::vector<int> firing_order;
  std::vector<double> rod_journals;
  std::vector<double> rotor_angles;
  std::vector<double> ignition_timings;
};

Engine engine;

#endif // ENGINE_STRUCT_H