cbuffer VS_CONST_BUFFER : register(b0)
{
    matrix normal;
    matrix model;
    matrix mvp;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), mvp);
    output.tex = input.tex;
    return output;
}