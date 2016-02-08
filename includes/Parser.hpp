#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <stack>
#include <string>

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
    void	scopeTranslation(double, double, double);			// Push a translation in scope
    void	scopeMirror(double, double, double);				// Push a mirror in scope
    void	scopeRotation(double, double, double);				// Push a rotation in scope
    void	scopeScale(double, double, double);				// Push a scale in scope
    void	scopeShear(double, double, double, double, double, double);	// Push a shear in scope
    // Scope materials
    void	scopeMaterial(std::string const &);		// Push a material in scope
    void	scopeColor(double, double, double);		// Push a color material in scope
    void	scopeAmbient(double, double, double);		// Push a ambient color material in scope
    void	scopeDiffuse(double, double, double);		// Push a diffuse color material in scope
    void	scopeSpecular(double, double, double, double);	// Push a specular color and shiness material in scope
    void	scopeReflection(double);			// Push a reflection material in scope
    void	scopeTransparency(double, double);		// Push a transparency and refraction material in scope
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
    void	lightDirectional(double, double, double, double, unsigned int);				// Add a directional light
    void	lightOcclusion(double, double, double, double, unsigned int);				// Add an occlusion light
    void	lightPoint(double, double, double, double, double, double, double, unsigned int);	// Add a point light
    // Light utilities
    void	lightPush(RT::AbstractLight *);	// Add a light in scene

    // Settings
    void	settingCamera();				// Set camera position for current scene
    void	settingResolution(unsigned int, unsigned int);	// Set resolution of current scene

    RT::AbstractTree *	import(std::string const &);	// Import file in current scope
    
  public:
    Parser();
    ~Parser();
    
    RT::Scene *		load(std::string const &);	// Generate CSG tree from script path
  };
};

#endif