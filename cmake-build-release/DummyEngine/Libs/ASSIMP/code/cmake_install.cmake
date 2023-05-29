# Install script for directory: D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code

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
  set(CMAKE_OBJDUMP "C:/Users/stark/Downloads/mingw64/mingw64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.4-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Kursach/DummyEngine/cmake-build-release/DummyEngine/Libs/ASSIMP/lib/libassimp.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.2.4" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Kursach/DummyEngine/Sandbox/libassimp-5.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Users/stark/Downloads/mingw64/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-5.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/anim.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/aabb.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ai_assert.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/camera.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/color4.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/color4.inl"
    "D:/Kursach/DummyEngine/cmake-build-release/DummyEngine/Libs/ASSIMP/code/../include/assimp/config.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ColladaMetaData.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/commonMetaData.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/defs.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/cfileio.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/light.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/material.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/material.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix3x3.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix3x3.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix4x4.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/matrix4x4.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/mesh.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ObjMaterial.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/pbrmaterial.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/GltfMaterial.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/postprocess.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/quaternion.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/quaternion.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/scene.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/metadata.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/texture.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/types.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector2.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector2.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector3.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/vector3.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/version.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/cimport.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/importerdesc.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Importer.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/DefaultLogger.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ProgressHandler.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/IOStream.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/IOSystem.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Logger.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/LogStream.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/NullLogger.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/cexport.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Exporter.hpp"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/DefaultIOStream.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/DefaultIOSystem.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SceneCombiner.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/fast_atof.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/qnan.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/BaseImporter.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Hash.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ParsingUtils.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/StreamReader.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/StreamWriter.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/StringComparison.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/StringUtils.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SGSpatialSort.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/GenericProperty.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SpatialSort.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SmallVector.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SmoothingGroups.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/SmoothingGroups.inl"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/StandardShapes.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/RemoveComments.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Subdivision.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Vertex.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/LineSplitter.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/TinyFormatter.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Profiler.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/LogAux.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Bitmap.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/XMLTools.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/IOStreamBuffer.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/CreateAnimMesh.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/XmlParser.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/BlobIOSystem.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/MathFunctions.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Exceptional.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/ByteSwapper.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Compiler/poppack1.h"
    "D:/Kursach/DummyEngine/DummyEngine/Libs/ASSIMP/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

