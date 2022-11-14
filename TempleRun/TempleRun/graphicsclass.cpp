////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	for (int i = 0; i < INDEX_MODEL; i++)
		m_Model[i] = 0;
	for (int i = 0; i < INDEX_FIRE; i++)
		m_Fire[i] = 0;
	for (int i = 0; i < INDEX_COLLIDER; i++)
		m_Collider[i] = 0;
	for (int i = 0; i < INDEX_TEXT; i++)
		m_Text[i] = 0;
	for (int i = 0; i < INDEX_BITMAP; i++)
		m_Bitmap[i] = 0;

	m_Sky = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_SkyShader = 0;
	m_TextureShader = 0;

	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
	m_FireShader = 0;
	m_FogShader = 0;
	m_FogWaterShader = 0;
	m_FogReflectionShader = 0;
	m_FogFireShader = 0;
	m_ParticleShader = 0;
	m_ParticleSystem = 0;


	RenderCollider = false;
	RenderFog = false;

	Play = false;
	Pause = false;
	Main = true;
	Success = false;
	Failure = false;
	TestMode = false;


	MouseLClick = false;

	CameraMoveFront = false;
	CameraMoveBack = false;
	CameraMoveLeft = false;
	CameraMoveRight = false;
	CameraMoveUp = false;
	CameraMoveDown = false;

	MoveFront = false;
	MoveBack = false;
	MoveLeft = false;
	MoveRight = false;
	TurnLeft = false;
	TurnRight = false;

	GetCoin = false;

	rotation = 0.0f;

	VertexTotalCount = 0;
	ObjectCount = 0;

	CarPosition.x = 0.0f;
	CarPosition.y = 0.0f;
	CarPosition.z = 0.0f;
	CarRotation.x = 0.0f;
	CarRotation.y = 0.0f;
	CarRotation.z = 0.0f;
	JumpSpeed = 0.0f;
	Acceleration = 0.0f;

	IsTurnPlace = false;
	IsTurned = false;
	Jumping = false;
	Slide = 0.0f;
	Sliding = false;
	for (int i = 0; i < 4; i++)
		IsMapping[i] = false;
	for (int i = 0; i < 4; i++)
		IsTrigger[i] = false;
	for (int i = 0; i < INDEX_MODEL; i++)
		IsRanderModel[i] = true;
	MapCount = 0;
	CoinCount = 0;

	m_mousePosition.x = 800;
	m_mousePosition.y = 450;

	

	for (int i = 0; i < INDEX_MODEL; i++)
	{
		scaleMatrix[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		transMatrix[i] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < INDEX_FIRE; i++)
	{
		fireScaleMatrix[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		fireRotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		fireTransMatrix[i] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < INDEX_COLLIDER; i++)
	{
		colliderScaleMatrix[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		colliderRotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		colliderTransMatrix[i] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	}

	skyScaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	skyRotateMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	skyTransMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	particleScaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	particleRotateMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	particleTransMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->GetBaseMatrix(baseViewMatrix);

	result = m_Camera->Initialize();
	if(!result)
	{
		return false;
	}

	// Create the model object.
	for (int i = 0; i < INDEX_MODEL; i++)
	{
		m_Model[i] = new ModelClass;
		if (!m_Model[i])
		{
			return false;
		}
	}
	for (int i = 0; i < INDEX_FIRE; i++)
	{
		m_Fire[i] = new FireModelClass;
		if (!m_Fire[i])
		{
			return false;
		}
	}
	for (int i = 0; i < INDEX_COLLIDER; i++)
	{
		m_Collider[i] = new ColliderModelClass;
		if (!m_Collider[i])
		{
			return false;
		}
	}

		m_Sky = new SkyModelClass;
		if (!m_Sky)
		{
			return false;
		}

	// Initialize the model object.

	//배경, 장식, 플레이어
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), L"./data/object/ground1.obj", L"./data/texture/ground.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object000.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), L"./data/object/car.obj", L"./data/texture/car.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object333.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[2]->Initialize(m_D3D->GetDevice(), L"./data/object/shack.obj", L"./data/texture/wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[3]->Initialize(m_D3D->GetDevice(), L"./data/object/startfloor.obj", L"./data/texture/grass.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
		return false;
	}
	//result = m_Model[4]->Initialize(m_D3D->GetDevice(), L"./data/object/tree.obj", L"./data/texture/tree.dds");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
	//	return false;
	//}
	//result = m_Model[5]->Initialize(m_D3D->GetDevice(), L"./data/object/rock3.obj", L"./data/texture/tree.dds");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object234.", L"Error", MB_OK);
	//	return false;
	//}

	// 일자형 맵
	result = m_Model[4]->Initialize(m_D3D->GetDevice(), L"./data/object/floorS1.obj", L"./data/texture/grass.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object222.", L"Error", MB_OK);
		return false;
	}
	m_Model[5]->SetInstanceCount(80);
	result = m_Model[5]->Initialize(m_D3D->GetDevice(), L"./data/object/fence.obj", L"./data/texture/wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[6]->Initialize(m_D3D->GetDevice(), L"./data/object/lowtree.obj", L"./data/texture/tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
		return false;
	}
	for (int i = 7; i < 27; i++)
		result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/object/coin.obj", L"./data/texture/coin.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}


	//T자형 맵
	result = m_Model[27]->Initialize(m_D3D->GetDevice(), L"./data/object/floorT1.obj", L"./data/texture/grass.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
		return false;
	}
	m_Model[28]->SetInstanceCount(236);
	result = m_Model[28]->Initialize(m_D3D->GetDevice(), L"./data/object/fence.obj", L"./data/texture/wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[29]->Initialize(m_D3D->GetDevice(), L"./data/object/rock1.obj", L"./data/texture/tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[30]->Initialize(m_D3D->GetDevice(), L"./data/object/rock2.obj", L"./data/texture/tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[31]->Initialize(m_D3D->GetDevice(), L"./data/object/rock3.obj", L"./data/texture/tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	for (int i = 32; i < 92; i++)
		result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/object/coin.obj", L"./data/texture/coin.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}

	//오른쪽코너 맵
	result = m_Model[92]->Initialize(m_D3D->GetDevice(), L"./data/object/floorR1.obj", L"./data/texture/grass.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
		return false;
	}
	m_Model[93]->SetInstanceCount(160);
	result = m_Model[93]->Initialize(m_D3D->GetDevice(), L"./data/object/fence.obj", L"./data/texture/wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[94]->Initialize(m_D3D->GetDevice(), L"./data/object/lowtree.obj", L"./data/texture/tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object4444.", L"Error", MB_OK);
		return false;
	}
	m_Model[95]->SetInstanceCount(3);
	result = m_Model[95]->Initialize(m_D3D->GetDevice(), L"./data/object/campfire.obj", L"./data/texture/campfire.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}
	for (int i = 96; i < 136; i++)
		result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/object/coin.obj", L"./data/texture/coin.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}

	//왼쪽코너 맵
	result = m_Model[136]->Initialize(m_D3D->GetDevice(), L"./data/object/floorL1.obj", L"./data/texture/grass.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object111.", L"Error", MB_OK);
		return false;
	}
	m_Model[137]->SetInstanceCount(161);
	result = m_Model[137]->Initialize(m_D3D->GetDevice(), L"./data/object/fence.obj", L"./data/texture/wood.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[138]->Initialize(m_D3D->GetDevice(), L"./data/object/rock1.obj", L"./data/texture/tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object444.", L"Error", MB_OK);
		return false;
	}
	m_Model[139]->SetInstanceCount(3);
	result = m_Model[139]->Initialize(m_D3D->GetDevice(), L"./data/object/campfire.obj", L"./data/texture/campfire.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}
	for (int i = 140; i < 180; i++)
		result = m_Model[i]->Initialize(m_D3D->GetDevice(), L"./data/object/coin.obj", L"./data/texture/coin.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}
	result = m_Model[180]->Initialize(m_D3D->GetDevice(), L"./data/object/water1.obj", L"./data/texture/water.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}

	result = m_Sky->Initialize(m_D3D->GetDevice(),L"./data/texture/skymap.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}
	m_Fire[0]->SetInstanceCount(3);
	result = m_Fire[0]->Initialize(m_D3D->GetDevice(), L"./data/object/square.obj", L"./data/texture/fire01.dds",
		L"./data/texture/noise01.dds", L"./data/texture/alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object4444.", L"Error", MB_OK);
		return false;
	}
	m_Fire[1]->SetInstanceCount(3);
	result = m_Fire[1]->Initialize(m_D3D->GetDevice(), L"./data/object/square.obj", L"./data/texture/fire01.dds",
		L"./data/texture/noise01.dds", L"./data/texture/alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object4444.", L"Error", MB_OK);
		return false;
	}


	for(int i =  0; i < INDEX_COLLIDER; i++)
	result = m_Collider[i]->Initialize(m_D3D->GetDevice(), L"./data/object/cube.obj");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object44444.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction shader object.
	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the height of the water.
	m_waterHeight = 2.0f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;


	// Create the fire shader object.
	m_FireShader = new FireShaderClass;
	if (!m_FireShader)
	{
		return false;
	}

	// Initialize the fire shader object.
	result = m_FireShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the fire shader object.
	m_FogShader = new FogShaderClass;
	if (!m_FogShader)
	{
		return false;
	}

	// Initialize the fire shader object.
	result = m_FogShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
		return false;
	}

	m_FogWaterShader = new FogWaterShaderClass;
	if (!m_FogWaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_FogWaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog water shader object.", L"Error", MB_OK);
		return false;
	}

	m_FogReflectionShader = new FogReflectionShaderClass;
	if (!m_FogReflectionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_FogReflectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog reflection shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the fog fire shader object.
	m_FogFireShader = new FogFireShaderClass;
	if (!m_FogFireShader)
	{
		return false;
	}

	// Initialize the fog fire shader object.
	result = m_FogFireShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(0.4f,0.4f, 0.4f, 1.0f);
	m_Light->SetDirection(0.1f, -1.0f, 0.1f);
	m_Light->SetSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_SkyShader = new SkyShaderClass;
	if (!m_SkyShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_SkyShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"./data/texture/star.dds");
	if (!result)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	// Create the bitmap object.

	for (int i = 0; i < INDEX_BITMAP; i++)
	{
		m_Bitmap[i] = new BitmapClass;
		if (!m_Bitmap[i])
		{
			return false;
		}
	}
	// Initialize the bitmap object.

	result = m_Bitmap[0]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/texture/gray.dds", 500, 270);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	result = m_Bitmap[1]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/texture/cursor2.dds",30, 30);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	for (int i = 0; i < INDEX_TEXT; i++)
	{
		m_Text[i] = new TextClass;
		if (!m_Text[i])
		{
			return false;
		}
	}
	// Initialize the text object.
	result = m_Text[0]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, L"./data//font/font1.txt", L"./data/texture/font1.dds");
	result = m_Text[1]->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, L"./data/font/font2.txt", L"./data/texture/font2.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the BT node
	player = new Node("Player");
	player->InitializeBT(player);

	for (int i = 0; i < 6; i++)
	{
		if (i >= 4)
		{
			startPosition[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
			startRotation[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			startPosition[i] = XMFLOAT3(0.0f, 50.0f, 0.0f);
			startRotation[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}
	move[0] = XMFLOAT3(0, 0, 40);
	move[1] = XMFLOAT3(42, 0, 38);
	move[2] = XMFLOAT3(-42, 0, 38);
	move[3] = XMFLOAT3(42, 0, 38);
	move[4] = XMFLOAT3(-42, 0, 38);

	srand(time(NULL));
	//맵 순서 지정
	for (int i = 0; i < MAP_COUNT; i++)
	{
		num[i] = rand() % 4;

		while (i >= 1)
		{
			if (num[i] == num[i - 1])
				num[i] = rand() % 4;
			else
				break;
		}
	}

	startPosition[num[MapCount]] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	startRotation[num[MapCount]] = XMFLOAT3(0.0f, 0.0f, 0.0f);

	IsMapping[num[MapCount]] = true;

	for (int i = 0; i < 6; i++)
	{
		secondRotateMatrix[i] = XMMatrixRotationRollPitchYaw(startRotation[i].x, startRotation[i].y, startRotation[i].z);
		secondTransMatrix[i] = XMMatrixTranslation(startPosition[i].x, startPosition[i].y, startPosition[i].z);
	}
	for (int i = 0; i < 6; i++)
		secondWorldMatrix[i] = secondRotateMatrix[i] * secondTransMatrix[i];

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	for (int i = 0; i < INDEX_MODEL; i++)
	{
		if (m_Model)
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}
	for (int i = 0; i < INDEX_COLLIDER; i++)
	{
		if (m_Collider)
		{
			m_Collider[i]->Shutdown();
			delete m_Collider[i];
			m_Collider[i] = 0;
		}
	}
	for (int i = 0; i < INDEX_FIRE; i++)
	{
		if (m_Fire)
		{
			m_Fire[i]->Shutdown();
			delete m_Fire[i];
			m_Fire[i] = 0;
		}
	}
	if (m_Sky)
	{
		m_Sky->Shutdown();
		delete m_Sky;
		m_Sky = 0;
	}
	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the light shader object.
	if (m_SkyShader)
	{
		m_SkyShader->Shutdown();
		delete m_SkyShader;
		m_SkyShader = 0;
	}

	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}
	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	 //Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the refraction shader object.
	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}

	// Release the fire shader object.
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	// Release the fog shader object.
	if (m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}

	// Release the fog water shader object.
	if (m_FogWaterShader)
	{
		m_FogWaterShader->Shutdown();
		delete m_FogWaterShader;
		m_FogWaterShader = 0;
	}

	// Release the fog reflection shader object.
	if (m_FogReflectionShader)
	{
		m_FogReflectionShader->Shutdown();
		delete m_FogReflectionShader;
		m_FogReflectionShader = 0;
	}

	// Release the fog fire shader object.
	if (m_FogFireShader)
	{
		m_FogFireShader->Shutdown();
		delete m_FogFireShader;
		m_FogFireShader = 0;
	}

	// Release the particle system object.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	for (int i = 0; i < INDEX_BITMAP; i++)
	{
		// Release the bitmap object.
		if (m_Bitmap[i])
		{
			m_Bitmap[i]->Shutdown();
			delete m_Bitmap[i];
			m_Bitmap[i] = 0;
		}
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the BT node.
	if (player)
	{
		delete player;
		player = 0;
	}

	return;
}

void GraphicsClass::SetCameraState()
{
	if (CameraMoveFront == true)
		m_Camera->MoveFront = true;
	else
		m_Camera->MoveFront = false;

	if (CameraMoveBack == true)
		m_Camera->MoveBack = true;
	else
		m_Camera->MoveBack = false;

	if (CameraMoveLeft == true)
		m_Camera->MoveLeft = true;
	else
		m_Camera->MoveLeft = false;

	if (CameraMoveRight == true)
		m_Camera->MoveRight = true;
	else
		m_Camera->MoveRight = false;
	if (CameraMoveUp == true)
		m_Camera->MoveUp = true;
	else
		m_Camera->MoveUp = false;
	if (CameraMoveDown == true)
		m_Camera->MoveDown = true;
	else
		m_Camera->MoveDown = false;
}
bool GraphicsClass::SetText(int fps, int cpu, float frameTime, float startTime)
{
	bool result;
	// Set the frames per second.
	result = m_Text[0]->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text[0]->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text[0]->SetCameraPos(m_Camera->GetPosition(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	ObjectCount = 0;
	for (int i = 0; i < INDEX_MODEL; i++)
		ObjectCount += m_Model[i]->GetInstanceCount();
	result = m_Text[0]->SetObject(ObjectCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	VertexTotalCount = 0;
	for (int i = 0; i < INDEX_MODEL; i++)
		VertexTotalCount += (m_Model[i]->GetVertexCount() * m_Model[i]->GetInstanceCount());
	result = m_Text[0]->SetPolygon(VertexTotalCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text[0]->SetScreenSize(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	result = m_Text[1]->SetScore(CoinCount, startTime, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	result = m_Text[1]->SetMain(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	result = m_Text[1]->SetPause(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	result = m_Text[1]->SetSuccess(CoinCount, startTime, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	result = m_Text[1]->SetFailure(CoinCount, startTime, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
}

void GraphicsClass::Restart()
{
	HRESULT result;

	//camera
	m_Camera->SetPosition(0.0f, 7.0f, 6.0f);
	m_Camera->SetRotation(XM_PI / 20, XM_PI, 0.0f);

	m_ParticleSystem->RestartParticleSystem();

	RenderCollider = false;
	RenderFog = false;

	//scene
	Success = false;
	Failure = false;
	TestMode = false;
	//car
	CameraMoveFront = false;
	CameraMoveBack = false;
	CameraMoveLeft = false;
	CameraMoveRight = false;
	CameraMoveUp = false;
	CameraMoveDown = false;

	MoveFront = false;
	MoveBack = false;
	MoveLeft = false;
	MoveRight = false;
	TurnLeft = false;
	TurnRight = false;

	CarPosition.x = 0.0f;
	CarPosition.y = 0.0f;
	CarPosition.z = 0.0f;
	CarRotation.x = 0.0f;
	CarRotation.y = 0.0f;
	CarRotation.z = 0.0f;
	JumpSpeed = 0.0f;
	Acceleration = 0.0f;

	IsTurnPlace = false;
	IsTurned = false;
	Jumping = false;
	Slide = 0.0f;
	Sliding = false;

	GetCoin = false;

	//map
	for (int i = 0; i < 4; i++)
		IsMapping[i] = false;
	for (int i = 0; i < 4; i++)
		IsTrigger[i] = false;
	for (int i = 0; i < INDEX_MODEL; i++)
		IsRanderModel[i] = true;
	MapCount = 0;
	CoinCount = 0;

	for (int i = 0; i < 6; i++)
	{
		if (i >= 4)
		{
			startPosition[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
			startRotation[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			startPosition[i] = XMFLOAT3(0.0f, 50.0f, 0.0f);
			startRotation[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}
	move[0] = XMFLOAT3(0, 0, 40);
	move[1] = XMFLOAT3(42, 0, 38);
	move[2] = XMFLOAT3(-42, 0, 38);
	move[3] = XMFLOAT3(42, 0, 38);
	move[4] = XMFLOAT3(-42, 0, 38);

	srand(time(NULL));	
	for (int i = 0; i < MAP_COUNT; i++)
	{
		num[i] = rand() % 4;

		while (i >= 1)
		{
			if (num[i] == num[i - 1])
				num[i] = rand() % 4;
			else
				break;
		}
	}

	startPosition[num[MapCount]] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	startRotation[num[MapCount]] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	IsMapping[num[MapCount]] = true;
	for (int i = 0; i < 6; i++)
	{
		secondRotateMatrix[i] = XMMatrixRotationRollPitchYaw(startRotation[i].x, startRotation[i].y, startRotation[i].z);
		secondTransMatrix[i] = XMMatrixTranslation(startPosition[i].x, startPosition[i].y, startPosition[i].z);
	}
	for (int i = 0; i < 6; i++)
		secondWorldMatrix[i] = secondRotateMatrix[i] * secondTransMatrix[i];

	player->restartBT();

	return;
}

void GraphicsClass::GameSuccess(float startTime)
{
	if (CoinCount * 100 + startTime * 100 > 70000)
	{
		Success = true;
	}

	return;
}
XMFLOAT3 GraphicsClass::GetCarRotation()
{
	return CarRotation;
}

XMFLOAT3 GraphicsClass::GetCarPosition()
{
	return CarPosition;
}

void GraphicsClass::SetMouseState(POINT mouseVariance, POINT mousePosition)
{
	m_mouseVariance = mouseVariance;
	m_mousePosition = mousePosition;

	return;
}
void GraphicsClass::SetLotation(float frameTime)
{
	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.001f * frameTime;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	return;
}
void GraphicsClass::SetWaterTranslation(float frameTime)
{
	// Update the rotation variable each frame.
	m_waterTranslation += 0.00005f * frameTime;
	if (m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	return;
}

void GraphicsClass::SetSceneState()
{
	if (TestMode == true)
		m_Camera->TestMode = true;
	else
		m_Camera->TestMode = false;

	if (Play == true)
	{
		m_Camera->Play = true;
		m_Text[1]->Play = true;
	}
	else
	{
		m_Camera->Play = false;
		m_Text[1]->Play = false;
	}

	if (Pause == true)
		m_Text[1]->Pause = true;
	else
		m_Text[1]->Pause = false;

	if (Main == true)
		m_Text[1]->Main = true;
	else
		m_Text[1]->Main = false;

	if (Success == true)
		m_Text[1]->Success = true;
	else
		m_Text[1]->Success = false;

	if (Failure == true)
		m_Text[1]->Failure = true;
	else
		m_Text[1]->Failure = false;

}

void GraphicsClass::UpdateCarRotation()
{


	if (TestMode == true)
	{
		if (TurnLeft == true && IsTurned == false)
		{
			CarRotation.y -= XM_PI / 2;
			IsTurned = true;
		}
		if (TurnRight == true && IsTurned == false)
		{
			CarRotation.y += XM_PI / 2;
			IsTurned = true;
		}
		if (TurnRight == false && TurnLeft == false)
			IsTurned = false;

	}
	else
	{
		if (Play == true)
		{
			if (TurnLeft == true && IsTurned == false && IsTurnPlace == true)
			{
				CarRotation.y -= XM_PI / 2;
				IsTurned = true;
			}
			if (TurnRight == true && IsTurned == false && IsTurnPlace == true)
			{
				CarRotation.y += XM_PI / 2;
				IsTurned = true;
			}
			if (TurnRight == false && TurnLeft == false && IsTurnPlace == false)
				IsTurned = false;
		}
	}

	return;
}


void GraphicsClass::UpdateCarPosition(float frameTime)
{
	float MoveSpeed = 0.025f;
	XMFLOAT3 tempMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 tempPosition;
	XMVECTOR tempVec;
	XMMATRIX tempMat;
	if (TestMode == true)
	{
		if (MoveFront == true)
			tempMove.z += MoveSpeed * frameTime;
		if (MoveBack == true)
			tempMove.z -= MoveSpeed * frameTime / 3;
		if (MoveLeft == true)
			tempMove.x -= MoveSpeed * frameTime / 3;
		if (MoveRight == true)
			tempMove.x += MoveSpeed * frameTime / 3;		

		tempMat = XMMatrixRotationRollPitchYaw(0.0f, CarRotation.y, 0.0f);
		tempVec = XMLoadFloat3(&tempMove);
		tempVec = XMVector3TransformCoord(tempVec, tempMat);
		XMStoreFloat3(&tempPosition, tempVec);

		CarPosition.x += tempPosition.x;
		CarPosition.y += tempPosition.y;
		CarPosition.z += tempPosition.z;

		if (MoveJump == true && JumpSpeed == 0.0f && CarPosition.y == 0.0f && Sliding == false)
		{
			Jumping = true;
			//Acceleration = 0.002f;
			//JumpSpeed = 0.2f;
			Acceleration = 0.057f;
		}
		Gravity(frameTime);
		CarPosition.y += JumpSpeed * frameTime / 15;
		if (MoveSlide == true && Jumping == false)
		{
			m_Camera->Slide = true;
			Sliding = true;
			Slide = -0.2f;
		}
		else
		{
			m_Camera->Slide = false;
			Sliding = false;
			Slide = 0.0f;
		}
	}
	else
	{
		if (Play == true)
		{
			tempMove.z += MoveSpeed * frameTime;
			if (MoveLeft == true)
				tempMove.x -= MoveSpeed * frameTime / 3;
			if (MoveRight == true)
				tempMove.x += MoveSpeed * frameTime / 3;

			tempMat = XMMatrixRotationRollPitchYaw(0.0f, CarRotation.y, 0.0f);
			tempVec = XMLoadFloat3(&tempMove);
			tempVec = XMVector3TransformCoord(tempVec, tempMat);
			XMStoreFloat3(&tempPosition, tempVec);

			CarPosition.x += tempPosition.x;
			CarPosition.y += tempPosition.y;
			CarPosition.z += tempPosition.z;




			if (MoveJump == true && JumpSpeed == 0.0f && CarPosition.y == 0.0f && Sliding == false)
			{
				Jumping = true;
				//Acceleration = 0.002f;
				//JumpSpeed = 0.2f;
				Acceleration = 0.057f;
			}
			Gravity(frameTime);
			CarPosition.y += JumpSpeed * frameTime / 15;
			if (MoveSlide == true && Jumping == false)
			{
				m_Camera->Slide = true;
				Sliding = true;
				Slide = -0.2f;
			}
			else
			{
				m_Camera->Slide = false;
				Sliding = false;
				Slide = 0.0f;
			}
		}
	}
	//else//지울예정
	//{
	//	if (MoveFront == true)
	//		tempMove.z += MoveSpeed / 3;
	//	if (MoveBack == true)
	//		tempMove.z -= MoveSpeed / 3;
	//	if (MoveLeft == true)
	//		tempMove.x -= MoveSpeed / 3;
	//	if (MoveRight == true)
	//		tempMove.x += MoveSpeed / 3;
	//}

	return;
}

void GraphicsClass::Gravity(float frameTime)
{
	JumpSpeed += Acceleration;
	if (CarPosition.y + JumpSpeed >= 0.0f && Jumping == true)
		Acceleration -= 0.0004f * frameTime;
	else
	{
		Jumping = false;
		JumpSpeed = 0.0f;
		Acceleration = 0.0f;
		CarPosition.y = 0.0f;
	}

	return;
}

void GraphicsClass::SetFog()
{
	// Set the color of the fog to grey.
	fog.color = 0.4f;

	// Set the start and end of the fog.
	fog.start = 10.0f;
	fog.end = 40.0f;

	return;
}

void GraphicsClass::SetFire(float frameTime)
{
	// Set the three scrolling speeds for the three different noise textures.
	fire.scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// Set the three scales which will be used to create the three different noise octave textures.
	fire.scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

	// Set the three different x and y distortion factors for the three different noise textures.
	fire.distortion1 = XMFLOAT2(0.1f, 0.2f);
	fire.distortion2 = XMFLOAT2(0.1f, 0.3f);
	fire.distortion3 = XMFLOAT2(0.1f, 0.1f);

	// The the scale and bias of the texture coordinate sampling perturbation.
	fire.distortionScale = 0.8f;
	fire.distortionBias = 0.5f;

	fire.frameTime += 0.001f * frameTime;
	if (fire.frameTime > 1000.0f)
	{
		fire.frameTime = 0.0f;
	}

	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, float startTime, POINT mouseVariance, POINT mousePosition)
{
	bool result;

	//안개, 불, 물에 필요한 정보
	SetFog();
	SetFire(frameTime);
	SetWaterTranslation(frameTime);

	//게임 승리 시 체크,설정
	GameSuccess(startTime);

	//마우스 위치,변화량
	SetMouseState(mouseVariance, mousePosition);

	//씬,카메라 상태 변화
	SetSceneState();
	SetCameraState();

	//행동트리 순회
	SetBTWorldInfo();
	player->initState(player, 0);
	player->searchBT(player, 0);
	GetBTInfo();

	//자동차 움직임
	UpdateCarRotation();
	UpdateCarPosition(frameTime);

	//매트리스 설정
	SetMatrix();

	//AABB 설정
	SetMaxMinBoundingBoxPoint();
	UpdateCollision();



	//회전값 설정
	SetLotation(frameTime);	
	
	result = SetText(fps, cpu, frameTime, startTime);
	if (!result)
	{
		return false;
	}

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext(), GetCarRotation(), GetCarPosition(), viewMatrix, projectionMatrix);

	// Render the graphics scene.
	result = Render(frameTime, fog, fire);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::RenderRefractionToTexture()
{
	XMFLOAT4 clipPlane;
	bool result;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 1.0f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->SetMouseVariance(m_mouseVariance);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (int i = 0; i < INDEX_MODEL; i++)
	{
		if (i == 0)
		{
			m_Model[i]->Render(m_D3D->GetDeviceContext());
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[4], viewMatrix,
				projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
		}
		else if (i >= 2 && i <= 3)
		{
			m_D3D->SetCullNone();
			m_Model[i]->Render(m_D3D->GetDeviceContext());
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[5], viewMatrix,
				projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
			m_D3D->SetCullBack();
		}
		else if (i >= 4 && i <= 6)
		{
			m_Model[i]->Render(m_D3D->GetDeviceContext());
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[0], viewMatrix,
				projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
		}
		else if (i >= 27 && i <= 31)
		{
			m_Model[i]->Render(m_D3D->GetDeviceContext());
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[1], viewMatrix,
				projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
		}
		else if (i >= 92 && i <= 95)
		{
			m_Model[i]->Render(m_D3D->GetDeviceContext());
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[2], viewMatrix,
				projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
		}
		else if (i >= 136 && i <= 139)
		{
			m_Model[i]->Render(m_D3D->GetDeviceContext());
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[3], viewMatrix,
				projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
		}
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderReflectionToTexture(float fogColor ,float fogStart, float fogEnd)
{
	XMMATRIX reflectionViewMatrix;
	bool result;
	
	XMFLOAT4 clipPlane;
	clipPlane = XMFLOAT4(0.0f, 1.0f, 0.0f, m_waterHeight - 4.4f);

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), fogColor, fogColor, fogColor, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_waterHeight+0.5f);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	if (RenderFog == false)
	{
		m_D3D->TurnZBufferOff();
		m_D3D->SetCullNone();
		m_Sky->Render(m_D3D->GetDeviceContext());
		result = m_SkyShader->Render(m_D3D->GetDeviceContext(), m_Sky->GetFaceCount(),
			skyWorldMatrix, secondWorldMatrix[4], reflectionViewMatrix, projectionMatrix, m_Sky->GetTexture());
		m_D3D->SetCullBack();
		m_D3D->TurnZBufferOn();

		for (int i = 0; i < INDEX_MODEL; i++)
		{
			if (IsRanderModel[i] == true)
			{
				if (i == 1)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[4], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
				}
				if (i >= 4 && i <= 26)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[0], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
				}
				else if (i >= 27 && i <= 91)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[1], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
				}
				else if (i >= 92 && i <= 135)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[2], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
				}
				else if (i >= 136 && i <= 179)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[3], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
				}
				else if (i == 2 || i == 3)
				{
					m_D3D->SetCullNone();
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[5], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
					m_D3D->SetCullBack();
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < INDEX_MODEL; i++)
		{
			if (IsRanderModel[i] == true)
			{
				if (i == 1)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogReflectionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[4], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, fogStart, fogEnd);
				}
				if (i >= 4 && i <= 26)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogReflectionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[0], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, fogStart, fogEnd);
				}
				else if (i >= 27 && i <= 91)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogReflectionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[1], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, fogStart, fogEnd);
				}
				else if (i >= 92 && i <= 135)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogReflectionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[2], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, fogStart, fogEnd);
				}
				else if (i >= 136 && i <= 179)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogReflectionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[3], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, fogStart, fogEnd);
				}
				else if (i == 2 || i == 3)
				{
					m_D3D->SetCullNone();
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogReflectionShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[5], reflectionViewMatrix,
						projectionMatrix, m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, fogStart, fogEnd);
					m_D3D->SetCullBack();
				}
			}			
		}
	}
	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	m_D3D->TurnOnAlphaBlending();
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetVertexCount(), m_ParticleSystem->GetInstanceCount(), particleWorldMatrix, reflectionViewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	m_D3D->TurnOffAlphaBlending();

	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderWater(float fogstart, float fogend)
{
	XMMATRIX  reflectionMatrix;
	bool result;

	// Get the camera reflection view matrix.
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model[180]->Render(m_D3D->GetDeviceContext());

	// Render the water model using the water shader.
	if (RenderFog == true)
		result = m_FogWaterShader->Render(m_D3D->GetDeviceContext(), m_Model[180]->GetVertexCount(), m_Model[180]->GetInstanceCount(), worldMatrix[180], viewMatrix,
			projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
			m_RefractionTexture->GetShaderResourceView(), m_Model[180]->GetTexture(),
			m_waterTranslation, 0.01f, fogstart, fogend);
	else
		result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_Model[180]->GetVertexCount(), m_Model[180]->GetInstanceCount(), worldMatrix[180], viewMatrix,
			projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
			m_RefractionTexture->GetShaderResourceView(), m_Model[180]->GetTexture(),
			m_waterTranslation, 0.01f);
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::SetMatrix( )
{
	//model
	scaleMatrix[0] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	scaleMatrix[1] = XMMatrixScaling(0.5f, 0.5f + Slide, 0.5f);
	scaleMatrix[2] = XMMatrixScaling(1.6f, 1.6f, 1.6f);
	scaleMatrix[5] = XMMatrixScaling(0.0108f, 0.0108f, 0.0108f);
	scaleMatrix[6] = XMMatrixScaling(1.0f, 2.0f, 1.0f);
	for (int i = 7; i < 27; i++)
		scaleMatrix[i] = XMMatrixScaling(0.002f, 0.002f, 0.002f);
	scaleMatrix[28] = XMMatrixScaling(0.0108f, 0.0108f, 0.0108f);
	scaleMatrix[29] = XMMatrixScaling(0.3f, 0.35f, 0.3f);
	scaleMatrix[30] = XMMatrixScaling(0.3f, 0.4f, 0.3f);
	scaleMatrix[31] = XMMatrixScaling(0.2f, 0.1f, 0.1f);
	for (int i = 32; i < 92; i++)
		scaleMatrix[i] = XMMatrixScaling(0.002f, 0.002f, 0.002f);
	scaleMatrix[93] = XMMatrixScaling(0.0108f, 0.0108f, 0.0108f);
	scaleMatrix[94] = XMMatrixScaling(1.0f, 2.0f, 1.0f);
	scaleMatrix[95] = XMMatrixScaling(0.06f, 0.06f, 0.06f);
	for (int i = 96; i < 136; i++)
		scaleMatrix[i] = XMMatrixScaling(0.002f, 0.002f, 0.002f);
	scaleMatrix[137] = XMMatrixScaling(0.0108f, 0.0108f, 0.0108f);
	scaleMatrix[138] = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	scaleMatrix[139] = XMMatrixScaling(0.06f, 0.06f, 0.06f);
	for (int i = 140; i < 180; i++)
		scaleMatrix[i] = XMMatrixScaling(0.002f, 0.002f, 0.002f);
	scaleMatrix[180] = XMMatrixScaling(0.6f, 0.5f, 0.6f);

	rotateMatrix[1] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI + CarRotation.y, 0.0f);
	rotateMatrix[2] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI, 0.0f);
	float rot = 0;
	for (int i = 7; i < 27; i++)
	{
		rotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, rotation + rot, 0.0f);
		rot += XM_PI / 10;
	}
	rotateMatrix[31] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI / 2, 0.0f);
	for (int i = 32; i < 92; i++)
	{
		rotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, rotation + rot, 0.0f);
		rot += XM_PI / 10;
	}
	for (int i = 96; i < 136; i++)
	{
		rotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, rotation + rot, 0.0f);
		rot += XM_PI / 10;
	}
	for (int i = 140; i < 180; i++)
	{
		rotateMatrix[i] = XMMatrixRotationRollPitchYaw(0.0f, rotation + rot, 0.0f);
		rot += XM_PI / 10;
	}
	//행동트리 좌우이동
	XMFLOAT3 tempMove = XMFLOAT3(player->Info.LRposition, 0.0f, 0.0f);
	XMFLOAT3 tempPosition;
	XMVECTOR tempVec;
	XMMATRIX tempMat;
	tempMat = XMMatrixRotationRollPitchYaw(0.0f, CarRotation.y, 0.0f);
	tempVec = XMLoadFloat3(&tempMove);
	tempVec = XMVector3TransformCoord(tempVec, tempMat);
	XMStoreFloat3(&tempPosition, tempVec);

	transMatrix[1] = XMMatrixTranslation(0.0f + CarPosition.x+ tempPosition.x, 3.0f + CarPosition.y, -3.0f + CarPosition.z+ tempPosition.z);
	transMatrix[2] = XMMatrixTranslation(1.9f, 3.0f, 0.0f);
	transMatrix[3] = XMMatrixTranslation(0.0f, 0.0f, -4.5f);
	transMatrix[6] = XMMatrixTranslation(0.5f, -3.4f, 20.0f);
	float gap = 0;
	float jumpY = 0;
	float v = 0.0f;
	float a = 0.50f;
	float g = 0.13f;

	for (int i = 7; i < 27; i++)
	{
		if (i >= 12)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		transMatrix[i] = XMMatrixTranslation(1.0f, 3.5f+jumpY, 10.0f + gap);
		gap += 1;
	}
	transMatrix[27] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	transMatrix[29] = XMMatrixTranslation(-12.0f, 3.0f, 20.0f);
	transMatrix[30] = XMMatrixTranslation(-26.0f, 1.0f, 36.5f);
	transMatrix[31] = XMMatrixTranslation(24.0f, 1.8f, 38.5f);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 32; i < 52; i++)
	{
		transMatrix[i] = XMMatrixTranslation(0.0f, 3.5f + jumpY, 10.0f + gap);
		gap += 1;
	}

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 52; i < 72; i++)
	{
		if (i >= 58)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		transMatrix[i] = XMMatrixTranslation(13.0f + gap, 3.5f + jumpY, 38.0f);
		gap += 1;
	}

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 72; i < 92; i++)
	{
		transMatrix[i] = XMMatrixTranslation(-11.0f - gap, 3.5f + jumpY, 39.0f);
		gap += 1;
	}
	transMatrix[92] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	transMatrix[94] = XMMatrixTranslation(-0.5f, -3.4f, 20.0f);
	transMatrix[95] = XMMatrixTranslation(18.8f, 3.0f, 37.0f);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 96; i < 116; i++)
	{
		if (i >= 102)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		transMatrix[i] = XMMatrixTranslation(-1.0f, 3.5f + jumpY, 10.0f + gap);
		gap += 1;
	}

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 116; i < 136; i++)
	{
		if (i >= 121)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		transMatrix[i] = XMMatrixTranslation(9.0f + gap, 3.5f + jumpY, 38.0f);
		gap += 1;
	}
	transMatrix[136] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	transMatrix[138] = XMMatrixTranslation(-12.0f, 3.3f, 20.0f);
	transMatrix[139] = XMMatrixTranslation(-18.0f, 3.0f, 37.0f);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 140; i < 160; i++)
	{
		transMatrix[i] = XMMatrixTranslation(0.0f, 3.5f + jumpY, 10.0f + gap);
		gap += 1;
	}

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	gap = 0;
	for (int i = 160; i < 180; i++)
	{
		if (i >= 168)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		transMatrix[i] = XMMatrixTranslation(-5.0f - gap, 3.5f + jumpY, 38.0f);
		gap += 1;
	}
	transMatrix[180] = XMMatrixTranslation(0.0f + CarPosition.x, m_waterHeight, 0.0f + CarPosition.z);

	//collider
	colliderScaleMatrix[0] = XMMatrixScaling(1.0f, 0.9f + Slide * 2, 1.8f);
	colliderScaleMatrix[1] = XMMatrixScaling(0.2f, 1.0f, 40.0f);	//울타리
	colliderScaleMatrix[2] = XMMatrixScaling(0.2f, 1.0f, 40.0f);	//울타리
	colliderScaleMatrix[3] = XMMatrixScaling(1.0f, 3.0f, 1.0f);		//나무
	colliderScaleMatrix[4] = XMMatrixScaling(1.0f, 1.8f, 1.0f);		//나무
	colliderScaleMatrix[5] = XMMatrixScaling(1.5f, 0.8f, 1.0f);		//나무
	colliderScaleMatrix[6] = XMMatrixScaling(0.6f, 0.9f, 1.0f);		//나무
	for (int i = 7; i < 27; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	colliderScaleMatrix[27] = XMMatrixScaling(4.0f, 4.0f, 0.2f);	//nextSpawn
	colliderScaleMatrix[28] = XMMatrixScaling(8.0f, 4.0f, 0.2f);	//end
	colliderScaleMatrix[29] = XMMatrixScaling(0.2f, 1.0f, 36.0f);	//울타리
	colliderScaleMatrix[30] = XMMatrixScaling(0.2f, 1.0f, 36.0f);	//울타리
	colliderScaleMatrix[31] = XMMatrixScaling(40.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[32] = XMMatrixScaling(40.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[33] = XMMatrixScaling(84.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[34] = XMMatrixScaling(1.0f, 1.5f, 1.0f);	//굴다리
	colliderScaleMatrix[35] = XMMatrixScaling(4.0f, 2.2f, 2.0f);	//굴다리
	colliderScaleMatrix[36] = XMMatrixScaling(1.0f, 1.5f, 1.0f);	//굴다리
	colliderScaleMatrix[37] = XMMatrixScaling(1.3f, 1.0f, 4.0f);	//오른쪽돌
	colliderScaleMatrix[38] = XMMatrixScaling(2.0f, 4.0f, 1.5f);	//왼쪽돌
	colliderScaleMatrix[39] = XMMatrixScaling(3.5f, 1.0f, 1.0f);	//왼쪽돌
	colliderScaleMatrix[40] = XMMatrixScaling(2.5f, 1.2f, 1.0f);	//왼쪽돌
	colliderScaleMatrix[41] = XMMatrixScaling(2.0f, 3.0f, 0.4f);	//왼쪽돌
	for (int i = 42; i < 62; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	for (int i = 62; i < 82; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	for (int i = 82; i < 102; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	colliderScaleMatrix[102] = XMMatrixScaling(4.0f, 4.0f, 0.2f);	//nextSpawn
	colliderScaleMatrix[103] = XMMatrixScaling(4.0f, 4.0f, 0.2f);	//nextSpawn
	colliderScaleMatrix[104] = XMMatrixScaling(8.0f, 4.0f, 0.2f);	//end
	colliderScaleMatrix[105] = XMMatrixScaling(8.0f, 4.0f, 0.2f);	//end
	colliderScaleMatrix[106] = XMMatrixScaling(2.0f, 4.0f, 0.6f);	//turn
	colliderScaleMatrix[107] = XMMatrixScaling(0.2f, 1.0f, 36.0f);	//울타리
	colliderScaleMatrix[108] = XMMatrixScaling(0.2f, 1.0f, 40.0f);	//울타리
	colliderScaleMatrix[109] = XMMatrixScaling(44.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[110] = XMMatrixScaling(40.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[111] = XMMatrixScaling(1.0f, 1.4f, 1.2f);		//나무
	colliderScaleMatrix[112] = XMMatrixScaling(1.0f, 0.9f, 1.2f);		//나무
	colliderScaleMatrix[113] = XMMatrixScaling(1.5f, 0.8f, 1.0f);		//나무
	colliderScaleMatrix[114] = XMMatrixScaling(1.5f, 2.0f, 1.5f);		//나무
	colliderScaleMatrix[115] = XMMatrixScaling(1.0f, 1.0f, 4.0f);		//불
	for (int i = 116; i < 136; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	for (int i = 136; i < 156; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	colliderScaleMatrix[156] = XMMatrixScaling(4.0f, 4.0f, 0.2f);	//nextSpawn
	colliderScaleMatrix[157] = XMMatrixScaling(8.0f, 4.0f, 0.2f);	//end
	colliderScaleMatrix[158] = XMMatrixScaling(2.0f, 4.0f, 0.6f);	//turn
	colliderScaleMatrix[159] = XMMatrixScaling(0.2f, 1.0f, 40.0f);	//울타리
	colliderScaleMatrix[160] = XMMatrixScaling(0.2f, 1.0f, 36.0f);	//울타리
	colliderScaleMatrix[161] = XMMatrixScaling(44.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[162] = XMMatrixScaling(40.0f, 1.0f, 0.2f);	//울타리
	colliderScaleMatrix[163] = XMMatrixScaling(1.0f, 1.5f, 1.0f);		//굴다리
	colliderScaleMatrix[164] = XMMatrixScaling(4.0f, 2.2f, 2.0f);		//굴다리
	colliderScaleMatrix[165] = XMMatrixScaling(1.0f, 1.5f, 1.0f);		//굴다리
	colliderScaleMatrix[166] = XMMatrixScaling(1.0f, 1.0f, 4.0f);		//불
	for (int i = 167; i < 187; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	for (int i = 187; i < 207; i++)
		colliderScaleMatrix[i] = XMMatrixScaling(0.5f, 0.5f, 0.5f);	//코인
	colliderScaleMatrix[207] = XMMatrixScaling(4.0f, 4.0f, 0.2f);	//nextSpawn
	colliderScaleMatrix[208] = XMMatrixScaling(8.0f, 4.0f, 0.2f);	//end
	colliderScaleMatrix[209] = XMMatrixScaling(2.0f, 4.0f, 0.6f);	//turn
	colliderScaleMatrix[210] = XMMatrixScaling(8.0f, 4.0f, 0.2f);

	colliderRotateMatrix[0] = XMMatrixRotationRollPitchYaw(0.0f, CarRotation.y, 0.0f);
	colliderRotateMatrix[102] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI / 2, 0.0f);
	colliderRotateMatrix[103] = XMMatrixRotationRollPitchYaw(0.0f, -XM_PI / 2, 0.0f);
	colliderRotateMatrix[104] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI / 2, 0.0f);
	colliderRotateMatrix[105] = XMMatrixRotationRollPitchYaw(0.0f, -XM_PI / 2, 0.0f);
	colliderRotateMatrix[156] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	colliderRotateMatrix[157] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI / 2, 0.0f);
	colliderRotateMatrix[207] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	colliderRotateMatrix[208] = XMMatrixRotationRollPitchYaw(0.0f, -XM_PI / 2, 0.0f);
	colliderRotateMatrix[210] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

	colliderTransMatrix[0] = XMMatrixTranslation(-0.0f + CarPosition.x+ tempPosition.x, 3.5f + CarPosition.y + Slide, -3.0f + CarPosition.z+ tempPosition.z);
	colliderTransMatrix[1] = XMMatrixTranslation(1.9f, 3.4f, 20.0f);
	colliderTransMatrix[2] = XMMatrixTranslation(-1.9f, 3.4f, 20.0f);
	colliderTransMatrix[3] = XMMatrixTranslation(-1.6f, 4.4f, 19.1f);
	colliderTransMatrix[4] = XMMatrixTranslation(-0.8f, 3.8f, 19.5f);
	colliderTransMatrix[5] = XMMatrixTranslation(0.5f, 3.3f, 20.0f);
	colliderTransMatrix[6] = XMMatrixTranslation(1.5f, 3.4f, 20.6f);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	for (int i = 7; i < 27; i++)
	{
		if (i >= 12)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		colliderTransMatrix[i] = XMMatrixTranslation(1.0f, 3.5f + jumpY, 3.0f + i);
	}
	colliderTransMatrix[27] = XMMatrixTranslation(0.0f, 5.0f, 10.0f);
	colliderTransMatrix[28] = XMMatrixTranslation(0.0f, 5.0f, 45.0f);
	colliderTransMatrix[29] = XMMatrixTranslation(1.9f, 3.4f, 18.0f);
	colliderTransMatrix[30] = XMMatrixTranslation(-1.9f, 3.4f, 18.0f);
	colliderTransMatrix[31] = XMMatrixTranslation(22.0f, 3.4f, 36.1f);
	colliderTransMatrix[32] = XMMatrixTranslation(-22.0f, 3.4f, 36.1f);
	colliderTransMatrix[33] = XMMatrixTranslation(0.0f, 3.4f, 39.9f);
	colliderTransMatrix[34] = XMMatrixTranslation(-1.6f, 3.6f, 20.0f);
	colliderTransMatrix[35] = XMMatrixTranslation(0.0f, 5.0f, 20.0f);
	colliderTransMatrix[36] = XMMatrixTranslation(1.3f, 3.6f, 20.0f);
	colliderTransMatrix[37] = XMMatrixTranslation(23.5f, 3.4f, 38.0f);
	colliderTransMatrix[38] = XMMatrixTranslation(-26.0f, 4.4f, 36.0f);
	colliderTransMatrix[39] = XMMatrixTranslation(-26.0f, 3.4f, 36.8f);
	colliderTransMatrix[40] = XMMatrixTranslation(-26.0f, 3.5f, 37.2f);
	colliderTransMatrix[41] = XMMatrixTranslation(-26.0f, 3.5f, 36.9f);
	for (int i = 42; i < 62; i++)
		colliderTransMatrix[i] = XMMatrixTranslation(0.0f, 3.5f, -32.0f + i);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	for (int i = 62; i < 82; i++)
	{
		if (i >= 68)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		colliderTransMatrix[i] = XMMatrixTranslation(-49.0f + i, 3.5f + jumpY, 38.0f);
	}
	for (int i = 82; i < 102; i++)
		colliderTransMatrix[i] = XMMatrixTranslation(71.0f - i, 3.5f, 39.0f);
	colliderTransMatrix[102] = XMMatrixTranslation(2.0f, 5.0f, 38.0f);
	colliderTransMatrix[103] = XMMatrixTranslation(-2.0f, 5.0f, 38.0f);
	colliderTransMatrix[104] = XMMatrixTranslation(47.0f, 5.0f, 38.0f);
	colliderTransMatrix[105] = XMMatrixTranslation(-47.0f, 5.0f, 38.0f);
	colliderTransMatrix[106] = XMMatrixTranslation(0.0f, 5.0f, 38.0f);
	colliderTransMatrix[107] = XMMatrixTranslation(1.9f, 3.4f, 18.0f);
	colliderTransMatrix[108] = XMMatrixTranslation(-1.9f, 3.4f, 20.0f);
	colliderTransMatrix[109] = XMMatrixTranslation(20.0f, 3.4f, 39.9f);
	colliderTransMatrix[110] = XMMatrixTranslation(22.0f, 3.4f, 36.1f);
	colliderTransMatrix[111] = XMMatrixTranslation(-1.8f, 3.6f, 19.5f);
	colliderTransMatrix[112] = XMMatrixTranslation(-0.8f, 3.2f, 19.8f);
	colliderTransMatrix[113] = XMMatrixTranslation(0.5f, 3.3f, 20.4f);
	colliderTransMatrix[114] = XMMatrixTranslation(1.5f, 3.9f, 21.2f);
	colliderTransMatrix[115] = XMMatrixTranslation(18.8f, 3.4f, 38.0f);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	for (int i = 116; i < 136; i++)
	{
		if (i >= 122)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		colliderTransMatrix[i] = XMMatrixTranslation(-1.0f, 3.5f + jumpY, -106.0f + i);
	}

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	for (int i = 136; i < 156; i++)
	{
		if (i >= 141)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		colliderTransMatrix[i] = XMMatrixTranslation(-127.0f + i, 3.5f + jumpY, 38.0f);
	}
	colliderTransMatrix[156] = XMMatrixTranslation(-0.0f, 5.0f, 10.0f);
	colliderTransMatrix[157] = XMMatrixTranslation(47.0f, 5.0f, 38.0f);
	colliderTransMatrix[158] = XMMatrixTranslation(0.0f, 5.0f, 38.0f);
	colliderTransMatrix[159] = XMMatrixTranslation(1.9f, 3.4f, 20.0f);
	colliderTransMatrix[160] = XMMatrixTranslation(-1.9f, 3.4f, 18.0f);
	colliderTransMatrix[161] = XMMatrixTranslation(-20.0f, 3.4f, 39.9f);
	colliderTransMatrix[162] = XMMatrixTranslation(-22.0f, 3.4f, 36.1f);
	colliderTransMatrix[163] = XMMatrixTranslation(-1.9f, 3.6f, 20.0f);
	colliderTransMatrix[164] = XMMatrixTranslation(0.0f, 5.0f, 20.0f);
	colliderTransMatrix[165] = XMMatrixTranslation(1.5f, 3.6f, 20.0f);
	colliderTransMatrix[166] = XMMatrixTranslation(-18.0f, 3.4f, 38.0f);
	for (int i = 167; i < 187; i++)
		colliderTransMatrix[i] = XMMatrixTranslation(0.0f, 3.5f, -157.0f + i);

	jumpY = 0;
	v = 0.0f;
	a = 0.50f;
	for (int i = 187; i < 207; i++)
	{
		if (i >= 195)
		{
			a -= g;
			v += a;
			jumpY += v;
			if (jumpY < 0)
				jumpY = 0;
		}
		colliderTransMatrix[i] = XMMatrixTranslation(182.0f - i, 3.5f + jumpY, 38.0f);
	}
	colliderTransMatrix[207] = XMMatrixTranslation(0.0f, 5.0f, 10.0f);
	colliderTransMatrix[208] = XMMatrixTranslation(-47.0f, 5.0f, 38.0f);
	colliderTransMatrix[209] = XMMatrixTranslation(0.0f, 5.0f, 38.0f);
	colliderTransMatrix[210] = XMMatrixTranslation(0.0f, 5.0f, 5.0f);

	//fire
	fireScaleMatrix[0] = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	fireScaleMatrix[1] = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	fireRotateMatrix[0] = XMMatrixRotationRollPitchYaw(0.0f, XM_PI / 2, 0.0f);
	fireRotateMatrix[1] = XMMatrixRotationRollPitchYaw(0.0f, -XM_PI / 2, 0.0f);

	fireTransMatrix[0] = XMMatrixTranslation(19.0f, 3.5f, 36.9f);
	fireTransMatrix[1] = XMMatrixTranslation(-18.2f, 3.5f, 37.0f);

	//sky
	skyScaleMatrix = XMMatrixScaling(1000.0f, 1000.0f, 1000.0f);
	skyTransMatrix = XMMatrixTranslation(0.0f + CarPosition.x, 3.0f + CarPosition.y, -3.0f + CarPosition.z);

	//particle
	XMFLOAT3 particleRot = m_Camera->GetRotation();
	particleRotateMatrix = XMMatrixRotationRollPitchYaw(particleRot.x, particleRot.y, particleRot.z);
	particleTransMatrix = XMMatrixTranslation(tempPosition.x, 0.0f, tempPosition.z);
	

	//multiply S, R, T
	for (int i = 0; i < INDEX_MODEL; i++)
		worldMatrix[i] = scaleMatrix[i] * rotateMatrix[i] * transMatrix[i];
	for (int i = 0; i < INDEX_COLLIDER; i++)
		colliderWorldMatrix[i] = colliderScaleMatrix[i] * colliderRotateMatrix[i] * colliderTransMatrix[i];
	for (int i = 0; i < INDEX_FIRE; i++)
		fireWorldMatrix[i] = fireScaleMatrix[i] * fireRotateMatrix[i] * fireTransMatrix[i];
	skyWorldMatrix = skyScaleMatrix * skyRotateMatrix * skyTransMatrix;
	particleWorldMatrix = particleScaleMatrix * particleRotateMatrix * particleTransMatrix;




	return;
}
void GraphicsClass::SetMaxMinBoundingBoxPoint()
{
	for (int i = 0; i < INDEX_COLLIDER; i++)
	{
		if (i >= 1 && i <= 28)
			m_Collider[i]->SetMaxMinWorldPoint(colliderWorldMatrix[i], secondWorldMatrix[0]);
		else if (i >= 29 && i <= 106)
			m_Collider[i]->SetMaxMinWorldPoint(colliderWorldMatrix[i], secondWorldMatrix[1]);
		else if (i >= 107 && i <= 158)
			m_Collider[i]->SetMaxMinWorldPoint(colliderWorldMatrix[i], secondWorldMatrix[2]);
		else if (i >= 159 && i <= 209)
			m_Collider[i]->SetMaxMinWorldPoint(colliderWorldMatrix[i], secondWorldMatrix[3]);
		else
			m_Collider[i]->SetMaxMinWorldPoint(colliderWorldMatrix[i], secondWorldMatrix[4]);
	}

	return;
}

void GraphicsClass::UpdateCollision()
{
	for (int i = 0; i < INDEX_COLLIDER; i++)
	{
		if (m_Collider[i]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true)
		{
			//end
			if (i == 28 && IsMapping[0] == true)
			{
				secondWorldMatrix[0] = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
				IsMapping[0] = false;
				IsTrigger[0] = false;
				for (int coinNum = 7; coinNum <= 26; coinNum++)//20
				{
					IsRanderModel[coinNum] = true;
				}
			}
			else if ((i == 104 || i == 105) && IsMapping[1] == true)
			{
				secondWorldMatrix[1] = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
				IsMapping[1] = false;
				IsTrigger[1] = false;
				for (int coinNum = 32; coinNum <= 91; coinNum++)//20
				{
					IsRanderModel[coinNum] = true;
				}
			}
			else if (i == 157 && IsMapping[2] == true)
			{
				secondWorldMatrix[2] = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
				IsMapping[2] = false;
				IsTrigger[2] = false;
				for (int coinNum = 96; coinNum <= 135; coinNum++)//20
				{
					IsRanderModel[coinNum] = true;
				}
			}
			else if (i == 208 && IsMapping[3] == true)
			{
				secondWorldMatrix[3] = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
				IsMapping[3] = false;
				IsTrigger[3] = false;
				for (int coinNum = 140; coinNum <= 179; coinNum++)//20
				{
					IsRanderModel[coinNum] = true;
				}
			}
			else if (i == 210)
			{
				secondWorldMatrix[5] = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
			}
			//next
			else if (i == 27 && IsTrigger[0] == false)
			{
				if (MapCount == 0)
				{
					tempMove = move[num[MapCount]];
				}
				else
				{
					//이전 endRotate 넣기
					tempMatrix = XMMatrixRotationRollPitchYaw(endRotation[num[MapCount - 1]].x, endRotation[num[MapCount - 1]].y, endRotation[num[MapCount - 1]].z);
					//현재 move 변환
					tempVector = XMLoadFloat3(&move[num[MapCount]]);
					tempVector = XMVector3TransformCoord(tempVector, tempMatrix);
					XMStoreFloat3(&tempMove, tempVector);
				}
				//현재 end값 설정
				endPosition[num[MapCount]] = XMFLOAT3(startPosition[num[MapCount]].x + tempMove.x, startPosition[num[MapCount]].y + tempMove.y, startPosition[num[MapCount]].z + tempMove.z);
				endRotation[num[MapCount]] = XMFLOAT3(startRotation[num[MapCount]].x, startRotation[num[MapCount]].y, startRotation[num[MapCount]].z);

				//다음 start값에 현재end넣기
				startPosition[num[MapCount + 1]] = endPosition[num[MapCount]];
				startRotation[num[MapCount + 1]] = endRotation[num[MapCount]];

				IsTrigger[num[MapCount]] = true;
				IsMapping[num[MapCount + 1]] = true;
				MapCount++;
			}
			else if (i == 102 && IsTrigger[1] == false)
			{
				if (MapCount == 0)
				{
					tempMove = move[num[MapCount]];
				}
				else
				{
					//이전 endRotate 넣기
					tempMatrix = XMMatrixRotationRollPitchYaw(endRotation[num[MapCount - 1]].x, endRotation[num[MapCount - 1]].y, endRotation[num[MapCount - 1]].z);
					//현재 move 변환
					tempVector = XMLoadFloat3(&move[num[MapCount]]);
					tempVector = XMVector3TransformCoord(tempVector, tempMatrix);
					XMStoreFloat3(&tempMove, tempVector);
				}
				//현재 end값 설정
				endPosition[num[MapCount]] = XMFLOAT3(startPosition[num[MapCount]].x + tempMove.x, startPosition[num[MapCount]].y + tempMove.y, startPosition[num[MapCount]].z + tempMove.z);
				endRotation[num[MapCount]] = XMFLOAT3(startRotation[num[MapCount]].x, startRotation[num[MapCount]].y + (XM_PI / 2), startRotation[num[MapCount]].z);

				//다음 start값에 현재end넣기
				startPosition[num[MapCount + 1]] = endPosition[num[MapCount]];
				startRotation[num[MapCount + 1]] = endRotation[num[MapCount]];

				IsTrigger[num[MapCount]] = true;
				IsMapping[num[MapCount + 1]] = true;
				MapCount++;
			}
			else if (i == 103 && IsTrigger[1] == false)
			{
				if (MapCount == 0)
				{
					tempMove = move[num[MapCount] + 1];
				}
				else
				{
					//이전 endRotate 넣기
					tempMatrix = XMMatrixRotationRollPitchYaw(endRotation[num[MapCount - 1]].x, endRotation[num[MapCount - 1]].y, endRotation[num[MapCount - 1]].z);
					//현재 move 변환
					tempVector = XMLoadFloat3(&move[num[MapCount] + 1]);
					tempVector = XMVector3TransformCoord(tempVector, tempMatrix);
					XMStoreFloat3(&tempMove, tempVector);
				}
				//현재 end값 설정
				endPosition[num[MapCount]] = XMFLOAT3(startPosition[num[MapCount]].x + tempMove.x, startPosition[num[MapCount]].y + tempMove.y, startPosition[num[MapCount]].z + tempMove.z);
				endRotation[num[MapCount]] = XMFLOAT3(startRotation[num[MapCount]].x, startRotation[num[MapCount]].y - (XM_PI / 2), startRotation[num[MapCount]].z);

				//다음 start값에 현재end넣기
				startPosition[num[MapCount + 1]] = endPosition[num[MapCount]];
				startRotation[num[MapCount + 1]] = endRotation[num[MapCount]];

				IsTrigger[num[MapCount]] = true;
				IsMapping[num[MapCount + 1]] = true;
				MapCount++;
			}
			else if (i == 156 && IsTrigger[2] == false)
			{
				if (MapCount == 0)
				{
					tempMove = move[num[MapCount] + 1];
				}
				else
				{
					//이전 endRotate 넣기
					tempMatrix = XMMatrixRotationRollPitchYaw(endRotation[num[MapCount - 1]].x, endRotation[num[MapCount - 1]].y, endRotation[num[MapCount - 1]].z);
					//현재 move 변환
					tempVector = XMLoadFloat3(&move[num[MapCount] + 1]);
					tempVector = XMVector3TransformCoord(tempVector, tempMatrix);
					XMStoreFloat3(&tempMove, tempVector);
				}
				//현재 end값 설정
				endPosition[num[MapCount]] = XMFLOAT3(startPosition[num[MapCount]].x + tempMove.x, startPosition[num[MapCount]].y + tempMove.y, startPosition[num[MapCount]].z + tempMove.z);
				endRotation[num[MapCount]] = XMFLOAT3(startRotation[num[MapCount]].x, startRotation[num[MapCount]].y + (XM_PI / 2), startRotation[num[MapCount]].z);

				//다음 start값에 현재end넣기
				startPosition[num[MapCount + 1]] = endPosition[num[MapCount]];
				startRotation[num[MapCount + 1]] = endRotation[num[MapCount]];

				IsTrigger[num[MapCount]] = true;
				IsMapping[num[MapCount + 1]] = true;
				MapCount++;
			}
			else if (i == 207 && IsTrigger[3] == false)
			{
				if (MapCount == 0)
				{
					tempMove = move[num[MapCount] + 1];
				}
				else
				{
					//이전 endRotate 넣기
					tempMatrix = XMMatrixRotationRollPitchYaw(endRotation[num[MapCount - 1]].x, endRotation[num[MapCount - 1]].y, endRotation[num[MapCount - 1]].z);
					//현재 move 변환
					tempVector = XMLoadFloat3(&move[num[MapCount] + 1]);
					tempVector = XMVector3TransformCoord(tempVector, tempMatrix);
					XMStoreFloat3(&tempMove, tempVector);
				}
				//현재 end값 설정
				endPosition[num[MapCount]] = XMFLOAT3(startPosition[num[MapCount]].x + tempMove.x, startPosition[num[MapCount]].y + tempMove.y, startPosition[num[MapCount]].z + tempMove.z);
				endRotation[num[MapCount]] = XMFLOAT3(startRotation[num[MapCount]].x, startRotation[num[MapCount]].y - (XM_PI / 2), startRotation[num[MapCount]].z);

				//다음 start값에 현재end넣기
				startPosition[num[MapCount + 1]] = endPosition[num[MapCount]];
				startRotation[num[MapCount + 1]] = endRotation[num[MapCount]];

				IsTrigger[num[MapCount]] = true;
				IsMapping[num[MapCount + 1]] = true;
				MapCount++;
			}

			//coin
			if (i >= 7 && i <= 26 && IsRanderModel[i] == true)//20
			{
				CoinCount++;
				IsRanderModel[i] = false;
				GetCoin = true;
				player->Info.CoinStack+=1;
			}
			else if (i >= 42 && i <= 101 && IsRanderModel[i - 10] == true)//60
			{
				CoinCount++;
				IsRanderModel[i - 10] = false;
				GetCoin = true;
				player->Info.CoinStack += 1;
			}
			else if (i >= 116 && i <= 155 && IsRanderModel[i - 20] == true)//40
			{
				CoinCount++;
				IsRanderModel[i - 20] = false;
				GetCoin = true;
				player->Info.CoinStack += 1;
			}
			else if (i >= 167 && i <= 206 && IsRanderModel[i - 27] == true)//40
			{
				CoinCount++;
				IsRanderModel[i - 27] = false;
				GetCoin = true;
				player->Info.CoinStack += 1;
			}
			else
				GetCoin = false;
			//turnPlace
			if (i == 106 || i == 158 || i == 209)
				IsTurnPlace = true;
			else
				IsTurnPlace = false;
			if ((i >= 1 && i <= 6)|| (i >= 29 && i <= 41)||
				(i >= 107 && i <= 115)|| (i >= 159 && i <= 166))
				Failure = true;

			m_Collider[i]->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
			m_Collider[i]->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	}

	//맵 이동 업데이트
	for (int i = 0; i < 5; i++)
	{
		secondRotateMatrix[i] = XMMatrixRotationRollPitchYaw(startRotation[i].x, startRotation[i].y, startRotation[i].z);
		secondTransMatrix[i] = XMMatrixTranslation(startPosition[i].x, startPosition[i].y, startPosition[i].z);
		if (IsMapping[i] == true)
			secondWorldMatrix[i] = secondRotateMatrix[i] * secondTransMatrix[i];
	}

	return;
}
void GraphicsClass::SetBTWorldInfo()
{
	if (m_Collider[106]->SamePosition(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true)
	{
		player->Info.IsTurnPlace = true;
		player->Info.TTurnPlace = true;
	}
	else if (m_Collider[158]->SamePosition(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true)
	{
		player->Info.IsTurnPlace = true;
		player->Info.RightTurnPlace = true;
	}
	else if (m_Collider[209]->SamePosition(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true)
	{
		player->Info.IsTurnPlace = true;
		player->Info.LeftTurnPlace = true;
	}
	else
	{
		player->Info.IsTurnPlace = false;
		player->Info.TTurnPlace = false;
		player->Info.RightTurnPlace = false;
		player->Info.LeftTurnPlace = false;

		player->Info.IsTurned = false;
	}	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_Collider[38]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[39]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[40]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[41]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true)
	{
		//cout << "MoveRock\n";
		player->Info.MoveModel = PositionType::Right;
	}
	else if (m_Collider[34]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[35]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[36]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[163]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[164]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[165]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true)
	{
		//cout << "SlideRock\n";
		player->Info.MoveModel = PositionType::Center;
	}
	else if (m_Collider[37]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true)
	{
		//cout << "JumpRock\n";
		player->Info.MoveModel = PositionType::Center;
	}
	else if(m_Collider[111]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||//왼쪽
		m_Collider[112]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[113]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[114]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true)
	{
		//cout << "JumpTree\n";
		player->Info.MoveModel = PositionType::Left;
	}
	else if (m_Collider[3]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||//오른쪽
		m_Collider[4]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[5]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[6]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true)
	{
		//cout << "JumpTree\n";
		player->Info.MoveModel = PositionType::Right;
	}
	else if (m_Collider[115]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true ||
		m_Collider[166]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 10) == true)
	{
		//cout << "JumpFire\n";
		player->Info.MoveModel = PositionType::Center;
	}
	else
	{
		player->Info.MoveModel = PositionType::Null;
	}

	if (m_Collider[106]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[158]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[209]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true)
	{
		//cout << "Corner\n";
		player->Info.MoveModel = PositionType::Center;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_Collider[38]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(),5) == true ||
		m_Collider[39]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[40]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[41]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true)
	{
		//cout << "MoveRock\n";
		player->Info.JumpModel = JumpType::Stop;
	}
	else if (m_Collider[34]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[35]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[36]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[163]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[164]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[165]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true)
	{
		//cout << "SlideRock\n";
		player->Info.JumpModel = JumpType::Slide;
	}
	else if (m_Collider[37]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true)
	{
		//cout << "JumpRock\n";
		player->Info.JumpModel = JumpType::Jump;
	}
	else if (m_Collider[3]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||//오른쪽
		m_Collider[4]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[5]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[6]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[111]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||//왼쪽
		m_Collider[112]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[113]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[114]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true)
	{
		//cout << "JumpTree\n";
		player->Info.JumpModel = JumpType::Jump;
	}
	else if (m_Collider[115]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true ||
		m_Collider[166]->Sensation(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint(), 5) == true)
	{
		//cout << "JumpFire\n";
		player->Info.JumpModel = JumpType::Jump;
	}
	else
	{
		player->Info.JumpModel = JumpType::Null;

		player->Info.IsJumped = false;
	}


	if (m_Collider[28]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true ||
		m_Collider[104]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true||
		m_Collider[105]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true ||
		m_Collider[106]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true||
		m_Collider[157]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true ||
		m_Collider[158]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true ||
		m_Collider[208]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true ||
		m_Collider[209]->Collision(m_Collider[0]->GetMaxPoint(), m_Collider[0]->GetMinPoint()) == true)
	{
		player->Info.CoinStack = 0;
	}

	return;
}

void GraphicsClass::GetBTInfo()
{
	TurnLeft = player->Info.TurnLeft;
	TurnRight = player->Info.TurnRight;
	MoveJump = player->Info.MoveJump;
	MoveSlide = player->Info.MoveSlide;

	player->Info.TurnLeft = false;
	player->Info.TurnRight = false;
	player->Info.MoveJump = false;
	player->Info.MoveSlide = false;

	return;
}

bool GraphicsClass::Render(float frameTime, FogInfo fog, FireInfo fire)
{
	//테스트매트리스
	XMMATRIX testWorldMatrix[3], testScaleMatrix[3], testRotateMatrix[3], testTransMatrix[3];

	testScaleMatrix[0] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	testRotateMatrix[0] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	testTransMatrix[0] = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
	
	testScaleMatrix[1] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	testRotateMatrix[1] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	testTransMatrix[1] = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	testScaleMatrix[2] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	testRotateMatrix[2] = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	testTransMatrix[2] = XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	for (int i = 0; i < 3; i++)
		testWorldMatrix[i] = testScaleMatrix[i] * testRotateMatrix[i] * testTransMatrix[i];


	bool result;

	// Follow camera
	m_Camera->GetPlayerMatrix(rotateMatrix[1], transMatrix[1], CarRotation, CarPosition);

	result = RenderReflectionToTexture(fog.color, fog.start, fog.end);
	result = RenderRefractionToTexture();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(fog.color, fog.color, fog.color, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->SetMouseVariance(m_mouseVariance);
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix[0]);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//sky Rendering
	if (RenderFog == false)
	{
		m_D3D->TurnZBufferOff();
		m_D3D->SetCullNone();
		m_Sky->Render(m_D3D->GetDeviceContext());
		result = m_SkyShader->Render(m_D3D->GetDeviceContext(), m_Sky->GetFaceCount(),
			skyWorldMatrix, secondWorldMatrix[4], viewMatrix, projectionMatrix, m_Sky->GetTexture());
		m_D3D->SetCullBack();
		m_D3D->TurnZBufferOn();
	}

	result = RenderWater(fog.start, fog.end);

	for (int i = 0; i < INDEX_MODEL; i++)
	{
		if (IsRanderModel[i] == true)
		{
			if (RenderFog == true)
			{
				if (i >= 4 && i <= 26)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogShader->Render(m_D3D->GetDeviceContext(),
						m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[0], viewMatrix, projectionMatrix,
						m_Model[i]->GetTexture(), fog.start, fog.end);
				}
				else if (i >= 27 && i <= 91)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogShader->Render(m_D3D->GetDeviceContext(),
						m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[1], viewMatrix, projectionMatrix,
						m_Model[i]->GetTexture(), fog.start, fog.end);
				}
				else if (i >= 92 && i <= 135)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogShader->Render(m_D3D->GetDeviceContext(),
						m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[2], viewMatrix, projectionMatrix,
						m_Model[i]->GetTexture(), fog.start, fog.end);
				}
				else if (i >= 136 && i <= 179)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogShader->Render(m_D3D->GetDeviceContext(),
						m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[3], viewMatrix, projectionMatrix,
						m_Model[i]->GetTexture(), fog.start, fog.end);
				}
				else if (i >= 2 && i <= 3)
				{
					m_D3D->SetCullNone();
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogShader->Render(m_D3D->GetDeviceContext(),
						m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[5], viewMatrix, projectionMatrix,
						m_Model[i]->GetTexture(), fog.start, fog.end);
					m_D3D->SetCullBack();
				}
				else if (i == 180 || i == 0)//water, ground
				{
					continue;
				}
				else
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_FogShader->Render(m_D3D->GetDeviceContext(),
						m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(), worldMatrix[i], secondWorldMatrix[4], viewMatrix, projectionMatrix,
						m_Model[i]->GetTexture(), fog.start, fog.end);
				}
			}
			else
			{
				if (i >= 4 && i <= 26)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(),
						worldMatrix[i], secondWorldMatrix[0], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
				else if (i >= 27 && i <= 91)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(),
						worldMatrix[i], secondWorldMatrix[1], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
				else if (i >= 92 && i <= 135)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(),
						worldMatrix[i], secondWorldMatrix[2], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
				else if (i >= 136 && i <= 179)
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(),
						worldMatrix[i], secondWorldMatrix[3], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
				else if (i >= 2 && i <= 3)
				{
					m_D3D->SetCullNone();
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(),
						worldMatrix[i], secondWorldMatrix[5], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
					m_D3D->SetCullBack();
				}
				else if (i == 180 || i == 0) //water, ground
				{
					continue;
				}
				else
				{
					m_Model[i]->Render(m_D3D->GetDeviceContext());
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetVertexCount(), m_Model[i]->GetInstanceCount(),
						worldMatrix[i], secondWorldMatrix[4], viewMatrix, projectionMatrix, m_Model[i]->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
			}
		}
	}

	if (RenderCollider == true)
	{
		m_D3D->SetWireframe();
		//collider Rendering
		if (RenderFog == true)
		{
			for (int i = 0; i < INDEX_COLLIDER; i++)
			{
				m_Collider[i]->Render(m_D3D->GetDeviceContext());

				if (i >= 1 && i <= 28)
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[0], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if (i >= 29 && i <= 106)
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[1], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if (i >= 107 && i <= 158)
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[2], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if (i >= 159 && i <= 209)
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[3], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if (i == 210)
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[5], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[4], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				if (!result)
				{
					return false;
				}
			}
		}
		else
		{
			for (int i = 0; i < INDEX_COLLIDER; i++)
			{
				m_Collider[i]->Render(m_D3D->GetDeviceContext());
				if ((i >= 1 && i<= 28))
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[0], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if ((i >= 29 && i <= 106))
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[1], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if ((i >= 107 && i <= 158))
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[2], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if ((i >= 159 && i <= 209))
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[3], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else if (i == 210)
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[5], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				else
				{
					result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Collider[i]->GetVertexCount(), m_Collider[i]->GetInstanceCount(),
						colliderWorldMatrix[i], secondWorldMatrix[4], viewMatrix, projectionMatrix, m_Collider[i]->GetColor());
				}
				if (!result)
				{
					return false;

				}
			}
		}
		m_D3D->SetSolid();
	}

	if (!result)
	{
		return false;
	}

	m_D3D->TurnOnAlphaBlending();
	//fire Rendering
	if (RenderFog == false)
	{
		for (int i = 0; i < INDEX_FIRE; i++)
		{
			m_Fire[i]->Render(m_D3D->GetDeviceContext());
			if (i == 0)
				result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetVertexCount(), m_Fire[i]->GetInstanceCount(),
					fireWorldMatrix[i], secondWorldMatrix[2], viewMatrix, projectionMatrix,
					m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fire.frameTime, fire.scrollSpeeds,
					fire.scales, fire.distortion1, fire.distortion2, fire.distortion3, fire.distortionScale, fire.distortionBias);
			else if (i == 1)
				result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetVertexCount(), m_Fire[i]->GetInstanceCount(),
					fireWorldMatrix[i], secondWorldMatrix[3], viewMatrix, projectionMatrix,
					m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fire.frameTime, fire.scrollSpeeds,
					fire.scales, fire.distortion1, fire.distortion2, fire.distortion3, fire.distortionScale, fire.distortionBias);
			if (!result)
			{
				return false;
			}
		}
	}
	else
	{
		for (int i = 0; i < INDEX_FIRE; i++)
		{
			m_Fire[i]->Render(m_D3D->GetDeviceContext());
			if (i == 0)
				result = m_FogFireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetVertexCount(), m_Fire[i]->GetInstanceCount(),
					fireWorldMatrix[i], secondWorldMatrix[2], viewMatrix, projectionMatrix,
					m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fire.frameTime, fire.scrollSpeeds,
					fire.scales, fire.distortion1, fire.distortion2, fire.distortion3, fire.distortionScale, fire.distortionBias, fog.start, fog.end);
			else if (i == 1)
				result = m_FogFireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetVertexCount(), m_Fire[i]->GetInstanceCount(),
					fireWorldMatrix[i], secondWorldMatrix[3], viewMatrix, projectionMatrix,
					m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fire.frameTime, fire.scrollSpeeds,
					fire.scales, fire.distortion1, fire.distortion2, fire.distortion3, fire.distortionScale, fire.distortionBias, fog.start, fog.end);
			if (!result)
			{
				return false;
			}
		}
	}

	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetVertexCount(), m_ParticleSystem->GetInstanceCount(), particleWorldMatrix, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	if (!result)
	{
		return false;
	}

	m_D3D->TurnZBufferOff();
	//UI Rendering
	if (TestMode == false)
	{
		if (Pause == true || Main == true || Success == true || Failure == true)
		{
			for (int i = 0; i < INDEX_BITMAP; i++)
			{
				if (i == 0)
				{
					result = m_Bitmap[i]->Render(m_D3D->GetDeviceContext(), m_screenWidth / 2 - 250, m_screenHeight / 2 - 135);
					result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap[i]->GetIndexCount(), worldMatrix[0], baseViewMatrix, orthoMatrix, m_Bitmap[i]->GetTexture());
				}
			}
		}

		//text Rendering
		for (int i = 0; i < INDEX_TEXT; i++)
		{
			result = m_Text[i]->Render(m_D3D->GetDeviceContext(), worldMatrix[0], orthoMatrix);
			if (!result)
			{
				return false;
			}
		}

		//UI Rendering
		if (Pause == true || Main == true || Success == true || Failure == true)
		{
			for (int i = 0; i < INDEX_BITMAP; i++)
			{
				if (i == 1)
				{
					result = m_Bitmap[i]->Render(m_D3D->GetDeviceContext(), m_mousePosition.x, m_mousePosition.y);
					result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap[i]->GetIndexCount(), worldMatrix[0], baseViewMatrix, orthoMatrix, m_Bitmap[i]->GetTexture());
				}
			}
		}
	}
	m_D3D->TurnZBufferOn();
	m_D3D->TurnOffAlphaBlending();

	m_D3D->EndScene();

	return true;
}