////////////////////////////////////////////////////////////////////////////////
// Filename: skyshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "skyshaderclass.h"


SkyShaderClass::SkyShaderClass()
{
	m_layout = 0;
	m_sampleState = 0;

	m_skymapVertexShader = 0;
	m_skymapPixelShader = 0;
}


SkyShaderClass::SkyShaderClass(const SkyShaderClass& other)
{
}


SkyShaderClass::~SkyShaderClass()
{
}

// The new light.vs and light.ps HLSL shader files are used as input to initialize the light shader.
bool SkyShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"./data/sky.fx");
	if (!result)
	{
		return false;
	}

	return true;
}


void SkyShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool SkyShaderClass::Render(ID3D11DeviceContext* deviceContext, int faceCount,
	XMMATRIX worldMatrix, XMMATRIX secondWorldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, secondWorldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, faceCount);

	return true;
}


bool SkyShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* fxFilename)
{
	HRESULT result;

	ID3D10Blob* errorMessage;
	ID3D10Blob* skymapVertexShaderBuffer;
	ID3D10Blob* skymapPixelShaderBuffer;

	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	skymapVertexShaderBuffer = 0;
	skymapPixelShaderBuffer = 0;

	result = D3DCompileFromFile(fxFilename, 0, 0, "SKYMAP_VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &skymapVertexShaderBuffer, &errorMessage);
	result = D3DCompileFromFile(fxFilename, 0, 0, "SKYMAP_PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &skymapPixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, fxFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, fxFilename, L"Missing Shader File", MB_OK);
		}
	}
	result = device->CreateVertexShader(skymapVertexShaderBuffer->GetBufferPointer(), skymapVertexShaderBuffer->GetBufferSize(), NULL, &m_skymapVertexShader);
	result = device->CreatePixelShader(skymapPixelShaderBuffer->GetBufferPointer(), skymapPixelShaderBuffer->GetBufferSize(), NULL, &m_skymapPixelShader);
	result = device->CreateInputLayout(layout, numElements, skymapVertexShaderBuffer->GetBufferPointer(),
		skymapVertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	result = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	skymapVertexShaderBuffer->Release();
	skymapVertexShaderBuffer = 0;

	skymapPixelShaderBuffer->Release();
	skymapPixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void SkyShaderClass::ShutdownShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_skymapPixelShader)
	{
		m_skymapPixelShader->Release();
		m_skymapPixelShader = 0;
	}

	// Release the vertex shader.
	if (m_skymapVertexShader)
	{
		m_skymapVertexShader->Release();
		m_skymapVertexShader = 0;
	}
	return;
}


void SkyShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// The SetShaderParameters function now takes in lightDirection and diffuseColor as inputs.
bool SkyShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX secondWorldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture)
{
	// Set the world view projection matrix and send it to the constant buffer in effect file
	WVP = worldMatrix * secondWorldMatrix * viewMatrix * projectionMatrix;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(worldMatrix);

	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	// Send our skymap resource view to pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}


void SkyShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int NumSphereFaces)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_skymapVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_skymapPixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);

	return;
}