////////////////////////////////////////////////////////////////////////////////
// Filename: firemodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIREMODELCLASS_H_
#define _FIREMODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: FireModelClass
////////////////////////////////////////////////////////////////////////////////
class FireModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct InstanceType
	{
		XMFLOAT3 position;
		XMFLOAT3 rotation;
	};

public:
	FireModelClass();
	FireModelClass(const FireModelClass&);
	~FireModelClass();

	void Shutdown();
	void Render(ID3D11DeviceContext*);

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*, const WCHAR*);
	bool LoadTextures(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR*);
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();

	//int GetIndexCount();
	int GetVertexCount();
	int GetInstanceCount();
	void SetInstanceCount(int);

	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(const WCHAR*);
	void ReleaseModel();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void ReleaseTexture();

	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);

private:
	ID3D11Buffer* m_vertexBuffer;/* , * m_indexBuffer;*/
	ID3D11Buffer* m_instanceBuffer;
	InstanceType* instances;
	int m_instanceCount;
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount;
	TextureClass* m_Texture;
	TextureClass* m_Texture1, * m_Texture2, * m_Texture3;

	ModelType* m_model;
};

#endif