# EasyIni
## About
EasyIni is a simple library for reading and writing INI files. It is written in C++ and is cross-platform. 
This library is a single header file that can be easily added to any project. Feel free to use it in your own projects.
## Usage
EasyIni is designed to be easy to use. It may not be the fastest or most efficient library, but it is simple and easy to
use.
### Reading
```cpp
#include "EasyIni.h"

int main() {
    // read the file and create a configuration object
    EasyIni::Configuration ini("config.ini");
    
    auto intVal = ini["Section"]["IntValue"].get<int>(); // intVal = 5
    auto strVal = ini["Section"]["StringValue"].get(); // Each value is returned as a std::string by default
    auto boolVal = ini["Section"]["BoolValue"].get<bool>(); // boolVal = true
    auto floatVal = ini["Section"]["FloatValue"].get<float>(); // floatVal = 3.14f
    auto doubleVal = ini["Section"]["DoubleValue"].get<double>(); // doubleVal = 3.14159265359
    
    // add a value to the configuration
    ini["Section"]["NewValue"] = "Hello World!";
    ini["Section"]["NewIntValue"] = 10;
    
    // write the configuration to a file
    ini.save(); // overwrites the file that was read
    ini.save("new_config.ini"); // writes to a new file
    return 0;
}
```