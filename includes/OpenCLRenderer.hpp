#ifndef _OPENCL_RENDERER_HPP_
#define _OPENCL_RENDERER_HPP_

#define __CL_ENABLE_EXCEPTIONS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.hpp>

#include "AbstractRenderer.hpp"
#include "Config.hpp"
#include "Scene.hpp"

namespace RT
{
  namespace Config
  {
    namespace Renderer
    {
      namespace OpenCL
      {
	std::string const	KernelFileName("OpenCLRenderer.cl");
	std::string const	KernelName("PreviewKernel");
      };
    };
  };

  class OpenCLRenderer : public RT::AbstractRenderer
  {
  private:
    cl::Program		_program;	// OpenCL program
    cl::Kernel		_kernel;	// OpenCL kernel
    cl::CommandQueue	_queue;		// Command cue of current context
    cl::Buffer		_image;		// Rendering zone
    cl::Buffer		_camera;	// Camera array
    cl::Buffer		_nodes;		// Nodes array
    cl::Buffer		_primitives;	// Primitives array
    RT::Scene *		_scene;		// Scene to render
    unsigned int	_progress;	// Progression of rendering

    void			begin() override;					// Method managing rendering threads
    static void CL_CALLBACK	error(char const *, void const *, ::size_t, void *);	// OpenCL error callback

  public:
    OpenCLRenderer();
    ~OpenCLRenderer();

    void	load(RT::Scene *) override;	// Load a new scene
    double	progress() override;		// Return current progress (0-1)
  };
};

#endif
