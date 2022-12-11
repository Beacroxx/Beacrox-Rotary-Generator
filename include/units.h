#ifndef UNITS_H
#define UNITS_H

#include <map>
#include <string>

struct Units {
  std::map<std::string, double> getUnit = {
      // Pi
      {"pi", 3.14159265359},

      // Mass
      {"kg", 1.0},
      {"g", 0.001},
      {"lb", 0.45359237},
      {"lbmol", 453.59237},

      // Length
      {"m", 1.0},
      {"cm", 0.01},
      {"mm", 0.001},
      {"km", 1000.0},
      {"inch", 0.0254},
      {"foot", 0.3048},
      {"thou", 0.0000254},
      {"mile", 1609.344},

      // Time
      {"sec", 1.0},
      {"minute", 60.0},
      {"hour", 3600.0},

      // Force
      {"N", 1.0},
      {"lbf", 4.44822},
      {"Nm", 1.0},
      {"lb_ft", 1.355818},

      // Volume
      {"m3", 1.0},
      {"cc", 0.000001},
      {"mL", 0.000001},
      {"L", 0.001},
      {"cubic_feet", 0.028316846592},
      {"cubic_inches", 0.000016387064},
      {"gal", 0.003785411784},

      // Amount of substance
      {"mol", 1.0},
      {"kmol", 0.001},
      {"mmol", 0.000001},
      {"mol_per_sec", 1.0},
      {"scfm", 0.0175204064},

      // Area
      {"m2", 1.0},
      {"cm2", 0.0001},

      // Pressure
      {"Pa", 1.0},
      {"kPa", 1000.0},
      {"MPa", 1000000.0},
      {"atm", 101325.0},
      {"psi", 6.89475729},
      {"psig", 6.89475729},
      {"inHg", 3386.3886666666713},
      {"inH2O", 249.08891},

      // Temperature
      {"K", 1.0},
      {"K0", 273.15},
      {"C", 1.0},
      {"F", 1.8},
      {"F0", -459.67},

      // Energy
      {"J", 1.0},
      {"kJ", 1000.0},
      {"MJ", 1000000.0},

      // Angle
      {"rad", 1.0},
      {"deg", 0.017453292519943295},

      // Speed
      {"rpm", 0.104719755},
      {"mph", 0.44704},

      // Other
      {"None", 1.0},
      {"hz", 1.0}};
};

Units units;

#endif // UNITS_H