////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_playerRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_playerPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_cameraDistance = XMFLOAT3(0.0f, 0.0f, 0.0f);
	MoveFront = false;
	MoveBack = false;
	MoveLeft = false;
	MoveRight = false;
	MoveUp = false;
	MoveDown = false;
	Slide = false;

}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}



void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

bool CameraClass::Initialize()
{
	SetPosition(0.0f, 7.0f, 6.0f);
	SetRotation(XM_PI / 20, XM_PI, 0.0f);

	return true;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::RenderReflection(float height)
{
	XMFLOAT3 pos;
	XMVECTOR position, up, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// yaw (Y 축), pitch (X 축) 및 roll (Z 축)의 회전값을 라디안 단위로 설정합니다.
	CalculateRotation();
	pitch = -m_rotation.x;
	yaw = m_rotation.y;
	roll = m_rotation.z;

	//  yaw, pitch, roll 값을 통해 회전 행렬을 만듭니다.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt 및 up 벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 회전되도록 합니다.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// 3D월드에서 카메라의 위치를 ​​설정합니다.
	CalculateMove(lookAt, up);
	pos.x = m_position.x;
	pos.y = -m_position.y + (height * 2.0f);
	pos.z = m_position.z;

	// XMVECTOR 구조체에 로드한다.
	position = XMLoadFloat3(&pos);

	// 회전 된 카메라 위치를 뷰어 위치로 변환합니다.
	lookAt = XMVectorAdd(position, lookAt);

	// 마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬을 만듭니다.
	m_reflectionViewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}

XMMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}

void CameraClass::SetMouseVariance(POINT m_pt)
{
	
	m_mouseVariance = m_pt;

	return;
}
void CameraClass::GetPlayerMatrix(XMMATRIX rotateMat, XMMATRIX transMat, XMFLOAT3 rotation, XMFLOAT3 position)
{
	m_playerRotateMatrix = rotateMat;
	m_playerTransMatrix = transMat;
	m_playerRotation = rotation;
	m_playerPosition = position;

	return;
}
void CameraClass::CalculateMove(XMVECTOR forward, XMVECTOR up)
{
	if (TestMode == true)
	{
		float MoveSpeed = 1.3f;

		if (MoveFront == true)
		{
			XMFLOAT3 Direction;
			XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
			ForwardNormalVec = XMVector3Normalize(forward);
			UpNormalVec = XMVector3Normalize(up);
			DirectionVec = ForwardNormalVec + UpNormalVec;
			XMStoreFloat3(&Direction, DirectionVec);
			m_position.x += Direction.x * MoveSpeed;
			m_position.z += Direction.z * MoveSpeed;
		}
		if (MoveBack == true)
		{
			XMFLOAT3 Direction;
			XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
			ForwardNormalVec = XMVector3Normalize(forward);
			UpNormalVec = XMVector3Normalize(up);
			DirectionVec = ForwardNormalVec + UpNormalVec;
			XMStoreFloat3(&Direction, DirectionVec);
			m_position.x -= Direction.x * MoveSpeed;
			m_position.z -= Direction.z * MoveSpeed;
		}
		if (MoveLeft == true)
		{
			XMFLOAT3 Direction;
			XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
			ForwardNormalVec = XMVector3Normalize(forward);
			UpNormalVec = XMVector3Normalize(up);
			DirectionVec = XMVector3Cross(ForwardNormalVec, UpNormalVec);
			XMStoreFloat3(&Direction, DirectionVec);
			m_position.x += Direction.x * MoveSpeed;
			m_position.z += Direction.z * MoveSpeed;
		}
		if (MoveRight == true)
		{
			XMFLOAT3 Direction;
			XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
			ForwardNormalVec = XMVector3Normalize(forward);
			UpNormalVec = XMVector3Normalize(up);
			DirectionVec = XMVector3Cross(ForwardNormalVec, UpNormalVec);
			XMStoreFloat3(&Direction, DirectionVec);
			m_position.x -= Direction.x * MoveSpeed;
			m_position.z -= Direction.z * MoveSpeed;
		}
		if (MoveUp == true)
		{
			m_position.y += MoveSpeed;
		}

		if (MoveDown == true)
		{
			m_position.y -= MoveSpeed;
		}
	}
	else
	{
		if (Play == false)
		{
			/*float MoveSpeed = 1.3f;

			if (MoveFront == true)
			{
				XMFLOAT3 Direction;
				XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
				ForwardNormalVec = XMVector3Normalize(forward);
				UpNormalVec = XMVector3Normalize(up);
				DirectionVec = ForwardNormalVec + UpNormalVec;
				XMStoreFloat3(&Direction, DirectionVec);
				m_position.x += Direction.x * MoveSpeed;
				m_position.z += Direction.z * MoveSpeed;
			}
			if (MoveBack == true)
			{
				XMFLOAT3 Direction;
				XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
				ForwardNormalVec = XMVector3Normalize(forward);
				UpNormalVec = XMVector3Normalize(up);
				DirectionVec = ForwardNormalVec + UpNormalVec;
				XMStoreFloat3(&Direction, DirectionVec);
				m_position.x -= Direction.x * MoveSpeed;
				m_position.z -= Direction.z * MoveSpeed;
			}
			if (MoveLeft == true)
			{
				XMFLOAT3 Direction;
				XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
				ForwardNormalVec = XMVector3Normalize(forward);
				UpNormalVec = XMVector3Normalize(up);
				DirectionVec = XMVector3Cross(ForwardNormalVec, UpNormalVec);
				XMStoreFloat3(&Direction, DirectionVec);
				m_position.x += Direction.x * MoveSpeed;
				m_position.z += Direction.z * MoveSpeed;
			}
			if (MoveRight == true)
			{
				XMFLOAT3 Direction;
				XMVECTOR DirectionVec, ForwardNormalVec, UpNormalVec;
				ForwardNormalVec = XMVector3Normalize(forward);
				UpNormalVec = XMVector3Normalize(up);
				DirectionVec = XMVector3Cross(ForwardNormalVec, UpNormalVec);
				XMStoreFloat3(&Direction, DirectionVec);
				m_position.x -= Direction.x * MoveSpeed;
				m_position.z -= Direction.z * MoveSpeed;
			}
			if (MoveUp == true)
			{
				m_position.y += MoveSpeed;
			}

			if (MoveDown == true)
			{
				m_position.y -= MoveSpeed;
			}*/
		}
		else
		{
			XMMATRIX tempMatrix;
			XMVECTOR tempVector;
			XMFLOAT3 tempMove;
			float MoveSpeed = 0.5f;
			static float SlideMove = 0.0f;

			m_cameraDistance = XMFLOAT3(0.0f, 4.0f, -9.0f);
			//m_playerPosition.y += 3.0f;
			m_playerPosition.z -= 3.0f;

			tempMatrix = XMMatrixRotationRollPitchYaw(0.0f, m_rotation.y, 0.0f);
			tempVector = XMLoadFloat3(&m_cameraDistance);
			tempVector = XMVector3TransformCoord(tempVector, tempMatrix);
			XMStoreFloat3(&m_cameraDistance, tempVector);

			m_position.x = m_playerPosition.x;
			//m_position.y = m_playerPosition.y;
			m_position.y = m_playerPosition.y / 2 + 3.0f;
			m_position.z = m_playerPosition.z;

			if (Slide == true)
			{
				if (SlideMove <= 2.0f)
					SlideMove += MoveSpeed;
				m_position.y -= SlideMove;
			}
			else if (Slide == false)
			{
				if (SlideMove >= 0.0f)
					SlideMove -= MoveSpeed;
				m_position.y -= SlideMove;
			}

			m_position.x += m_cameraDistance.x;
			m_position.y += m_cameraDistance.y;
			m_position.z += m_cameraDistance.z;
		}
	}
	return;
}

void CameraClass::CalculateRotation()
{
	if (TestMode == true)
	{
		float RotationSpeed = 0.001f;

		if (m_mouseVariance.y < 500 && m_mouseVariance.y > -500)
		{
			m_rotation.x += m_mouseVariance.y * RotationSpeed;

			if (m_rotation.x < -90)
				m_rotation.x = -90;
			else if (m_rotation.x > 90)
				m_rotation.x = 90;
		}
		if (m_mouseVariance.x < 500 && m_mouseVariance.x > -500)
			m_rotation.y += m_mouseVariance.x * RotationSpeed;
	}
	else
	{
		if (Play == false)
		{
			/*float RotationSpeed = 0.001f;

			if (m_point.y < 500 && m_point.y > -500)
			{
				m_rotation.x += m_point.y * RotationSpeed;

				if (m_rotation.x < -90)
					m_rotation.x = -90;
				else if (m_rotation.x > 90)
					m_rotation.x = 90;
			}
			if (m_point.x < 500 && m_point.x > -500)
				m_rotation.y += m_point.x * RotationSpeed;*/
		}
		else
		{
			float RotationSpeed = XM_PI / 20;
			static float SlideRotate = 0.0f;

			m_rotation.x = XM_PI / 20;

			if ((m_rotation.y - m_playerRotation.y) < RotationSpeed && (m_rotation.y - m_playerRotation.y) > -RotationSpeed)
			{
				m_rotation.y = m_playerRotation.y;
			}
			else if (m_rotation.y > m_playerRotation.y)
			{
				m_rotation.y -= RotationSpeed;
			}
			else if (m_rotation.y < m_playerRotation.y)
			{
				m_rotation.y += RotationSpeed;
			}

			if (Slide == true)
			{
				if (SlideRotate <= XM_PI / 20)
					SlideRotate += RotationSpeed / 4;
				m_rotation.x -= SlideRotate;
			}
			else if (Slide == false)
			{
				if (SlideRotate >= 0.0f)
					SlideRotate -= RotationSpeed / 4;
				m_rotation.x -= SlideRotate;
			}
		}
	}
	return;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render( )
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//CalculateRotation();

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x;
	yaw = m_rotation.y;
	roll = m_rotation.z;
	
	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	//CalculateMove(lookAt, up);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}

void CameraClass::GetBaseMatrix(XMMATRIX& viewMatrix)
{
	XMVECTOR up, position, lookAt;
	XMMATRIX rotationMatrix;
	SetPosition(0.0f, 0.0f, -1.0f);

	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	rotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	position = XMLoadFloat3(&m_position);
	lookAt = position + lookAt;

	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
	viewMatrix = m_viewMatrix;

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;

	return;
}