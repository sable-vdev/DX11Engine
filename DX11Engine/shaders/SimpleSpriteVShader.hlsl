struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Tex : TEXCOORD0;
};

cbuffer VS_CONST_BUFFER : register(b0)
{
    matrix mvp;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(mvp, float4(input.Position, 1.0f));
    
    output.Tex = input.Tex;
    
    return output;
}