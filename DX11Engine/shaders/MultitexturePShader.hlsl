Texture2D tex : register(t0);
Texture2D tex1 : register(t1);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;
    uv.x = 1.0f - uv.x;
    
    //texture sampling
    float4 textureCol = tex.Sample(samplerState, uv);
    float4 textureCol2 = tex1.Sample(samplerState, uv);
    
    float4 finalColor = textureCol * textureCol2 * 2.0f;
    
    return saturate(finalColor);
}