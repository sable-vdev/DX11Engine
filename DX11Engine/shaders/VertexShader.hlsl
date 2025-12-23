struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
};

cbuffer VS_CONST_CAM_BUFFER : register(b1)
{
    float4 cameraPosition;
}

cbuffer VS_CONST_BUFFER : register(b0)
{
    matrix normal;
    matrix model;
    matrix mvp;
}

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 viewDir : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(float4(input.Position, 1.0f), mvp);
    output.Tex = input.Tex;
    //transform normal to world space
    output.Normal = normalize(mul((float3x3) normal, input.Normal));
    
    //transform position to world space
    float4 worldPos = mul(model, float4(input.Position, 1.0f));
    
    output.viewDir = normalize(cameraPosition.xyz - worldPos.xyz);
    
    return output;
}