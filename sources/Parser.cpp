#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <exception>
#include <functional>
#include <iostream>

#include "Config.hpp"
#include "Exception.hpp"
#include "Parser.hpp"

#include "BoundingNode.hpp"
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
#include "TriangleLeaf.hpp"

#include "DirectionalLight.hpp"
#include "OcclusionLight.hpp"
#include "PointLight.hpp"

RT::Parser::Parser()
{}

RT::Parser::~Parser()
{}

RT::Scene *		RT::Parser::load(std::string const & path)
{
  _scene = new RT::Scene();
  
  _transformation.push(Math::Matrix<4, 4>::identite());

  try
  {
    // Parse file
    _scene->tree() = import(path);
  }
  catch (std::exception e)
  {
    std::cout << "[Parser] ERROR: failed to parse file '" << path << "' (" << std::string(e.what()) << ")." << std::endl;

    // Clean scope
    while (_scope.size() > 1)
      _scope.pop();
    if (_scope.size() == 1)
    {
      delete _scope.top();
      _scope.pop();
    }

    // Clean transformations
    while (!_transformation.empty())
      _transformation.pop();

    // Clean lights
    while (!_scene->light().empty())
    {
      delete _scene->light().back();
      _scene->light().pop_back();
    }
    
    return _scene;
  }

  // If no light, force basic configuration
  if (_scene->light().empty())
  {
    _scene->light().push_back(new RT::OcclusionLight(RT::Color(0.4f), 0));
    _scene->light().push_back(new RT::DirectionalLight(Math::Matrix<4, 4>::rotation(0, 60, 30), RT::Color(0.6f), 0));
  }

  _transformation.pop();

  return _scene;
}

RT::AbstractTree *		RT::Parser::import(std::string const & path)
{
  // Fail if maximum include/import deph reached
  if (_files.size() > RT::Config::Parser::MaxFileDeph)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  _script.push(new chaiscript::ChaiScript(chaiscript::Std_Lib::library()));
  
  // Set up script parser
  // Scope CSG
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeDifference, this), "difference");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeIntersection, this), "intersection");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeUnion, this), "union");
  // Scope transformations
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeTransformation, this), "transformation");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeTranslation, this), "translation");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeMirror, this), "mirror");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeRotation, this), "rotation");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeScale, this), "scale");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeShear, this), "shear");
  // Scope materials
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeMaterial, this), "material");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeColor, this), "color");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &)>(std::bind(&RT::Parser::scopeLight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 1.f, RT::Config::Material::Quality))), "light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, double)>(std::bind(&RT::Parser::scopeLight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, RT::Config::Material::Quality))), "light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &, double, unsigned int)>(std::bind(&RT::Parser::scopeLight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))), "light");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, 1.f, 0.f, RT::Config::Material::Quality))), "transparency");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2, 0.f, RT::Config::Material::Quality))), "transparency");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, RT::Config::Material::Quality))), "transparency");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, double, unsigned int)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "transparency");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeReflection, this, std::placeholders::_1, 0.f, RT::Config::Material::Quality))), "reflection");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeReflection, this, std::placeholders::_1, std::placeholders::_2, RT::Config::Material::Quality))), "reflection");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, unsigned int)>(std::bind(&RT::Parser::scopeReflection, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "reflection");
  // Scope others
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeBounding, this), "bounding");
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeMesh, this), "mesh");
  // Scope utilities
  _script.top()->add(chaiscript::fun(&RT::Parser::scopeEnd, this), "end");
  // Primitives
  _script.top()->add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveBox, this, 1.f, 1.f, 1.f, false))), "box");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, false))), "box");
  _script.top()->add(chaiscript::fun(std::function<void(double, bool)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2))), "box");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false))), "box");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, double, bool)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "box");
  _script.top()->add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveCone, this, 0.5f, 0.f, 1.f, false))), "cone");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, 0.f, 1.f, false))), "cone");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, 1.f, false))), "cone");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false))), "cone");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, double, bool)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "cone");
  _script.top()->add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveCone, this, 0.5f, 0.5f, 1.f, false))), "cylinder");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, 1.f, false))), "cylinder");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2, false))), "cylinder");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, bool)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "cylinder");
  _script.top()->add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveSphere, this, 0.5f))), "sphere");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveSphere, this, std::placeholders::_1))), "sphere");
  _script.top()->add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveTangle, this, 11.8f))), "tangle");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveTangle, this, std::placeholders::_1))), "tangle");
  _script.top()->add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveTorus, this, 1.f, 0.25f))), "torus");
  _script.top()->add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveTorus, this, std::placeholders::_1, 0.25f))), "torus");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveTorus, this, std::placeholders::_1, std::placeholders::_2))), "torus");
  _script.top()->add(chaiscript::fun(&RT::Parser::primitiveMesh, this), "mesh");
  _script.top()->add(chaiscript::fun(&RT::Parser::primitiveTriangle, this), "triangle");
  // Light
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, 0.f))), "directional_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, double)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, std::placeholders::_2))), "directional_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, 0.f))), "ambient_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, 0.f))), "occlusion_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, double)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, std::placeholders::_2))), "occlusion_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, 0.f, 0.f, 0.f, 0.f))), "point_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, 0.f, 0.f, 0.f))), "point_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 0.f, 0.f))), "point_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, double, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_4))), "point_light");
  _script.top()->add(chaiscript::fun(std::function<void(std::vector<chaiscript::Boxed_Value> const &, double, double, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))), "point_light");
  // Settings
  _script.top()->add(chaiscript::fun(&RT::Parser::settingCamera, this), "camera");
  _script.top()->add(chaiscript::fun(&RT::Parser::settingResolution, this), "resolution");
  _script.top()->add(chaiscript::fun(std::function<void(unsigned int)>(std::bind(&RT::Parser::settingAntiAliasing, this, std::placeholders::_1, 0))), "antialiasing");
  _script.top()->add(chaiscript::fun(std::function<void(unsigned int, unsigned int)>(std::bind(&RT::Parser::settingAntiAliasing, this, std::placeholders::_1, std::placeholders::_2))), "antialiasing");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::settingDephOfField, this, std::placeholders::_1, std::placeholders::_2, RT::Config::DephOfField::Quality))), "deph_of_field");
  _script.top()->add(chaiscript::fun(std::function<void(double, double)>(std::bind(static_cast<void(RT::Parser::*)(double, double, RT::Color const &, RT::Color const &)>(&RT::Parser::settingAnaglyph3D), this, std::placeholders::_1, std::placeholders::_2, RT::Config::Anaglyph3D::MaskLeft, RT::Config::Anaglyph3D::MaskRight))), "anaglyph_3d");
  _script.top()->add(chaiscript::fun(std::function<void(double, double, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &)>(std::bind(static_cast<void(RT::Parser::*)(double, double, std::vector<chaiscript::Boxed_Value> const &, std::vector<chaiscript::Boxed_Value> const &)>(&RT::Parser::settingAnaglyph3D), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "anaglyph_3d");
  _script.top()->add(chaiscript::fun(std::function<void(unsigned int)>(std::bind(&RT::Parser::settingThread, this, std::placeholders::_1))), "thread");
  // Others
  _script.top()->add(chaiscript::fun(&RT::Parser::import, this), "import");
  _script.top()->add(chaiscript::fun(&RT::Parser::include, this), "include");
  // Math functions
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::cos)), "cos");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::sin)), "sin");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::tan)), "tan");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::acos)), "acos");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::asin)), "asin");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::atan)), "atan");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::exp)), "exp");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::log)), "log");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::log10)), "log10");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::sqrt)), "sqrt");
  _script.top()->add(chaiscript::fun((double(*)(double))(&std::cbrt)), "cbrt");
  _script.top()->add(chaiscript::fun((double(*)(double, double))(&std::pow)), "pow");
  _script.top()->add(chaiscript::var(Math::Pi), "pi");

  // Initailize scope
  RT::UnionNode *	topNode;
  size_t		scopeDepth;
  
  topNode = new RT::UnionNode();
  if (!_scope.empty())
    _scope.top()->push(topNode);
  _scope.push(topNode);
  scopeDepth = _scope.size();

  //_files.push(_files.empty() ? path : std::string(directory(_files.top)) + path);
  _files.push(_files.empty() ? path : directory(_files.top()).append(path));
  _scene->dependencies().push_back(_files.top());

  // Parsing file
  _script.top()->eval_file(_files.top());
  
  // Check scope status
  if (scopeDepth != _scope.size())
    throw RT::Exception("Invalid scope depth at end of file '" + path + "'.");

  // Clean initial scope
  delete _script.top();
  _script.pop();
  _scope.pop();
  _files.pop();

  return topNode;
}

void	RT::Parser::include(std::string const & path)
{
  // Fail if maximum include/import deph reached
  if (_files.size() > RT::Config::Parser::MaxFileDeph)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  _files.push(_files.empty() ? path : directory(_files.top()).append(path));
  _scene->dependencies().push_back(_files.top());

  // Parsing file
  _script.top()->eval_file(_files.top());

  _files.pop();
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

void	RT::Parser::scopeTransformation(std::vector<chaiscript::Boxed_Value> const & v)
{
  Math::Matrix<4, 4>			matrix;

  if (v.size() != 4)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  else
    for (unsigned int row = 0; row < 4; row++)
    {
      std::vector<double> t = convertVector<double>(chaiscript::boxed_cast<std::vector<chaiscript::Boxed_Value>>(v[row]));

      if (t.size() != 4)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
      else
	for (unsigned int col = 0; col < 4; col++)
	  matrix(row, col) = t[col];
    }

  scopeStart(new RT::TransformationNode(matrix));
}

void	RT::Parser::scopeTranslation(std::vector<chaiscript::Boxed_Value> const & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  
  if (transformation.size() != 3)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::translation(transformation[0], transformation[1], transformation[2])));
}

void	RT::Parser::scopeMirror(std::vector<chaiscript::Boxed_Value> const & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  
  if (transformation.size() != 3)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::reflection(transformation[0], transformation[1], transformation[2])));
}

void	RT::Parser::scopeRotation(std::vector<chaiscript::Boxed_Value> const & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  
  if (transformation.size() != 3)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::rotation(transformation[0], transformation[1], transformation[2])));
}

void	RT::Parser::scopeScale(std::vector<chaiscript::Boxed_Value> const & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  double	      x, y, z;

  if (transformation.size() == 1)
  {
    x = transformation[0];
    y = transformation[0];
    z = transformation[0];
  }
  else if (transformation.size() == 3)
  {
    x = transformation[0];
    y = transformation[1];
    z = transformation[2];
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

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

void	RT::Parser::scopeColor(std::vector<chaiscript::Boxed_Value> const & v)
{
  RT::Material	material;

  material.color = color(v);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeLight(std::vector<chaiscript::Boxed_Value> const & ambient, std::vector<chaiscript::Boxed_Value> const & diffuse, std::vector<chaiscript::Boxed_Value> const & specular, double shininess, unsigned int quality)
{
  RT::Material	material;

  material.light.ambient = color(ambient);
  material.light.diffuse = color(diffuse);
  material.light.specular = color(specular);
  material.light.shininess = shininess;
  material.light.quality = quality;
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeTransparency(double t, double r, double g, unsigned int q)
{
  RT::Material	material;

  material.transparency.intensity = t;
  material.transparency.refraction = r;
  material.transparency.glossiness = g;
  material.transparency.quality = q;
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeReflection(double r, double g, unsigned int q)
{
  RT::Material	material;

  material.reflection.intensity = r;
  material.reflection.glossiness = g;
  material.reflection.quality = q;
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeBounding()
{
  scopeStart(new RT::BoundingNode());
}

void	RT::Parser::scopeMesh()
{
  scopeStart(new RT::MeshNode());
}

void	RT::Parser::scopeStart(RT::AbstractNode * node)
{
  if (dynamic_cast<RT::TransformationNode *>(node))
    _transformation.push(_transformation.top() * dynamic_cast<RT::TransformationNode *>(node)->transformation());

  _scope.top()->push(node);
  _scope.push(node);
}

void	RT::Parser::scopeEnd()
{
  if (_scope.size() > 1)
  {
    if (dynamic_cast<RT::TransformationNode *>(_scope.top()))
      _transformation.pop();

    _scope.pop();
  }
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

void	RT::Parser::primitiveTriangle(std::vector<chaiscript::Boxed_Value> const & p0, std::vector<chaiscript::Boxed_Value> const & p1, std::vector<chaiscript::Boxed_Value> const & p2)
{
  // If not in a mesh, error
  if (dynamic_cast<RT::MeshNode *>(_scope.top()) == nullptr)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  std::vector<double> v0 = convertVector<double>(p0), v1 = convertVector<double>(p1), v2 = convertVector<double>(p2);

  if (v0.size() != 3 || v1.size() != 3 || v2.size() != 3)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  primitivePush(new RT::TriangleLeaf(std::tuple<double, double, double>(v0[0], v0[1], v0[2]), std::tuple<double, double, double>(v1[0], v1[1], v1[2]), std::tuple<double, double, double>(v2[0], v2[1], v2[2])));
}

void	RT::Parser::primitiveMesh(std::string const & path)
{
  std::string file = directory(_files.top()).append(path);

  _scene->dependencies().push_back(file);
  primitivePush(new RT::MeshNode(file));
}

void	RT::Parser::primitivePush(RT::AbstractTree * tree)
{
  _scope.top()->push(tree);
}

void	RT::Parser::lightDirectional(std::vector<chaiscript::Boxed_Value> const & v, double angle)
{
  lightPush(new RT::DirectionalLight(_transformation.top(), color(v), angle));
}

void	RT::Parser::lightOcclusion(std::vector<chaiscript::Boxed_Value> const & v, double radius)
{
  lightPush(new RT::OcclusionLight(color(v), radius));
}

void	RT::Parser::lightPoint(std::vector<chaiscript::Boxed_Value> const & v, double radius, double intensity, double angle1, double angle2)
{
  lightPush(new RT::PointLight(_transformation.top(), color(v), radius, intensity, angle1, angle2));
}

void	RT::Parser::lightPush(RT::AbstractLight * light)
{
  _scene->light().push_back(light);
}

void	RT::Parser::settingCamera()
{
  // Set camera only if in main file
  if (_files.size() == 1)
    _scene->camera() = _transformation.top();
}

void	RT::Parser::settingResolution(unsigned int width, unsigned int height)
{
  // Set resolution only if in main file
  if (_files.size() == 1)
    _scene->image().create(width, height);
}

void	RT::Parser::settingAntiAliasing(unsigned int live, unsigned int post)
{
  // Set antialiasing only if in main file
  if (_files.size() == 1)
  {
    _scene->antialiasing().live = live + 1;
    _scene->antialiasing().post = post;
  }
}

void	RT::Parser::settingDephOfField(double aperture, double focal, unsigned int quality)
{
  // Set deph of field configuration only if in main file
  if (_files.size() == 1)
  {
    _scene->dof().aperture = aperture;
    _scene->dof().focal = focal;
    _scene->dof().quality = quality;
  }
}

void	RT::Parser::settingAnaglyph3D(double offset, double focal, std::vector<chaiscript::Boxed_Value> const & left, std::vector<chaiscript::Boxed_Value> const & right)
{
  settingAnaglyph3D(offset, focal, color(left), color(right));
}

void	RT::Parser::settingAnaglyph3D(double offset, double focal, RT::Color const & left, RT::Color const & right)
{
  // Set deph of field configuration only if in main file
  if (_files.size() == 1)
  {
    _scene->anaglyph().offset = offset;
    _scene->anaglyph().focal = focal;
    _scene->anaglyph().maskLeft = left;
    _scene->anaglyph().maskRight = right;
  }
}

void	RT::Parser::settingThread(unsigned int thread)
{
  // Set number of rendering thread only in main file
  if (_files.size() == 1)
  {
    // Cancel if invalid number of thread
    if (thread < 1)
      throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

    _scene->config().threadNumber = thread;
  }
}

std::string	RT::Parser::directory(std::string const & file) const
{
#ifdef _WIN32
  return file.substr(0, file.find_last_of('\\')) + std::string("\\");
#else
  if (file.find_last_of('/') == std::string::npos)
    return std::string("./");
  else
    return file.substr(0, file.find_last_of('/')) + std::string("/");
#endif
}

RT::Color	RT::Parser::color(std::vector<chaiscript::Boxed_Value> const & v) const
{
  std::vector<double>	clr = convertVector<double>(v);

  if (clr.size() == 1)
    return RT::Color(clr[0] / 255.f, clr[0] / 255.f, clr[0] / 255.f);
  else if (clr.size() == 3)
    return RT::Color(clr[0] / 255.f, clr[1] / 255.f, clr[2] / 255.f);
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}