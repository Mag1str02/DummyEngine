# DummyEngine 

![Dummy](/.github/EnginePreview.png?raw=true)



## About
DummyEngine is a small cross-platform 3D game engine, capable of rendering, simple physics simulation and scripting.

## Features
At the moment, the engine has not so much functionality. Here is a full functionality list:

### Engine features:

* Stable:

    * `Renderer` - creates a layer of abstraction from rendering API and provides a lot of rendering wrappers. Capable of 3D model rendering and several post-processing effects
    * `PBR and Phong` - engine provides 2 most popular rasterization techniques. PBR stands for Physical Based Rendering and allows to render realistic looking objects utilizing a widely used set of material properties like roughness, metallic, ambient occlusion and normal maps. PBR also allows to use enviroment as a light source. Phong model is much simpler and gives less realistic looking results while being more performant
    * `HDR and Tone Mapping` - allows to render in High Dynamic Range to prevent color values from clipping into the standard range [0, 255], while Tone Mapping gives control over conversion to the standard range, taking into account gamma and exposure
    * `Bloom` - creates bloom effect for overly bright objects and emissive surfaces
    * `ECS` - stands for Entity Component System and allows to create various objects using components instread of inheritence. All components are stored in contiguous arrays, which reduces the number of cache misses and results in better performance of Systems. ECS also allows to create custom Systems, which can operate on component arrays of specified types
    * `Scripting` - allows to write game code in C++ scripts and then attach them to a entities. Scripts are loaded through dynamic libraries, which allows to change the game code and see the changes without recompiling the entire engine and editor 
    * `Scenes` - provides scene file format for loading and saving game scenes
    * `Logger and Profiler` - logger allows to track game and engine state while profiler allows to measure performance of engine, by measuring time of specified scopes
    * `Model Loading` - support of loading 3D models of most popular formats like fbx, gltf and obj   

* Experimental (WIP):

    * `Animations` - allows to load skelets and animations for meshes and play them
    * `Physics Engine` - allows to simulate simple physics like a gravity and collision of boxes
    * `Adudio Engine` - allows to play 3D sounds for a more immersive gaming experience

###  Editor features:

* `Inspector` - a panel that allows to modify components of entities
* `SceneHierarchy` - a panel that allows to organize scene entities into folders for more convinient scene navigating
* `Profiler` - a panel that allows to monitor performance of engine
* `Renderer` - a panel that allows to control rendering process and post-processing effects
* `Themes` - a panel that allows to change editor color theme
* `Viewport` - a panel that displays a scene and allows to play, stop, step and pause scene

## Short term goals

* `Render Graph` - a graph that manages render passes and resources, allowing to exceute rendering in other thread
* `Task System` - a system that will allow to pass long background tasks, like loading textures or modles to worker threads
* `Shadows` - support of rendering with shadows


## Dependencies

* [ASSIMP](https://github.com/assimp/assimp) - library for loading various 3D model formats
* [GLAD](https://github.com/Mag1str02/GLAD/) - library for finding OpenGL function and creating rendering context
* [GLFW](https://github.com/glfw/glfw/) - cross-platform windowing library for handing inputs and creating native os windows
* [GLM](https://github.com/g-truc/glm/) - math library with vectors, matrices and quaternions. Also implements a lot of usefull math functions
* [IMGUI](https://github.com/ocornut/imgui) - GUI library for editor interface
* [NDF](https://github.com/btzy/nativefiledialog-extended) - library for creating native os file dialogs
* [STB Image](https://github.com/nothings/stb/) - library for loading various texture formats
* [YAMLCPP](https://github.com/jbeder/yaml-cpp/) - yaml parser

## Build

**Requirements:**
* OpenGL compatible  GPU
* Supported C++ compiler
* Supported OS

**Supported OS:**
* Windows
* Linux
* Macos (partial)

**C++ Compiler:**
* GCC (10.0.0+)
* Clang (12.0.0+)
* Apple Clang (12.0.0+)

Errors may occur on other OS and/or compilers

**Build:**

At first you can try running `build.sh` script from project root dir. If some errors occur here is step-by-step guide:

1. Start by cloning the repository with `git clone --recurse https://github.com/Mag1str02/DummyEngine` 
2. Create build directory by `mkdir build`
3. Move to build dir by `cd build`
4. Configure by `cmake -DCMAKE_BUILD_TYPE=Release ..`
5. Build by `cmake --build . --target=DummyEditor --config=Release` 
6. Go to sandbox directory by `cd ../Sandbox`
7. Run editor by `./DummyEditor` (file extension may vary across different os)

To change cmake generator add `-G [your generator]` arg on step 4  
To change build mode change Release to your build mode on steps 4 and 5
If you need to build only engine library change target on step 5 to `DummyEngine`