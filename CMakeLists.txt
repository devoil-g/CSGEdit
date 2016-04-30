CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(CSGEdit)

SET(SAMPLE_SRCS
	./sources/AbstractCsgTree.cpp
	./sources/EmptyCsgTree.cpp
	./sources/ExternCsgTree.cpp

	./sources/AbstractCsgNode.cpp
	./sources/BoundingCsgNode.cpp
	./sources/DifferenceCsgNode.cpp
	./sources/IntersectionCsgNode.cpp
	./sources/MaterialCsgNode.cpp
	./sources/MeshCsgNode.cpp
	./sources/TransformationCsgNode.cpp
	./sources/UnionCsgNode.cpp
	
	./sources/AbstractCsgLeaf.cpp
	./sources/BoxCsgLeaf.cpp
	./sources/ConeCsgLeaf.cpp
	./sources/MobiusCsgLeaf.cpp
	./sources/SphereCsgLeaf.cpp
	./sources/TangleCsgLeaf.cpp
	./sources/TorusCsgLeaf.cpp
	./sources/TriangleCsgLeaf.cpp

	./sources/AbstractLightTree.cpp
	./sources/EmptyLightTree.cpp
	./sources/ExternLightTree.cpp

	./sources/AbstractLightNode.cpp
	./sources/TransformationLightNode.cpp
	./sources/UnionLightNode.cpp

	./sources/AbstractLightLeaf.cpp
	./sources/DirectionalLightLeaf.cpp
	./sources/OcclusionLightLeaf.cpp
	./sources/PointLightLeaf.cpp
	
	./sources/AbstractRaytracer.cpp
	./sources/PreviewRaytracer.cpp
	./sources/RenderRaytracer.cpp
	
	./sources/Color.cpp
	./sources/Intersection.cpp
	./sources/Material.cpp
	./sources/Parser.cpp
	./sources/Scene.cpp
	./sources/SceneLibrary.cpp

	./sources/Config.cpp
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
	./includes/AbstractCsgTree.hpp
	./includes/EmptyCsgTree.hpp
	./includes/ExternCsgTree.hpp

	./includes/AbstractCsgNode.hpp
	./includes/BoundingCsgNode.hpp
	./includes/DifferenceCsgNode.hpp
	./includes/IntersectionCsgNode.hpp
	./includes/MaterialCsgNode.hpp
	./includes/MeshCsgNode.hpp
	./includes/TransformationCsgNode.hpp
	./includes/UnionCsgNode.hpp

	./includes/AbstractCsgLeaf.hpp
	./includes/BoxCsgLeaf.hpp
	./includes/ConeCsgLeaf.hpp
	./includes/MobiusCsgLeaf.hpp
	./includes/SphereCsgLeaf.hpp
	./includes/TangleCsgLeaf.hpp
	./includes/TorusCsgLeaf.hpp
	./includes/TriangleCsgLeaf.hpp
	
	./includes/AbstractLightTree.hpp
	./includes/EmptyLightTree.hpp
	./includes/ExternLightTree.hpp

	./includes/AbstractLightNode.hpp
	./includes/TransformationLightNode.hpp
	./includes/UnionLightNode.hpp
	
	./includes/AbstractLightLeaf.hpp
	./includes/DirectionalLightLeaf.hpp
	./includes/OcclusionLightLeaf.hpp
	./includes/PointLightLeaf.hpp
	
	./includes/AbstractRaytracer.hpp
	./includes/PreviewRaytracer.hpp
	./includes/RenderRaytracer.hpp
	
	./includes/Color.hpp
	./includes/Intersection.hpp
	./includes/Material.hpp
	./includes/Parser.hpp
	./includes/Scene.hpp
	./includes/SceneLibrary.hpp

	./includes/Config.hpp
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
