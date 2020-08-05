//====================================================================================================
// Filename:	Standard.fx
// Description: Standard shader for CooEngine.
//====================================================================================================

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

cbuffer MaterialBuffer : register(b2)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialPower;
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
    float3 normal : TEXCOORD0;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.position = mul(input.position, WVP);
    output.normal = mul(input.normal, (float3x3) World);
    output.dirToLight = -LightDirection.xyz;
    output.dirToView = normalize(ViewPosition.xyz - mul(input.position, World).xyz);
	output.texCoord = input.texCoord;
    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    float3 normal = normalize(input.normal);
    float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);

    float4 ambient = LightAmbient * MaterialAmbient;
    
    float d = saturate(dot(dirToLight, normal));
    float4 diffuse = d * LightDiffuse * MaterialDiffuse;

	float3 r = reflect(LightDirection.xyz, normal);
    float s = saturate(dot(dirToView, r));
    float4 specular = pow(s, MaterialPower.x) * LightSpecular * MaterialSpecular;

	return ((ambient + diffuse)) + (specular);
}
