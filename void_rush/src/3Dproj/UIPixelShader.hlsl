struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

Texture2D diffuseTex : register(t0);
SamplerState testSampler;

float4 main(PixelInput input) : SV_TARGET
{
    return float4(diffuseTex.Sample(testSampler, input.uv).xyz,1);
    //return float4(1,0,0,1);
}
