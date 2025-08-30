#include "EnemyBullet.h"
using namespace KamataEngine;

// 静的メンバ定義
Model* EnemyBullet::sModel_ = nullptr;

void EnemyBullet::Initialize(Model* /*model*/, const Vector3& position, const Vector3& velocity) {
	// 互換API：渡された model は使わず、新実装へ
	Initialize(position, velocity);
}

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	if (!sModel_) {
		// ★ 一度だけ cube.obj をロード
		sModel_ = Model::CreateFromOBJ("cube");
	}
	model_ = sModel_;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.8f, 0.8f, 0.8f}; // 見やすいサイズ
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	worldTransform_.UpdateMatrix();

	// 当たり
	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(0.6f);

	deathTimer_ = kLifeTime;
	isDead_ = false;
}

// pos だけ指定 → デフォルトで +Z 方向へ（プレイヤー側に飛ぶ想定）
void EnemyBullet::Initialize(const Vector3& position) { Initialize(position, Vector3{0.0f, 0.0f, +0.7f}); }

void EnemyBullet::Update() {
	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	collision_.SetPosition(worldTransform_.translation_);
}

void EnemyBullet::Draw(const Camera& camera) {
	if (!model_)
		return;
	// ★ OBJそのまま描画（テクスチャ引数は不要）
	model_->Draw(worldTransform_, camera);
}
