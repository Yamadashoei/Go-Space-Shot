#pragma once
#include "Collision.h"
#include "KamataEngine.h"
#include "kMath.h"
#include <cassert>

using namespace KamataEngine;

class PlayerBullet {
public:
	// 互換：従来の呼び出しを壊さない（渡された model は使いません）
	void Initialize(KamataEngine::Model* /*model*/, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 新：直接 position/velocity だけで初期化（内部で cube を共有ロード）
	void Initialize(const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	void Update();
	void Draw(const KamataEngine::Camera& viewProjection);

	// 状態
	bool IsDead() const { return isDead_; }
	bool IsAlive() const { return !isDead_; } // ★ 追加（GameSceneで使用）
	const Collision& GetCollision() const { return collision_; }

	// 位置・当たり
	const KamataEngine::Vector3& GetPos() const { return worldTransform_.translation_; } // ★ 追加（GameSceneで使用）
	float GetRadius() const { return collision_.GetRadius(); }                           // ★ 追加（GameSceneで使用）

	// ダメージ
	static constexpr int kDamage = 10;
	int GetDamage() const { return kDamage; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Vector3 velocity_{};

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	Collision collision_;

	// OBJを全弾で共有
	static KamataEngine::Model* sModel_;
};
