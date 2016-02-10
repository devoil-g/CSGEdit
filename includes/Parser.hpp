#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <chaiscript/chaiscript.hpp>
#include <stack>
#include <string>
#include <vector>

#include "AbstractLight.hpp"
#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "Matrix.hpp"
#include "Scene.hpp"

namespace RT
{
  class Parser
  {
  private:
    std::stack<RT::AbstractNode *>	_scope;			// Stack of scopes
    std::stack<Math::Matrix<4, 4>>	_transformation;	// Stack of transformations
    std::stack<std::string>		_files;			// Stack of loaded files
    RT::Scene *				_scene;			// Scene being currently loaded

    // Scope CSG
    void	scopeDifference();	// Push a CSG difference in scope
    void	scopeIntersection();	// Push a CSG intersection in scope
    void	scopeUnion();		// Push a CSG union in scope
    // Scope transformations
    void	scopeTranslation(const std::vector<chaiscript::Boxed_Value> &);		// Push a translation in scope
    void	scopeMirror(const std::vector<chaiscript::Boxed_Value> &);		// Push a mirror in scope
    void	scopeRotation(const std::vector<chaiscript::Boxed_Value> &);		// Push a rotation in scope
    void	scopeScale(const std::vector<chaiscript::Boxed_Value> &);		// Push a scale in scope
    void	scopeShear(double, double, double, double, double, double);		// Push a shear in scope
    // Scope materials
    void	scopeMaterial(std::string const &);					// Push a material in scope
    void	scopeColor(const std::vector<chaiscript::Boxed_Value> &);		// Push a color material in scope
    void	scopeAmbient(const std::vector<chaiscript::Boxed_Value> &);		// Push a ambient color material in scope
    void	scopeDiffuse(const std::vector<chaiscript::Boxed_Value> &);		// Push a diffuse color material in scope
    void	scopeSpecular(const std::vector<chaiscript::Boxed_Value> &, double);	// Push a specular color and shiness material in scope
    void	scopeReflection(double);						// Push a reflection material in scope
    void	scopeTransparency(double, double);					// Push a transparency and refraction material in scope
    // Scope others
    void	scopeBounding();	// Push a bounding node in scope
    // Scope utlilites
    void	scopeStart(RT::AbstractNode *);	// Push a new scope in stack
    void	scopeEnd();			// Pop last scope in stack

    // Primitives
    void	primitiveBox(double, double, double, bool);	// Push a box in top scope
    void	primitiveCone(double, double, double, bool);	// Push a cone in top scope
    void	primitiveSphere(double);			// Push a sphere in top scope
    void	primitiveTangle(double);			// Push a tangle in top scope
    void	primitiveTorus(double, double);			// Push a torus in top scope
    void	primitiveMesh(std::string const &);		// Push a primitive in top scope
    // Primitives utilities
    void	primitivePush(RT::AbstractTree *);	// Push a primitive in top scope

    // Light
    void	lightDirectional(const std::vector<chaiscript::Boxed_Value> &, double, unsigned int);			// Add a directional light
    void	lightOcclusion(const std::vector<chaiscript::Boxed_Value> &, double, unsigned int);			// Add an occlusion light
    void	lightPoint(const std::vector<chaiscript::Boxed_Value> &, double, double, double, double, unsigned int);	// Add a point light
    // Light utilities
    void	lightPush(RT::AbstractLight *);	// Add a light in scene

    // Settings
    void	settingCamera();				// Set camera position for current scene
    void	settingResolution(unsigned int, unsigned int);	// Set resolution of current scene

    // Utilities
    RT::AbstractTree *	import(std::string const &);	// Import file in current scope

    template<typename X>
    std::vector<X>	convertVector(const std::vector<chaiscript::Boxed_Value> & v) const	// Convert ChaiScript vector to std::vector of type X
    {
      std::vector<X>  result;

      std::transform(v.begin(), v.end(), std::back_inserter(result), [](const chaiscript::Boxed_Value &bv) { return chaiscript::Boxed_Number(bv).get_as<X>(); });
      return result;
    }
    
  public:
    Parser();
    ~Parser();
    
    RT::Scene *		load(std::string const &);	// Generate CSG tree from script path
  };
};

#endif