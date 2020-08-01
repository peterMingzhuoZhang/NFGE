# NFGE
> Nicolas Four Game Engine

> This my own C++ game engine that developed using Direct3D 11.

---

## Frameworks of the engine
- **AI**
    - Implemented `Finite State Machine` for AI agent's state switching
    - Implemented `Breadth First Search`, `Depth First Search`, `Dijkstra Search`, `A-Star Seach` path-finding algorithm with policy design pattern
    - Implemented `Perception Module` that allows adding customized sensor and importance caculation to manipulate the memory record to achieve decision making.
    [Project Demo](#perception-3d)
    - Implemented `Steering Module` that can calculate a final velocity for agent base on added Steering Behaviors. This can allow AI agent to have a natrul and smooth movement. Supported Behaviors are : `Arrive Behavior`, `Evade Behavior`, `Flee Behavior`, `Hide Behavior`, `Obstacle Avoid Behavior`, `Path Follow Behavior`, `Seek Behavior`, `Wall Avoid Behavior`, `Group Alignment Behavior`, `Group Cohesion Behavior`, `Group Separation Behavior`

- **Core**
    - Implemented the window initialization using Win32.
    - Implemented `WindowMessageHandler` for window's message handling between system. System who need to monitering the window's message will have a static instance of `WindowMessageHandler` for hooking, unhooking customized callback function and track the previous callback function from other system. And by using ForwardMessage to achieve chain of callback functions.
    - Implemented `Block Allocator` that can allcoate a chunk of memroy for smaller data to use which can avoid memory fregments.
    - Implemented `Handle` system that monitoring the pointer validity by tracking the generation of each pointer which can avoid danglging pointer and memory stomp.
    - Implemented `Meta` system allows to create a class that stores information of how to 'Create, destory, serialize, deserialize, and access its member' of a certain class. Which can achieve run-time creating and editing of a certain class.
    
    
- **Graphics**
    - Implemented a graphic system that contains `Direct3D11 Device` pointer and `Direct3D11 Context` pointer which can achieve the interaction with 3D rendering pipeline in Direct3D11. 
    - Implemented `MeshBuffer`, `ConstantBuffer`, `RenderTarget`, `Sampler`, `BlendState`, `RasterizerState`, `PixelShader`, `VertexShader` to achieve the loading and manipulating of Direct3D11 data in the CPU side.
    - Implemented `Effect` system that manage the render task of all supported Mesh type in NFGE, such as: `OnScreen2DMesh Effect`, `StandardMesh Effect`, `SkeletonModelMesh Effect`. Using of different `Effect` required different `Effect Context` to be used.
    - Implemented `Post-processing` system that bind with `Effect` system, so it can apply active post-processing to what ever mesh that render by the `Effect` system. There are four function in the main loop to achieve post-processing pipeline: `RegisterRenderObject()` which allows render object register itself with the active post-process unit. `PreparePostProcess()` that allows active post-process unit to create essential data like z-value texture and lightWVP matrix for shadow post-process unit. Each post-process unit has its own `RenderTarget` for creating customize texture. `MainRender()` is the real render function that render every render objects with certain post-process data that create in the last function into the master render target. `FinalPostProcess()` is allow active post-process unit to manipulate the pixel on the final picture.
    - Implemented `AnimationClip`, `BoneAnimation`, `AnimationBuilder` to achieve creation and management of skeleton animations. Using of `Hint` machinic achieves the optimsation when retriving keyframe from very long animation. (Skipping loop through all keyframe by know what next key frame should be from `Hint`).
    - Implemented `Animator`, `PartialAnimator`, `BlendTree` to achieve smooth and dynamic skeleton animation transition that include:
        * Playing single animation
        * Transition from 1st animation to 2rd animation with percentage of how much of 2rd animation will blend in.
        * Transition from a mixed animation to 2rd animation with percentage of how much of 2rd animation will blend in.
        * Partially playing animation or blend animation
        * Blend of more than two animation with custom `ControlContext`, and `BlendPolicy` with blend tree.

- **Math**
    - Implemented essential data structure for 3D graphic calculation include:
        * Vector2,3,4
        * Matrix3, Matrix4
        * Quaternion
        * Circle, AABB, OBB, Ray
    - Implemented useful interpolation function include:
        * LinearSpline
        * QuadraticBezier
        * CubicBezier
        * CutumlRomspline
- **Physics**
    - Implemented `Verlet` particle physics system
    
---


## Projects using NFGE

### NFGE Editor
> Demonstrate the use of reflection system

![Alt Text](https://1.bp.blogspot.com/-XPqueHSjVPA/XxnixJoj-OI/AAAAAAAAAWY/YKpd0rUa2RA4zbkK5268pLUNBbAYala7ACLcBGAsYHQ/s1600/ComponentEditing.gif)
![Alt Text](https://1.bp.blogspot.com/-Q5nOygOw2Hw/XxnjMVE-nyI/AAAAAAAAAWg/WZ8H9XGCkQcwNuDy0j9Q9zTC3LlSEKAzwCLcBGAsYHQ/s1600/ChoosingTemplate.gif)
![Alt Text](https://1.bp.blogspot.com/-ZL8LYlD4wi4/Xxnjjovvr4I/AAAAAAAAAWo/hrFtbJnE88AWcs_SgnzMlEE52dDfH0aiQCLcBGAsYHQ/s1600/Parenting_1.gif)
 
 ---
 
 ### Perception 3D
> Demonstrate the use of AI:Perception modual and AI:Steering behavior
![Alt Text](https://1.bp.blogspot.com/-mKGvKU25zUA/XyT6vBlJN_I/AAAAAAAAAW8/NLzK-Y6jt3UwbbUICGt18GOhzZq3RVe0ACLcBGAsYHQ/s480/Perception.gif)
 
 ---
 
### YBot Simulation
> Demonstrate the use of Animation system

### LaCelle Craft
> Demonstrate the use of pathfinding and State Machine

---

## Integrated External Library:

- Assimp                          -- For 3D model importing
- Dear Imgui                    -- For Debug UI
- DirectXTK                    -- For graphic & audio
- RapidJSON                   -- For loading and saving Json file
 
 ## Authors

> Mingzhuo Zhang

| <a href="https://github.com/peterMingzhuoZhang" target="_blank">**GitHub**</a>| <a href="https://www.linkedin.com/in/mingzhuo-zhang-a4115b178/" target="_blank">**LinkedIn**</a> | <a href="https://www.lcieducation.com/en/portfolios/students/62606#fndtn-projects" target="_blank">**School Profile**</a> |
| :---:| :---: |:---:|
| <img src="https://avatars2.githubusercontent.com/u/48110473?s=460&v=4" width="100"/>  | <img src="https://lh3.googleusercontent.com/fqYJHtyzZzA4vacRzeJoB93QNvA5-mvR-8UB5oVLxdYDSTpfLp_KgYD4IqVGJUgFEJo" width="100" />  | <img src="https://pbs.twimg.com/profile_images/1207081801077776384/Ba8-rA5Z_400x400.jpg" width="100" /> |
---
## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
- Copyright (c) 2020 peterzmz <a href="https://github.com/peterMingzhuoZhang/NFGE/blob/master/LICENSE" target="_blank">LICENSE.md</a>.

## Acknowledgments
- Peter Chan (My instructor who has guided me making this engine)

 
 

