////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
#include "windows.h"
#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	void RenderReflection(float);
	XMMATRIX GetReflectionViewMatrix();

	void SetMouseVariance(POINT m_pt);

	bool Initialize();
	void CalculateMove(XMVECTOR, XMVECTOR);
	void CalculateRotation();
	void Render();
	void GetViewMatrix(XMMATRIX&);
	void GetBaseMatrix(XMMATRIX&);
	void GetPlayerMatrix(XMMATRIX, XMMATRIX, XMFLOAT3, XMFLOAT3);

	bool MoveFront;
	bool MoveBack;
	bool MoveLeft;
	bool MoveRight;
	bool MoveUp;
	bool MoveDown;
	bool Slide;

	bool Play;
	bool TestMode;

private:
	POINT m_mouseVariance;

	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_reflectionViewMatrix;

	XMMATRIX m_playerRotateMatrix, m_playerTransMatrix;
	XMFLOAT3 m_playerRotation, m_playerPosition;
	XMFLOAT3 m_cameraDistance;
};

#endif