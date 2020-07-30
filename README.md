# NFGE
> This my own C++ game engine that developed using Direct3D 11.

---

## Frameworks of the engine
- **AI**
    - Implemented `Finite State Machine` for AI agent's state switching
    - Implemented `Breadth First Search`, `Depth First Search`, `Dijkstra Search`, `A-Star Seach` path-finding algorithm with policy design pattern
    - Implemented `Perception Module` that allows adding customized sensor and importance caculation to manipulate the memory record to achieve decision making.
    - Implemented `Steering Module` that can calculate a final velocity for agent base on added Steering Behaviors. This can allow AI agent to have a natrul and smooth movement. Supported Behaviors are : `Arrive Behavior`, `Evade Behavior`, `Flee Behavior`, `Hide Behavior`, `Obstacle Avoid Behavior`, `Path Follow Behavior`, `Seek Behavior`, `Wall Avoid Behavior`, `Group Alignment Behavior`, `Group Cohesion Behavior`, `Group Separation Behavior`

- **Core**
    - Implemented the window initialization and window message handling. System who need to monitering the window's message will have a static instance of `WindowMessageHandler` for hooking, unhooking customized callback function and track the previous callback function from other system. And by using ForwardMessage to achieve chain of callback functions.
    - Implemented `Block Allocator`
    
- **Graphics**
    - 1 the `code`
    - 2 nothing

- **Math**
    - 1 the `code`
    - 2 nothing

- **Physics**
    - 1 the `code`
    
---


## Projects using NFGE

- NFGE Editor
> Demonstrate the use of reflection system

![Alt Text](https://1.bp.blogspot.com/-XPqueHSjVPA/XxnixJoj-OI/AAAAAAAAAWY/YKpd0rUa2RA4zbkK5268pLUNBbAYala7ACLcBGAsYHQ/s1600/ComponentEditing.gif)
 
- YBot Simulation
> Demonstrate the use of Animation system

- LaCelle Craft
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

 
 

