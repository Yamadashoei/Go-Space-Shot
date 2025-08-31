#include "GameScene.h"
using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelEnemy_;

 
	delete player_;
	
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// --- カメラ：z=-10 から +Z 方向を見る（rotation 0 の既定向き想定）
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_.UpdateMatrix();
	camera_.TransferMatrix(); // ★重要：初回に必ず送る

	// --- モデルを GameScene で生成して注入
	modelPlayer_ = Model::CreateFromOBJ("cube");
	modelEnemy_ = Model::CreateFromOBJ("cube");

player_ = new Player();    
	player_->Initialize(modelPlayer_);
	player_->SetPosition({0.0f, -5.0f, 20.0f}); // 手前（原点）

enemy_ = new Enemy(); 
	enemy_->Initialize(modelEnemy_, {0.0f, 0.0f, 20.0f});
	enemy_->SetPosition({0.0f, 0.0f, 40.0f}); // 奥
}

void GameScene::Update() {
	// カメラを毎フレーム GPU に反映（描画ズレ防止）
	camera_.UpdateMatrix();
	camera_.TransferMatrix();

	// いまは描画確認だけ
	player_->Update();
	enemy_->Update();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* cmd = dxCommon_->GetCommandList();
	Sprite::PreDraw(cmd);
	Sprite::PostDraw();
	// 2Dは描かない

	// 3D
	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(); // ★引数付き
	player_->Draw(camera_);
	enemy_->Draw(camera_);
	Model::PostDraw();
}
