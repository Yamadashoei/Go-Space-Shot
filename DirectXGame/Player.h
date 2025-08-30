#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	~Player();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update(); // 入力は内部で読む
	void Draw(KamataEngine::Camera& viewProjection);

	// HP
	void TakeDamage(int damage);
	void Damage(int damage) { TakeDamage(damage); } // GameScene 互換
	bool IsDead() const { return hp_ <= 0; }
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return maxHp_; }

	// 位置・当たり判定
	const KamataEngine::Vector3& GetWorldPosition() const { return worldTransform_.translation_; }
	const KamataEngine::Vector3& GetPos() const { return worldTransform_.translation_; } // 互換
	float GetRadius() const { return 1.0f; }
	const Collision& GetCollision() const { return collision_; }

	// 弾
	const std::list<PlayerBullet*>& GetBullets() const { return playerBullets_; }

	void SetHit();
	void SetReticlePosition(const KamataEngine::Vector2& pos);

private:
	void Attack(); // 左クリック / SPACEで発射

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* playerModel = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	std::list<PlayerBullet*> playerBullets_;
	Collision collision_;

	int hp_ = 100;
	const int maxHp_ = 100;

	bool isHit_ = false;
	int hitEffectTimer_ = 0;

	KamataEngine::Vector2 reticlePosition_ = {640.0f / 2, 360.0f / 2}; // 画面中央

	// スムーズ移動（XZ）
	struct MoveTuning {
		float maxSpeed = 0.6f;
		float accel = 0.08f;
		float damping = 0.86f;
	} move_;
	KamataEngine::Vector3 velocity_{0.0f, 0.0f, 0.0f};
};
