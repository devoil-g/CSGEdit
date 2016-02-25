CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(CSGEdit)

SET(SAMPLE_SRCS
	./sources/AbstractLeaf.cpp
	./sources/AbstractNode.cpp
	./sources/AbstractTree.cpp

	./sources/BoxLeaf.cpp
	./sources/ConeLeaf.cpp
	./sources/SphereLeaf.cpp
	./sources/TangleLeaf.cpp
	./sources/TorusLeaf.cpp
	./sources/TriangleLeaf.cpp

	./sources/BoundingNode.cpp
	./sources/DifferenceNode.cpp
	./sources/IntersectionNode.cpp
	./sources/MaterialNode.cpp
	./sources/MeshNode.cpp
	./sources/TransformationNode.cpp
	./sources/UnionNode.cpp
	
	./sources/AbstractLight.cpp
	./sources/DirectionalLight.cpp
	./sources/OcclusionLight.cpp
	./sources/PointLight.cpp
	
	./sources/AbstractRaytracer.cpp
	./sources/PreviewRaytracer.cpp
	./sources/RenderRaytracer.cpp
	
	./sources/Color.cpp
	./sources/Intersection.cpp
	./sources/Material.cpp
	./sources/Parser.cpp
	./sources/Scene.cpp

	./sources/Config.cpp
	./sources/Exception.cpp
	./sources/Math.cpp
	./sources/Matrix.cpp
	./sources/Ray.cpp
	./sources/Vector.cpp
	./sources/Window.cpp

	./sources/StateMachine.cpp
	./sources/AbstractState.cpp
	./sources/ControlState.cpp
	./sources/RenderState.cpp
	./sources/RenderPauseState.cpp

	./sources/main.cpp
)

SET(SAMPLE_HDRS
	./includes/AbstractLeaf.hpp
	./includes/AbstractNode.hpp
	./includes/AbstractTree.hpp

	./includes/BoxLeaf.hpp
	./includes/ConeLeaf.hpp
	./includes/SphereLeaf.hpp
	./includes/TangleLeaf.hpp
	./includes/TorusLeaf.hpp
	./includes/TriangleLeaf.hpp

	./includes/BoundingNode.hpp
	./includes/DifferenceNode.hpp
	./includes/IntersectionNode.hpp
	./includes/MaterialNode.hpp
	./includes/MeshNode.hpp
	./includes/TransformationNode.hpp
	./includes/UnionNode.hpp
	
	./includes/AbstractLight.hpp
	./includes/DirectionalLight.hpp
	./includes/OcclusionLight.hpp
	./includes/PointLight.hpp
	
	./includes/AbstractRaytracer.hpp
	./includes/PreviewRaytracer.hpp
	./includes/RenderRaytracer.hpp
	
	./includes/Color.hpp
	./includes/Intersection.hpp
	./includes/Material.hpp
	./includes/Parser.hpp
	./includes/Scene.hpp

	./includes/Config.hpp
	./includes/Exception.hpp
	./includes/Math.hpp
	./includes/Matrix.hpp
	./includes/Ray.hpp
	./includes/Vector.hpp
	./includes/Window.hpp

	./includes/StateMachine.hpp
	./includes/AbstractState.hpp
	./includes/ControlState.hpp
	./includes/RenderState.hpp
	./includes/RenderPauseState.hpp
)

SET(SFML_STATIC_LIBRARIES TRUE)
FIND_PACKAGE(SFML 2 COMPONENTS graphics window system REQUIRED)

FIND_PATH(CHAISCRIPT_INCLUDE_DIR chaiscript/chaiscript.hpp)

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR} ${CHAISCRIPT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/includes/)
ADD_EXECUTABLE(CSGEdit ${SAMPLE_SRCS} ${SAMPLE_HDRS})
TARGET_LINK_LIBRARIES(CSGEdit ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${CMAKE_DL_LIBS})

IF (UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Ofast -Wall -Wextra")
ELSE (UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /bigobj")
ENDIF(UNIX)
