struct VertexShaderInput
{
    float2 position : POSITION;
    float2 uv : UV;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

cbuffer CBuf
{
    float2 size;
    float2 position;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    //output.position = float4(input.position.x * size.x + position.x, input.position.y * size.y + position.y, 0, 0);
    output.position = float4(input.position.x * 0.5f, input.position.y * 0.5f, 0, 1);
    //output.position = float4(0, 0, 0, 0);
    output.uv = input.uv;
    //output.uv = float2(0.5,0.5);

    return output;
}