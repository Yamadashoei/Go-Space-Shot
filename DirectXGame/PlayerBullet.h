// PlayerBullet.h
#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class PlayerBullet {
public:
	void Initialize(const Vector3& startPos);
	void Update();
	void Draw(Camera& cam);

	bool IsAlive() const { return alive_; }
	void Kill() { alive_ = false; }
	const Vector3& GetPos() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }

private:
	Model* model_ = nullptr;
	WorldTransform wt_{};
	Vector3 vel_{0, 0, -0.8f}; // -Zへ
	float radius_ = 0.6f;
	bool alive_ = true;
};
