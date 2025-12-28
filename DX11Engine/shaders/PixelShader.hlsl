Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer LightInput : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    
    float specularPower;
    float3 padding1;
    
    float4 lightDirection;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 viewDir : TEXCOORD1;
};

struct Light
{
    float3 diffuse;
    float3 specular;
};

void BlinnPhong(float3 viewDir, float3 normal, out float4 diffuseAmbient, out float4 specular)
{
    float3 L = -lightDirection.xyz;
    
    //diffuse light
    float NdotL = saturate(dot(normal , L));
    float4 diffuse = diffuseColor * NdotL;
    
    float4 ambient = ambientColor;
    
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if(NdotL > 0.0f)
    {
        float3 H = normalize(NdotL + viewDir);
        float NdotH = pow(saturate(dot(normal, H)), specularPower);
        specular = specularColor * NdotH;
    }
    
    diffuseAmbient = saturate(diffuse + ambient);   
}

float4 main(PS_INPUT input) : SV_TARGET
{
    //texture sampling
    float4 textureCol = tex.Sample(samplerState, input.Tex);
    
    float4 diffambient, specularLight;
    BlinnPhong(input.Normal, input.viewDir, diffambient, specularLight);
    
    float4 finalColor = textureCol * diffambient + specularLight;
    
    return saturate(finalColor);
}