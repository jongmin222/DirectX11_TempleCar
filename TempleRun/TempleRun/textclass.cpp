///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"


TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;
	m_screenWidth = 0;
	m_screenHeight = 0;
	for (int i = 0; i < TEXT_COUNT; i++)
		m_sentence[i] = 0;
	TextCount = 0;
	for (int i = 0; i < TEXT_COUNT; i++)
	SetRender[i] = false;

	Play = false;
	Pause = true;
	Main = true;
	Success = false;
	Failure = false;

}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
	int screenWidth, int screenHeight, XMMATRIX baseViewMatrix, const WCHAR* fontFilename, const WCHAR* textureFilename)
{
	bool result;


	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if(!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, fontFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	for (int i = 0; i < TEXT_COUNT; i++)
	{
		result = InitializeSentence(&m_sentence[i], 16, device);
		if (!result)
		{
			return false;
		}
	}

	return true;
}


void TextClass::Shutdown()
{
	// Release the first sentence.
	for (int i = 0; i < TEXT_COUNT; i++)
		ReleaseSentence(&m_sentence[i]);

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	// Draw the first sentence.

	for (int i = 0; i < 9; i++)
	{
		if (SetRender[i] == true)
		{
			result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	if ((Play == true || Pause == true) && Main == false)
	{
		for (int i = 9; i < 11; i++)
		{
			if (SetRender[i] == true)
			{
				result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}
	if (Main == true)
	{
		for (int i = 11; i < 13; i++)
		{			
			if (SetRender[i] == true)
			{
				result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}

	if (Pause == true)
	{
		for (int i = 13; i < 16; i++)
		{
			if (SetRender[i] == true)
			{
				result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}

	if (Success == true)
	{
		for (int i = 16; i < 21; i++)
		{
			if (SetRender[i] == true)
			{
				result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}

	if (Failure == true)
	{
		for (int i = 21; i < 26; i++)
		{
			if (SetRender[i] == true)
			{
				result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}
	return true;
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, const char* text, int positionX, int positionY, 
	float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX worldMatrix, 
							   XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, 
		orthoMatrix, m_Font->GetTexture(), pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}


bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;


	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	SetRender[0] = true;
	result = UpdateSentence(m_sentence[0], fpsString, 20, 20, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	SetRender[1] = true;
	result = UpdateSentence(m_sentence[1], cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCameraPos(XMFLOAT3 m_position, ID3D11DeviceContext* deviceContext)
{
	bool result;
	float posX, posY, posZ;
	char xString[20], yString[20], zString[20];
	char cameraPosXString[20], cameraPosYString[20], cameraPosZString[20];

	posX = m_position.x * 10;
	posY = m_position.y * 10;
	posZ = m_position.z * 10;

	_itoa_s(posX, xString, 10);
	_itoa_s(posZ, zString, 10);
	_itoa_s(posY, yString, 10);

	strcpy_s(cameraPosXString, "camera: x:");
	strcat_s(cameraPosXString, xString);

	strcpy_s(cameraPosYString, " y:");
	strcat_s(cameraPosYString, yString);

	strcpy_s(cameraPosZString, " z:");
	strcat_s(cameraPosZString, zString);
	SetRender[2] = true;
	result = UpdateSentence(m_sentence[2], cameraPosXString, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	SetRender[3] = true;
	result = UpdateSentence(m_sentence[3], cameraPosZString, 230, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	SetRender[4] = true;
	result = UpdateSentence(m_sentence[4], cameraPosYString, 340, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}
bool TextClass::SetObject(int object, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char objectString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(object, tempString, 10);

	// Setup the cpu string.
	strcpy_s(objectString, "Object: ");
	strcat_s(objectString, tempString);

	// Update the sentence vertex buffer with the new string information.
	SetRender[5] = true;
	result = UpdateSentence(m_sentence[5], objectString, 20, 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetPolygon(int polygon, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char polygonString[32];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(polygon, tempString, 10);

	// Setup the cpu string.
	strcpy_s(polygonString, "Polygon: ");
	strcat_s(polygonString, tempString);

	// Update the sentence vertex buffer with the new string information.
	SetRender[6] = true;
	result = UpdateSentence(m_sentence[6], polygonString, 20, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetScreenSize(ID3D11DeviceContext* deviceContext)
{
	char widthTempString[16],heightTempString[16];
	char widthString[16], heightString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(m_screenWidth, widthTempString, 10);
	strcpy_s(widthString, "Size : ");
	strcat_s(widthString, widthTempString);

	_itoa_s(m_screenHeight, heightTempString, 10);
	strcpy_s(heightString, "/ ");
	strcat_s(heightString, heightTempString);

	// Update the sentence vertex buffer with the new string information.
	SetRender[7] = true;
	result = UpdateSentence(m_sentence[7], widthString, 20, 120, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	SetRender[8] = true;
	result = UpdateSentence(m_sentence[8], heightString, 150, 120, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetScore(int coin, float time, ID3D11DeviceContext* deviceContext)
{
	char coinTempString[16], scoreTempString[16];
	char coinString[16], scoreString[16];
	bool result;
	int score;


	score = time * 100 + coin * 100;

	_itoa_s(coin, coinTempString, 10);
	strcpy_s(coinString, "Coin: ");
	strcat_s(coinString, coinTempString);

	_itoa_s(score, scoreTempString, 10);
	strcpy_s(scoreString, "Score: ");
	strcat_s(scoreString, scoreTempString);

	SetRender[9] = true;
	result = UpdateSentence(m_sentence[9], scoreString, m_screenWidth - 200, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[10] = true;
	result = UpdateSentence(m_sentence[10], coinString, m_screenWidth - 200, 50, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}



bool TextClass::SetMain(ID3D11DeviceContext* deviceContext)
{
	char startString[16], tutorialString[16];

	bool result;

	strcpy_s(startString, "START");
	strcpy_s(tutorialString, "TUTORIAL");

	SetRender[11] = true;
	result = UpdateSentence(m_sentence[11], startString, m_screenWidth / 2-50, m_screenHeight / 2-40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[12] = true;
	result = UpdateSentence(m_sentence[12], tutorialString, m_screenWidth / 2-80, m_screenHeight / 2, 1.0f, 0.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetPause(ID3D11DeviceContext* deviceContext)
{
	char resumeString[16], restartString[16], mainString[16];

	bool result;
	strcpy_s(resumeString, "RESUME");
	strcpy_s(restartString, "RESTART");
	strcpy_s(mainString, "MAIN");

	SetRender[13] = true;
	result = UpdateSentence(m_sentence[13], resumeString, m_screenWidth/2-60, m_screenHeight/2-60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[14] = true;
	result = UpdateSentence(m_sentence[14], restartString, m_screenWidth/2-70, m_screenHeight / 2-20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	SetRender[15] = true;
	result = UpdateSentence(m_sentence[15], mainString, m_screenWidth/2-40, m_screenHeight / 2+20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetSuccess(int coin, float time, ID3D11DeviceContext* deviceContext)
{
	char coinTempString[16], scoreTempString[16];
	char coinString[16], scoreString[16];
	char successString[16], restartString[16], mainString[16];
	int score;
	bool result;

	score = time * 100 + coin * 100;

	_itoa_s(coin, coinTempString, 10);
	strcpy_s(coinString, "Coin: ");
	strcat_s(coinString, coinTempString);

	_itoa_s(score, scoreTempString, 10);
	strcpy_s(scoreString, "Score: ");
	strcat_s(scoreString, scoreTempString);

	strcpy_s(successString, "SUCCESS!");
	strcpy_s(restartString, "RESTART");
	strcpy_s(mainString, "MAIN");

	SetRender[18] = true;
	result = UpdateSentence(m_sentence[18], successString, m_screenWidth / 2 - 60, m_screenHeight / 2 - 140, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[16] = true;
	result = UpdateSentence(m_sentence[16], scoreString, m_screenWidth / 2 - 100, m_screenHeight / 2 - 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[17] = true;
	result = UpdateSentence(m_sentence[17], coinString, m_screenWidth / 2 - 100, m_screenHeight / 2 - 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[19] = true;
	result = UpdateSentence(m_sentence[19], restartString, m_screenWidth / 2 - 70, m_screenHeight / 2 - 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	SetRender[20] = true;
	result = UpdateSentence(m_sentence[20], mainString, m_screenWidth / 2 - 40, m_screenHeight / 2 + 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TextClass::SetFailure(int coin, float time, ID3D11DeviceContext* deviceContext)
{
	char coinTempString[16], scoreTempString[16];
	char coinString[16], scoreString[16];
	char failureString[16], restartString[16], mainString[16];
	int score;
	bool result;

	score = time * 100 + coin * 100;

	_itoa_s(coin, coinTempString, 10);
	strcpy_s(coinString, "Coin: ");
	strcat_s(coinString, coinTempString);

	_itoa_s(score, scoreTempString, 10);
	strcpy_s(scoreString, "Score: ");
	strcat_s(scoreString, scoreTempString);

	strcpy_s(failureString, "FAILURE..");
	strcpy_s(restartString, "RESTART");
	strcpy_s(mainString, "MAIN");

	SetRender[23] = true;
	result = UpdateSentence(m_sentence[23], failureString, m_screenWidth / 2 - 60, m_screenHeight / 2 - 140, 1.0f, 0.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[21] = true;
	result = UpdateSentence(m_sentence[21], scoreString, m_screenWidth / 2 - 100, m_screenHeight / 2 - 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[22] = true;
	result = UpdateSentence(m_sentence[22], coinString, m_screenWidth / 2 - 100, m_screenHeight / 2 - 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	SetRender[24] = true;
	result = UpdateSentence(m_sentence[24], restartString, m_screenWidth / 2 - 70, m_screenHeight / 2 - 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	SetRender[25] = true;
	result = UpdateSentence(m_sentence[25], mainString, m_screenWidth / 2 - 40, m_screenHeight / 2 + 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}