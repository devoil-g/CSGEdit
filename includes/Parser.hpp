#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <stack>
#include <string>

#include "AbstractNode.hpp"
#include "AbstractTree.hpp"

namespace RT
{
  class Parser
  {
  private:
    std::stack<RT::AbstractNode *>	_scope;	// Stack of scopes

    // Scope CSG
    void  scopeDifference();	// Push a CSG difference in scope
    void  scopeIntersection();	// Push a CSG intersection in scope
    void  scopeUnion();		// Push a CSG union in scope
    // Scope transformations
    void  scopeTranslation(double, double, double);			// Push a translation in scope
    void  scopeMirror(double, double, double);				// Push a mirror in scope
    void  scopeRotation(double, double, double);			// Push a rotation in scope
    void  scopeScale(double, double, double);				// Push a scale in scope
    void  scopeScale(double);						// Push a scale in scope
    void  scopeShear(double, double, double, double, double, double);	// Push a shear in scope
    // Scope materials
    void  scopeColor(double, double, double);			// Push a color material in scope
    void  scopeAmbient(double, double, double);			// Push a ambient color material in scope
    void  scopeDiffuse(double, double, double);			// Push a diffuse color material in scope
    void  scopeSpecular(double, double, double, double);	// Push a specular color and shiness material in scope
    void  scopeReflection(double);				// Push a reflection material in scope
    void  scopeTransparency(double, double);			// Push a transparency and refraction material in scope
    // Scope utlilites
    void  scopeStart(RT::AbstractNode *);	// Push a new scope in stack
    void  scopeEnd();				// Pop last scope in stack

    // Primitives
    void  primitiveBox(double = 1, double = 1, double = 1, bool = false);	// Push a box in top scope
    void  primitiveBox(double = 1, bool = true);				// Push a box in top scope
    void  primitiveCone(double = 1, double = 1, double = 0.5f, bool = false);	// Push a cone in top scope
    void  primitiveCylinder(double = 1, double = 0.5f, bool = false);		// Push a cylinder in top scope
    void  primitiveSphere(double = 0.5f);					// Push a sphere in top scope
    void  primitiveTangle(double = 11.8f);					// Push a tangle in top scope
    void  primitiveTorus(double = 1.f, double = 0.25f);				// Push a torus in top scope
    void  primitiveMesh(std::string const &);					// Push a primitive in top scope
    // Primitives utilities
    void  primitivePush(RT::AbstractTree *);	// Push a box in top scope

  public:
    Parser();
    ~Parser();
    
    RT::AbstractTree *	load(std::string const &);  // Generate CSG tree from script path
  };
};

#endif