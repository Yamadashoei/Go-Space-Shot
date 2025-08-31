#pragma once
#include "KamataEngine.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model);
	void SetPosition(const KamataEngine::Vector3& pos);
	void Update();                        // いまは行列更新のみ
	void Draw(KamataEngine::Camera& cam); // 描画

private:
	KamataEngine::WorldTransform wt_{};
	KamataEngine::Model* model_ = nullptr;
};
