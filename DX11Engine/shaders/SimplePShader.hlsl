Texture2D diffTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D specularTex : register(t2);

SamplerState samplerState : register(s0);

cbuffer LightData : register(b2)
{
    float4 pos;
    float4 dir; // W used for light type 0 = directional, 1 = point
    
    float4 ambient;
    float4 diffuse;
    float4 specular;
    
    float specularPower;
    float range;
    float innerCone;
    float outerCone;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 viewDir : TEXCOORD1;
};

void BlinnPhong(float3 viewDir, float3 normal, out float4 diffuseAmbient, out float4 spec)
{
    float3 L = normalize(-dir.xyz);
    
    //diffuse light
    float NdotL = saturate(dot(normal, L));
    float4 diff = diffuse * NdotL;
    
    float4 calcAmbient = ambient;
    
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if (NdotL > 0.0f)
    {
        float3 H = normalize(L + viewDir);
        float NdotH = pow(saturate(dot(normal, H)), specularPower);
        spec = specular * NdotH;
    }
    
    diffuseAmbient = saturate(diff + calcAmbient);
}

float4 main(PSInput input) : SV_TARGET
{
    float4 color = diffTex.Sample(samplerState, input.tex);
    float4 spec = specularTex.Sample(samplerState, input.tex);
    float4 diffAmbient, specularL;
    BlinnPhong(input.viewDir, input.Normal, diffAmbient, specularL);
    
    color = color * diffAmbient + specularL * spec;
    
    return saturate(color);
}