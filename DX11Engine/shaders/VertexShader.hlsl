struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
};

cbuffer VS_CONST_BUFFER : register(b0)
{
    matrix wvp;
}

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.Position, 1.0f);
    output.Position = mul(pos, wvp);
    output.Tex = input.Tex;
    
    return output;
}