# EasyIni
## About
EasyIni is a simple library for reading and writing INI files. It is written in C++ and is cross-platform. 
This library is a single header file that can be easily added to any project. Feel free to use it in your own projects.
It may not be the most efficient or feature-rich library, but it is simple and easy to use.
## Usage
### Reading
```cpp
#include <EasyIni/EasyIni.hpp>

int main() {
    // read the file and create a configuration object
    EasyIni::Configuration ini("config.ini");
    
    auto intVal = ini["Section"]["IntValue"].get<int>(); // intVal = 5
    auto strVal = ini["Section"]["StringValue"].get<std::string>(); // std::string = "Hello World!"
    auto boolVal = ini["Section"]["BoolValue"].get<bool>(); // boolVal = true
    auto floatVal = ini["Section"]["FloatValue"].get<float>(); // floatVal = 3.14f
    auto doubleVal = ini["Section"]["DoubleValue"].get<double>(); // doubleVal = 3.14159265359
    
    // get a value or a default value if it doesn't exist. If it doesn't exist, it will be added to the configuration
    auto doesNotExist = ini["Section"]["DoesNotExist"].getOrDefault<std::string>("Default"); // doesNotExist = "Default"
    
    // add a value to the configuration
    ini["Section"]["NewValue"] = "Hello World!";
    ini["Section"]["NewIntValue"] = 10;
    
    return 0;
}
```
### Writing
```cpp
#include <EasyIni/EasyIni.hpp>

int main() {
    // create a configuration object
    EasyIni::Configuration ini("Config.ini");
    bool overwrite = true;
    
    // add a section
    ini["Section"]["IntValue"] = 5;
    ini["Section"]["StringValue"] = "Hello World!";
    ini["Section"]["BoolValue"] = true;
    ini["Section"]["FloatValue"] = 3.14f;
    ini["Section"]["DoubleValue"] = 3.14159265359;
    
    // write the configuration to a file
    if (overwrite)
        ini.save(); // overwrites the file that was read
    else
        ini.save("new_config.ini"); // writes to a new file
    return 0;
}
```