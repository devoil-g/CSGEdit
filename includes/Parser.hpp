#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <chaiscript/chaiscript.hpp>
#include <stack>
#include <string>
#include <vector>

#include "AbstractCsgNode.hpp"
#include "AbstractCsgTree.hpp"
#include "AbstractLightNode.hpp"
#include "AbstractLightTree.hpp"
#include "Scene.hpp"

namespace RT
{
  namespace Config
  {
    namespace Parser
    {
      unsigned int const	MaxFileDeph(8);	// Maximum deph of included files
    };
  };

  class Parser
  {
  private:
    std::stack<std::pair<RT::AbstractCsgNode *, RT::AbstractLightNode *>>	_scope;		// Stack of scopes
    std::stack<std::string>							_files;		// Stack of loaded files
    chaiscript::ChaiScript							_script;	// Stack of scripts
    RT::Scene									_scene;		// Scene being currently loaded

    Parser();
    ~Parser();

    // Scope CSG
    void	scopeDifference();	// Push a CSG difference in scope
    void	scopeIntersection();	// Push a CSG intersection in scope
    void	scopeUnion();		// Push a CSG union in scope
    // Scope transformations
    void	scopeTransformation(std::vector<std::vector<double>> const &);	// Push a transformation in scope
    void	scopeTranslation(std::vector<double> const &);			// Push a translation in scope
    void	scopeMirror(std::vector<double> const &);			// Push a mirror in scope
    void	scopeRotation(std::vector<double> const &);			// Push a rotation in scope
    void	scopeScale(std::vector<double> const &);			// Push a scale in scope
    void	scopeShear(double, double, double, double, double, double);	// Push a shear in scope
    // Scope materials
    void	scopeMaterial(std::string const &);				// Push a material in scope
    void	scopeColor(RT::Color const &);					// Push a color material in scope
    void	scopeDirect(double, double, double, double, unsigned int);	// Push a direct illumination material in scope
    void	scopeIndirect(double, double, double, double, unsigned int);	// Push a global illumination material in scope
    void	scopeTransparency(double, double, double, unsigned int);	// Push a transparency material in scope
    void	scopeReflection(double, double, unsigned int);			// Push a reflection material in scope
    // Scope others
    void	scopeBounding();		// Push a bounding node in scope
    void	scopeMesh();			// Push a mesh node in scope
    void	scopeDeph(unsigned int);	// Push a deph limit in scope
    void	scopeDephCsg(unsigned int);	// Push a CSG deph limit in scope
    void	scopeDephLight(unsigned int);	// Push a light deph limit in scope
    // Scope utlilites
    void	scopeStart(RT::AbstractCsgNode *);				// Push a new scope in stack
    void	scopeStart(RT::AbstractLightNode *);				// Push a new scope in stack
    void	scopeStart(RT::AbstractCsgNode *, RT::AbstractLightNode *);	// Push a new scope in stack
    void	scopeEnd();							// Pop last scope in stack
    void	scopeSimplify(RT::AbstractCsgNode *);				// Simplify a CSG node
    void	scopeSimplify(RT::AbstractLightNode *);				// Simplify a light node

    // Primitives
    void	primitiveBox(double, double, double, bool);									// Push a box in top scope
    void	primitiveCone(double, double, double, bool);									// Push a cone in top scope
    void	primitiveMobius(double, double, double);									// Push a mobius strip in top scope
    void	primitiveSphere(double);											// Push a sphere in top scope
    void	primitiveTangle(double);											// Push a tangle in top scope
    void	primitiveTorus(double, double);											// Push a torus in top scope
    void	primitiveTriangle(std::vector<double> const &, std::vector<double> const &, std::vector<double> const &);	// Push a triangle in top scope (only if mesh node)
    // Primitives utilities
    void	primitivePush(RT::AbstractCsgTree *);	// Push a primitive in top scope

    // Light
    void	lightDirectional(RT::Color const &, double);			// Add a directional light
    void	lightOcclusion(RT::Color const &, double);			// Add an occlusion light
    void	lightPoint(RT::Color const &, double, double, double, double);	// Add a point light
    // Light utilities
    void	lightPush(RT::AbstractLightTree *);	// Add a light in scene

    // Settings
    void	settingCamera(std::vector<double> const &, std::vector<double> const &, std::vector<double> const &);	// Set camera position for current scene
    void	settingResolution(unsigned int, unsigned int);								// Set resolution of current scene
    void	settingAntiAliasing(unsigned int, unsigned int);							// Set antialiasing level of current scene
    void	settingDephOfField(double, double, unsigned int);							// Set deph of field parameters of current scene
    void	settingVirtualReality(double, double, double);								// Set virtual reality parameters of current scene
    void	settingThread(unsigned int);										// Set the number of thread for rendering

    // Utilities
    void	import(std::string const &);	// Import file in current scope
    void	include(std::string const &);	// Include file in current scope

    std::string	directory(std::string const &) const;	// Return directory path of file (for relative import)
    RT::Scene	parse(std::string const &);		// Generate CSG tree from script path

  public:
    static RT::Scene	load(std::string const &);	// Generate CSG tree from script path
  };
};

#endif
