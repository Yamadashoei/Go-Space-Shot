#include "Enemy.h"
#include "kMath.h"
using namespace KamataEngine;

void Enemy::Initialize(Model* model, const Vector3& position) {
	model_ = model;
	wt_.Initialize();
	wt_.translation_ = position;
	//scale
	wt_.UpdateMatrix();
	wt_.TransferMatrix();

	bullets_.clear();
	shotTimerSec_ = 0.0f;
	hp_ = 300;
}

void Enemy::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Enemy::Update(const Vector3& playerPos, float deltaSec) {
	// 5秒ごとにプレイヤー方向へ発射
	shotTimerSec_ += deltaSec;
	if (shotTimerSec_ >= shotIntervalSec_) {
		shotTimerSec_ = 0.0f;

		Vector3 dir = Subtract(playerPos, wt_.translation_);
		float len2 = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
		if (len2 < 1e-6f)
			dir = {0.0f, 0.0f, -1.0f};
		else
			dir = Normalize(dir);

		Vector3 spawn = wt_.translation_ + dir * 0.6f;
		Vector3 vel = dir * bulletSpeed_;

		auto& b = bullets_.emplace_back();
		b.Initialize(model_, spawn, vel); 
	}

	// 弾更新 & 寿命削除
	for (auto& b : bullets_)
		b.Update();
	bullets_.remove_if([](const EnemyBullet& b) { return b.IsDead(); });

	// 行列更新
	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Enemy::Draw(Camera& camera) {
	if (model_)
		model_->Draw(wt_, camera);
	for (auto& b : bullets_)
		b.Draw(camera);
}
