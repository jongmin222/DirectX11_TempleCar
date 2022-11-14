////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
// These globals can be modified externally from the framework(C++) code.
// Use cbuffer for most globals. 
// Logically organizing these buffers is important for efficient execution of 
// shaders as well as how the graphics card will store the buffers. 
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix secondWorldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
// Use different types such as float4 that are available to HLSL which make 
// programming shaders easier and readable. 
// The POSITION, COLOR, and SV_POSITION are semantics that convey to the GPU 
// the use of the variable. 
// e.g. POSITION: vertex shader, SV_POSITION: pixel shader, COLOR: both. 
// e.g. Same type: COLOR0, COLOR1, ...
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float3 instancePosition : TEXCOORD0;
    float3 instanceRotation : TEXCOORD1;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
// The vertex shader is called by the GPU when it is processing data from the 
// vertex buffers that have been sent to it. This vertex shader will be called 
// for every single vertex in the vertex buffer. 
// The input to the vertex shader must match the data format in the vertex buffer 
// as well as the type definition in the shader source file which in this case is 
// VertexInputType. 
// The output of the vertex shader will be sent to the pixel shader. 
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
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
    output.position = mul(output.position, secondWorldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the input color for the pixel shader to use.
    output.color = input.color;

    return output;
}