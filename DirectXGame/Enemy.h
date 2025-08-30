#pragma once
#include "KamataEngine.h"

class Enemy {
public:
	// モデルは敵クラス内で cube を作成
	void Initialize(const KamataEngine::Vector3& startPos, float speedX = 0.35f);
	// 左右の境界で折り返す。deltaSec は 1/60 既定（タイマ加算に使用）
	void Update(float left, float right, float deltaSec = 1.0f / 60.0f);
	void Draw(KamataEngine::Camera& cam);

	// 状態/インタフェース
	void Damage(int value);
	int GetHP() const { return hp_; }
	bool IsDead() const { return hp_ <= 0; }

	// 一定秒ごとに true を返す
	bool CanShoot(float intervalSec); // 既存：任意間隔指定
	bool CanShoot();                  // 追加：固定間隔(既定5秒)

	// 固定間隔の変更が必要なら
	void SetShotInterval(float seconds) { fixedShotIntervalSec_ = seconds; }

	// 当たり判定用
	const KamataEngine::Vector3& GetPos() const { return wt_.translation_; }
	float GetRadius() const { return radius_; }

	// マズル位置（ちょい前）
	KamataEngine::Vector3 GetMuzzlePos() const {
		auto p = wt_.translation_;
		p.z += 0.5f;
		return p;
	}

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform wt_{};

	float speedX_ = 0.35f;
	int hp_ = 300;        // ご指定
	float radius_ = 1.8f; // 目安（モデルに合わせて微調整OK）

	// 点滅用（被弾のチラつき）
	bool isDamaged_ = false;
	int damageTimer_ = 0;

	// 発射タイマ
	float shotTimer_ = 0.0f;
	float fixedShotIntervalSec_ = 5.0f; // ★ 追加：既定5秒
};
