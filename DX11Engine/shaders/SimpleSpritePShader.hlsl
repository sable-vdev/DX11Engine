Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return tex.Sample(samplerState, input.Tex);
}