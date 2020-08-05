 //====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: Shader that applies transformation matrices.
//====================================================================================================
static matrix Identity =
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1,
};

Texture2D displacementMap: register(t0);
SamplerState textureSampler : register(s0);

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix WVP;
	float CamerDepth;
}

cbuffer OptionsBuffer : register (b1)
{
	bool isUsingDisplacementMap : packoffset(c0.x);
	float displacementWeight : packoffset(c0.y);
	bool isUsingSkeleton : packoffset(c0.z);
}

cbuffer BoneTransformBuffer : register (b2)
{
	matrix boneTransforms[128];
}


matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f)
		return Identity;

	matrix transform;
	transform = mul(boneTransforms[indices[0]], weights[0]);
	transform += mul(boneTransforms[indices[1]], weights[1]);
	transform += mul(boneTransforms[indices[2]], weights[2]);
	transform += mul(boneTransforms[indices[3]], weights[3]);
	return transform;
}

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	int4 blendIndices: BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float depth : DEPTH;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;

	matrix WV = mul(World, View);

	float4 thePosition = float4(input.position, 1.0f);

	float displacement = (int)(isUsingDisplacementMap)* displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;

	thePosition = thePosition + (float4(input.normal, 0.0f) * displacement * displacementWeight);

	if (isUsingSkeleton)
	{
		matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
		output.position = mul(mul(thePosition, boneTransform), WVP);
	}
	else
	{
		output.position = mul(thePosition, WVP);
	}

	output.depth = mul(mul(thePosition, World), View).z / CamerDepth;

	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return float4(input.depth,input.depth,input.depth, 1.0f);
}
