#ifndef INCLUDE_NFGE_GRAPHICS_H
#define INCLUDE_NFGE_GRAPHICS_H

#include "Common.h"

#include "Animation.h"
#include "AnimationBuilder.h"
#include "AnimationClip.h"
#include "AnimationSet.h"
#include "AnimationSetIO.h"
#include "Animator.h"
#include "BlendStateManager.h"
#include "BlendTree.h"
#include "Bone.h"
#include "BuildInGeometryMesh.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "DebugUi.h"
#include "DepthStencilState.h"
#include "DepthStencilStateManager.h"
#include "DrawCommand.h"
#include "Effect.h"
#include "EffectManager.h"
#include "Font.h"
#include "GeometryMesh.h"
#include "GeometryMeshManager.h"
#include "GeometryShader.h"
#include "GraphicsSystem.h"
#include "Keyframe.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MeshBuffer.h"
#include "MeshIO.h"
#include "MeshTextureMaterial.h"
#include "Model.h"
#include "ModelIO.h"
#include "ModelManager.h"
#include "ObjLoader.h"
#include "PartialAnimator.h"
#include "PixelShader.h"
#include "PostProcessUnit.h"
#include "PostProcessManager.h"
#include "RasterizerState.h"
#include "RasterizerStateManager.h"
#include "RenderTarget.h"
#include "Sampler.h"
#include "SamplerManager.h"
#include "SimpleDraw.h"
#include "Skeleton.h"
#include "SkeletonIO.h"
#include "SkeletonModel.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "VertexType.h"
#include "VertexType.h"

// Meta headers
#include "MetaRegistration.h"

#endif // !INCLUDE_NFGE_GRAPHICS_H