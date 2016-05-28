#include <fstream>
#include <iostream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

#include "OpenCLStructures.hpp"
#include "OpenCLRenderer.hpp"

RT::OpenCLRenderer::OpenCLRenderer()
  : _scene(nullptr)
{
  std::ifstream	filestream(RT::Config::ExecutablePath + RT::Config::Renderer::OpenCL::KernelFileName);

  // In case of failure
  if (filestream.bad())
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  std::string	sources((std::istreambuf_iterator<char>(filestream)), std::istreambuf_iterator<char>());

  // Error if invalid file
  if (sources.empty())
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Build OpenCL program
  try
  {
    _program = cl::Program(sources);
    _program.build();
    _queue = cl::CommandQueue(_program.getInfo<CL_PROGRAM_CONTEXT>());
    _kernel = cl::Kernel(_program, RT::Config::Renderer::OpenCL::KernelName.c_str());
    _camera = cl::Buffer(_program.getInfo<CL_PROGRAM_CONTEXT>(), CL_MEM_READ_ONLY, sizeof(float) * 16);
  }
  catch (cl::Error)
  {
    // Display build log
    std::cerr << "[OpenCL] Failed to build program." << std::endl;
    for (cl::Device device : _program.getInfo<CL_PROGRAM_DEVICES>())
      std::cerr << "Device: " << device.getInfo<CL_DEVICE_NAME>() << ", " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl
      << " Build status: " << _program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device) << std::endl
      << " Build options: " << _program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(device) << std::endl
      << " Build log:" << std::endl << _program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  }
}

RT::OpenCLRenderer::~OpenCLRenderer()
{}

void			RT::OpenCLRenderer::load(RT::Scene * scene)
{
  stop();
  
  _scene = scene;
  _progress = 0;

  std::vector<RT::OpenCL::Node>	      nodes;
  std::vector<RT::OpenCL::Primitive>  primitives;

  // Build data structure
  _scene->csg()->build(nodes, primitives, Math::Matrix<4, 4>::identite(), RT::Material());

  // OpenCL allocations
  _image = cl::Buffer(_program.getInfo<CL_PROGRAM_CONTEXT>(), CL_MEM_WRITE_ONLY, sizeof(unsigned char) * _scene->image().getSize().x * _scene->image().getSize().y * 4);
  _nodes = cl::Buffer(_program.getInfo<CL_PROGRAM_CONTEXT>(), CL_MEM_READ_ONLY, sizeof(RT::OpenCL::Node) * nodes.size());
  _primitives = cl::Buffer(_program.getInfo<CL_PROGRAM_CONTEXT>(), CL_MEM_READ_ONLY, sizeof(RT::OpenCL::Primitive) * primitives.size());

  // Get camera
  float		camera[16];

  for (unsigned row = 0; row < 4; row++)
    for (unsigned col = 0; col < 4; col++)  
      camera[row * 4 + col] = (float)_scene->camera()(row, col);

  // Write data to OpenCL memory
  _queue.enqueueWriteBuffer(_nodes, CL_TRUE, 0, sizeof(RT::OpenCL::Node) * nodes.size(), nodes.data());
  _queue.enqueueWriteBuffer(_primitives, CL_TRUE, 0, sizeof(RT::OpenCL::Primitive) * primitives.size(), primitives.data());
  _queue.enqueueWriteBuffer(_camera, CL_TRUE, 0, sizeof(float) * 16, camera);

  // Set kernel parameters
  _kernel.setArg(0, _nodes);
  _kernel.setArg(1, _primitives);
  _kernel.setArg(2, _image);
  _kernel.setArg(3, _camera);
  _kernel.setArg(4, (int)_scene->image().getSize().x);
  _kernel.setArg(5, (int)_scene->image().getSize().y);

  _kernel.setArg(6, (int)primitives.size());
}

void			RT::OpenCLRenderer::begin()
{
  try
  {
    // Launch kernel (one thread for each pixel)
    _queue.enqueueNDRangeKernel(_kernel, cl::NullRange, cl::NDRange(_scene->image().getSize().x, _scene->image().getSize().y), cl::NullRange);

    // Write computed data 
    _queue.enqueueReadBuffer(_image, CL_TRUE, 0, sizeof(unsigned char) * _scene->image().getSize().x * _scene->image().getSize().y * 4, (void *)_scene->image().getPixelsPtr());
  }
  catch (cl::Error)
  {
    std::cerr << "[OpenCL] Critical error." << std::endl;
  }

  _progress = 1;
}

void CL_CALLBACK	RT::OpenCLRenderer::error(char const * info, void const *, ::size_t, void *)
{
  std::cerr << "[OpenCL] Error: " << std::string(info) << "." << std::endl;
}

double			RT::OpenCLRenderer::progress()
{
  return (double)_progress;
}
