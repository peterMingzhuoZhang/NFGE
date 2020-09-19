![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/NFGE.gif?raw=true)
# NFGE
> Nicolas Four Game Engine

> This is my own C++ game engine that developed using Direct3D 11.

> I build this engine for exploring and understanging what happens under the hood when making games.

---

## Frameworks of the engine
- **AI**
    - Implemented `Finite State Machine` for AI agent's state switching
    - Implemented `Breadth First Search`, `Depth First Search`, `Dijkstra Search`, `A-Star Search` path-finding algorithm with policy design pattern
    - Implemented `Perception Module` that allows adding customized sensor and importance calculation to manipulate and prioritize the memory record for decision making.
    [Project Demo: Perception 3D](#perception-3d)
    - Implemented `Steering Module` that can calculate a final velocity for an agent based on added individual Steering Behaviors. This can allow AI agents to have a natural and smooth movement. Supported Behaviors are : `Arrive Behavior`, `Evade Behavior`, `Flee Behavior`, `Hide Behavior`, `Obstacle Avoid Behavior`, `Path Follow Behavior`, `Seek Behavior`, `Wall Avoid Behavior`, `Group Alignment Behavior`, `Group Cohesion Behavior`, `Group Separation Behavior`
    [Project Demo: Lacelle Craft](#lacelle-craft)

- **Core**
    - Implemented the window initialization using Win32 API.
    - Implemented `WindowMessageHandler` for window's message handling between system. System who need to monitor the window's message will have a static instance of `WindowMessageHandler` for hooking, unhooking customized callback function and track the previous callback function from another system. And by using ForwardMessage to achieve chain of callback functions.
    - Implemented `Block Allocator` that can preallocate a chunk of memory for smaller data to use which can avoid memory fragmentation.
    - Implemented a `Handle` system that verifies the pointer validity by tracking the generation of each pointer which can avoid dangling pointer and memory stomp.
    - Implemented `Meta` system to stores information on how to 'Create, destroy, serialize, deserialize, and access its member' of a certain class. Which can achieve run-time creating and editing of class instances.[Project Demo: NFGE Editor](#nfge-editor)
    
    
- **Graphics**
    - Implemented a graphic system that contains `Direct3D11 Device` pointer and `Direct3D11 Context` pointer which can achieve the interaction with 3D rendering pipeline in Direct3D11. 
    - Implemented `MeshBuffer`, `ConstantBuffer`, `RenderTarget`, `Sampler`, `BlendState`, `RasterizerState`, `PixelShader`, `VertexShader` to achieve the loading and manipulating of Direct3D11 data in the CPU side.
    - Implemented an `Effect` system that manages the render task of all supported Mesh types in NFGE, such as: `OnScreen2DMesh Effect`, `StandardMesh Effect`, `SkeletonModelMesh Effect`. The Use of different `Effect` required different `Effect Context` to be used.
    - Implemented `Post-processing` system that binds with `Effect` system, so it can apply active post-processing to whatever mesh that is rendered by the `Effect` system. There are four functions in the main loop to achieve the post-processing pipeline: `RegisterRenderObject()` which allows the render object register itself with the active post-process unit. `PreparePostProcess()` allows active post-process units to create essential data like z-value texture and lightWVP matrix for shadow post-process units. Each post-process unit has its own `RenderTarget` for creating customized texture. `MainRender()` is the real render function that renders every render object with certain post-process data that create in the last function into the master render target. `FinalPostProcess()` allows an active post-process unit to manipulate the pixel on the final picture.
    [Project Demo: 3D Graphic Model](#3d-graphic-model)
    - Implemented `AnimationClip`, `BoneAnimation`, `AnimationBuilder` to achieve creation and management of skeleton animations. Using the `Hint` mechanic achieves the optimization when retrieving keyframes from very long animation. (Skipping loop through all keyframe by know what next keyframe should be from `Hint`).
    - Implemented `Animator`, `PartialAnimator`, `BlendTree` to achieve smooth and dynamic skeleton animation transition that include:
        * Playing single animation
        * Transition from 1st animation to 2rd animation with percentage of how much of 2rd animation will blend in.
        * Transition from a mixed animation to 2rd animation with percentage of how much of 2rd animation will blend in.
        * Partially playing animation or blend animation
        * Blend of more than two animations with custom `ControlContext`, and `BlendPolicy` with blend tree.
        [Project Demo: YBot Simulation](#ybot-simulation)
        
        
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
    [Project Demo: Grass Simulation](#grass-simulation)
    
---


## Projects using NFGE

### <a href="http://www.peterzmzprojects.com/2020/07/nfge-editor.html">**NFGE Editor**</a> 
> Demonstrate the use of reflection system

![Alt Text](https://1.bp.blogspot.com/-XPqueHSjVPA/XxnixJoj-OI/AAAAAAAAAWY/YKpd0rUa2RA4zbkK5268pLUNBbAYala7ACLcBGAsYHQ/s1600/ComponentEditing.gif)
![Alt Text](https://1.bp.blogspot.com/-Q5nOygOw2Hw/XxnjMVE-nyI/AAAAAAAAAWg/WZ8H9XGCkQcwNuDy0j9Q9zTC3LlSEKAzwCLcBGAsYHQ/s1600/ChoosingTemplate.gif)
![Alt Text](https://1.bp.blogspot.com/-ZL8LYlD4wi4/Xxnjjovvr4I/AAAAAAAAAWo/hrFtbJnE88AWcs_SgnzMlEE52dDfH0aiQCLcBGAsYHQ/s1600/Parenting_1.gif)
 
 ---
 
 ### <a href="http://www.peterzmzprojects.com/2020/07/ai-demo-3d-perception.html">**Perception 3D**</a>
> Demonstrate the use of AI:Perception module and AI:Steering behavior

![Alt Text](https://1.bp.blogspot.com/-mKGvKU25zUA/XyT6vBlJN_I/AAAAAAAAAW8/NLzK-Y6jt3UwbbUICGt18GOhzZq3RVe0ACLcBGAsYHQ/s480/Perception.gif)

 ---
 
 ### <a href="http://www.peterzmzprojects.com/2020/08/lacelle-craft-in-process.html">**LaCelle Craft**</a> 
> Demonstrate the use of AI:Path-finding, AI:Steering behavior and AI:State Machine

![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/LacelleCraft_00.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/LacelleCraft_01.gif?raw=true)
 
 ---
 
 ### <a href="http://www.peterzmzprojects.com/2020/08/3d-graphic-demo-effect-post-processing.html">**3D Graphic Model**</a> 
> Demonstrate the use of post-processing

![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/Graphic3D_DepthBlur_00.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/Graphic3D_shadow_00.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/Graphic3D_shadow_01.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/Graphic3D_Pixelate_00.gif?raw=true)
 
 ---
 
### <a href="http://www.peterzmzprojects.com/2020/08/3d-graphic-demo-ybot-simulationin.html">**YBot Simulation**</a> 
> Demonstrate the use of skeleton Animation system.

![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/yBot_blending.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/yBot_clipSpeedModdifier.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/yBot_Partial.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/yBot_HeadRigging.gif?raw=true)

> This demo had implemented a `BlendTree` with project specific `ControlContext` and `BlendPolicy`s to achieve nature movement blending from 21 `AnimationClip`s.
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/YBot_BlendTree.png?raw=true)
- Ybot blend tree mechanism:
    * All leaf nodes contain blend information of a `AnimationClips` and a default `BlendPolicy`, They are going to share total weight of 100%.
    * Non-leaf nodes have a custom `BlendPolicy` about how to distribute current weight to its child. There are:
        + Stand_To_Move_Policy: Distribute current weight based on `mCurrentSpeed` from 0 to MAX_WALK_SPEED to 2 nodes.
        + Idle_To_Turn_Policy: Distribute current weight based on the dot product of `mCameraFacing` and `mFacingDirection` to 3 nodes.
        + Walk_To_Run_Policy: Distribute current weight based on `mCurrentSpeed` from MAX_WALK_SPEED to RUN_MAX_SPEED to 2 nodes.
        + Move_Policy: Distribute current weight based on dot product of `mMoveDirection` and `mFacingDirection` to 8 nodes.
 

![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/yBot_blendTree.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/yBot_blendTree_01.gif?raw=true)
        
---

 ### <a href="http://www.peterzmzprojects.com/2020/08/physics-demo-grass-simulation.html">**Grass Simulation**</a> 
> Demonstrate the use of Verlet Particle Phhysics

![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/Grass_00.gif?raw=true)
![Alt Text](https://github.com/peterMingzhuoZhang/NFGE/blob/master/NFGE/Assets/Images/DemoGif/Grass_01.gif?raw=true)
 
 ---

## Integrated External Library:

- <a href="https://github.com/assimp/assimp">**Assimp**</a>                          -- For 3D model importing
- <a href="https://github.com/ocornut/imgui">**Dear Imgui**</a>                    -- For Debug UI
- <a href="https://github.com/microsoft/DirectXTK">**DirectXTK**</a>                    -- For graphic & audio
- <a href="https://github.com/Tencent/rapidjson">**RapidJSON**</a>                   -- For loading and saving Json file
- <a href="https://www.mixamo.com/#/">**Mixamo**</a>                        --For 3D model resources
 
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

 
 


