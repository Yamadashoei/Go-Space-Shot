#include "Enemy.h"
using namespace KamataEngine;

void Enemy::Initialize(const Vector3& startPos, float speedX) {
	model_ = Model::CreateFromOBJ("cube"); // キューブでOK
	wt_.Initialize();
	wt_.translation_ = startPos;
	wt_.UpdateMatrix();

	speedX_ = speedX;
	hp_ = 300;
	isDamaged_ = false;
	damageTimer_ = 0;

	shotTimer_ = 0.0f;
	fixedShotIntervalSec_ = 5.0f; // 既定値
}

void Enemy::Update(float left, float right, float deltaSec) {
	// 左右往復
	wt_.translation_.x += speedX_;
	if (wt_.translation_.x <= left || wt_.translation_.x >= right) {
		speedX_ *= -1.0f;
		// 折り返しで軽い演出を入れたいなら rotationY += 〜
	}
	wt_.UpdateMatrix();

	if (isDamaged_) {
		--damageTimer_;
		if (damageTimer_ <= 0)
			isDamaged_ = false;
	}

	// 発射タイマ
	shotTimer_ += deltaSec;
}

void Enemy::Draw(Camera& cam) {
	if (!model_)
		return;
	// 被弾点滅（3フレ毎にON/OFF）
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

bool Enemy::CanShoot() {
	// ★ 固定間隔（既定5秒）
	if (shotTimer_ >= fixedShotIntervalSec_) {
		shotTimer_ = 0.0f;
		return true;
	}
	return false;
}
