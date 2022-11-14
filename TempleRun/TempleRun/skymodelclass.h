////////////////////////////////////////////////////////////////////////////////
// Filename: skymodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYMODELCLASS_H_
#define _SKYMODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "skytextureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: SkyModelClass
////////////////////////////////////////////////////////////////////////////////
class SkyModelClass
{
private:

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

public:
	SkyModelClass();
	SkyModelClass(const SkyModelClass&);
	~SkyModelClass();

	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetFaceCount();

	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();


private:
	ID3D11Buffer* m_vertexBuffer;/* , * m_indexBuffer;*/

	SkyTextureClass* m_SkyTexture;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	void CreateSphere(ID3D11Device*, int, int);
	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX Rotationx;
	XMMATRIX Rotationy;
	XMMATRIX Rotationz;
};

#endif