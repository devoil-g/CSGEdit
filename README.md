# CSG-Raytracer

![Alt text](/assets/screenshots/test_1.png "Example")

## How to build (Windows):

1. Get SFML prebuilt libraries at http://www.sfml-dev.org/download/sfml/2.3.2/
2. Extract SFML library archive in directory \<library\>/SFML-2.3.2
3. Set environment variable SFML_ROOT to \<library\>/SFML-2.3.2
4. Get ChaiScript 5.7.1 (installer) at https://github.com/ChaiScript/ChaiScript/releases
5. Install ChaiScript in directory \<library\>/chaiscript 5.7.1
6. Clone repository in directory \<sources\>
7. Create a directory \<build\>
8. CMake-GUI :
  1. Set source code directory to \<sources\>
  2. Set build directory to \<build\>
  3. Configure (it will fail)
  4. Add an entry CMAKE_MODULE_PATH of type PATH with value \<library\>/SFML-2.3.2/cmake/Modules
  5. Set entry CHAISCRIPT_INCLUDE_DIR to \<library\>/chaiscript 5.7.1/include
  6. Configure
  7. Generate
9. Project has been generated in \<build\>

## How tu build (Linux):

You use Linux. You're strong. Do it yourself. Use CMake.
