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
  namespace Config
  {
    namespace Parser
    {
      unsigned int const	MaxFileDeph(8);	// Maximum deph of included/imported files
    };
  };

  class Parser
  {
  private:
    std::stack<RT::AbstractNode *>		_scope;			// Stack of scopes
    std::stack<Math::Matrix<4, 4>>		_transformation;	// Stack of transformations
    std::stack<std::string>			_files;			// Stack of loaded files
    std::stack<chaiscript::ChaiScript *>	_script;		// Stack of scripts
    RT::Scene *					_scene;			// Scene being currently loaded

    // Scope CSG
    void	scopeDifference();	// Push a CSG difference in scope
    void	scopeIntersection();	// Push a CSG intersection in scope
    void	scopeUnion();		// Push a CSG union in scope
    // Scope transformations
    void	scopeTransformation(std::vector<chaiscript::Boxed_Value> const &);	// Push a transformation in scope
    void	scopeTranslation(std::vector<chaiscript::Boxed_Value> const &);		// Push a translation in scope
    void	scopeMirror(std::vector<chaiscript::Boxed_Value> const &);		// Push a mirror in scope
    void	scopeRotation(std::vector<chaiscript::Boxed_Value> const &);		// Push a rotation in scope
    void	scopeScale(std::vector<chaiscript::Boxed_Value> const &);		// Push a scale in scope
    void	scopeShear(double, double, double, double, double, double);		// Push a shear in scope
    // Scope materials
    void	scopeMaterial(std::string const &);																		// Push a material in scope
    void	scopeColor(std::vector<chaiscript::Boxed_Value> const &);															// Push a color material in scope
    void	scopeLight(std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, double, unsigned int);	// Push a light material in scope
    void	scopeTransparency(double, double, double, unsigned int);															// Push a transparency material in scope
    void	scopeReflection(double, double, unsigned int);																	// Push a reflection material in scope
    // Scope others
    void	scopeBounding();	// Push a bounding node in scope
    void	scopeMesh();		// Push a mesh node in scope
    // Scope utlilites
    void	scopeStart(RT::AbstractNode *);	// Push a new scope in stack
    void	scopeEnd();			// Pop last scope in stack

    // Primitives
    void	primitiveBox(double, double, double, bool);															// Push a box in top scope
    void	primitiveCone(double, double, double, bool);															// Push a cone in top scope
    void	primitiveSphere(double);																	// Push a sphere in top scope
    void	primitiveTangle(double);																	// Push a tangle in top scope
    void	primitiveTorus(double, double);																	// Push a torus in top scope
    void	primitiveTriangle(std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &);	// Push a triangle in top scope (only if mesh node)
    void	primitiveMesh(std::string const &);																// Push a primitive in top scope
    // Primitives utilities
    void	primitivePush(RT::AbstractTree *);	// Push a primitive in top scope

    // Light
    void	lightDirectional(std::vector<chaiscript::Boxed_Value> const &, double);				// Add a directional light
    void	lightOcclusion(std::vector<chaiscript::Boxed_Value> const &, double);				// Add an occlusion light
    void	lightPoint(std::vector<chaiscript::Boxed_Value> const &, double, double, double, double);	// Add a point light
    // Light utilities
    void	lightPush(RT::AbstractLight *);	// Add a light in scene

    // Settings
    void	settingCamera();														// Set camera position for current scene
    void	settingResolution(unsigned int, unsigned int);											// Set resolution of current scene
    void	settingAntiAliasing(unsigned int, unsigned int);										// Set antialiasing level of current scene
    void	settingDephOfField(double, double, unsigned int);										// Set deph of field parameters of current scene
    void	settingAnaglyph3D(double, double, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &);	// Set 3D anaglyph parameters of current scene
    void	settingAnaglyph3D(double, double, RT::Color const &, RT::Color const &);							// Set 3D anaglyph parameters of current scene
    void	settingThread(unsigned int);													// Set the number of thread for rendering

    // Utilities
    RT::AbstractTree *	import(std::string const &);	// Import file in current scope
    void		include(std::string const &);	// Include file in current scope

    template<typename X> 
    std::vector<X>	convertVector(std::vector<chaiscript::Boxed_Value> const & v) const	// Convert ChaiScript vector to std::vector of type X
    {
      std::vector<X>	result;
      
      std::transform(v.begin(), v.end(), std::back_inserter(result), [](const chaiscript::Boxed_Value &bv) { return chaiscript::Boxed_Number(bv).get_as<X>(); });
      return result;
    }
    
    std::string		directory(std::string const &) const;				// Return directory path of file (for relative import)
    RT::Color		color(std::vector<chaiscript::Boxed_Value> const &) const;	// Convert the array to a RT::Color

  public:
    Parser();
    ~Parser();
    
    RT::Scene *		load(std::string const &);	// Generate CSG tree from script path
  };
};

#endif