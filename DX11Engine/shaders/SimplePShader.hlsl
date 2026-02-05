Texture2D diffuse : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 color = diffuse.Sample(samplerState, input.tex);
    return color; //* float4(1.0f, 1.0f, 1.0f, 1.0f); // Simple white color
}