////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	instances = 0;
	m_Texture = 0;
	m_model = 0;

	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;

	m_Texture1 = 0;
	m_Texture2 = 0;
	m_Texture3 = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}
int ModelClass::GetInstanceCount()
{
	return m_instanceCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}
void ModelClass::SetInstanceCount(int count)
{
	m_instanceCount = count;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	//unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc; /*indexBufferDesc;*/
    D3D11_SUBRESOURCE_DATA vertexData, instanceData;/*indexData;*/
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		//indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	if (m_instanceCount == 80)//¿œ¿⁄∏  ∆ÊΩ∫
	{
		XMFLOAT3 pos = { 1.9f, 3.3f, 0.5f };
		XMFLOAT3 rot = { 0.0f, XM_PI / -2, 0.0f };
		instances = new InstanceType[m_instanceCount];
		for (int i = 0; i < 40; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
		pos = { -1.9f, 3.3f, 0.5f };
		rot = { 0.0f,  XM_PI / 2, 0.0f };
		for (int i = 40; i < m_instanceCount; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
	}
	else if (m_instanceCount == 236)//T¿⁄∏  ∆ÊΩ∫
	{
		XMFLOAT3 pos = { 1.9f, 3.3f, 0.5f };
		XMFLOAT3 rot = { 0.0f, XM_PI / -2, 0.0f };
		instances = new InstanceType[m_instanceCount];
		for (int i = 0; i < 36; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
		pos = { -1.9f, 3.3f, 0.5f };
		rot = { 0.0f,  XM_PI / 2, 0.0f };
		for (int i = 36; i < 72; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}

		pos = { -2.5f, 3.3f, 36.1f };
		rot = { 0.0f,  0.0f, 0.0f };
		for (int i = 72; i < 112; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x -= 1.0f;
		}
		pos = { 2.5f, 3.3f, 36.1f };
		for (int i = 112; i < 152; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x += 1.0f;
		}
		pos = { -41.5f, 3.3f, 39.9f };
		rot = { 0.0f,  XM_PI, 0.0f };
		for (int i = 152; i < m_instanceCount; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x += 1.0f;
		}
	}
	else if (m_instanceCount == 160)// ø¿∏•¬ ∏  ∆ÊΩ∫
	{
		XMFLOAT3 pos = { 1.9f, 3.3f, 0.5f };
		XMFLOAT3 rot = { 0.0f, XM_PI / -2, 0.0f };
		instances = new InstanceType[m_instanceCount];
		for (int i = 0; i < 36; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
		pos = { -1.9f, 3.3f, 0.5f };
		rot = { 0.0f,  XM_PI / 2, 0.0f };
		for (int i = 36; i < 76; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
		pos = { 2.5f, 3.3f, 36.1f };
		rot = { 0.0f,  0.0f, 0.0f };
		for (int i = 76; i < 116; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x += 1.0f;
		}

		pos = { -1.5f, 3.3f, 39.9f };
		rot = { 0.0f,  XM_PI, 0.0f };
		for (int i = 116; i < m_instanceCount; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x += 1.0f;
		}
	}
	else if (m_instanceCount == 161)//ºˆ¡§«ÿæﬂ«‘ 160¿∏∑Œ // øﬁ¬ ∏  ∆ÊΩ∫
	{
		XMFLOAT3 pos = { 1.9f, 3.3f, 0.5f };
		XMFLOAT3 rot = { 0.0f, XM_PI / -2, 0.0f };
		instances = new InstanceType[m_instanceCount];
		for (int i = 0; i < 40; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
		pos = { -1.9f, 3.3f, 0.5f };
		rot = { 0.0f,  XM_PI / 2, 0.0f };
		for (int i = 40; i < 76; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.0f;
		}
		pos = { -2.5f, 3.3f, 36.1f };
		rot = { 0.0f,  0.0f, 0.0f };
		for (int i = 76; i < 116; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x -= 1.0f;
		}

		pos = { 1.5f, 3.3f, 39.9f };
		rot = { 0.0f,  XM_PI, 0.0f };
		for (int i = 116; i < 160; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.x -= 1.0f;
		}
	}
	else if (m_instanceCount == 3)
	{
		XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 rot = { 0.0f, 0.0f, 0.0f };
		instances = new InstanceType[m_instanceCount];
		for (int i = 0; i < 3; i++)
		{
			instances[i].position = pos;
			instances[i].rotation = rot;
			pos.z += 1.1f;
		}
	}
	else
	{
		m_instanceCount = 1;
		XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 rot = { 0.0f, 0.0f, 0.0f };
		instances = new InstanceType;
		instances[0].position = pos;
		instances[0].rotation = rot;
	}	

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if(FAILED(result))
	{
		return false;
	}

	delete [] instances;
	instances = 0;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	//delete [] indices;
	//indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set vertex buffer stride and offset.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);
	offsets[0] = 0;
	offsets[1] = 0;
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(const WCHAR* filename)
{
	ReadFileCounts(filename);

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::ReadFileCounts(const WCHAR* filename)
{
	ifstream fin;
	char input;
	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);

	return true;
}

bool ModelClass::LoadDataStructures(const WCHAR* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	XMFLOAT3* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new XMFLOAT3[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new XMFLOAT3[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new XMFLOAT3[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
					faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	m_vertexCount = faceCount * 3;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		m_model[i * 3].x = vertices[vIndex].x;
		m_model[i * 3].y = vertices[vIndex].y;
		m_model[i * 3].z = vertices[vIndex].z;

		m_model[i * 3].tu = texcoords[tIndex].x;
		m_model[i * 3].tv = texcoords[tIndex].y;

		m_model[i * 3].nx = normals[nIndex].x;
		m_model[i * 3].ny = normals[nIndex].y;
		m_model[i * 3].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		m_model[i * 3 + 1].x = vertices[vIndex].x;
		m_model[i * 3 + 1].y = vertices[vIndex].y;
		m_model[i * 3 + 1].z = vertices[vIndex].z;

		m_model[i * 3 + 1].tu = texcoords[tIndex].x;
		m_model[i * 3 + 1].tv = texcoords[tIndex].y;

		m_model[i * 3 + 1].nx = normals[nIndex].x;
		m_model[i * 3 + 1].ny = normals[nIndex].y;
		m_model[i * 3 + 1].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		m_model[i * 3 + 2].x = vertices[vIndex].x;
		m_model[i * 3 + 2].y = vertices[vIndex].y;
		m_model[i * 3 + 2].z = vertices[vIndex].z;

		m_model[i * 3 + 2].tu = texcoords[tIndex].x;
		m_model[i * 3 + 2].tv = texcoords[tIndex].y;

		m_model[i * 3 + 2].nx = normals[nIndex].x;
		m_model[i * 3 + 2].ny = normals[nIndex].y;
		m_model[i * 3 + 2].nz = normals[nIndex].z;
	}

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}