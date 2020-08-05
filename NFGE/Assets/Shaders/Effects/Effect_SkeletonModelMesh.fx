//====================================================================================================
// Filename:	Effect_SkeletonModelMesh.fx
// Created by:	Mingzhuo Zhang
// Description: Shader that does texture mapping, transform and post-process on the SkeletonModelMesh.
//====================================================================================================

static matrix Identity =
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1,
};

static const uint TextureType_DIFFUSE = 0x1 << 0;
static const uint TextureType_SPECULAR = 0x1 << 1;
static const uint TextureType_AMBIENT = 0x1 << 2;
static const uint TextureType_EMISSIVE = 0x1 << 3;
static const uint TextureType_HEIGHT = 0x1 << 4;
static const uint TextureType_NORMALS = 0x1 << 5;
static const uint TextureType_SHININESS = 0x1 << 6;
static const uint TextureType_OPACITY = 0x1 << 7;
static const uint TextureType_DISPLACEMENT = 0x1 << 8;

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D ambientMap: register(t2);
Texture2D emissiveMap: register(t3);
Texture2D heightMap : register(t4);
Texture2D normalMap : register(t5);
Texture2D shininessMap: register(t6);
Texture2D opacityMap: register(t7);
Texture2D displacementMap: register(t8);
Texture2D depthMap: register(t20);

SamplerState textureSampler : register(s0);

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float4 ViewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float4 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer: register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float4 MaterialPower;
}

cbuffer OptionsBuffer : register (b3)
{
	float displacementWeight : packoffset(c0.x);
	uint containTextureTypeMask : packoffset(c0.y);
	bool useShadow : packoffset(c0.z);
	float depthBias : packoffset(c0.w);
}

cbuffer BoneTransformBuffer : register (b4)
{
	matrix boneTransforms[128];
}

cbuffer ShadowBufer : register(b5)
{
	matrix WVPLight;
}

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	int4 blendIndices: BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 dirToLight : TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	float2 texCoord : TEXCOORD2;
	float4 positionNDC : TEXCOORD4; // new
};

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

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	
	float displacement = (containTextureTypeMask & TextureType_DISPLACEMENT) ?
		displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x : 0.0f;
	//float4 position = input.position + (float4(input.normal, 0.0f) * displacement * displacementWeight);
	float4 bonePosition = input.position + (float4(input.normal, 0.0f) * displacement * displacementWeight.x);

	matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
	matrix toWorld = mul(boneTransform, World);
	matrix toNDC = mul(boneTransform, WVP);

	float3 normal = mul(input.normal, (float3x3)toWorld);
	float3 tangent = mul(input.tangent, (float3x3)toWorld);
	float3 binormal = cross(normal, tangent);

	output.position = mul(bonePosition, toNDC);
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	output.dirToLight = -LightDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - mul(bonePosition, toWorld).xyz);
	output.texCoord = input.texCoord;

	if (useShadow)
	{
		output.positionNDC = mul(mul(bonePosition, boneTransform), WVPLight);
	}

	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 normalColor = (containTextureTypeMask & TextureType_NORMALS) ?
		normalMap.Sample(textureSampler, input.texCoord) : 0.5f;
	float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);

	float3 n = normalize(input.normal);
	float3 b = normalize(input.binormal);
	float3 t = normalize(input.tangent);
	float3x3 tbnw = float3x3(t, b, n);

	float3 normal = normalize(mul(sampledNormal, tbnw));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float4 ambient = LightAmbient * MaterialAmbient;

	float d = saturate(dot(dirToLight, normal));
	float4 diffuse = d * LightDiffuse * MaterialDiffuse;

	float3 r = reflect(LightDirection.xyz, normal);
	float s = saturate(dot(dirToView, r));
	float4 specular = pow(s, MaterialPower.x) * LightSpecular * MaterialSpecular;

	float4 diffuseColor = (containTextureTypeMask & TextureType_DIFFUSE) ?
		diffuseMap.Sample(textureSampler, input.texCoord) : 1.0f;

	float4 specularColor = (containTextureTypeMask & TextureType_SPECULAR) ?
		specularMap.Sample(textureSampler, input.texCoord) : 0.0f;
	float4 shininessColor = (containTextureTypeMask & TextureType_SHININESS) ?
		shininessMap.Sample(textureSampler, input.texCoord) : 0.0f;

	float4 textureSpecular = specularColor + shininessColor;

	float4 color = (ambient + diffuse) * diffuseColor + specular * textureSpecular.x;

	if (useShadow)
	{
		float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w; // flip it
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;

		if (saturate(shadowUV.x) == shadowUV.x && saturate(shadowUV.y) == shadowUV.y)
		{
			float saveDepth = depthMap.Sample(textureSampler, shadowUV).x;

			if (saveDepth > actualDepth + depthBias) // flip it 
				color = ambient * diffuseColor;
		}
	}

	return color;
}
