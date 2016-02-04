#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <exception>
#include <iostream>

#include "Exception.hpp"
#include "Parser.hpp"

#include "DifferenceNode.hpp"
#include "IntersectionNode.hpp"
#include "MaterialNode.hpp"
#include "MeshNode.hpp"
#include "TransformationNode.hpp"
#include "UnionNode.hpp"

#include "BoxLeaf.hpp"
#include "ConeLeaf.hpp"
#include "SphereLeaf.hpp"
#include "TangleLeaf.hpp"
#include "TorusLeaf.hpp"

RT::Parser::Parser()
{}

RT::Parser::~Parser()
{}

RT::AbstractTree *		RT::Parser::load(std::string const & path)
{
  chaiscript::ChaiScript	script(chaiscript::Std_Lib::library());
  
  // Set up script parser
  // Scope related methods
  script.add(chaiscript::fun(&RT::Parser::scopeDifference, this), "difference");
  script.add(chaiscript::fun(&RT::Parser::scopeIntersection, this), "intersection");
  script.add(chaiscript::fun(&RT::Parser::scopeUnion, this), "union");
  script.add(chaiscript::fun(&RT::Parser::scopeTranslation, this), "translation");
  script.add(chaiscript::fun(&RT::Parser::scopeMirror, this), "mirror");
  script.add(chaiscript::fun(&RT::Parser::scopeRotation, this), "rotation");
  script.add(chaiscript::fun(static_cast<void (RT::Parser::*)(double, double, double)>(&RT::Parser::scopeScale), this), "scale");
  script.add(chaiscript::fun(static_cast<void (RT::Parser::*)(double)>(&RT::Parser::scopeScale), this), "scale");
  script.add(chaiscript::fun(&RT::Parser::scopeShear, this), "shear");
  script.add(chaiscript::fun(&RT::Parser::scopeEnd, this), "end");
  // Materials
  script.add(chaiscript::fun(&RT::Parser::scopeColor, this), "color");
  script.add(chaiscript::fun(&RT::Parser::scopeAmbient, this), "ambient");
  script.add(chaiscript::fun(&RT::Parser::scopeDiffuse, this), "diffuse");
  script.add(chaiscript::fun(&RT::Parser::scopeSpecular, this), "specular");
  script.add(chaiscript::fun(&RT::Parser::scopeReflection, this), "reflection");
  script.add(chaiscript::fun(&RT::Parser::scopeTransparency, this), "transparency");
  // Primitives
  script.add(chaiscript::fun(static_cast<void (RT::Parser::*)(double, double, double, bool)>(&RT::Parser::primitiveBox), this), "box");
  script.add(chaiscript::fun(static_cast<void (RT::Parser::*)(double, bool)>(&RT::Parser::primitiveBox), this), "box");
  script.add(chaiscript::fun(&RT::Parser::primitiveCone, this), "cone");
  script.add(chaiscript::fun(&RT::Parser::primitiveCylinder, this), "cylinder");
  script.add(chaiscript::fun(&RT::Parser::primitiveSphere, this), "sphere");
  script.add(chaiscript::fun(&RT::Parser::primitiveTangle, this), "tangle");
  script.add(chaiscript::fun(&RT::Parser::primitiveTorus, this), "torus");
  script.add(chaiscript::fun(&RT::Parser::primitiveMesh, this), "mesh");
  // Others
  script.add(chaiscript::fun(&RT::Parser::load, this), "import");

  // Initailize scope
  RT::UnionNode *	topNode;
  size_t		scopeDepth;
  
  topNode = new RT::UnionNode();
  _scope.push(topNode);
  scopeDepth = _scope.size();

  // Parsing file
  try {
    script.eval_file(path);
  }
  catch (std::exception e) {
    std::cerr << "[Parser] ERROR: failed to parse file '" << path << "' (" << std::string(e.what()) << ")." << std::endl;
  }

  // Check scope status
  if (scopeDepth != _scope.size())
  {
    std::cerr << "[Parser] ERROR: Invalid scope depth at end of file '" << path << "'." << std::endl;
    while (_scope.size() > scopeDepth)
      _scope.pop();
  }

  // Clean initial scope
  _scope.pop();
  return topNode;
}

void	RT::Parser::scopeDifference()
{
  scopeStart(new RT::DifferenceNode());
}

void	RT::Parser::scopeIntersection()
{
  scopeStart(new RT::IntersectionNode());
}

void	RT::Parser::scopeUnion()
{
  scopeStart(new RT::UnionNode());
}

void	RT::Parser::scopeTranslation(double x, double y, double z)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::translation(x, y, z)));
}

void	RT::Parser::scopeMirror(double x, double y, double z)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::reflection(x, y, z)));
}

void	RT::Parser::scopeRotation(double x, double y, double z)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::rotation(x, y, z)));
}

void	RT::Parser::scopeScale(double x, double y, double z)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::scale(x, y, z)));
}

void	RT::Parser::scopeScale(double s)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::scale(s)));
}

void	RT::Parser::scopeShear(double xy, double xz, double yx, double yz, double zx, double zy)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::shear(xy, xz, yx, yz, zx, zy)));
}

void	RT::Parser::scopeColor(double r, double g, double b)
{
  RT::Material	material;

  material.color = RT::Color(r, g, b);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeAmbient(double r, double g, double b)
{
  RT::Material	material;

  material.ambient = RT::Color(r, g, b);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeDiffuse(double r, double g, double b)
{
  RT::Material	material;

  material.diffuse = RT::Color(r, g, b);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeSpecular(double r, double g, double b, double s)
{
  RT::Material	material;

  material.specular = RT::Color(r, g, b);
  material.shine = s;
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeReflection(double r)
{
  RT::Material	material;

  material.reflection = r;
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeTransparency(double t, double r)
{
  RT::Material	material;

  material.transparency = t;
  material.refraction = r;
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeStart(RT::AbstractNode * node)
{
  _scope.top()->push(node);
  _scope.push(node);
}

void	RT::Parser::scopeEnd()
{
  if (_scope.size() > 1)
    _scope.pop();
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

void	RT::Parser::primitiveBox(double x, double y, double z, bool center)
{
  primitivePush(new RT::BoxLeaf(x, y, z, center));
}

void	RT::Parser::primitiveBox(double l, bool center)
{
  primitivePush(new RT::BoxLeaf(l, center));
}

void	RT::Parser::primitiveCone(double r1, double r2, double h, bool center)
{
  primitivePush(new RT::ConeLeaf(r1, r2, h, center));
}

void	RT::Parser::primitiveCylinder(double r, double h, bool center)
{
  primitivePush(new RT::ConeLeaf(r, h, center));
}

void	RT::Parser::primitiveSphere(double r)
{
  primitivePush(new RT::SphereLeaf(r));
}

void	RT::Parser::primitiveTangle(double c)
{
  primitivePush(new RT::TangleLeaf(c));
}

void	RT::Parser::primitiveTorus(double r1, double r2)
{
  primitivePush(new RT::TorusLeaf(r1, r2));
}

void	RT::Parser::primitiveMesh(std::string const & path)
{
  primitivePush(new RT::MeshNode(path));
}

void	RT::Parser::primitivePush(RT::AbstractTree * tree)
{
  _scope.top()->push(tree);
}