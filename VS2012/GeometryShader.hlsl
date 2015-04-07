//#version 150
//layout(triangles) in;
//layout(triangle_strip, max_vertices = 3) out;

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION,
	inout TriangleStream< GSOutput > output
	)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i]+1;
		output.Append(element);
	}
}