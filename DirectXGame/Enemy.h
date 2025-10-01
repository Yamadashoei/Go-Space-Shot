#pragma once
#include "EnemyBullet.h"
#include "KamataEngine.h"
#include <list>

class Enemy {
public:
	// GameScene で作った cube モデルを渡してください
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& pos);

	// ★ プレイヤー位置を受けて、5秒ごとにその方向へ発射
	void Update(const KamataEngine::Vector3& playerPos, float deltaSec = 1.0f / 60.0f);
	void Draw(KamataEngine::Camera& camera);

	// 弾アクセス（GameSceneで衝突消去するため mutable）
	std::list<EnemyBullet>& GetBullets() { return bullets_; }

	// 位置・判定・HP
	const KamataEngine::Vector3& GetPosition() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }

	void Damage(int d) { hp_ = (hp_ - d < 0) ? 0 : hp_ - d; }
	bool IsDead() const { return hp_ <= 0; }
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return 300; }

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;

	// 発射まわり
	float shotTimerSec_ = 0.0f;
	float shotIntervalSec_ = 3.0f; // 5秒ごと
	float bulletSpeed_ = 0.6f;

	// 弾
	std::list<EnemyBullet> bullets_;

	// 判定・HP
	float radius_ = 1.2f;
	int hp_ = 300;
};
