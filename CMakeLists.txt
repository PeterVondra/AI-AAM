cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

project(AI)

set(
    headers
    include/Math/Math.h
    include/Math/Matrix.h
    include/Math/Vector.h
    include/Math/Quaternion.h
	include/Math/Transform.h
	include/Utils/Logger.h
    NNetwork.h
    NNetworkTrainer.h
    MNISTDataLoader.h
    Regression.h
)

set(
    sources
    include/Math/src/Math.cpp
    include/Math/src/Matrix.cpp
    include/Math/src/Vector.cpp
    include/Math/src/Quaternion.cpp
    include/Math/src/Transform.cpp
    include/Utils/src/Logger.cpp
    NNetwork.cpp
    NNetworkTrainer.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(
    AI
    AutoAssociativeImages.cpp
    HopField.h
    HopField.cpp
	${headers}
    ${sources}
	resources/icon.rc
)
