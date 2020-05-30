//====================================================================================================
// Filename:	Standard.fx
// Description: Standard shader for CooEngine.
//====================================================================================================

static const float Pi = 3.14159265;
static const float TwoPi = 6.28318530718;

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

cbuffer DynamicValueBuffer : register(b3)
{
	float currentTime;
	float effectRange;
	float bottomY;
	float padding;
}


struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float3 dirToLight : TEXCOORD1;
    float3 dirToView : TEXCOORD2;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;
	output.position = input.position;

	float waveMid = currentTime * (bottomY - effectRange);
	float waveBegin = waveMid + effectRange;
	float waveEnd = waveMid;
	if ((input.position.y > waveEnd) && (input.position.y < waveBegin))
	{
		float factor = (currentTime) / (effectRange) * TwoPi;
		output.position.x = sin(factor) * output.position.x * 0.5f + output.position.x;
		output.position.z = sin(factor) * output.position.z * 0.5f + output.position.z;
	}

    output.position = mul(output.position, WVP);
    output.normal = mul(input.normal, (float3x3) World);
    output.dirToLight = -LightDirection.xyz;
    output.dirToView = normalize(ViewPosition.xyz - mul(input.position, World).xyz);
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
