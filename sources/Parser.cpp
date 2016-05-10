#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <stdexcept>
#include <functional>
#include <iostream>

#include "Config.hpp"
#include "Parser.hpp"
#include "SceneLibrary.hpp"

// CSG tree
#include "EmptyCsgTree.hpp"
#include "ExternCsgTree.hpp"
#include "BoxCsgLeaf.hpp"
#include "ConeCsgLeaf.hpp"
#include "MobiusCsgLeaf.hpp"
#include "SphereCsgLeaf.hpp"
#include "TangleCsgLeaf.hpp"
#include "TorusCsgLeaf.hpp"
#include "TriangleCsgLeaf.hpp"
#include "BoundingCsgNode.hpp"
#include "DifferenceCsgNode.hpp"
#include "IntersectionCsgNode.hpp"
#include "MaterialCsgNode.hpp"
#include "MeshCsgNode.hpp"
#include "TransformationCsgNode.hpp"
#include "UnionCsgNode.hpp"

// Light tree
#include "EmptyLightTree.hpp"
#include "ExternLightTree.hpp"
#include "TransformationLightNode.hpp"
#include "UnionLightNode.hpp"
#include "DirectionalLightLeaf.hpp"
#include "OcclusionLightLeaf.hpp"
#include "PointLightLeaf.hpp"

RT::Parser::Parser()
  : _csg({ new RT::UnionCsgNode() }), _light({ new RT::UnionLightNode() }), _files(), _script(chaiscript::Std_Lib::library()), _scene()
{
  // Set up script parser
  // Scope CSG
  _script.add(chaiscript::fun(&RT::Parser::scopeDifference, this), "difference");
  _script.add(chaiscript::fun(&RT::Parser::scopeIntersection, this), "intersection");
  _script.add(chaiscript::fun(&RT::Parser::scopeUnion, this), "union");
  // Scope transformations
  _script.add(chaiscript::fun(&RT::Parser::scopeTransformation, this), "transformation");
  _script.add(chaiscript::fun(&RT::Parser::scopeTranslation, this), "translation");
  _script.add(chaiscript::fun(&RT::Parser::scopeMirror, this), "mirror");
  _script.add(chaiscript::fun(&RT::Parser::scopeRotation, this), "rotation");
  _script.add(chaiscript::fun(&RT::Parser::scopeScale, this), "scale");
  _script.add(chaiscript::fun(&RT::Parser::scopeShear, this), "shear");
  // Scope materials
  _script.add(chaiscript::fun(&RT::Parser::scopeMaterial, this), "material");
  _script.add(chaiscript::fun(&RT::Parser::scopeColor, this), "color");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, RT::Color const &, RT::Color const &)>(std::bind(&RT::Parser::scopeDirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 1.f, RT::Config::Material::Quality))), "direct_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, RT::Color const &, RT::Color const &, double)>(std::bind(&RT::Parser::scopeDirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, RT::Config::Material::Quality))), "direct_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, RT::Color const &, RT::Color const &, double, unsigned int)>(std::bind(&RT::Parser::scopeDirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))), "direct_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, RT::Color const &, RT::Color const &)>(std::bind(&RT::Parser::scopeIndirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 1.f, RT::Config::Material::Quality))), "indirect_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, RT::Color const &, RT::Color const &, double)>(std::bind(&RT::Parser::scopeIndirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, RT::Config::Material::Quality))), "indirect_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, RT::Color const &, RT::Color const &, double, unsigned int)>(std::bind(&RT::Parser::scopeIndirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))), "indirect_light");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, 1.f, 0.f, RT::Config::Material::Quality))), "transparency");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2, 0.f, RT::Config::Material::Quality))), "transparency");
  _script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, RT::Config::Material::Quality))), "transparency");
  _script.add(chaiscript::fun(std::function<void(double, double, double, unsigned int)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "transparency");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeReflection, this, std::placeholders::_1, 0.f, RT::Config::Material::Quality))), "reflection");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeReflection, this, std::placeholders::_1, std::placeholders::_2, RT::Config::Material::Quality))), "reflection");
  _script.add(chaiscript::fun(std::function<void(double, double, unsigned int)>(std::bind(&RT::Parser::scopeReflection, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "reflection");
  // Scope others
  _script.add(chaiscript::fun(&RT::Parser::scopeBounding, this), "bounding");
  _script.add(chaiscript::fun(&RT::Parser::scopeMesh, this), "mesh");
  // Scope utilities
  _script.add(chaiscript::fun(&RT::Parser::scopeEnd, this), "end");
  // Primitives
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveBox, this, 1.f, 1.f, 1.f, false))), "box");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, false))), "box");
  _script.add(chaiscript::fun(std::function<void(double, bool)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2))), "box");
  _script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false))), "box");
  _script.add(chaiscript::fun(std::function<void(double, double, double, bool)>(std::bind(&RT::Parser::primitiveBox, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "box");
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveCone, this, 0.5f, 0.f, 1.f, false))), "cone");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, 0.f, 1.f, false))), "cone");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, 1.f, false))), "cone");
  _script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, false))), "cone");
  _script.add(chaiscript::fun(std::function<void(double, double, double, bool)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))), "cone");
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveCone, this, 0.5f, 0.5f, 1.f, false))), "cylinder");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, 1.f, false))), "cylinder");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2, false))), "cylinder");
  _script.add(chaiscript::fun(std::function<void(double, double, bool)>(std::bind(&RT::Parser::primitiveCone, this, std::placeholders::_1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "cylinder");
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveMobius, this, 0.5f, 0.5f / 4.f, Math::Shift))), "mobius");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveMobius, this, std::placeholders::_1, std::placeholders::_2, Math::Shift))), "mobius");
  _script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::primitiveMobius, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "mobius");
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveSphere, this, 0.5f))), "sphere");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveSphere, this, std::placeholders::_1))), "sphere");
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveTangle, this, 11.8f))), "tangle");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveTangle, this, std::placeholders::_1))), "tangle");
  _script.add(chaiscript::fun(std::function<void()>(std::bind(&RT::Parser::primitiveTorus, this, 1.f, 0.25f))), "torus");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::primitiveTorus, this, std::placeholders::_1, 0.25f))), "torus");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::primitiveTorus, this, std::placeholders::_1, std::placeholders::_2))), "torus");
  _script.add(chaiscript::fun(&RT::Parser::primitiveTriangle, this), "triangle");
  // Light
  _script.add(chaiscript::fun(std::function<void(RT::Color const &)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, 0.f))), "directional_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, double)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, std::placeholders::_2))), "directional_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, 0.f))), "ambient_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, 0.f))), "occlusion_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, double)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, std::placeholders::_2))), "occlusion_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, 0.f, 0.f, 0.f, 0.f))), "point_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, 0.f, 0.f, 0.f))), "point_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 0.f, 0.f))), "point_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, double, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_4))), "point_light");
  _script.add(chaiscript::fun(std::function<void(RT::Color const &, double, double, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))), "point_light");
  // Settings
  _script.add(chaiscript::fun(&RT::Parser::settingCamera, this), "camera");
  _script.add(chaiscript::fun(&RT::Parser::settingResolution, this), "resolution");
  _script.add(chaiscript::fun(std::function<void(unsigned int)>(std::bind(&RT::Parser::settingAntiAliasing, this, std::placeholders::_1, 0))), "antialiasing");
  _script.add(chaiscript::fun(std::function<void(unsigned int, unsigned int)>(std::bind(&RT::Parser::settingAntiAliasing, this, std::placeholders::_1, std::placeholders::_2))), "antialiasing");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::settingDephOfField, this, std::placeholders::_1, std::placeholders::_2, RT::Config::DephOfField::Quality))), "deph_of_field");
  _script.add(chaiscript::fun(std::function<void(double, double, unsigned int)>(std::bind(&RT::Parser::settingDephOfField, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "deph_of_field");
  _script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::settingVirtualReality, this, std::placeholders::_1, RT::Config::VirtualReality::Distortion, RT::Config::VirtualReality::Center))), "virtual_reality");
  _script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::settingVirtualReality, this, std::placeholders::_1, std::placeholders::_2, RT::Config::VirtualReality::Center))), "virtual_reality");
  _script.add(chaiscript::fun(std::function<void(double, double, double)>(std::bind(&RT::Parser::settingVirtualReality, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "virtual_reality");
  _script.add(chaiscript::fun(std::function<void(unsigned int)>(std::bind(&RT::Parser::settingThread, this, std::placeholders::_1))), "thread");
  // Others
  _script.add(chaiscript::fun(&RT::Parser::import, this), "import");
  _script.add(chaiscript::fun(&RT::Parser::import, this), "mesh");
  _script.add(chaiscript::fun(&RT::Parser::include, this), "include");
  // Math functions
  _script.add(chaiscript::fun((double(*)(double))(&std::cos)), "cos");
  _script.add(chaiscript::fun((double(*)(double))(&std::sin)), "sin");
  _script.add(chaiscript::fun((double(*)(double))(&std::tan)), "tan");
  _script.add(chaiscript::fun((double(*)(double))(&std::acos)), "acos");
  _script.add(chaiscript::fun((double(*)(double))(&std::asin)), "asin");
  _script.add(chaiscript::fun((double(*)(double))(&std::atan)), "atan");
  _script.add(chaiscript::fun((double(*)(double))(&std::exp)), "exp");
  _script.add(chaiscript::fun((double(*)(double))(&std::log)), "log");
  _script.add(chaiscript::fun((double(*)(double))(&std::log10)), "log10");
  _script.add(chaiscript::fun((double(*)(double))(&std::sqrt)), "sqrt");
  _script.add(chaiscript::fun((double(*)(double))(&std::cbrt)), "cbrt");
  _script.add(chaiscript::fun((double(*)(double, double))(&std::pow)), "pow");
  _script.add(chaiscript::fun((double(*)(double))(&Math::Random::rand)), "rand");
  _script.add_global_const(chaiscript::const_var(Math::Pi), "pi");

  // Vector conversion: std::vector<double>
  _script.add(chaiscript::type_conversion<std::vector<chaiscript::Boxed_Value>, std::vector<double>>(
    [](const std::vector<chaiscript::Boxed_Value> & v)
  {
    std::vector<double>	result;
    for (chaiscript::Boxed_Value const & it : v)
      result.push_back(chaiscript::Boxed_Number(it).get_as<double>());
    return result;
  }
  ));

  // Vector conversion: std::vector<std::vector<double>>
  _script.add(chaiscript::type_conversion<std::vector<chaiscript::Boxed_Value>, std::vector<std::vector<double>>>(
    [](const std::vector<chaiscript::Boxed_Value> & v)
  {
    std::vector<std::vector<double>>	result;

    for (chaiscript::Boxed_Value const & it1 : v)
    {
      result.push_back(std::vector<double>());
      for (chaiscript::Boxed_Value const & it2 : chaiscript::boxed_cast<std::vector<chaiscript::Boxed_Value>>(it1))
	result.back().push_back(chaiscript::Boxed_Number(it2).get_as<double>());
    }

    return result;
  }
  ));

  // Vector conversion: RT::Color
  _script.add(chaiscript::type_conversion<std::vector<chaiscript::Boxed_Value>, RT::Color>(
    [](const std::vector<chaiscript::Boxed_Value> & v)
  {
    if (v.size() == 1)
      return RT::Color(chaiscript::Boxed_Number(v[0]).get_as<double>() / 255.f, chaiscript::Boxed_Number(v[0]).get_as<double>() / 255.f, chaiscript::Boxed_Number(v[0]).get_as<double>() / 255.f);
    else if (v.size() == 3)
      return RT::Color(chaiscript::Boxed_Number(v[0]).get_as<double>() / 255.f, chaiscript::Boxed_Number(v[1]).get_as<double>() / 255.f, chaiscript::Boxed_Number(v[2]).get_as<double>() / 255.f);
    else
      throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  }
  ));

  // Assign CSG / Light node to scene
  _scene.csg() = _csg.top();
  _scene.light() = _light.top();
}

RT::Parser::~Parser()
{}

RT::Scene	RT::Parser::load(std::string const & path)
{
  return RT::Parser().parse(path);
}

RT::Scene	RT::Parser::parse(std::string const & path)
{
  // Add file to file scope
  _files.push(path);

  try
  {
    // If file is a mesh
    if (RT::MeshCsgNode::extension(path.substr(path.rfind('.'))))
    {
      // Clean scene
      _scene.clear();

      // Load mesh
      _scene.csg() = new RT::MeshCsgNode(path);
      _scene.light() = new RT::EmptyLightTree();
    }
    // If file is a standard scene description
    else
    {
      // Parsing file
      _script.eval_file(path);

      // Check for invalid scope at end of file
      if (_csg.size() != 1 || _light.size() != 1)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
    }
  }
  catch (std::exception e)
  {
    std::cout << "[Parser] ERROR: failed to parse file '" << path << "' (" << std::string(e.what()) << ")." << std::endl;

    // Clean scene
    _scene.clear();
    
    // Generate empty scene configuration
    _scene.csg() = new RT::EmptyCsgTree();
    _scene.light() = new RT::EmptyLightTree();
  }

  return _scene;
}

void	RT::Parser::import(std::string const & path)
{
  _csg.top()->push(new RT::ExternCsgTree(RT::SceneLibrary::Instance().get(directory(_files.top()).append(path))->csg()));
  _light.top()->push(new RT::ExternLightTree(RT::SceneLibrary::Instance().get(directory(_files.top()).append(path))->light()));
}

void	RT::Parser::include(std::string const & path)
{
  // Fail if maximum include/import deph reached
  if (_files.size() > RT::Config::Parser::MaxFileDeph)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Parsing file
  _files.push(directory(_files.top()).append(path));
  _script.eval_file(_files.top());
  _files.pop();
}

void	RT::Parser::scopeDifference()
{
  scopeStart(new RT::DifferenceCsgNode());
}

void	RT::Parser::scopeIntersection()
{
  scopeStart(new RT::IntersectionCsgNode());
}

void	RT::Parser::scopeUnion()
{
  scopeStart(new RT::UnionCsgNode());
}

void	RT::Parser::scopeTransformation(std::vector<std::vector<double>> const & v)
{
  Math::Matrix<4, 4>	matrix;

  if (v.size() != 4)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  else
    for (unsigned int row = 0; row < 4; row++)
    {
      if (v[row].size() != 4)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
      else
	for (unsigned int col = 0; col < 4; col++)
	  matrix(row, col) = v[row][col];
    }

  scopeStart(new RT::TransformationCsgNode(matrix));
}

void	RT::Parser::scopeTranslation(std::vector<double> const & v)
{
  if (v.size() != 3)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  scopeStart(new RT::TransformationCsgNode(Math::Matrix<4, 4>::translation(v[0], v[1], v[2])));
}

void	RT::Parser::scopeMirror(std::vector<double> const & v)
{
  if (v.size() != 3)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  scopeStart(new RT::TransformationCsgNode(Math::Matrix<4, 4>::reflection(v[0], v[1], v[2])));
}

void	RT::Parser::scopeRotation(std::vector<double> const & v)
{
  if (v.size() != 3)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  scopeStart(new RT::TransformationCsgNode(Math::Matrix<4, 4>::rotation(v[0], v[1], v[2])));
}

void	RT::Parser::scopeScale(std::vector<double> const & v)
{
  if (v.size() == 1)
    scopeStart(new RT::TransformationCsgNode(Math::Matrix<4, 4>::scale(v[0], v[0], v[0])));
  else if (v.size() == 3)
    scopeStart(new RT::TransformationCsgNode(Math::Matrix<4, 4>::scale(v[0], v[1], v[1])));
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

void	RT::Parser::scopeShear(double xy, double xz, double yx, double yz, double zx, double zy)
{
  scopeStart(new RT::TransformationCsgNode(Math::Matrix<4, 4>::shear(xy, xz, yx, yz, zx, zy)));
}

void	RT::Parser::scopeMaterial(std::string const & material)
{
  scopeStart(new RT::MaterialCsgNode(RT::Material::getMaterial(material)));
}

void	RT::Parser::scopeColor(RT::Color const & clr)
{
  RT::Material	material;

  material.color = clr;
  scopeStart(new RT::MaterialCsgNode(material));
}

void	RT::Parser::scopeDirect(RT::Color const & ambient, RT::Color const & diffuse, RT::Color const & specular, double shininess, unsigned int quality)
{
  RT::Material	material;

  material.direct.ambient = ambient;
  material.direct.diffuse = diffuse;
  material.direct.specular = specular;
  material.direct.shininess = shininess;
  material.direct.quality = quality;
  scopeStart(new RT::MaterialCsgNode(material));
}

void	RT::Parser::scopeIndirect(RT::Color const & ambient, RT::Color const & diffuse, RT::Color const & specular, double shininess, unsigned int quality)
{
  RT::Material	material;

  material.indirect.ambient = ambient;
  material.indirect.diffuse = diffuse;
  material.indirect.specular = specular;
  material.indirect.shininess = shininess;
  material.indirect.quality = quality;
  scopeStart(new RT::MaterialCsgNode(material));
}

void	RT::Parser::scopeTransparency(double t, double r, double g, unsigned int q)
{
  RT::Material	material;

  material.transparency.intensity = t;
  material.transparency.refraction = r;
  material.transparency.glossiness = g;
  material.transparency.quality = q;
  scopeStart(new RT::MaterialCsgNode(material));
}

void	RT::Parser::scopeReflection(double r, double g, unsigned int q)
{
  RT::Material	material;

  material.reflection.intensity = r;
  material.reflection.glossiness = g;
  material.reflection.quality = q;
  scopeStart(new RT::MaterialCsgNode(material));
}

void	RT::Parser::scopeBounding()
{
  scopeStart(new RT::BoundingCsgNode());
}

void	RT::Parser::scopeMesh()
{
  scopeStart(new RT::MeshCsgNode());
}

void	RT::Parser::scopeStart(RT::AbstractCsgNode * node)
{
  _csg.top()->push(node);
  _csg.push(node);

  // Synchronise CSG and Light tree scope
  if (dynamic_cast<RT::TransformationCsgNode *>(node))
  {
    RT::TransformationLightNode * light = new RT::TransformationLightNode(dynamic_cast<RT::TransformationCsgNode *>(node)->transformation());

    _light.top()->push(light);
    _light.push(light);
  }
  else
  {
    _light.push(_light.top());
  }
}

void	RT::Parser::scopeEnd()
{
  if (_csg.size() > 1 && _light.size() > 1)
  {
    // Delete CSG tree if no children
    if (_csg.top()->children().empty())
    {
      _csg.pop();
      _csg.top()->pop();
    }
    else
    {
      scopeSimplifyCsg();
      _csg.pop();
    }

    // Delete light tree if no children
    if (_light.top()->children().empty())
    {
      RT::AbstractLightNode * top = _light.top();

      _light.pop();
      if (_light.top() != top)
	_light.top()->pop();
    }
    else
    {
      scopeSimplifyLight();
      _light.pop();
    }
  }
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

void	RT::Parser::scopeSimplifyCsg()
{
  // Simplify multiple transformation node
  if (dynamic_cast<RT::TransformationCsgNode *>(_csg.top()) != nullptr && _csg.top()->children().size() == 1 && dynamic_cast<RT::TransformationCsgNode *>(_csg.top()->children().front()) != nullptr)
  {
    dynamic_cast<RT::TransformationCsgNode *>(_csg.top())->transformation() *= dynamic_cast<RT::TransformationCsgNode *>(_csg.top()->children().front())->transformation();
    _csg.top()->children().splice(_csg.top()->children().end(), dynamic_cast<RT::TransformationCsgNode *>(_csg.top()->children().front())->children());
    delete _csg.top()->children().front();
    _csg.top()->children().pop_front();

    return;
  }

  // Simplify multiple material node
  if (dynamic_cast<RT::MaterialCsgNode *>(_csg.top()) != nullptr && _csg.top()->children().size() == 1 && dynamic_cast<RT::MaterialCsgNode *>(_csg.top()->children().front()) != nullptr)
  {
    dynamic_cast<RT::MaterialCsgNode *>(_csg.top())->material() *= dynamic_cast<RT::MaterialCsgNode *>(_csg.top()->children().front())->material();
    _csg.top()->children().splice(_csg.top()->children().end(), dynamic_cast<RT::MaterialCsgNode *>(_csg.top()->children().front())->children());
    delete _csg.top()->children().front();
    _csg.top()->children().pop_front();

    return;
  }
}

void	RT::Parser::scopeSimplifyLight()
{
  // Simplify multiple transformation node
  if (dynamic_cast<RT::TransformationLightNode *>(_light.top()) != nullptr && _light.top()->children().size() == 1 && dynamic_cast<RT::TransformationLightNode *>(_light.top()->children().front()) != nullptr)
  {
    dynamic_cast<RT::TransformationLightNode *>(_light.top())->transformation() *= dynamic_cast<RT::TransformationLightNode *>(_light.top()->children().front())->transformation();
    _light.top()->children().splice(_light.top()->children().end(), dynamic_cast<RT::TransformationLightNode *>(_light.top()->children().front())->children());
    delete _light.top()->children().front();
    _light.top()->children().pop_front();

    return;
  }
}

void	RT::Parser::primitiveBox(double x, double y, double z, bool center)
{
  primitivePush(new RT::BoxCsgLeaf(x, y, z, center));
}

void	RT::Parser::primitiveCone(double r1, double r2, double h, bool center)
{
  primitivePush(new RT::ConeCsgLeaf(r1, r2, h, center));
}

void	RT::Parser::primitiveMobius(double r1, double r2, double t)
{
  primitivePush(new RT::MobiusCsgLeaf(r1, r2, t));
}

void	RT::Parser::primitiveSphere(double r)
{
  primitivePush(new RT::SphereCsgLeaf(r));
}

void	RT::Parser::primitiveTangle(double c)
{
  primitivePush(new RT::TangleCsgLeaf(c));
}

void	RT::Parser::primitiveTorus(double r1, double r2)
{
  primitivePush(new RT::TorusCsgLeaf(r1, r2));
}

void	RT::Parser::primitiveTriangle(std::vector<double> const & p0, std::vector<double> const & p1, std::vector<double> const & p2)
{
  // If not in a mesh scope, error
  if (dynamic_cast<RT::MeshCsgNode *>(_csg.top()) == nullptr)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  if (p0.size() != 3 || p1.size() != 3 || p2.size() != 3)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  primitivePush(new RT::TriangleCsgLeaf({ p0[0], p0[1], p0[2] }, { p1[0], p1[1], p1[2] }, { p2[0], p2[1], p2[2] }));
}

void	RT::Parser::primitivePush(RT::AbstractCsgTree * tree)
{
  _csg.top()->push(tree);
}

void	RT::Parser::lightDirectional(RT::Color const & clr, double angle)
{
  lightPush(new RT::DirectionalLightLeaf(clr, angle));
}

void	RT::Parser::lightOcclusion(RT::Color const & clr, double radius)
{
  lightPush(new RT::OcclusionLightLeaf(clr, radius));
}

void	RT::Parser::lightPoint(RT::Color const & clr, double radius, double intensity, double angle1, double angle2)
{
  lightPush(new RT::PointLightLeaf(clr, radius, intensity, angle1, angle2));
}

void	RT::Parser::lightPush(RT::AbstractLightTree * light)
{
  _light.top()->push(light);
}

void	RT::Parser::settingCamera(std::vector<double> const & t, std::vector<double> const & r, std::vector<double> const & s)
{
  if (t.size() != 3 || r.size() != 3 || s.size() != 3)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Set camera only if in main file
  if (_files.size() == 1)
    _scene.camera() = Math::Matrix<4, 4>::translation(t[0], t[1], t[2]) * Math::Matrix<4, 4>::rotation(r[0], r[1], r[2]) * Math::Matrix<4, 4>::scale(s[0], s[1], s[2]);
}

void	RT::Parser::settingResolution(unsigned int width, unsigned int height)
{
  // Set resolution only if in main file
  if (_files.size() == 1)
  {
    _scene.image().create(width, height, RT::Color(0.f).sfml());
    _scene.hud().create(width, height, RT::Color(0.f).sfml(0.f));
  }
}

void	RT::Parser::settingAntiAliasing(unsigned int live, unsigned int post)
{
  // Set antialiasing only if in main file
  if (_files.size() == 1)
  {
    _scene.antialiasing().live = live;
    _scene.antialiasing().post = post;
  }
}

void	RT::Parser::settingDephOfField(double aperture, double focal, unsigned int quality)
{
  // Set deph of field configuration only if in main file
  if (_files.size() == 1)
  {
    _scene.dof().aperture = aperture;
    _scene.dof().focal = focal;
    _scene.dof().quality = quality;
  }
}

void	RT::Parser::settingVirtualReality(double offset, double distortion, double center)
{
  // Set virtual reality configuration only if in main file
  if (_files.size() == 1)
  {
    _scene.vr().offset = offset;
    _scene.vr().distortion = distortion;
    _scene.vr().center = center;
  }
}

void	RT::Parser::settingThread(unsigned int thread)
{
  // Set number of rendering thread only in main file
  if (_files.size() == 1)
  {
    if (thread == 0)
      _scene.config().threadNumber = RT::Config::ThreadNumber - 1;
    else
      _scene.config().threadNumber = thread;
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
