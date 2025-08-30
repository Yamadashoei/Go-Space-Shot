// Player.cpp
#include "Player.h"

void Player::Initialize() {
	model_ = Model::CreateFromOBJ("cube");
	wt_.Initialize();
	wt_.translation_ = {0.0f, 0.0f, 7.0f}; // 手前スタート
	wt_.UpdateMatrix();
}

void Player::Update(Input* input, float left, float right, float nearZ, float farZ) {
	if (input->PushKey(DIK_A)) {
		wt_.translation_.x -= moveSpeed_;
	}
	if (input->PushKey(DIK_D)) {
		wt_.translation_.x += moveSpeed_;
	}
	if (input->PushKey(DIK_W)) {
		wt_.translation_.z -= moveSpeed_;
	} // -Zへ
	if (input->PushKey(DIK_S)) {
		wt_.translation_.z += moveSpeed_;
	}

	if (wt_.translation_.x < left)
		wt_.translation_.x = left;
	if (wt_.translation_.x > right)
		wt_.translation_.x = right;
	if (wt_.translation_.z < nearZ)
		wt_.translation_.z = nearZ;
	if (wt_.translation_.z > farZ)
		wt_.translation_.z = farZ;

	wt_.UpdateMatrix();
}

void Player::Draw(Camera& cam) {
	if (model_)
		model_->Draw(wt_, cam);
}
