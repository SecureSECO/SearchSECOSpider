# Spider Revisited

This is the Spider of the SearchSECO project. The Spider is responsible for retrieving all relevant data (source files and author data) from (Git-based) sources on the internet and comminicating this data to the Controller component of the system. The spider needs a stable connection to the source in question to be able to clone projects, to then process these locally.

# Installation

Download the .zip containing the source files and extract the files.
# Dependencies
* c++17, we use std::filesystem.
* Windows 10.
# Building

## Stand-alone
The standalone .exe file can only be built using Visual Studio.
### Visual Studio
Open the solution file and build the project in Visual Studio, with Standalone as startup project. Note that this will always spider the same repository, as defined in main.

## Library
The spider can be built as a library using both Visual Studio and CMake.
### Visual Studio
Open the solution file and build the Spider-Revisited project in Visual Studio. The library can then be found under ../x64/Debug/Spider-Revisited.lib.

### CMake
Make sure to have CMake [https://cmake.org/](https://cmake.org/) installed. Start a command prompt in the root folder and run the following commands:
- `mkdir build && cd build`
- `cmake ../Spider-Revisited`
- `cmake --build .`
The library can then be found from the build directory under /Debug/spider.lib.

# Copyright

This program has been developed by students from the bachelor Computer Science at Utrecht University within the Software Project course.

© Copyright Utrecht University (Department of Information and Computing Sciences)


