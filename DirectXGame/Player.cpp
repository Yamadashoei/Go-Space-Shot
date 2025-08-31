#include "Player.h"
#include "kMath.h"
using namespace KamataEngine;

void Player::Initialize(Model* model) {
	model_ = model;
	wt_.Initialize();
	wt_.translation_ = {0.0f, 0.0f, 0.0f}; // 原点（カメラは z=-10）
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Player::SetPosition(const Vector3& pos) {
	wt_.translation_ = pos;
	wt_.UpdateMatrix();
	wt_.TransferMatrix();
}

void Player::Update() {
	//移動処理




	wt_.matWorld_ = MakeAffineMatrix(wt_.scale_, wt_.rotation_, wt_.translation_);
	wt_.TransferMatrix();
}

void Player::Draw(Camera& cam) {
	if (model_)
		model_->Draw(wt_, cam);
}
