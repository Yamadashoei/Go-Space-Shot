#pragma once
#include "Enemy.h"
#include "KamataEngine.h"
#include "Player.h"
#include "SceneState.h"

using namespace KamataEngine;

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	// main.cpp のインタフェース（今は遷移なし）
	bool IsNextSceneRequested() const { return false; }
	SceneState GetNextScene() const { return SceneState::Title; }

private:
	// 基盤
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// カメラ
	Camera camera_;

	// モデル（★GameScene で作る）
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;

	// 実体
	Player* player_;
	Enemy* enemy_;
};
