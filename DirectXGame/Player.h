// Player.h
#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class Player {
public:
	void Initialize();
	void Update(Input* input, float left, float right, float nearZ, float farZ);
	void Draw(Camera& cam);

	const Vector3& GetPos() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }

private:
	Model* model_ = nullptr;
	WorldTransform wt_{};
	float moveSpeed_ = 0.3f;
	float radius_ = 1.0f;
};
