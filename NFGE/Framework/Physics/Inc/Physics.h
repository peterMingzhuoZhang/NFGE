//====================================================================================================
// NFGE - Nicolas Four Game Engine
// Copyright (c) 2019 MingzhuoZhang. All rights reserved.
//====================================================================================================


#ifndef INCLUDE_NFGE_PHYSICS_H
#define INCLUDE_NFGE_PHYSICS_H

/*
Physics Engine
|
+-- Simulation
	|
	+--- Object Representation
		 +--- Particles (point mass)(center of mass)
		 +--- Rigibodies
		 +--- Soft Bodies
	+--- Kinematics/Motion
		 +--- Newton's Laws of Motion
	+--- Collision Detection
		 +--- N^2 Problem
		 +--- Optimisation
			   +--- Board Phase
					+-- Hierachical 
					+-- Spatial Partitioning
			   +--- Narrow Phase
					+-- Proxy Shapes
	+--- Collision Resolution
		 +--- Apply physics equations (momentum/energy)
*/

#include "Constraints.h"
#include "Common.h"
#include "Particle.h"
#include "PhysicsWorld.h"
#include "RigBone.h"
#include "SimplePhysicsShape.h"


#endif // !INCLUDE_NFGE_PHYSICS_H
