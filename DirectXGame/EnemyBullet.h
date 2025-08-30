#pragma once
#include "Collision.h"
#include "KamataEngine.h"
#include "kMath.h"
#include <cassert>

using namespace KamataEngine;

class EnemyBullet {
public:
	// 互換：従来API（渡された model は使いません）
	void Initialize(KamataEngine::Model* /*model*/, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 推奨：pos/vel だけ指定（内部で cube を共有ロード）
	void Initialize(const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 省略版：pos のみ（デフォルトで+Z方向へ飛ばす）
	void Initialize(const KamataEngine::Vector3& position);

	void Update();
	void Draw(const KamataEngine::Camera& camera);

	// 状態
	bool IsDead() const { return isDead_; }
	bool IsAlive() const { return !isDead_; }

	// 当たり
	const Collision& GetCollision() const { return collision_; }
	const KamataEngine::Vector3& GetPos() const { return worldTransform_.translation_; }
	float GetRadius() const { return collision_.GetRadius(); }

	// ダメージ
	static constexpr int kDamage = 50;
	int GetDamage() const { return kDamage; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Vector3 velocity_{};

	static const int32_t kLifeTime = 60 * 5; // 5秒
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	Collision collision_;

	// OBJ共有
	static KamataEngine::Model* sModel_;
};
