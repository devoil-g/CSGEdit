# CSG-Raytracer
How to build (Windows):

1. Get SFML prebuilt libraries at http://www.sfml-dev.org/download/sfml/2.3.2/
2. Extract SFML library archive in directory \<library\>/SFML-2.3.2
3. Set environment variable SFML_ROOT to \<library\>/SFML-2.3.2
4. Clone repository in directory \<sources\>
5. Create a directory \<build\>
6. CMake-GUI :

  a. Set source code directory to \<sources\>

  b. Set build directory to \<build\>

  c. Configure (it will fail)

  d. Add an entry CMAKE_MODULE_PATH of type PATH with value \<library\>/SFML-2.3.2/cmake/Modules

  e. Configure

  d. Generate

7. Project has been generated in \<build\>
