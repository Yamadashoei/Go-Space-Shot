// Enemy.h
#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class Enemy {
public:
	void Initialize(const Vector3& startPos, float speedX);
	void Update(float left, float right, float deltaSec = 1.0f / 60.0f);
	void Draw(Camera& cam);

	const Vector3& GetPos() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }
	int GetHP() const { return hp_; }
	void Damage(int value);

	bool CanShoot(float intervalSec); // 一定秒ごとにtrue

private:
	Model* model_ = nullptr;
	WorldTransform wt_{};

	float speedX_ = 0.25f;
	float radius_ = 1.8f;
	int hp_ = 100;

	bool isDamaged_ = false;
	int damageTimer_ = 0;
	float shotTimer_ = 0.0f;
};
