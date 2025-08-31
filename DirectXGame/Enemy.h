#pragma once
#include "KamataEngine.h"

class Enemy {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& pos);
	void Update(); // いまは行列更新のみ
	void Draw(KamataEngine::Camera& camera); // 描画

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;

};
