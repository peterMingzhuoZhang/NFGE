//====================================================================================================
// Filename:	DoLighting.fx
// Created by:	Peter Chan
// Description: Shader that applies transformation matrices.
//====================================================================================================
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
	float displacementWeight;
	uint containTextureTypeMask;
	float2 padding;
}

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
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

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;
	
	float displacement = (containTextureTypeMask & TextureType_DISPLACEMENT) ?
		displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x : 0.0f;
	float4 position = input.position + (float4(input.normal, 0.0f) * displacement * displacementWeight);
	
	float3 normal = mul(input.normal, (float3x3)World);
	float3 tangent = mul(input.tangent, (float3x3)World);
	float3 binormal = cross(normal, tangent);

	output.position = mul(position, WVP);
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	output.dirToLight = -LightDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - mul(position, World).xyz);
	output.texCoord = input.texCoord;

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

	return (ambient + diffuse) * diffuseColor + specular * textureSpecular.x;
}
