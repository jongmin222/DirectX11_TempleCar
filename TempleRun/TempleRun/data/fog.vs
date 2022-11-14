////////////////////////////////////////////////////////////////////////////////
// Filename: fog.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer PerFrameBuffer
{
	matrix worldMatrix;
    matrix secondWorldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer FogBuffer
{
	float fogStart;
	float fogEnd;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 instancePosition : TEXCOORD1;
    float3 instanceRotation : TEXCOORD2;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType FogVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 cameraPosition;

    matrix <float, 4, 4> translation =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        input.instancePosition.x, input.instancePosition.y, input.instancePosition.z, 1.0f
    };
    matrix <float, 4, 4> rotationAroundY =
    {
          cos(input.instanceRotation.y), 0.0f, -sin(input.instanceRotation.y), 0.0f,
          0.0f, 1.0f, 0.0f, 0.0f,
          sin(input.instanceRotation.y), 0.0f, cos(input.instanceRotation.y), 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f   
    };
    matrix <float, 4, 4> composition;
    composition = mul(rotationAroundY,translation);

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, composition);
    output.position = mul(output.position,secondWorldMatrix);
    output.position = mul(output.position, viewMatrix);

    // Calculate the camera position.
    cameraPosition = output.position;

    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

    // Calculate linear fog.    
    output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

    return output;
}