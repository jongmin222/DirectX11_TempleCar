////////////////////////////////////////////////////////////////////////////////
// Filename: particlesystemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "particlesystemclass.h"


ParticleSystemClass::ParticleSystemClass()
{
	m_Texture = 0;
	m_particleList = 0;
	m_vertices = 0;
	m_instances = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;

}


ParticleSystemClass::ParticleSystemClass(const ParticleSystemClass& other)
{
}


ParticleSystemClass::~ParticleSystemClass()
{
}


bool ParticleSystemClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename)
{
	bool result;


	// Load the texture that is used for the particles.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the particle system.
	result = InitializeParticleSystem();
	if (!result)
	{
		return false;
	}

	// Create the buffers that will be used to render the particles with.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}


void ParticleSystemClass::Shutdown()
{
	// Release the buffers.
	ShutdownBuffers();

	// Release the particle system.
	ShutdownParticleSystem();

	// Release the texture used for the particles.
	ReleaseTexture();

	return;
}


bool ParticleSystemClass::Frame(float frameTime, ID3D11DeviceContext* deviceContext, XMFLOAT3 CarRotation, XMFLOAT3 CarPosition,
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// Release old particles.
	KillParticles();

	// Emit new particles.
	EmitParticles(frameTime, CarRotation, CarPosition,  viewMatrix, projectionMatrix);

	// Update the position of the particles.
	UpdateParticles(frameTime,  viewMatrix, projectionMatrix);

	// Update the dynamic vertex buffer with the new position of each particle.
	result = UpdateBuffers(deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}


void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


ID3D11ShaderResourceView* ParticleSystemClass::GetTexture()
{
	return m_Texture->GetTexture();
}


//int ParticleSystemClass::GetIndexCount()
//{
//	return m_indexCount;
//}

int ParticleSystemClass::GetInstanceCount()
{
	return m_instanceCount;
}

int ParticleSystemClass::GetVertexCount()
{
	return m_vertexCount;
}

bool ParticleSystemClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void ParticleSystemClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
void ParticleSystemClass::RestartParticleSystem()
{
	//// Initialize the particle list.
	//for (int i = 0; i < m_maxParticles; i++)
	//{
	//	m_particleList[i].active = false;
	//}

	//// Initialize the current particle count to zero since none are emitted yet.
	//m_currentParticleCount = 0;

	//// Clear the initial accumulated time for the particle per second emission rate.
	//m_accumulatedTime = 0.0f;
	ShutdownParticleSystem();
	InitializeParticleSystem();
	return;
}
bool ParticleSystemClass::InitializeParticleSystem()
{
	int i;

	// Set the random deviation of where the particles can be located when emitted.
	m_particleDeviation = XMFLOAT3(0.1f, 0.1f, 0.0f);

	// Set the speed and speed variation of particles.
	m_particleVelocity = 0.75f;
	m_particleVelocityVariation = 0.25f;//0.25f

	// Set the physical size of the particles.
	m_particleSize = 0.1f;

	// Set the number of particles to emit per second.
	m_particlesPerSecond = 60.0f;

	// Set the maximum number of particles allowed in the particle system.
	m_maxParticles = 1000;

	// Create the particle list.
	m_particleList = new ParticleType[m_maxParticles];
	if (!m_particleList)
	{
		return false;
	}

	// Initialize the particle list.
	for (i = 0; i < m_maxParticles; i++)
	{
		m_particleList[i].active = false;
	}

	// Initialize the current particle count to zero since none are emitted yet.
	m_currentParticleCount = 0;

	// Clear the initial accumulated time for the particle per second emission rate.
	m_accumulatedTime = 0.0f;

	return true;
}


void ParticleSystemClass::ShutdownParticleSystem()
{
	// Release the particle list.
	if (m_particleList)
	{
		delete[] m_particleList;
		m_particleList = 0;
	}

	return;
}


bool ParticleSystemClass::InitializeBuffers(ID3D11Device* device)
{
	//unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBufferDesc/*, indexBufferDesc*/, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData/*, indexData*/, instanceData;
	HRESULT result;


	// Set the maximum number of vertices in the vertex array.
	m_vertexCount = /*m_maxParticles **/ 6;

	// Set the maximum number of indices in the index array.
	//m_indexCount = m_vertexCount;

	// Create the vertex array for the particles that will be rendered.
	m_vertices = new VertexType[m_vertexCount];
	if (!m_vertices)
	{
		return false;
	}
	// Bottom left.
	m_vertices[0].position = XMFLOAT3(-m_particleSize, -m_particleSize, 0.0f);
	m_vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	m_vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Top left.
	m_vertices[1].position = XMFLOAT3(-m_particleSize, m_particleSize, 0.0f);
	m_vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	m_vertices[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// Bottom right.
	m_vertices[2].position = XMFLOAT3(m_particleSize, -m_particleSize, 0.0f);
	m_vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	m_vertices[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// Bottom right.
	m_vertices[3].position = XMFLOAT3(m_particleSize, -m_particleSize, 0.0f);
	m_vertices[3].texture = XMFLOAT2(1.0f, 1.0f);
	m_vertices[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// Top left.
	m_vertices[4].position = XMFLOAT3(-m_particleSize, m_particleSize, 0.0f);
	m_vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
	m_vertices[4].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// Top right.
	m_vertices[5].position = XMFLOAT3(m_particleSize, m_particleSize, 0.0f);
	m_vertices[5].texture = XMFLOAT2(1.0f, 0.0f);
	m_vertices[5].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Create the index array.
	//indices = new unsigned long[m_indexCount];
	//if (!indices)
	//{
	//	return false;
	//}

	// Initialize vertex array to zeros at first.
	//memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Initialize the index array.
	//for (i = 0; i < m_indexCount; i++)
	//{
	//	indices[i] = i;
	//}

	// Set up the description of the dynamic vertex buffer.
	//vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//// Set up the description of the static index buffer.
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// Give the subresource structure a pointer to the index data.
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// Create the index buffer.
	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Release the index array since it is no longer needed.
	//delete[] indices;
	//indices = 0;

	m_instanceCount = m_maxParticles;

	m_instances = new InstanceType[m_instanceCount];
	memset(m_instances, 0, (sizeof(InstanceType) * m_instanceCount));

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;
	instanceData.pSysMem = m_instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void ParticleSystemClass::ShutdownBuffers()
{
	// Release the index buffer.
	//if (m_indexBuffer)
	//{
	//	m_indexBuffer->Release();
	//	m_indexBuffer = 0;
	//}

	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ParticleSystemClass::EmitParticles(float frameTime, XMFLOAT3 CarRotation, XMFLOAT3 CarPosition, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool emitParticle, found;
	float positionX, positionY, positionZ, velocity, red, green, blue;
	int index, i, j;

	//배기구 위치 설정
	XMMATRIX tempMat;
	XMVECTOR tempVec;
	XMFLOAT3 tempMove = XMFLOAT3(0.3f, 0.0f, -0.8f);
	tempMat = XMMatrixRotationRollPitchYaw(0.0f, CarRotation.y, 0.0f);
	tempVec = XMLoadFloat3(&tempMove);
	tempVec = XMVector3TransformCoord(tempVec, tempMat);
	XMStoreFloat3(&tempMove, tempVec);

	XMFLOAT3 tempDeviation = m_particleDeviation;
	tempMat = XMMatrixRotationRollPitchYaw(0.0f, CarRotation.y, 0.0f);
	tempVec = XMLoadFloat3(&tempDeviation);
	tempVec = XMVector3TransformCoord(tempVec, tempMat);
	XMStoreFloat3(&tempDeviation, tempVec);

	// Increment the frame time.
	m_accumulatedTime += frameTime;

	// Set emit particle to false for now.
	emitParticle = false;

	// Check if it is time to emit a new particle or not.
	if (m_accumulatedTime > (1000.0f / m_particlesPerSecond))
	{
		m_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per frame.
	if ((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
	{
		m_currentParticleCount++;

		// Now generate the randomized particle properties.
		positionX = (((float)rand() - (float)rand()) / RAND_MAX) * tempDeviation.x + CarPosition.x + tempMove.x;
		positionY = (((float)rand() - (float)rand()) / RAND_MAX) * tempDeviation.y + CarPosition.y + tempMove.y + 3.1f;
		positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * tempDeviation.z + CarPosition.z + tempMove.z - 3.0f;

		//카메라 거리 계산
		float distance;
		XMFLOAT4 tempPos;
		float depthValue;
		tempPos.x = positionX;
		tempPos.y = positionY;
		tempPos.z = positionZ;
		tempPos.w = 1.0f;
		tempVec = XMLoadFloat4(&tempPos);
		tempVec = XMVector3TransformCoord(tempVec, viewMatrix);
		tempVec = XMVector3TransformCoord(tempVec, projectionMatrix);
		XMStoreFloat4(&tempPos, tempVec);
		depthValue = tempPos.z / tempPos.w;
		distance = depthValue;

		velocity = m_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_particleVelocityVariation;

		red = (((float)rand() - (float)rand()) / RAND_MAX) / 3 + 0.3f;
		//green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		//blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		green = red;
		blue = red;
		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array.
		// We will sort using Z depth so we need to find where in the list the particle should be inserted.
		index = 0;
		found = false;
		while (!found)
		{
			if ((m_particleList[index].active == false) || (m_particleList[index].distance < distance))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle.
		i = m_currentParticleCount;
		j = i - 1;

		while (i != index)
		{
			m_particleList[i].positionX = m_particleList[j].positionX;
			m_particleList[i].positionY = m_particleList[j].positionY;
			m_particleList[i].positionZ = m_particleList[j].positionZ;
			m_particleList[i].red = m_particleList[j].red;
			m_particleList[i].green = m_particleList[j].green;
			m_particleList[i].blue = m_particleList[j].blue;
			m_particleList[i].velocity = m_particleList[j].velocity;
			m_particleList[i].active = m_particleList[j].active;
			m_particleList[i].killTime = m_particleList[j].killTime;
			m_particleList[i].rotation = m_particleList[j].rotation;
			m_particleList[i].distance = m_particleList[j].distance;
			i--;
			j--;
		}

		// Now insert it into the particle array in the correct depth order.
		m_particleList[index].positionX = positionX;
		m_particleList[index].positionY = positionY;
		m_particleList[index].positionZ = positionZ;
		m_particleList[index].red = red;
		m_particleList[index].green = green;
		m_particleList[index].blue = blue;
		m_particleList[index].velocity = velocity;
		m_particleList[index].active = true;
		m_particleList[index].killTime = 0.0f;
		m_particleList[index].rotation = CarRotation;
		m_particleList[index].distance = distance;
	}

	return;
}


void ParticleSystemClass::UpdateParticles(float frameTime, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	XMMATRIX tempMat;
	XMVECTOR tempVec;
	XMFLOAT3 tempMove;

	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
	for (int i = 0; i < m_currentParticleCount; i++)
	{
		tempMove = XMFLOAT3(0.0f, 0.0f, -3.0f);
		tempMat = XMMatrixRotationRollPitchYaw(0.0f, m_particleList[i].rotation.y, 0.0f);
		tempVec = XMLoadFloat3(&tempMove);
		tempVec = XMVector3TransformCoord(tempVec, tempMat);
		XMStoreFloat3(&tempMove, tempVec);

		m_particleList[i].positionX += tempMove.x * m_particleList[i].velocity * frameTime * 0.001f;
		m_particleList[i].positionY += tempMove.y * m_particleList[i].velocity * frameTime * 0.001f;
		m_particleList[i].positionZ += tempMove.z * m_particleList[i].velocity * frameTime * 0.001f;

		m_particleList[i].killTime += frameTime;

		//depthValue 구하기
		XMFLOAT4 tempPos;
		float depthValue;
		tempPos.x = m_particleList[i].positionX;
		tempPos.y = m_particleList[i].positionY;
		tempPos.z = m_particleList[i].positionZ;
		tempPos.w = 1.0f;
		tempVec = XMLoadFloat4(&tempPos);
		tempVec = XMVector3TransformCoord(tempVec, viewMatrix);
		tempVec = XMVector3TransformCoord(tempVec, projectionMatrix);
		XMStoreFloat4(&tempPos, tempVec);
		depthValue = tempPos.z / tempPos.w;

		m_particleList[i].distance = depthValue;
	}

	//거리에 따른 선택정렬
	for (int i = 0; i < m_currentParticleCount-1; i++)
	{
		int max_position = i;
		for (int j = i + 1; j <= m_currentParticleCount; j++)
		{
			if (m_particleList[j].distance > m_particleList[max_position].distance)
			{
				max_position = j;
			}
		}

		ParticleType temp;

		temp.positionX = m_particleList[i].positionX;
		temp.positionY = m_particleList[i].positionY;
		temp.positionZ = m_particleList[i].positionZ;
		temp.red = m_particleList[i].red;
		temp.green = m_particleList[i].green;
		temp.blue = m_particleList[i].blue;
		temp.velocity = m_particleList[i].velocity;
		temp.active = m_particleList[i].active;
		temp.killTime = m_particleList[i].killTime;
		temp.rotation = m_particleList[i].rotation;
		temp.distance = m_particleList[i].distance;

		m_particleList[i].positionX = m_particleList[max_position].positionX;
		m_particleList[i].positionY = m_particleList[max_position].positionY;
		m_particleList[i].positionZ = m_particleList[max_position].positionZ;
		m_particleList[i].red = m_particleList[max_position].red;
		m_particleList[i].green = m_particleList[max_position].green;
		m_particleList[i].blue = m_particleList[max_position].blue;
		m_particleList[i].velocity = m_particleList[max_position].velocity;
		m_particleList[i].active = m_particleList[max_position].active;
		m_particleList[i].killTime = m_particleList[max_position].killTime;
		m_particleList[i].rotation = m_particleList[max_position].rotation;
		m_particleList[i].distance = m_particleList[max_position].distance;

		m_particleList[max_position].positionX = temp.positionX;
		m_particleList[max_position].positionY = temp.positionY;
		m_particleList[max_position].positionZ = temp.positionZ;
		m_particleList[max_position].red = temp.red;
		m_particleList[max_position].green = temp.green;
		m_particleList[max_position].blue = temp.blue;
		m_particleList[max_position].velocity = temp.velocity;
		m_particleList[max_position].active = temp.active;
		m_particleList[max_position].killTime = temp.killTime;
		m_particleList[max_position].rotation = temp.rotation;
		m_particleList[max_position].distance = temp.distance;
	}

	return;
}


void ParticleSystemClass::KillParticles()
{
	// Kill all the particles that have gone below a certain height range.
	for (int i = 0; i < m_maxParticles; i++)
	{
		if (m_particleList[i].active == true && m_particleList[i].killTime > 1500.0f)
		{
			m_particleList[i].active = false;
			m_particleList[i].killTime = 0.0f;
			m_currentParticleCount--;

			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
			for (int j = i; j < m_maxParticles - 1; j++)
			{
				m_particleList[j].positionX = m_particleList[j + 1].positionX;
				m_particleList[j].positionY = m_particleList[j + 1].positionY;
				m_particleList[j].positionZ = m_particleList[j + 1].positionZ;
				m_particleList[j].red = m_particleList[j + 1].red;
				m_particleList[j].green = m_particleList[j + 1].green;
				m_particleList[j].blue = m_particleList[j + 1].blue;
				m_particleList[j].velocity = m_particleList[j + 1].velocity;
				m_particleList[j].active = m_particleList[j + 1].active;
				m_particleList[j].killTime = m_particleList[j + 1].killTime;
				m_particleList[j].rotation = m_particleList[j + 1].rotation;
				m_particleList[j].distance = m_particleList[j + 1].distance;
			}
		}
	}

	return;
}


bool ParticleSystemClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//VertexType* verticesPtr;
	InstanceType* instancesPtr;

	// Initialize vertex array to zeros at first.
	//memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));
	memset(m_instances, 0, (sizeof(InstanceType) * m_instanceCount));
	
	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.


	//for (i = 0; i < m_currentParticleCount; i++)
	//{
	//	// Bottom left.
	//	m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
	//	m_vertices[index].texture = XMFLOAT2(0.0f, 1.0f);
	//	m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	//	index++;

	//	// Top left.
	//	m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
	//	m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
	//	m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	//	index++;

	//	// Bottom right.
	//	m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
	//	m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
	//	m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	//	index++;

	//	// Bottom right.
	//	m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
	//	m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
	//	m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	//	index++;

	//	// Top left.
	//	m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
	//	m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
	//	m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	//	index++;

	//	// Top right.
	//	m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
	//	m_vertices[index].texture = XMFLOAT2(1.0f, 0.0f);
	//	m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	//	index++;
	//}
	for (int i = 0; i < m_currentParticleCount; i++)
	{
		m_instances[i].position = XMFLOAT3(m_particleList[i].positionX, m_particleList[i].positionY, m_particleList[i].positionZ);
		m_instances[i].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
	}


	for (int i = 0; i < m_instanceCount; i++)
	{
		if (i < m_currentParticleCount)
		{
			m_instances[i].position = XMFLOAT3(m_particleList[i].positionX, m_particleList[i].positionY, m_particleList[i].positionZ);
			m_instances[i].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		}
		else
			m_instances[i].position = XMFLOAT3(0.0f, 50.0f, 0.0f);
	}

	// Lock the vertex buffer.
	//result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	//verticesPtr = (VertexType*)mappedResource.pData;
	instancesPtr = (InstanceType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	//memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType) * m_vertexCount));
	memcpy(instancesPtr, (void*)m_instances, (sizeof(InstanceType) * m_instanceCount));

	// Unlock the vertex buffer.
	//deviceContext->Unmap(m_vertexBuffer, 0);
	deviceContext->Unmap(m_instanceBuffer, 0);
	return true;
}


void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
	//deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	// Set the index buffer to active in the input assembler so it can be rendered.
	//deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}