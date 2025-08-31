#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	// GameSceneで作成した Model::CreateFromOBJ("cube") を渡してください
	void Initialize(KamataEngine::Model* model);

	void SetPosition(const KamataEngine::Vector3& pos);
	void SetMoveSpeed(float s) { moveSpeed_ = s; }

	// 移動＆弾更新＆発射
	void Update();

	// 自機と弾の描画
	void Draw(KamataEngine::Camera& cam);

	// 弾アクセス（当たり判定等で使うなら）
	const std::list<PlayerBullet>& GetBullets() const { return bullets_; }

private:
	// 入力
	KamataEngine::Input* input_ = nullptr;

	// 見た目＆変換
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;

	// 移動
	float moveSpeed_ = 0.2f;

	// 弾
	std::list<PlayerBullet> bullets_;
	float bulletSpeed_ = 0.8f; // Y+ 方向
};
