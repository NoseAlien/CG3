#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float rot : ROTATION, float scale : SCALE)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.rot = rot;
	output.scale = scale;
	return output;
}