#pragma once
#include "Enemy.h"
#include "EnemyBullet.h"
#include "KamataEngine.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "SceneState.h"
#include "SkyDome.h"
#include <list>

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();

	bool IsNextSceneRequested() const { return next_; }
	SceneState GetNextScene() const { return nextScene_; }

private:
	// 基盤
	KamataEngine::DirectXCommon* dx_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera camera_;

	// 2Dスプライト（背景・地球）
	KamataEngine::Sprite* sprBG_ = nullptr;
	KamataEngine::Sprite* sprEarth_ = nullptr;

	// 3D
	SkyDome skydome_;
	Player player_;
	Enemy enemy_;
	std::list<PlayerBullet> pBullets_;
	std::list<EnemyBullet> eBullets_;

	// ロジック
	float enemyShotInterval_ = 3.0f; // 敵は約3秒ごとに発射
	// 2D時代の移動範囲を XZ 矩形に置換
	const float left_ = -12.0f;
	const float right_ = +12.0f;
	const float nearZ_ = 0.0f; // 奥
	const float farZ_ = 8.0f;  // 手前

	// シーン遷移
	bool next_ = false;
	SceneState nextScene_ = SceneState::GameClear;

	// 内部ヘルパ
	void SpawnPlayerBullet();
	void SpawnEnemyBullet();
	void HandleCollisions();
};
