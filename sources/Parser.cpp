// TODO: scene parameter (aa, post-aa, 3D anaglyph, deph of field...) in script
// TODO: import relative to parent file path

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
  _scene->file = path;
  
  _transformation.push(Math::Matrix<4, 4>::identite());

  try
  {
    // Parse file
    _scene->tree = import(path);
  }
  catch (std::exception e)
  {
    std::cerr << "[Parser] ERROR: failed to parse file '" << path << "' (" << std::string(e.what()) << ")." << std::endl;

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

    delete _scene;
    return nullptr;
  }

  // If no light, force basic configuration
  if (_scene->light.empty())
  {
    _scene->light.push_back(new RT::OcclusionLight(RT::Color(1.f), 0));
    _scene->light.push_back(new RT::DirectionalLight(Math::Matrix<4, 4>::rotation(0, 60, 30), RT::Color(1.f), 0));
  }

  _transformation.pop();

  return _scene;
}

RT::AbstractTree *		RT::Parser::import(std::string const & path)
{
  chaiscript::ChaiScript	script(chaiscript::Std_Lib::library());
  
  // Set up script parser
  // Scope CSG
  script.add(chaiscript::fun(&RT::Parser::scopeDifference, this), "difference");
  script.add(chaiscript::fun(&RT::Parser::scopeIntersection, this), "intersection");
  script.add(chaiscript::fun(&RT::Parser::scopeUnion, this), "union");
  // Scope transformations
  script.add(chaiscript::fun(&RT::Parser::scopeTranslation, this), "translation");
  script.add(chaiscript::fun(&RT::Parser::scopeMirror, this), "mirror");
  script.add(chaiscript::fun(&RT::Parser::scopeRotation, this), "rotation");
  script.add(chaiscript::fun(&RT::Parser::scopeScale, this), "scale");
  script.add(chaiscript::fun(&RT::Parser::scopeShear, this), "shear");
  // Scope materials
  script.add(chaiscript::fun(&RT::Parser::scopeMaterial, this), "material");
  script.add(chaiscript::fun(&RT::Parser::scopeColor, this), "color");
  script.add(chaiscript::fun(&RT::Parser::scopeColor, this), "ambient");
  script.add(chaiscript::fun(&RT::Parser::scopeColor, this), "diffuse");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &)>(std::bind(&RT::Parser::scopeSpecular, this, std::placeholders::_1, 1.f))), "specular");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double)>(std::bind(&RT::Parser::scopeSpecular, this, std::placeholders::_1, std::placeholders::_2))), "specular");
  script.add(chaiscript::fun(&RT::Parser::scopeReflection, this), "reflection");
  script.add(chaiscript::fun(std::function<void(double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, 1.f))), "transparency");
  script.add(chaiscript::fun(std::function<void(double, double)>(std::bind(&RT::Parser::scopeTransparency, this, std::placeholders::_1, std::placeholders::_2))), "transparency");
  // Scope others
  script.add(chaiscript::fun(&RT::Parser::scopeBounding, this), "bounding");
  // Scope utilities
  script.add(chaiscript::fun(&RT::Parser::scopeEnd, this), "end");
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
  // Light
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, 0.f, RT::Config::Light::Quality))), "directional_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, std::placeholders::_2, RT::Config::Light::Quality))), "directional_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double, unsigned int)>(std::bind(&RT::Parser::lightDirectional, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "directional_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, 0.f, RT::Config::Light::Quality))), "ambient_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, 0.f, RT::Config::Light::Quality))), "occlusion_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, std::placeholders::_2, RT::Config::Light::Quality))), "occlusion_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double, unsigned int)>(std::bind(&RT::Parser::lightOcclusion, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "occlusion_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, 0.f, 1.f, 0.f, 0.f, RT::Config::Light::Quality))), "point_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, 0.f, 0.f, RT::Config::Light::Quality))), "point_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double, double, double, double)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, RT::Config::Light::Quality))), "point_light");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, double, double, double, double, unsigned int)>(std::bind(&RT::Parser::lightPoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6))), "point_light");
  // Settings
  script.add(chaiscript::fun(&RT::Parser::settingCamera, this), "camera");
  script.add(chaiscript::fun(&RT::Parser::settingResolution, this), "resolution");
  script.add(chaiscript::fun(std::function<void(unsigned int)>(std::bind(&RT::Parser::settingAntiAliasing, this, std::placeholders::_1, 0))), "antialiasing");
  script.add(chaiscript::fun(std::function<void(unsigned int, unsigned int)>(std::bind(&RT::Parser::settingAntiAliasing, this, std::placeholders::_1, std::placeholders::_2))), "antialiasing");
  script.add(chaiscript::fun(std::function<void(const std::vector<chaiscript::Boxed_Value> &, const std::vector<chaiscript::Boxed_Value> &, const std::vector<chaiscript::Boxed_Value> &)>(std::bind(&RT::Parser::settingLight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))), "light");
  // Others
  script.add(chaiscript::fun(&RT::Parser::import, this), "import");

  // Initailize scope
  RT::UnionNode *	topNode;
  size_t		scopeDepth;
  
  topNode = new RT::UnionNode();
  if (!_scope.empty())
    _scope.top()->push(topNode);
  _scope.push(topNode);
  _files.push(path);
  scopeDepth = _scope.size();

  // Parsing file
  script.eval_file(path);
  
  // Check scope status
  if (scopeDepth != _scope.size())
    throw RT::Exception("Invalid scope depth at end of file '" + path + "'.");

  // Clean initial scope
  _scope.pop();
  _files.pop();

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

void	RT::Parser::scopeTranslation(const std::vector<chaiscript::Boxed_Value> & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  double	      x, y, z;

  if (transformation.size() == 3)
  {
    x = transformation[0];
    y = transformation[1];
    z = transformation[2];
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::translation(x, y, z)));
}

void	RT::Parser::scopeMirror(const std::vector<chaiscript::Boxed_Value> & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  double	      x, y, z;

  if (transformation.size() == 3)
  {
    x = transformation[0];
    y = transformation[1];
    z = transformation[2];
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::reflection(x, y, z)));
}

void	RT::Parser::scopeRotation(const std::vector<chaiscript::Boxed_Value> & v)
{
  std::vector<double> transformation = convertVector<double>(v);
  double	      x, y, z;

  if (transformation.size() == 3)
  {
    x = transformation[0];
    y = transformation[1];
    z = transformation[2];
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  scopeStart(new RT::TransformationNode(Math::Matrix<4, 4>::rotation(x, y, z)));
}

void	RT::Parser::scopeScale(const std::vector<chaiscript::Boxed_Value> & v)
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

void	RT::Parser::scopeColor(const std::vector<chaiscript::Boxed_Value> & v)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  RT::Material	material;

  material.color = RT::Color(r, g, b);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeAmbient(const std::vector<chaiscript::Boxed_Value> & v)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  RT::Material	material;

  material.ambient = RT::Color(r, g, b);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeDiffuse(const std::vector<chaiscript::Boxed_Value> & v)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  RT::Material	material;

  material.diffuse = RT::Color(r, g, b);
  scopeStart(new RT::MaterialNode(material));
}

void	RT::Parser::scopeSpecular(const std::vector<chaiscript::Boxed_Value> & v, double s)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

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

void	RT::Parser::scopeBounding()
{
  scopeStart(new RT::BoundingNode());
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

void	RT::Parser::primitiveMesh(std::string const & path)
{
  primitivePush(new RT::MeshNode(path));
}

void	RT::Parser::primitivePush(RT::AbstractTree * tree)
{
  _scope.top()->push(tree);
}

void	RT::Parser::lightDirectional(const std::vector<chaiscript::Boxed_Value> & v, double angle, unsigned int quality)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  lightPush(new RT::DirectionalLight(_transformation.top(), RT::Color(r, g, b), angle, quality));
}

void	RT::Parser::lightOcclusion(const std::vector<chaiscript::Boxed_Value> & v, double radius, unsigned int quality)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  lightPush(new RT::OcclusionLight(RT::Color(r, g, b), radius, quality));
}

void	RT::Parser::lightPoint(const std::vector<chaiscript::Boxed_Value> & v, double radius, double intensity, double angle1, double angle2, unsigned int quality)
{
  std::vector<double> clr = convertVector<double>(v);
  double	      r, g, b;

  if (clr.size() == 1)
  {
    r = clr[0] / 255.f;
    g = clr[0] / 255.f;
    b = clr[0] / 255.f;
  }
  else if (clr.size() == 3)
  {
    r = clr[0] / 255.f;
    g = clr[1] / 255.f;
    b = clr[2] / 255.f;
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  lightPush(new RT::PointLight(_transformation.top(), RT::Color(r, g, b), radius, intensity, angle1, angle2, quality));
}

void	RT::Parser::lightPush(RT::AbstractLight * light)
{
  _scene->light.push_back(light);
}

void	RT::Parser::settingCamera()
{
  // Set camera only if in main file
  if (_files.size() == 1)
    _scene->camera = _transformation.top();
}

void	RT::Parser::settingResolution(unsigned int width, unsigned int height)
{
  // Set resolution only if in main file
  if (_files.size() == 1)
    _scene->image.create(width, height);
}

void	RT::Parser::settingAntiAliasing(unsigned int live, unsigned int post)
{
  // Set antialiasing only if in main file
  if (_files.size() == 1)
  {
    _scene->config.liveAntiAliasing = live + 1;
    _scene->config.postAntiAliasing = post;
  }
}

void	RT::Parser::settingLight(const std::vector<chaiscript::Boxed_Value> & a, const std::vector<chaiscript::Boxed_Value> & d, const std::vector<chaiscript::Boxed_Value> & s)
{
  // Set global light multiplier only if in main file
  if (_files.size() == 1)
  {
    std::vector<double> clr;
    double		r, g, b;

    clr = convertVector<double>(a);
    if (clr.size() == 1)
    {
      r = clr[0] / 255.f;
      g = clr[0] / 255.f;
      b = clr[0] / 255.f;
    }
    else if (clr.size() == 3)
    {
      r = clr[0] / 255.f;
      g = clr[1] / 255.f;
      b = clr[2] / 255.f;
    }
    else
      throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

    _scene->config.lightAmbient = RT::Color(r, g, b);

    clr = convertVector<double>(d);
    if (clr.size() == 1)
    {
      r = clr[0] / 255.f;
      g = clr[0] / 255.f;
      b = clr[0] / 255.f;
    }
    else if (clr.size() == 3)
    {
      r = clr[0] / 255.f;
      g = clr[1] / 255.f;
      b = clr[2] / 255.f;
    }
    else
      throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

    _scene->config.lightDiffuse = RT::Color(r, g, b);

    clr = convertVector<double>(s);
    if (clr.size() == 1)
    {
      r = clr[0] / 255.f;
      g = clr[0] / 255.f;
      b = clr[0] / 255.f;
    }
    else if (clr.size() == 3)
    {
      r = clr[0] / 255.f;
      g = clr[1] / 255.f;
      b = clr[2] / 255.f;
    }
    else
      throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

    _scene->config.lightSpecular = RT::Color(r, g, b);
  }
}