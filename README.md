# CSGEdit

![Alt text](/assets/screenshots/test_1.png "Example")

## How to build (Windows):

1. Get SFML prebuilt libraries at http://www.sfml-dev.org/download.php
2. Extract SFML library archive in directory \<library\>/SFML-2.x.x
3. Set environment variable SFML_ROOT to \<library\>/SFML-2.x.x
4. Get ChaiScript 5.X.X (installer) at https://github.com/ChaiScript/ChaiScript/releases
5. Install ChaiScript in directory \<library\>/chaiscript 5.x.x
6. Get an OpenCL SDK for your platform:
  1. NVidia: https://developer.nvidia.com/opencl
  2. AMD: http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/
  3. Intel: https://software.intel.com/en-us/intel-opencl/download
7. Install your OpenCL SDK
8. Clone repository in directory \<sources\>
9. Create a directory \<build\>
10. CMake-GUI :
  1. Set source code directory to \<sources\>
  2. Set build directory to \<build\>
  3. Configure (it will fail)
  4. Add an entry CMAKE_MODULE_PATH of type PATH with value \<library\>/SFML-2.x.x/cmake/Modules
  5. Set entry CHAISCRIPT_INCLUDE_DIR to \<library\>/chaiscript 5.x.x/include
  6. Configure
  7. Generate
11. Project has been generated in \<build\>

## How tu build (Linux):

You use Linux. You're strong. Do it yourself. Use CMake.
