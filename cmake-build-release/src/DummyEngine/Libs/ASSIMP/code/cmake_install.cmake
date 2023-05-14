# Install script for directory: C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/DummyEngine")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/JetBrains/CLion 2022.3.2/bin/mingw/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.4-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/balanda/DummyEngine/cmake-build-release/src/DummyEngine/Libs/ASSIMP/lib/libassimp.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.4" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/balanda/DummyEngine/Sandbox/libassimp-5.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Program Files/JetBrains/CLion 2022.3.2/bin/mingw/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/anim.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/aabb.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ai_assert.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/camera.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/color4.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/color4.inl"
    "C:/Users/balanda/DummyEngine/cmake-build-release/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/config.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ColladaMetaData.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/commonMetaData.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/defs.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/cfileio.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/light.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/material.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/material.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix3x3.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix3x3.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix4x4.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix4x4.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/mesh.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ObjMaterial.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/pbrmaterial.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/GltfMaterial.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/postprocess.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/quaternion.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/quaternion.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/scene.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/metadata.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/texture.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/types.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector2.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector2.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector3.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector3.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/version.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/cimport.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/importerdesc.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Importer.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/DefaultLogger.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ProgressHandler.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/IOStream.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/IOSystem.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Logger.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/LogStream.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/NullLogger.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/cexport.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Exporter.hpp"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/DefaultIOStream.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/DefaultIOSystem.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ZipArchiveIOSystem.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SceneCombiner.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/fast_atof.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/qnan.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/BaseImporter.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Hash.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/MemoryIOWrapper.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ParsingUtils.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/StreamReader.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/StreamWriter.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/StringComparison.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/StringUtils.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SGSpatialSort.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/GenericProperty.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SpatialSort.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SkeletonMeshBuilder.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SmallVector.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SmoothingGroups.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/SmoothingGroups.inl"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/StandardShapes.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/RemoveComments.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Subdivision.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Vertex.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/LineSplitter.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/TinyFormatter.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Profiler.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/LogAux.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Bitmap.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/XMLTools.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/IOStreamBuffer.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/CreateAnimMesh.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/XmlParser.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/BlobIOSystem.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/MathFunctions.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Exceptional.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/ByteSwapper.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Compiler/pushpack1.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Compiler/poppack1.h"
    "C:/Users/balanda/DummyEngine/src/DummyEngine/Libs/ASSIMP/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

