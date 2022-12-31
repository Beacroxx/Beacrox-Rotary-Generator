#include "../include/engine_struct.h"
#include "../include/units.h"

Engine engine;

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

int createDir(const char *dir) {
#ifdef __linux__
  return mkdir(dir, 0777);
#else
  return mkdir(dir); /* Windows Fix */
#endif
}

void generate_engine() {

  // Generator status
  std::cout << std::endl;
  std::cout << "===== Status =====" << std::endl;

  // Generate Folder
  const std::string folder = "generated";

  std::cout << "Generating folder './" << folder << "'..." << std::endl;

  int result = createDir(folder.c_str());
  if (result != 0) {
    if (errno == EEXIST) {
      std::cout << "Folder exists, Continuing..." << std::endl;
    } else {
      std::cerr << "Error creating Folder 'generated'." << std::endl;
    }
  }

  // Generate File
  std::string filename("./generated/" + engine.node + ".mr");
  std::fstream file_out;
  file_out.open(filename, std::ios_base::out);
  if (!file_out.is_open()) {
    std::cerr << "Failed to open '" << filename << "'." << std::endl;
  }

  // Set precision and decimal position for the file
  file_out.setf(std::ios::fixed);
  file_out.precision(2);

  std::cout << "Generating '" << filename << "'..." << std::endl;

  // The main part of the Generator
  file_out << "import \"engine_sim.mr\"" << std::endl;
  file_out << "" << std::endl;
  file_out << "units units()" << std::endl;
  file_out << "constants constants()" << std::endl;
  file_out << "impulse_response_library ir_lib()" << std::endl;
  file_out << "label cycle(3 * 360 * units.deg)" << std::endl;
  file_out << "" << std::endl;
  file_out << "private node wires {" << std::endl;
  // Generate wire nodes
  for (int i = 1; i <= engine.rotors; i++) {
    file_out << "  output wire" << i << ": ignition_wire();" << std::endl;
  }
  file_out << "}" << std::endl;
  file_out << "" << std::endl;
  file_out << "private node " << engine.node << " {" << std::endl;
  file_out << "    alias output __out: engine;" << std::endl;
  file_out << "" << std::endl;
  file_out << "    wankel_engine engine(" << std::endl;
  file_out << "        name: \"" << engine.name << "\"," << std::endl;
  file_out << "        starter_torque: 100 * units.lb_ft," << std::endl;
  file_out << "        starter_speed: 1000 * units.rpm," << std::endl;
  file_out << "        redline: " << engine.redline / units.getUnit["rpm"]
           << " * units.rpm," << std::endl;
  file_out << "        fuel: fuel(" << std::endl;
  file_out << "            max_burning_efficiency: 0.9," << std::endl;
  file_out << "            max_dilution_effect: 1.5," << std::endl;
  file_out << "            burning_efficiency_randomness: 1.0" << std::endl;
  file_out << "        )," << std::endl;
  file_out << "        throttle_gamma: 4.0," << std::endl;
  file_out << "        hf_gain: 0.01," << std::endl;
  file_out << "        noise: 1.0," << std::endl;
  file_out << "        jitter: 0.5," << std::endl;
  file_out << "        simulation_frequency: "
           << engine.simulation_frequency / units.getUnit["hz"] << ","
           << std::endl;
  file_out << "        rotor_calculation_quality: 1024," << std::endl;
  file_out << "        rotor_housing_resolution: 1024" << std::endl;
  file_out << "    )" << std::endl;
  file_out << "" << std::endl;
  file_out << "    wires wires()" << std::endl;
  file_out << "" << std::endl;
  file_out << "    label stroke(" << engine.stroke / units.getUnit["cm"]
           << " * units.cm)" << std::endl;
  file_out << "    label radius(" << engine.radius / units.getUnit["cm"]
           << " * units.cm)" << std::endl;
  file_out << "    label depth(" << engine.depth / units.getUnit["cm"]
           << " * units.cm)" << std::endl;
  file_out << "    label intake_port_start("
           << engine.intake_port_start / units.getUnit["deg"] << " * units.deg)"
           << std::endl;
  file_out << "    label intake_port_end("
           << engine.intake_port_end / units.getUnit["deg"] << " * units.deg)"
           << std::endl;
  file_out << "    label exhaust_port_start("
           << engine.exhaust_port_start / units.getUnit["deg"]
           << " * units.deg)" << std::endl;
  file_out << "    label exhaust_port_end("
           << engine.exhaust_port_end / units.getUnit["deg"] << " * units.deg)"
           << std::endl;
  file_out << "    label rotor_mass(" << engine.rotor_mass / units.getUnit["g"]
           << " * units.g)" << std::endl;
  file_out << "    label rotor_I(disk_moment_of_inertia(mass: rotor_mass, "
              "radius: radius))"
           << std::endl;
  file_out << "" << std::endl;
  file_out << "    crankshaft c0(" << std::endl;
  file_out << "        throw: 0.5 * stroke," << std::endl;
  file_out << "        flywheel_mass: 5 * units.lb," << std::endl;
  file_out << "        mass: 5 * units.lb," << std::endl;
  file_out << "        friction_torque: 15.0 * units.lb_ft," << std::endl;
  file_out << "        moment_of_inertia: 0.22986844776863666 * 0.5,"
           << std::endl;
  file_out << "        position_x: 0.0," << std::endl;
  file_out << "        position_y: 0.0," << std::endl;
  file_out << "        tdc: 0.0" << std::endl;
  file_out << "    )" << std::endl;
  file_out << "" << std::endl;
  // Generate rod journals
  for (int i = 0; i <= engine.rotors - 1; i++) {
    file_out << "    rod_journal rj" << i
             << "(angle: " << engine.rod_journals[i] / units.getUnit["deg"]
             << " * units.deg)" << std::endl;
  }
  file_out << "    c0" << std::endl;
  // Add rod journals to crankshaft
  for (int i = 0; i <= engine.rotors - 1; i++) {
    file_out << "        .add_rod_journal(rj" << i << ")" << std::endl;
  }
  file_out << "" << std::endl;
  file_out << "    engine.add_crankshaft(c0)" << std::endl;
  file_out << "" << std::endl;
  // Generate rotor nodes
  for (int i = 0; i <= engine.rotors - 1; i++) {
    file_out << "    wankel_rotor rotor" << i << "(" << std::endl;
    file_out << "        mass: rotor_mass," << std::endl;
    file_out << "        moment_of_inertia: rotor_I," << std::endl;
    file_out << "        radius: radius," << std::endl;
    file_out << "        crown_gear_radius: 3 * 0.5 * stroke," << std::endl;
    file_out << "        angle: "
             << engine.rotor_angles[i] / units.getUnit["deg"] << " * units.deg,"
             << std::endl;
    file_out << "        depth: depth," << std::endl;
    file_out << "        depression_volume: "
             << engine.depression_volume / units.getUnit["cc"] << " * units.cc"
             << std::endl;
    file_out << "    )" << std::endl;
    file_out << "" << std::endl;
  }
  file_out << "    chevy_bbc_stock_intake intake(" << std::endl;
  file_out << "        carburetor_cfm: " << engine.maxCFM << "," << std::endl;
  file_out << "        idle_flow_rate_cfm: 0.15, //0.15," << std::endl;
  file_out << "        idle_throttle_plate_position: 0.9985" << std::endl;
  file_out << "    )" << std::endl;
  file_out << "" << std::endl;
  file_out << "    exhaust_system_parameters es_params(" << std::endl;
  file_out << "        outlet_flow_rate: k_carb(" << engine.maxCFM * 1.6 << "),"
           << std::endl;
  file_out << "        primary_tube_length: 10.0 * units.inch," << std::endl;
  file_out << "        primary_flow_rate: k_carb(" << engine.maxCFM / 2 << "),"
           << std::endl;
  file_out << "        velocity_decay: 1.0, //0.5" << std::endl;
  file_out << "        length: 100.0 * units.inch" << std::endl;
  file_out << "    )" << std::endl;
  file_out << "" << std::endl;
  file_out << "    exhaust_system exhaust0(" << std::endl;
  file_out << "        es_params," << std::endl;
  file_out << "        length: (180 + 72.0) * units.inch," << std::endl;
  file_out << "        audio_volume: 5.5," << std::endl;
  file_out << "        impulse_response: ir_lib.minimal_muffling_02)"
           << std::endl;
  file_out << "" << std::endl;
  // Add rotors to engine
  for (int i = 0; i <= engine.rotors - 1; i++) {
    file_out << "    engine.add_rotor(" << std::endl;
    file_out << "        intake," << std::endl;
    file_out << "        exhaust0," << std::endl;
    file_out << "        rotor" << i << "," << std::endl;
    file_out << "        rj" << i << "," << std::endl;
    file_out << "        wires.wire" << i + 1 << "," << std::endl;
    file_out << "        sound_attenuation: 1.0," << std::endl;
    file_out << "        primary_length: 3 * units.inch," << std::endl;
    file_out << "        intake_port: "
                "default_wankel_peripheral_port(intake_port_start, "
                "intake_port_end, k_carb("
             << engine.maxCFM / 2 << "))," << std::endl;
    file_out << "        exhaust_port: "
                "default_wankel_peripheral_port(exhaust_port_start, "
                "exhaust_port_end, k_carb("
             << engine.maxCFM / 2 << "))," << std::endl;
    file_out << "        intake_runner_cross_section_area: 4 * units.cm2,"
             << std::endl;
    file_out << "        exhaust_cross_section_area: 4 * units.cm2,"
             << std::endl;
    file_out << "        spark_plug_angle: 0" << std::endl;
    file_out << "    )" << std::endl;
    file_out << "" << std::endl;
  }
  file_out << "    function timing_curve(1000 * units.rpm)" << std::endl;
  file_out << "    timing_curve" << std::endl;
  file_out << "        .add_sample(0000 * units.rpm, 20 * units.deg)"
           << std::endl;
  file_out << "        .add_sample(1000 * units.rpm, 20 * units.deg)"
           << std::endl;
  file_out << "        .add_sample(2000 * units.rpm, 30 * units.deg)"
           << std::endl;
  file_out << "        .add_sample(3000 * units.rpm, 50 * units.deg)"
           << std::endl;
  file_out << "        .add_sample(4000 * units.rpm, 50 * units.deg)"
           << std::endl;
  file_out << "" << std::endl;
  file_out << "    engine.add_ignition_module(" << std::endl;
  file_out << "        ignition_module(timing_curve: timing_curve, rev_limit: "
           << engine.rev_limit / units.getUnit["rpm"]
           << " * units.rpm, limiter_duration: 0.05)" << std::endl;
  // Generate ignition timings
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i <= engine.rotors - 1; i++) {
      int wireN = 1 + ((i + j * engine.rotors) % engine.rotors);
      file_out << "            .connect_wire(wires.wire" << wireN << ", ("
               << engine.ignition_timings[i + j * engine.rotors] << " / "
               << engine.rotors * 3 << ".0) * cycle)" << std::endl;
    }
  }
  file_out << "    )" << std::endl;
  file_out << "}" << std::endl;
  file_out << "" << std::endl;
  file_out << "label car_mass(1500 * units.kg)" << std::endl;
  file_out << "" << std::endl;
  file_out << "private node generic_racecar {" << std::endl;
  file_out << "    alias output __out:" << std::endl;
  file_out << "        vehicle(" << std::endl;
  file_out << "            mass: car_mass," << std::endl;
  file_out << "            drag_coefficient: 0.5," << std::endl;
  file_out << "            cross_sectional_area: (72 * units.inch) * (32 * "
              "units.inch),"
           << std::endl;
  file_out << "            diff_ratio: 3.9," << std::endl;
  file_out << "            tire_radius: 16 * units.inch," << std::endl;
  file_out << "            rolling_resistance: 0.015 * car_mass * 9.81,"
           << std::endl;
  file_out << "            stiffness: 50 * units.lb_ft / units.deg,"
           << std::endl;
  file_out << "            damping: 15.0," << std::endl;
  file_out << "            max_flex: 5 * units.deg," << std::endl;
  file_out << "            limit_flex: true," << std::endl;
  file_out << "            simulate_flex: true" << std::endl;
  file_out << "        );" << std::endl;
  file_out << "}" << std::endl;
  file_out << "" << std::endl;
  file_out << "private node racecar_transmission {" << std::endl;
  file_out << "    alias output __out:" << std::endl;
  file_out << "        transmission(" << std::endl;
  file_out << "            max_clutch_torque: 700 * units.lb_ft," << std::endl;
  file_out << "            max_clutch_flex: 5 * units.deg," << std::endl;
  file_out << "            limit_clutch_flex: true," << std::endl;
  file_out << "            clutch_stiffness: 50 * units.lb_ft / units.deg,"
           << std::endl;
  file_out << "            clutch_damping: 2.0," << std::endl;
  file_out << "            simulate_flex: true" << std::endl;
  file_out << "        )" << std::endl;
  file_out << "        .add_gear(3.140)" << std::endl;
  file_out << "        .add_gear(2.308)" << std::endl;
  file_out << "        .add_gear(1.75)" << std::endl;
  file_out << "        .add_gear(1.5)" << std::endl;
  file_out << "        .add_gear(1.2);" << std::endl;
  file_out << "}" << std::endl;
  file_out << "" << std::endl;
  file_out << "public node main {" << std::endl;
  file_out << "    set_engine(" << engine.node << "())" << std::endl;
  file_out << "    set_vehicle(generic_racecar())" << std::endl;
  file_out << "    set_transmission(racecar_transmission())" << std::endl;
  file_out << "}" << std::endl;
  file_out << "" << std::endl;
  file_out << "main()" << std::endl;

  std::cout << "Generated '" << filename << "'." << std::endl;
}
