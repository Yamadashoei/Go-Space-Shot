#define NOMINMAX
#include <Windows.h>

#include "Player.h"
#include "PlayerBullet.h"
#include "kMath.h"
#include <algorithm>
#include <cassert>
#include <cmath>

#include <base/TextureManager.h>
#include <base/WinApp.h>
#include <input/Input.h>
#include <math/MathUtility.h>

using namespace KamataEngine;

Player::~Player() {
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	playerBullets_.clear();
}

void Player::Initialize(Model* model, Camera* camera) {
	assert(model);
	playerModel = model;
	camera_ = camera;
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, -30.0f}; // ★ カメラの前（+Z）
	worldTransform_.UpdateMatrix();

	collision_.SetPosition(worldTransform_.translation_);
	collision_.SetRadius(1.0f);
}

void Player::Update() {
	// --- 攻撃（左クリック / SPACE） ---
	Attack();

	// 自機の行動（仮でWASD移動）
	if (input_->PushKey(DIK_W))
		worldTransform_.translation_.y += 0.5f;
	if (input_->PushKey(DIK_S))
		worldTransform_.translation_.y -= 0.5f;
	if (input_->PushKey(DIK_A))
		worldTransform_.translation_.x -= 0.5f;
	if (input_->PushKey(DIK_D))
		worldTransform_.translation_.x += 0.5f;

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();


	// 被弾点滅
	if (isHit_) {
		if (--hitEffectTimer_ <= 0)
			isHit_ = false;
	}

	// 弾の更新と破棄
	for (PlayerBullet* bullet : playerBullets_)
		bullet->Update();
	playerBullets_.remove_if([](PlayerBullet* b) {
		if (b->IsDead()) {
			delete b;
			return true;
		}
		return false;
	});
}

void Player::Draw(Camera& viewProjection) {
	if (playerModel)
		playerModel->Draw(worldTransform_, viewProjection);
	for (PlayerBullet* bullet : playerBullets_)
		bullet->Draw(viewProjection);
}

void Player::TakeDamage(int damage) {
	hp_ -= damage;
	if (hp_ < 0)
		hp_ = 0;
}

void Player::SetHit() {
	isHit_ = true;
	hitEffectTimer_ = 20;
}

void Player::Attack() {
	// 左クリック or SPACE で射撃
	const bool mouseTrig = input_->IsTriggerMouse(0);
	const bool spaceTrig = input_->TriggerKey(DIK_SPACE);
	if (!(mouseTrig || spaceTrig))
		return;

	// マウス位置 → ワールド座標（画面から少し手前）
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mousePos);

	Vector2 screenPos = {static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
	// カメラの向きとプレイヤー位置を考慮して、手前側に少し（例：自身より 4 小さい z）
	float zDepth = worldTransform_.translation_.z + 10.0f;

	Vector3 targetWorldPos = ScreenToWorld(screenPos, zDepth, camera_->matView, camera_->matProjection, WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 自機→ターゲット方向
	Vector3 toTarget = Subtract(targetWorldPos, worldTransform_.translation_);
	Vector3 dir = Normalize(toTarget);
	Vector3 velocity = dir * 1.2f;

	auto* newBullet = new PlayerBullet();
	// ★ 既存の PlayerBullet に合わせて（Initialize(model, pos, vel)）
	newBullet->Initialize(playerModel, worldTransform_.translation_, velocity);
	playerBullets_.push_back(newBullet);
}

void Player::SetReticlePosition(const KamataEngine::Vector2& pos) { reticlePosition_ = pos; }
