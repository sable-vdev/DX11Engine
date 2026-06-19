cbuffer VS_CONST_BUFFER : register(b0)
{
    matrix normal;
    matrix model;
    matrix mvp;
}

cbuffer VS_CONST_CAM_BUFFER : register(b1)
{
    float4 cameraPosition;
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
    float3 norm : NORMAL;
    float3 viewDir : TEXCOORD1;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = mul(mvp, float4(input.position, 1.0f));
    output.tex = input.tex;
    //transform normal to world space
    output.norm = normalize(mul((float3x3) normal, input.normal));
    
    //transform position to world space
    float4 worldPos = mul(model, float4(input.position, 1.0f));
    
    output.viewDir = normalize(cameraPosition.xyz - worldPos.xyz);
    
    return output;
}