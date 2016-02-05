#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <exception>
#include <functional>
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
  try {
    return import(path);
  }
  catch (std::exception e) {
    std::cerr << "[Parser] ERROR: failed to parse file '" << path << "' (" << std::string(e.what()) << ")." << std::endl;

    // Clean scope
    while (_scope.size() > 1)
      _scope.pop();
    if (_scope.size() == 1)
    {
      delete _scope.top();
      _scope.pop();
    }

    return nullptr;
  }
}

RT::AbstractTree *		RT::Parser::import(std::string const & path)
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
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeScale, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1))), "scale");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeScale, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "scale");
  script.add(chaiscript::fun(&RT::Parser::scopeShear, this), "shear");
  script.add(chaiscript::fun(&RT::Parser::scopeEnd, this), "end");
  // Materials
  script.add(chaiscript::fun(&RT::Parser::scopeMaterial, this), "material");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeColor, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1))), "color");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeColor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "color");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeAmbient, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1))), "ambient");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeAmbient, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "ambient");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeDiffuse, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1))), "diffuse");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeDiffuse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "diffuse");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeSpecular, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, 1.f))), "specular");
  script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeSpecular, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2))), "specular");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeSpecular, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 1.f))), "specular");
  script.add(chaiscript::fun(std::function<void(double, double, double, double)>(std::bind(&RT::Parser::scopeSpecular, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "specular");
  script.add(chaiscript::fun(&RT::Parser::scopeReflection, this), "reflection");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, 1.f))), "transparency");
  script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2))), "transparency");
  // Primitives
  script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveBox, this, 1.f, 1.f, 1.f, false))), "box");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, false))), "box");
  script.add(chaiscript::fun(std::function<void(double, bool)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2))), "box");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false))), "box");
  script.add(chaiscript::fun(std::function<void(double, double, double, bool)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "box");
  script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveCone, this, 0.5f, 0.f, 1.f, false))), "cone");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, 0.f, 1.f, false))), "cone");
  script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, 1.f, false))), "cone");
  script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false))), "cone");
  script.add(chaiscript::fun(std::function<void(double, double, double, bool)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "cone");
  script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveCone, this, 0.5f, 0.5f, 1.f, false))), "cylinder");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, 1.f, false))), "cylinder");
  script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2, false))), "cylinder");
  script.add(chaiscript::fun(std::function<void(double, double, bool)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "cylinder");
  script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveSphere, this, 0.5f))), "sphere");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveSphere, this, std::placeholders::_1))), "sphere");
  script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveTangle, this, 11.8f))), "tangle");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveTangle, this, std::placeholders::_1))), "tangle");
  script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveTorus, this, 1.f, 0.25f))), "torus");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveTorus, this, std::placeholders::_1, 0.25f))), "torus");
  script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveTorus, this, std::placeholders::_1, std::placeholders::_2))), "torus");
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
  script.eval_file(path);
  
  // Check scope status
  if (scopeDepth != _scope.size())
    throw RT::Exception("Invalid scope depth at end of file '" + path + "'.");

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

void	RT::Parser::scopeShear(double xy, double xz, double yx, double yz, double zx, double zy)
{
  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::shear(xy, xz, yx, yz, zx, zy)));
}

void	RT::Parser::scopeMaterial(std::string const & material)
{
  scopeStart(new RT::MaterialNode(RT::Material::getMaterial(material)));
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

void	RT::Parser::primitiveCone(double r1, double r2, double h, bool center)
{
  primitivePush(new RT::ConeLeaf(r1, r2, h, center));
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