// Enemy.cpp
#include "Enemy.h"

void Enemy::Initialize(const Vector3& startPos, float speedX) {
	model_ = Model::CreateFromOBJ("cube");
	wt_.Initialize();
	wt_.translation_ = startPos;
	wt_.UpdateMatrix();
	speedX_ = speedX;
	hp_ = 100;
	isDamaged_ = false;
	damageTimer_ = 0;
	shotTimer_ = 0.0f;
}

void Enemy::Update(float left, float right, float deltaSec) {
	wt_.translation_.x += speedX_;
	if (wt_.translation_.x <= left || wt_.translation_.x >= right)
		speedX_ *= -1.0f;
	wt_.UpdateMatrix();

	if (isDamaged_) {
		--damageTimer_;
		if (damageTimer_ <= 0)
			isDamaged_ = false;
	}
	shotTimer_ += deltaSec;
}

void Enemy::Draw(Camera& cam) {
	if (!model_)
		return;
	if (!isDamaged_ || ((damageTimer_ / 3) % 2 == 0)) {
		model_->Draw(wt_, cam);
	}
}

void Enemy::Damage(int value) {
	hp_ -= value;
	if (hp_ < 0)
		hp_ = 0;
	isDamaged_ = true;
	damageTimer_ = 18;
}

bool Enemy::CanShoot(float intervalSec) {
	if (shotTimer_ >= intervalSec) {
		shotTimer_ = 0.0f;
		return true;
	}
	return false;
}
