////////////////////////////////////////////////////////////////////////////////
// Filename: collidermodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLLIDERMODELCLASS_H_
#define _COLLIDERMODELCLASS_H_


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
// Class name: ColliderModelClass
////////////////////////////////////////////////////////////////////////////////
class ColliderModelClass
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
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
	ColliderModelClass();
	ColliderModelClass(const ColliderModelClass&);
	~ColliderModelClass();
	
	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	void SetInstanceCount(int);

	bool LoadModel(const WCHAR*);
	void ReleaseModel();

	XMFLOAT3 GetMaxPoint();
	XMFLOAT3 GetMinPoint();
	bool Collision(XMFLOAT3, XMFLOAT3);
	bool SamePosition(XMFLOAT3, XMFLOAT3);
	bool Sensation(XMFLOAT3, XMFLOAT3, float);

	bool SetMaxMinWorldPoint(XMMATRIX, XMMATRIX);
	void SetColor(float , float , float , float );
	XMFLOAT4 GetColor();
private:
	void ShutdownBuffers();

	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);

	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_instanceBuffer;
	InstanceType* instances;
	int m_instanceCount;
	int m_vertexCount, m_indexCount, m_normalCount, m_faceCount;
	ModelType* m_model;
	XMFLOAT3 m_maxPoint, m_minPoint;
	XMFLOAT4 m_color;
};

#endif