////////////////////////////////////////////////////////////////////////////////
// Filename: particlesystemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PARTICLESYSTEMCLASS_H_
#define _PARTICLESYSTEMCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#include <fstream>

using namespace std;
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ParticleSystemClass
////////////////////////////////////////////////////////////////////////////////
class ParticleSystemClass
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
		float killTime;
		XMFLOAT3 rotation;
		float distance;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

	struct InstanceType
	{
		XMFLOAT3 position;
		//XMFLOAT3 rotation;
		XMFLOAT4 color;
	};

public:
	ParticleSystemClass();
	ParticleSystemClass(const ParticleSystemClass&);
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, const WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*, XMFLOAT3, XMFLOAT3,  XMMATRIX, XMMATRIX);
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	//int GetIndexCount();
	int GetInstanceCount();
	int GetVertexCount();

	void RestartParticleSystem();

private:
	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float, XMFLOAT3, XMFLOAT3, XMMATRIX, XMMATRIX);
	void UpdateParticles(float, XMMATRIX, XMMATRIX);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*);

private:
	//float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	XMFLOAT3 m_particleDeviation;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

	int m_currentParticleCount;
	float m_accumulatedTime;
	
	TextureClass* m_Texture;
	ParticleType* m_particleList;
	int m_vertexCount, /*m_indexCount*/ m_instanceCount;
	VertexType* m_vertices;
	InstanceType* m_instances;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer, * m_instanceBuffer;
};

#endif