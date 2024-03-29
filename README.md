# SearchSECOSpider
![Testing](https://github.com/SecureSECO/SearchSECOSpider/actions/workflows/testing.yml/badge.svg)

This is the Spider of the SearchSECO project. The Spider is responsible for retrieving data (source files and author data) from (Git-based) sources on the internet and communicating this data to the Controller component of the system. The spider needs a stable internet connection to the source in question to be able to clone projects, to then process these locally.

# Installation

Download the .zip containing the source files and extract the files.
# Dependencies
* c++20
* Windows 10. Might also work on other systems, but has not been tested.
# Building

## Stand-alone
Open a command line at the root of the project file structure, and run
```
mkdir build && cd build
cmake ../SearchSECOSpider
cmake --build .
```
This will build an executable file called `SearchSECOSpider`, which can be run.
### Visual Studio
Open the solution file and build the project in Visual Studio 2019, with SearchSECOSpider as the startup project.
The standalone project was made for testing purposes to be able to run the project, as opposed to only building a library.
There are no practical uses for the standalone project apart from testing the SearchSECOSpider library.

## Library
The spider can be built as a library using both Visual Studio 2019 and CMake.
### Visual Studio
Open the solution file and build the Spider-Revisited project in Visual Studio 2019. The library can then be found under ../x64/Debug/libSearchSECOSpider.lib.

### CMake
Make sure to have CMake [https://cmake.org/](https://cmake.org/) installed. Open a command line at the root of the project file structure, and run
```
mkdir build && cd build
cmake ../libSearchSECOSpider
cmake --build .
```
The library can then be found from the build directory under /Debug/spider.lib.

# License

This project is licensed under the GNU Affero General Public License v3.0. See [LICENSE](LICENSE) for more info.

This program has been developed by students from the bachelor Computer Science at Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)


