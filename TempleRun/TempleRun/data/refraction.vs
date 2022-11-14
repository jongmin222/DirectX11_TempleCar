////////////////////////////////////////////////////////////////////////////////
// Filename: refraction.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix secondWorldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer ClipPlaneBuffer
{
    float4 clipPlane;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 instancePosition : TEXCOORD1;
    float3 instanceRotation : TEXCOORD2;

};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float clip : SV_ClipDistance0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType RefractionVertexShader(VertexInputType input)
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
    output.position = mul(output.position,secondWorldMatrix);


    // Set the clipping plane.
    output.clip = dot(output.position, clipPlane);
    

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Set the clipping plane.
    //output.clip = dot(mul(input.position, worldMatrix), clipPlane);

    return output;
}
