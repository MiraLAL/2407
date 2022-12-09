#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Collision.h"
#include "Stage.h"
#include "StageManager.h"
#include "StageMain.h"
#include "StageMoveFloor.h"

// ������
void SceneGame::Initialize()
{
	// �Q�[��BGM
	//GameBGM = Audio::Instance().LoadAudioSource("Data/Audio/GameBGM.wav");

	// �X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);

	// �v���C���[
	player = new Player();

	// �J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// �J�����R���g���[���[������
	cameraController = new CameraController();

	// �G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		enemyManager.Register(slime);
	}

	// �Q�[�W�X�v���C�g
	gauge = new Sprite();
}

// �I����
void SceneGame::Finalize()
{
	// �Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}
	
	// �G�l�~�[�I����
	EnemyManager::Instance().Clear();

	// �J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	// �v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	// �X�e�[�W�I����
	StageManager::Instance().Clear();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	// �Q�[��BGM
	//GameBGM->Play(true);

	// �J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	// �v���C���[�X�V����
	player->Update(elapsedTime);

	// �G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	// �G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	rc.viewPosition = camera.GetEye();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader(0);
		shader->Begin(dc, rc);

		// �X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	{
		Shader* shader = graphics.GetShader(1);
		shader->Begin(dc, rc);

		// �v���C���[�`��
		player->Render(dc, shader);

		// �G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	}
}

// �G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	// �r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// �S�Ă̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	
	DirectX::XMVECTOR ScreenPosition;
	DirectX::XMVECTOR WorldPosition;

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();

		worldPosition.y += enemy->GetHeight();
		WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

		ScreenPosition = DirectX::XMVector3Project(WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);

		DirectX::XMFLOAT3 screenPosition;
		DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);

		// �Q�[�W�̕�
		const float Gaugew = 50.0f;
		const float Gaugeh = 5.0f;

		// HP�̊����v�Z
		float healthRate = static_cast<float>(enemy->GetHealth()) / static_cast<float>(enemy->GetMaxHealth());
		DirectX::XMFLOAT4 healthColor = { 1.0f,0.0f,0.0f,1.0f };

		if (healthRate < 0.5)
		{
			healthColor = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		// �`��
		gauge->Render(
			dc,
			screenPosition.x - Gaugew * 0.5f,
			screenPosition.y - Gaugeh,
			Gaugew * healthRate,
			Gaugeh,
			0, 0,
			static_cast<float>(gauge->GetTextureWidth()),
			static_cast<float>(gauge->GetTextureHeight()),
			0.0f,
			healthColor.x, healthColor.y, healthColor.z, healthColor.w);
	}

	// �G�l�~�[�z�u����
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		// �}�E�X�J�[�\�����W���擾
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		screenPosition.z = 0.0f;

		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);

		DirectX::XMFLOAT3 RayStart;
		DirectX::XMStoreFloat3(&RayStart, WorldPosition);

		screenPosition.z = 1.0f;

		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);

		DirectX::XMFLOAT3 RayEnd;
		DirectX::XMStoreFloat3(&RayEnd, WorldPosition);

		// ��������RayCast���g�p���ăX�e�[�W�ɓ����������[���h���W���v�Z��
		// �����ɃX���C�����o��������

		HitResult hit;
		if (StageManager::Instance().RayCast(RayStart, RayEnd, hit))
		{
			EnemyManager& enemyManager = EnemyManager::Instance();
			EnemySlime* slime = new EnemySlime();
			slime->SetPosition(hit.position);
			enemyManager.Register(slime);
		}
	}
}

