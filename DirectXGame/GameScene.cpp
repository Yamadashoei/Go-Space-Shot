#include "GameScene.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include <base/DirectXCommon.h>
#include <base/WinApp.h>

using namespace KamataEngine;

bool GameScene::SphereHit(const Vector3& a, float ra, const Vector3& b, float rb) {
	const float dx = a.x - b.x;
	const float dy = a.y - b.y;
	const float dz = a.z - b.z;
	const float rr = (ra + rb) * (ra + rb);
	return (dx * dx + dy * dy + dz * dz) < rr;
}

GameScene::~GameScene() {
	delete player_;
	player_ = nullptr;
	delete enemy_;
	enemy_ = nullptr;
	delete modelPlayer_;
	modelPlayer_ = nullptr;
	delete modelEnemy_;
	modelEnemy_ = nullptr;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// カメラ
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// モデル
	modelPlayer_ = Model::CreateFromOBJ("cube");
	modelEnemy_ = Model::CreateFromOBJ("cube");

	// 実体
	player_ = new Player();
	player_->Initialize(modelPlayer_);
	player_->SetPosition({0.0f, 0.0f, 20.0f}); 

	enemy_ = new Enemy();
	enemy_->Initialize(modelEnemy_, {0.0f, 0.0f, 40.0f}); 


	// 遷移フラグ初期化
	next_ = false;
	nextScene_ = SceneState::Title;
}

void GameScene::Update() {
	// 更新
	player_->Update();
	enemy_->Update(player_->GetPosition(), 1.0f / 60.0f);

	// 衝突
	HandleCollisions();

	// ===== 勝敗チェック → シーン遷移 =====
	if (!next_) {
		if (enemy_->IsDead()) {
			next_ = true;
			nextScene_ = SceneState::GameClear;
		} else if (player_->IsDead()) {
			next_ = true;
			nextScene_ = SceneState::GameOver;
		}
	}
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* cmd = dxCommon_->GetCommandList();

	Sprite::PreDraw(cmd);
	Sprite::PostDraw();

	dxCommon_->ClearDepthBuffer();

	Model::PreDraw();
	enemy_->Draw(camera_);
	player_->Draw(camera_);
	Model::PostDraw();
}

void GameScene::HandleCollisions() {
	auto& pBullets = player_->GetBullets();
	auto& eBullets = enemy_->GetBullets();

	for (auto it = pBullets.begin(); it != pBullets.end();) {
		if (SphereHit(it->GetPos(), it->GetRadius(), enemy_->GetPosition(), enemy_->GetRadius())) {
			enemy_->Damage(it->GetDamage()); // 20
			it = pBullets.erase(it);
		} else {
			++it;
		}
	}

	for (auto it = eBullets.begin(); it != eBullets.end();) {
		if (SphereHit(it->GetPos(), it->GetRadius(), player_->GetPosition(), player_->GetRadius())) {
			player_->Damage(it->GetDamage()); // 50
			it = eBullets.erase(it);
		} else {
			++it;
		}
	}

	// 弾同士の相殺（両方消滅）
	for (auto pit = pBullets.begin(); pit != pBullets.end();) {
		bool eraseP = false;
		for (auto eit = eBullets.begin(); eit != eBullets.end();) {
			if (SphereHit(pit->GetPos(), pit->GetRadius(), eit->GetPos(), eit->GetRadius())) {
				eit = eBullets.erase(eit);
				eraseP = true;
				break;
			} else {
				++eit;
			}
		}
		if (eraseP)
			pit = pBullets.erase(pit);
		else
			++pit;
	}
}
