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
    float4 UVSizeWXY;//first 2 is uv size the other 2 are in what index
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.position = float4(input.position.x * size.x + position.x, input.position.y * size.y + position.y, 0, 1);
    //output.position = float4(input.position.x * size.x, input.position.y * 1, 0, 1);
    //output.position = float4(0, 0, 0, 0);
    output.uv = float2((input.uv.x + UVSizeWXY.z) * UVSizeWXY.x, (input.uv.y + UVSizeWXY.w) * UVSizeWXY.y);
    //output.uv = float2(0.5,0.5);

    return output;
}