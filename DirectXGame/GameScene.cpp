#include "GameScene.h"
#include "kMath.h" // Subtract() を使用

using namespace KamataEngine;

void GameScene::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// カメラ（少し俯瞰）
	camera_.Initialize();
	camera_.translation_ = {0.0f, 6.0f, 18.0f};
	camera_.rotation_ = {0.30f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// 2Dスプライト（パスは仮でOK）
	//sprEarth_ = Sprite::Create(TextureManager::Load("./Resources/Scene/earth.png"), {0.0f, 620.0f});

	// 3D（各クラス内で Model::CreateFromOBJ("cube") を使用）
	skydome_.Initialize();
	player_.Initialize();
	enemy_.Initialize({0.0f, 0.0f, 0.0f}, 0.25f);

	pBullets_.clear();
	eBullets_.clear();
	next_ = false;
	nextScene_ = SceneState::GameClear;
}

void GameScene::SpawnPlayerBullet() {
	// コピーを発生させない（std::construct_at エラー対策）
	auto& b = pBullets_.emplace_back();
	b.Initialize(player_.GetPos());
}

void GameScene::SpawnEnemyBullet() {
	auto& b = eBullets_.emplace_back();
	auto p = enemy_.GetPos();
	p.z += 0.5f;
	b.Initialize(p);
}

void GameScene::HandleCollisions() {
	// player bullet → enemy
	for (auto it = pBullets_.begin(); it != pBullets_.end();) {
		const Vector3 d = Subtract(enemy_.GetPos(), it->GetPos()); // operator- ではなく Subtract を使用
		const float dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
		const float r = enemy_.GetRadius() + it->GetRadius();
		if (dist2 < r * r && enemy_.GetHP() > 0) {
			enemy_.Damage(5);
			it = pBullets_.erase(it);
			if (enemy_.GetHP() <= 0) {
				next_ = true;
				nextScene_ = SceneState::GameClear;
			}
		} else {
			if (!it->IsAlive())
				it = pBullets_.erase(it);
			else
				++it;
		}
	}

	// enemy bullet → player
	for (auto it = eBullets_.begin(); it != eBullets_.end();) {
		const Vector3 d = Subtract(player_.GetPos(), it->GetPos());
		const float dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
		const float r = player_.GetRadius() + it->GetRadius();
		if (dist2 < r * r) {
			next_ = true;
			nextScene_ = SceneState::GameOver;
			it = eBullets_.erase(it);
		} else {
			if (!it->IsAlive())
				it = eBullets_.erase(it);
			else
				++it;
		}
	}
}

void GameScene::Update() {
	// 射撃
	if (input_->TriggerKey(DIK_SPACE)) {
		SpawnPlayerBullet();
	}

	// 自機＆敵
	player_.Update(input_, left_, right_, nearZ_, farZ_);
	enemy_.Update(left_ + 1.0f, right_ - 1.0f);

	// 敵の射撃タイミング
	if (enemy_.CanShoot(enemyShotInterval_)) {
		SpawnEnemyBullet();
	}

	// 弾更新
	for (auto& b : pBullets_)
		b.Update();
	for (auto& b : eBullets_)
		b.Update();

	// 当たり判定
	HandleCollisions();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dx_->GetCommandList();

	// 背景2D
	Sprite::PreDraw(commandList);
	if (sprBG_)
		sprBG_->Draw();
	if (sprEarth_)
		sprEarth_->Draw();
	Sprite::PostDraw();

	dx_->ClearDepthBuffer();

	// 3D
	Model::PreDraw();
	skydome_.Draw(camera_);
	enemy_.Draw(camera_);
	player_.Draw(camera_);
	for (auto& b : pBullets_)
		b.Draw(camera_);
	for (auto& b : eBullets_)
		b.Draw(camera_);
	Model::PostDraw();
}
