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

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap: register(t2);
Texture2D normalMap: register(t3);
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
	float4 displacementWeight;
}

cbuffer BoneTransformBuffer : register (b4)
{
	matrix boneTransforms[128];
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
	
	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float4 bonePosition = input.position +(float4(input.normal, 0.0f) * displacement * displacementWeight.x);
	
	matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
	matrix toWorld = mul(boneTransform, World);
	matrix toNDC = mul(boneTransform, WVP);

	float3 normal = mul(input.normal, (float3x3)(toWorld));
	float3 tangent = mul(input.tangent, (float3x3)(toWorld));
	float3 binormal = cross(normal, tangent);

	output.position = mul(bonePosition, toNDC);
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	output.dirToLight = -LightDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - mul(bonePosition, toWorld).xyz);
	output.texCoord = input.texCoord;

	/*output.position = mul(input.position, WVP);
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	output.dirToLight = -LightDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - mul(input.position, World).xyz);
	output.texCoord = input.texCoord;*/

	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
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

	float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 textureSpecular = specularMap.Sample(textureSampler, input.texCoord);

	return (ambient + diffuse) * diffuseColor + specular * textureSpecular.x;
}
