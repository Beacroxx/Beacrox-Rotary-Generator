# Rotary Engine Generator  
This repository contains C++ code for generating rotary engines for use in the engine simulator by Ange Yaghi.  
## Usage  
- Default units are in (brackets) and default values are in [square brackets].  
- Leave the input empty to use defaults.  
- If no unit specified, the default unit is used.  
- To specify units, separate the value and unit with a space. Ex: '5 cm' or '25 cc'.  
- All units available in the Engine Simulator are also available in this Generator.  
## Requirements   
- The engine simulator by Ange Yaghi. (0.1.12.0a minimum)
- g++
## Downloads  
Downloads of the precompiled binary (Linux) can be found in the [Releases](https://github.com/StanTheAwesomeMan/Beacrox-Rotary-Generator/releases).  
## How to Compile 
- Clone this repository to your local machine  
- Navigate to the root folder  
Example: ```cd ~/Beacrox-Rotary-Generator/```  
- Compile the code using g++  
```g++ -o rotary_gen ./src/main.cpp```   
- Run the compiled binary to generate the rotary engine files  
```./rotary_gen```  
- Import the generated files into the engine simulator  
## License  
This project is licensed under the MIT License - see the [LICENSE](https://github.com/StanTheAwesomeMan/Beacrox-Rotary-Generator/blob/master/LICENSE) for details.  
