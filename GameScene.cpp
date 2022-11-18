#include "GameScene.h"
#include <cassert>

#include "WinApp.h"

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete object3d;
	delete billboard;
	delete ybillboard;
	delete particleMan;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/MEGNOSE.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f });
	spriteBG->SetSize({ WinApp::kWindowWidth * 1.4, WinApp::kWindowHeight * 1.4 });

	// 3Dオブジェクト生成
	object3d = Object3d::Create();
	object3d->Update();

	billboard = Object3d::Create();
	billboard->SetBillboardMode(Billboard);
	billboard->SetPosition({3,0,0});
	billboard->Update();

	ybillboard = Object3d::Create();
	ybillboard->SetBillboardMode(YBillboard);
	ybillboard->SetPosition({ -3,0,0 });
	ybillboard->Update();

	// パーティクルマネージャー生成
	particleMan = ParticleManager::Create();
	particleMan->Update();
}

void GameScene::Update()
{
	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { ParticleManager::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { ParticleManager::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { ParticleManager::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { ParticleManager::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
	}

	Object3d::SetEye(ParticleManager::GetEye());
	Object3d::SetTarget(ParticleManager::GetTarget());

	const float rnd_pos = 10.0f;
	XMFLOAT3 pos{};
	pos.x = ((float)rand() / RAND_MAX - 0.5f) * rnd_pos;
	pos.y = ((float)rand() / RAND_MAX - 0.5f) * rnd_pos;
	pos.z = ((float)rand() / RAND_MAX - 0.5f) * rnd_pos;

	const float rnd_vel = 0.1f;
	XMFLOAT3 vel{};
	vel.x = ((float)rand() / RAND_MAX - 0.5f) * rnd_vel;
	vel.y = ((float)rand() / RAND_MAX - 0.5f) * rnd_vel;
	vel.z = ((float)rand() / RAND_MAX - 0.5f) * rnd_vel;

	const float rnd_acc = 0.001f;
	XMFLOAT3 acc{};
	acc.x = ((float)rand() / RAND_MAX - 0.5f) * rnd_acc;
	acc.y = ((float)rand() / RAND_MAX - 0.5f) * rnd_acc;
	acc.z = ((float)rand() / RAND_MAX - 0.5f) * rnd_acc;

	particleMan->Add(60, pos, vel, acc);

	particleMan->Update();

	object3d->Update();
	billboard->Update();
	ybillboard->Update();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	object3d->Draw();
	billboard->Draw();
	ybillboard->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region パーティクル描画
	// パーティクル描画前処理
	ParticleManager::PreDraw(cmdList);

	// パーティクルの描画
	particleMan->Draw();

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>

	// パーティクル描画後処理
	ParticleManager::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
