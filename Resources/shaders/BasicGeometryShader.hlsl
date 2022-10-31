#include "BasicShaderHeader.hlsli"

[maxvertexcount(3)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	/*for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos = input[i].svpos;
		element.normal = input[i].normal;
		element.uv = input[i].uv;
		output.Append(element);
	}*/
	GSOutput element;
	//共通
	element.normal = input[0].normal;
	element.uv = input[0].uv;
	//1点目
	element.svpos = input[0].svpos;
	output.Append(element);
	//2点目
	element.svpos = input[0].svpos + float4(10.0f,10.0f,0,0);
	output.Append(element);
	//3点目
	element.svpos = input[0].svpos + float4(10.0f, 0, 0, 0);
	output.Append(element);
}