#include "../include/units.h"
#include "../include/engine_struct.h"

// Function to split string into Array using a Delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
  std::string token;
  std::istringstream token_stream(s);
  while (std::getline(token_stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

void initializeArrays() {
  int length = engine.rotors;
  for (int i = 0; i <= length - 1; i++) {
    engine.rod_journals.push_back(0);
    engine.firing_order.push_back(0);
    engine.rotor_angles.push_back(0);
    for (int j = 0; j < 3; j++) {
      engine.ignition_timings.push_back(0);
    }
  }
}

// Calculate displacement
void calculateDisplacement() {
  double normal_volume = (engine.stroke / units.getUnit["cm"] * 0.5) * engine.radius / units.getUnit["cm"] * engine.depth / units.getUnit["cm"] * engine.rotors;
  double depression_volume = engine.depression_volume * 3 * engine.rotors;
  engine.displacement = (3 * sqrt(3) * normal_volume + depression_volume) * units.getUnit["cc"];
}

// Calculate maximum intake CFM
void calculateMaxCFM() {
  calculateDisplacement();
  double efficiency = 1.7;
  engine.maxCFM = ((engine.displacement / units.getUnit["cubic_inches"]) * (engine.redline / units.getUnit["rpm"]) * efficiency) / 3456.0;;
}

// Generate default firing order
void setDefaultFiringOrder() {
  int length = engine.rotors;
  for (int i = 0; i <= length - 1; i++) {
    int value = (i <= (length - 1) / 2) ? i * 2 : (i - length / 2) * 2 + (length % 2 == 0 ? 1 : -1);
    engine.firing_order[i] = value;
  }
}

// Generate rotor angles
void setRotorAngles() {
  int length = engine.rotors;
  for (int i = 0; i <= length - 1; i++) {
    double angle = (60 + engine.rod_journals[i] / units.getUnit["deg"] / 360.0 * 120) * units.getUnit["deg"];
    engine.rotor_angles[i] = angle;
  }
}

// Generate rod journals
void setRodJournals() {
  int length = engine.rotors;
  for (int i = 0; i <= length -1; i++) {
    double journal = (engine.firing_order[i] * (360.0 / length)) * units.getUnit["deg"];
    engine.rod_journals[i] = journal;
  }
}

// Generate ignition timings
void setIgnitionTimings() {
  int length = engine.rotors;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i <= length - 1; i++) {
      int timing = engine.firing_order[i] + j * length;
      engine.ignition_timings[i + j * length] = timing;
    }
  }
}

// Check for Valid name/node string
bool is_valid_string(const std::string& str, const bool canContainSpace) {
    return std::all_of(str.begin(), str.end(), [&](unsigned char c){
        return std::isalnum(c) || c == '_' || (canContainSpace && c == ' ');
    });
}


// Get integer input
void getIntInput(std::string prompt, int& inp, std::string defaultUnit) {
  bool recheck = true;
  while (recheck) {
    std::string input;
    std::cout << prompt << " [" << (int)(inp / units.getUnit[defaultUnit]) << "]: ";
    std::getline(std::cin, input);
    input = (input == "") ? std::to_string(inp / units.getUnit[defaultUnit]) + defaultUnit : input;
    std::stringstream ss(input);
    std::string value; std::string unit;
    try {
      ss >> value >> unit;
      unit = (unit == "") ? defaultUnit : unit;
      auto it = units.getUnit.find(unit);
      if (it != units.getUnit.end()) {
        inp = std::stoi(value) * units.getUnit[unit];
        recheck = false;
      } else {
        std::cerr << "Invalid argument: " << input << std::endl;
      }
    } catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument: " << input << std::endl;
    }
  }
}

// Get decimal input
void getDoubleInput(std::string prompt, double& inp, std::string defaultUnit) {
  bool recheck = true;
  while (recheck) {
    std::string input;
    std::cout << prompt << " [" << inp / units.getUnit[defaultUnit] << "]: ";
    std::getline(std::cin, input);
    input = (input == "") ? std::to_string(inp / units.getUnit[defaultUnit]) + defaultUnit : input;
    std::stringstream ss(input);
    try {
      std::string value; std::string unit;
      ss >> value >> unit;
      unit = (unit == "") ? defaultUnit : unit;
      auto it = units.getUnit.find(unit);
      if (it != units.getUnit.end()) {
        inp = std::stod(value) * units.getUnit[unit];
        recheck = false;
      } else {
        std::cerr << "Invalid argument: " << input << std::endl;
      }
    } catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument: " << input << std::endl;
    }
  }
}

// Get string input
void getStringInput(std::string prompt, std::string& inp) {
  bool recheck = true;
  while (recheck) {
    std::string input;
    std::cout << prompt << " [" << inp << "]: ";
    std::getline(std::cin, input);
    input = (input == "") ? inp : input;
    if (is_valid_string(input, true)) {
      inp = input;
      recheck = false;
    } else {
      std::cout << "Invalid argument: " << input << std::endl;
    }
  }
}

// Get firing order input
void getFiringOrder() {
  int length = engine.rotors;
  std::stringstream ss;
  for (int i = 0; i <= length - 1; ++i) {
    ss << engine.firing_order[i];
    if (i < length - 1) {
      ss << ", ";
    }
  }
  std::string firing_order_string = ss.str();
  bool recheck = true;
  while (recheck) {
    std::string input;
    std::cout << "Firing Order [" << firing_order_string << "]: ";
    std::getline(std::cin, input);
    input = (input == "") ? firing_order_string : input;
    try {
      std::regex pattern("^((\\d+, ){" + std::to_string(length - 1) + "}\\d+|\\d+)$");
      if (std::regex_match(input, pattern)) {
        std::vector<std::string> num_strs = split(input, ',');
        for (int i = 0; i <= length - 1; ++i) {
          engine.firing_order[i] = std::stod(num_strs[i]);
        }
        recheck = false;
      } else {
        std::cerr << "Invalid argument: " << input << std::endl;
      }
    } catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument: " << input << std::endl;
    }
  }
}