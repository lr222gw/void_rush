struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float4 fragpos : FRAG_POS;
};

cbuffer CBuf
{
    float4 kd;
    float4 ks;
    float4 ka;
};

Texture2D diffuseTex : register(t0); // diffuse base color
Texture2DArray<float4> shadowMapping : register(t1);
SamplerState testSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
    return diffuseTex.Sample(testSampler, input.uv);
}