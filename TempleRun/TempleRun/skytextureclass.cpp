////////////////////////////////////////////////////////////////////////////////
// Filename: skytextureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "skytextureclass.h"

SkyTextureClass::SkyTextureClass()
{
	m_texture = 0;
}


SkyTextureClass::SkyTextureClass(const SkyTextureClass& other)
{
}


SkyTextureClass::~SkyTextureClass()
{
}


bool SkyTextureClass::Initialize(ID3D11Device* device, const WCHAR* filename)
{
	HRESULT result;
	ID3D11Texture2D* SMTexture = 0;

	result = CreateDDSTextureFromFileEx(device, filename, 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE,
		0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, (ID3D11Resource**)&SMTexture, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(SMTexture, &SMViewDesc, &m_texture);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void SkyTextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* SkyTextureClass::GetTexture()
{
	return m_texture;
}