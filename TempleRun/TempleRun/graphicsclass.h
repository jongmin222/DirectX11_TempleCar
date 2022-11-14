////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#define INDEX_MODEL 181
#define INDEX_FIRE 2
#define INDEX_COLLIDER 211
#define MAP_COUNT 100
#define INDEX_TEXT 2
#define INDEX_BITMAP 2
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "firemodelclass.h"
#include "collidermodelclass.h"
#include "skymodelclass.h"

#include "lightshaderclass.h"
#include "lightclass.h"
#include "skyshaderclass.h"
#include "textureshaderclass.h"
#include "colorshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "rendertextureclass.h"
#include "refractionshaderclass.h"
#include "watershaderclass.h"
#include "fireshaderclass.h"
#include "fogshaderclass.h"
#include "fogwatershaderclass.h"
#include "fogreflectionshaderclass.h"
#include "fogfireshaderclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
#include "behaviortreeclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

struct BehaviorTreeInfo
{
	float rotation;


};
////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, float, POINT, POINT);

	void Restart();
	XMFLOAT3 GetCarRotation();
	XMFLOAT3 GetCarPosition();

	bool RenderCollider;
	bool RenderFog;

	bool Play;
	bool Pause;
	bool Main;
	bool Success;
	bool Failure;
	bool TestMode;

	bool CameraMoveFront;
	bool CameraMoveBack;
	bool CameraMoveLeft;
	bool CameraMoveRight;
	bool CameraMoveUp;
	bool CameraMoveDown;

	bool MoveFront;
	bool MoveBack;
	bool MoveLeft;
	bool MoveRight;
	bool MoveJump;
	bool MoveSlide;
	bool TurnLeft;
	bool TurnRight;

	bool GetCoin;

	int VertexTotalCount;
	int ObjectCount;

	bool MouseLClick;

	BehaviorTreeInfo BTInfo;
private:
	struct FogInfo
	{
		float color, start, end;
	};
	void SetFog();

	struct FireInfo
	{
		XMFLOAT3 scrollSpeeds, scales;
		XMFLOAT2 distortion1, distortion2, distortion3;
		float distortionScale, distortionBias, frameTime;
	};

	void SetFire(float);

	bool Render(float, FogInfo, FireInfo);
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture(float, float, float);
	bool RenderWater(float, float);

	void UpdateCarRotation();
	void UpdateCarPosition(float);
	void Gravity(float);

	void SetLotation(float);
	void SetWaterTranslation(float);
	void SetMouseState(POINT, POINT);
	void SetSceneState();
	void SetCameraState();
	void SetMatrix();
	bool SetText(int, int, float, float);

	void GameSuccess(float);

	void SetMaxMinBoundingBoxPoint();
	void UpdateCollision();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model[INDEX_MODEL];
	FireModelClass* m_Fire[INDEX_FIRE];
	ColliderModelClass* m_Collider[INDEX_COLLIDER];
	TextClass* m_Text[INDEX_TEXT];
	BitmapClass* m_Bitmap[INDEX_BITMAP];

	SkyModelClass* m_Sky;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	SkyShaderClass* m_SkyShader;
	TextureShaderClass* m_TextureShader;
	ColorShaderClass* m_ColorShader;

	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;
	FireShaderClass* m_FireShader;
	FogShaderClass* m_FogShader;
	FogWaterShaderClass* m_FogWaterShader;
	FogReflectionShaderClass* m_FogReflectionShader;
	FogFireShaderClass* m_FogFireShader;
	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;

	FogInfo fog;
	FireInfo fire;

	float rotation;
	int m_screenWidth, m_screenHeight;
	float m_waterHeight, m_waterTranslation;
	XMFLOAT3 CarPosition, CarRotation;
	float Slide, JumpSpeed, Acceleration;
	bool IsTurnPlace, IsTurned, Jumping, Sliding, IsMapping[4], IsTrigger[4], IsRanderModel[INDEX_MODEL];
	int MapCount, CoinCount;
	POINT m_mouseVariance;
	POINT m_mousePosition;

	XMMATRIX baseViewMatrix;
	XMMATRIX viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX worldMatrix[INDEX_MODEL], transMatrix[INDEX_MODEL], rotateMatrix[INDEX_MODEL], scaleMatrix[INDEX_MODEL];
	XMMATRIX colliderWorldMatrix[INDEX_COLLIDER], colliderTransMatrix[INDEX_COLLIDER], colliderRotateMatrix[INDEX_COLLIDER], colliderScaleMatrix[INDEX_COLLIDER];
	XMMATRIX fireWorldMatrix[INDEX_FIRE], fireTransMatrix[INDEX_FIRE], fireRotateMatrix[INDEX_FIRE], fireScaleMatrix[INDEX_FIRE];
	XMMATRIX skyWorldMatrix, skyTransMatrix, skyRotateMatrix, skyScaleMatrix;
	XMMATRIX particleWorldMatrix, particleTransMatrix, particleRotateMatrix, particleScaleMatrix;

	XMFLOAT3 startPosition[6], startRotation[6], endPosition[5], endRotation[5];
	XMVECTOR tempVector;
	XMMATRIX secondWorldMatrix[6], secondRotateMatrix[6], secondTransMatrix[6];
	XMMATRIX tempMatrix;
	XMFLOAT3 move[5], tempMove;
	int num[MAP_COUNT];

	Node* player;
	void GetBTInfo();
	void SetBTWorldInfo();
};

#endif