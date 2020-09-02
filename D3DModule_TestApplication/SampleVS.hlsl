cbuffer Trnasform : register(b0)
{
    matrix World;
}

cbuffer ConstantMatrices : register(b1)
{
    matrix View;
    matrix Projection;
}

struct VSInput
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 UV : TEXCOORD0;
};
struct VSOutput
{
	float4 Position : SV_POSITION0;
	float3 Normal : TEXCOORD0;
	float2 UV : TEXCOORD1;
};

VSOutput SampleVS(VSInput Input)
{
    VSOutput Output = (VSOutput) 0;
    float4 WorldPosition;
    float4 WorldNormal = float4(Input.Normal, 1.0f);
    Input.Position.w = 1.0f;
    WorldPosition = mul(Input.Position, World);
    
    Output.Position = mul(WorldPosition, View);
    Output.Position = mul(Output.Position, Projection);
    
    WorldNormal = mul(WorldNormal, World);
    
    Output.Normal = WorldNormal;
    Output.UV = Input.UV;
    
    return Output;

    
}

float4 SamplePS(VSOutput Output) : SV_Target0
{
    float4 Position = Output.Position;
    
	return Position;
}