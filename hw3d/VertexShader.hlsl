// RC VertexShader -> Properties -> HLSL Compiler
// 1) -> ouput file -> OuputDir -> ProjectDir
// 2) -> General -> Shader Type -> Vertex Shader
// apply
float4 main(float2 pos : Position) : SV_Position
{
	return float4(pos.x,pos.y,0.0f,1.0f);
}