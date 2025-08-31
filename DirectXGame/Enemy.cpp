#include "Enemy.h"
#include "kMath.h"
using namespace KamataEngine;

void Enemy::Initialize(Model* model, const KamataEngine::Vector3& position) {
	model_ = model;
	wt_.Initialize();
	wt_.translation_ = position; // 奥に置く（カメラ z=-10 → 手前にプレイヤー、奥に敵）
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Enemy::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Enemy::Update() {
//移動処理

	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Enemy::Draw(Camera& cam) {
	if (model_)
		model_->Draw(wt_, cam);
}
