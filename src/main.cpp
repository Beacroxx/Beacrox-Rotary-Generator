#include <iostream>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <regex>
#include <string>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <map>
#include <ctime>
#include "./functions.cpp"
#include "./engine.cpp"
#include "../include/engine_struct.h"
#include "../include/units.h"

int main() {

  std::string version = "v1.0";

  // Set std::cout precision and decimal position
  std::cout << std::fixed << std::setprecision(2) << std::defaultfloat;
  std::cout.setf(std::ios::fixed);

  std::time_t t = std::time(nullptr);
  std::tm* now = std::localtime(&t);

  // Generator Information
  std::cout << "========== Info ==========" << std::endl;
  std::cout << "© Copyright | 2022 | Beacrox#8888" << std::endl;
  std::cout << "Version " << version << std::endl;
  std::cout << "Idle is Tuned to 9500 RPM with default Dimensions." << std::endl;
  std::cout << "A default Timing curve is used. You may need to tune it yourself." << std::endl;
  std::cout << "Default units are in (brackets) and default values are in [square brackets]." << std::endl;
  std::cout << "Leave the input empty to use defaults." << std::endl;
  std::cout << "If no unit specified, the default unit is used." << std::endl;
  std::cout << "To specify units, separate the value and unit with a space. Ex: '5 cm'." << std::endl;
  std::cout << "All units available in the Engine Simulator are also available in this Generator." << std::endl;
  std::cout << "Generator is WIP. Some stuff might break." << std::endl;
  std::cout << std::endl;
  
  // Get inputs
  std::cout << "========== Inputs ==========" << std::endl;
  getIntInput("Simulation Frequency (hz)", engine.simulation_frequency, "hz");
  getIntInput("Number of Rotors", engine.rotors, "None");
  getDoubleInput("Stroke (cm)", engine.stroke, "cm");
  getDoubleInput("Radius (cm)", engine.radius, "cm");
  getDoubleInput("Depth (cm)", engine.depth, "cm");
  getDoubleInput("Intake Port Start (deg)", engine.intake_port_start, "deg");
  getDoubleInput("Intake Port End (deg)", engine.intake_port_end, "deg");
  getDoubleInput("Exhaust Port Start (deg)", engine.exhaust_port_start, "deg");
  getDoubleInput("Exhaust Port End (deg)", engine.exhaust_port_end, "deg");
  getDoubleInput("Rotor Mass (g)", engine.rotor_mass, "g");
  getDoubleInput("Redline (rpm)", engine.redline, "rpm");
  getDoubleInput("Rev Limit (rpm)", engine.rev_limit, "rpm");
  getDoubleInput("Depression Volume (cc)", engine.depression_volume, "cc");
  engine.node = "wankel_" + std::to_string(engine.rotors) + "_rotor";
  engine.name = std::to_string(engine.rotors) + " Rotor";
  getStringInput("Node Name", engine.node);
  getStringInput("Engine Name", engine.name);

  // Calculations
  calculateMaxCFM();
  initializeArrays();
  setDefaultFiringOrder();
  setRodJournals();
  setRotorAngles();
  setIgnitionTimings();

  // Get firing order input
  getFiringOrder();

  // Engine information
  std::cout << std::endl;
  std::cout << "========== Engine Info ==========" << std::endl;
  double engine_volume = engine.displacement / (engine.displacement / units.getUnit["cc"] > 1000 ? units.getUnit["L"] : units.getUnit["cc"]);
  std::string engine_unit = (engine.displacement / units.getUnit["cc"] > 1000 ? " L" : " cc");
  std::cout << "Engine Volume: '" << engine_volume << engine_unit << "' | '" << engine.displacement / units.getUnit["cubic_inches"] << " ci'." << std::endl;
  std::cout << "Maximum theoretical intake CFM: '" << engine.maxCFM << "'." << std::endl;

  // Warning about dimensions
  if (engine.stroke * 2 > engine.radius) {
    std::cout << "Warning: if the Stroke is bigger than half of the Radius, the Engine might break or won't work realistically." << std::endl;
  }

  generate_engine();

  return 0;
}