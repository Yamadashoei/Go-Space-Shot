#include "PlayerBullet.h"

using namespace KamataEngine;

// 静的メンバ定義
Model* PlayerBullet::sModel_ = nullptr;

void PlayerBullet::Initialize(Model* /*model*/, const Vector3& position, const Vector3& velocity) {
	// 互換API：渡された model は使わず内部の cube を利用
	Initialize(position, velocity);
}

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	if (!sModel_) {
		// 一度だけ cube.obj をロード（パスはエンジンの検索パスに依存）
		sModel_ = Model::CreateFromOBJ("cube");
	}
	model_ = sModel_;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.6f, 0.6f, 0.6f}; // 見やすいサイズ
	worldTransform_.translation_ = position;
	velocity_ = velocity;

	worldTransform_.UpdateMatrix();

	// 当たり判定
	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(0.5f);

	deathTimer_ = kLifeTime;
	isDead_ = false;
}

void PlayerBullet::Update() {
	// 位置更新
	worldTransform_.translation_ += velocity_;

	// 寿命
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 当たり判定更新
	collision_.SetPosition(worldTransform_.translation_);
}

void PlayerBullet::Draw(const Camera& viewProjection) {
	if (model_) {
		// OBJそのまま描画（テクスチャ指定は不要）
		model_->Draw(worldTransform_, viewProjection);
	}
}
