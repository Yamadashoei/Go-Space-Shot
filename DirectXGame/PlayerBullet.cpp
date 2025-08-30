// PlayerBullet.cpp
#include "PlayerBullet.h"

void PlayerBullet::Initialize(const Vector3& startPos) {
	model_ = Model::CreateFromOBJ("cube");
	wt_.Initialize();
	wt_.translation_ = startPos;
	wt_.UpdateMatrix();
	alive_ = true;
}

void PlayerBullet::Update() {
	wt_.translation_.x += vel_.x;
	wt_.translation_.y += vel_.y;
	wt_.translation_.z += vel_.z;
	wt_.UpdateMatrix();
	if (wt_.translation_.z < -100.0f)
		alive_ = false;
}

void PlayerBullet::Draw(Camera& cam) {
	if (alive_ && model_)
		model_->Draw(wt_, cam);
}
