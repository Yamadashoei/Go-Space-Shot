#pragma once
#include "KamataEngine.h"

#include "Enemy.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "PlayerBullet.h"

#include "SceneState.h"
#include <list>

class GameScene {
public:
	~GameScene();
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

	// 2D（未使用なら nullptr のままでOK）
	KamataEngine::Sprite* sprBG_ = nullptr;
	KamataEngine::Sprite* sprEarth_ = nullptr;

	// モデル（Player用だけ保持。Enemyは自前でCreateFromOBJしている実装に合わせる）
	KamataEngine::Model* modelPlayer_ = nullptr;

	// 3Dオブジェクト
	Player player_;
	Enemy enemy_;

	// GameSceneが管理する弾
	std::list<PlayerBullet> pBullets_;
	std::list<EnemyBullet> eBullets_;

	// ロジック
	// （EnemyのCanShoot()は固定5秒版を持っているが、明示間隔で使う場合はこれ）
	float enemyShotInterval_ = 3.0f;

	// 可動範囲（参考）
	const float left_ = -12.0f;
	const float right_ = +12.0f;
	const float nearZ_ = 0.0f; // 奥
	const float farZ_ = 8.0f;  // 手前

	// シーン遷移
	bool next_ = false;
	SceneState nextScene_ = SceneState::GameClear;

	// マウス左クリックのトリガ検出用（GameScene管理の弾）
	bool mousePrev_ = false;

private:
	void SpawnPlayerBullet();
	void SpawnEnemyBullet();
	void HandleCollisions();
};
