#include "GameScene.h"
#include "kMath.h"   // Subtract()
#include <Windows.h> // GetAsyncKeyState

using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
	modelPlayer_ = nullptr;
}

void GameScene::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// カメラ（軽く俯瞰）
	camera_.Initialize();
	camera_.translation_ = {0.0f, 6.0f, 18.0f};
	camera_.rotation_ = {0.30f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// 2Dは任意
	// sprBG_    = Sprite::Create(TextureManager::Load("./Resources/Scene/bg.png"),    {0.0f, 0.0f});
	// sprEarth_ = Sprite::Create(TextureManager::Load("./Resources/Scene/earth.png"), {0.0f, 620.0f});

	// Player用モデルのみ GameScene 側で生成
	modelPlayer_ = Model::CreateFromOBJ("cube"); // パスに注意
	player_.Initialize(modelPlayer_, &camera_);

	// Enemy はクラス内で CreateFromOBJ("cube") 済みの実装に合わせる
	enemy_.Initialize({0.0f, 0.0f, 0.0f}, 0.35f);

	pBullets_.clear();
	eBullets_.clear();

	next_ = false;
	nextScene_ = SceneState::GameClear;
	mousePrev_ = false;
}

void GameScene::SpawnPlayerBullet() {
	auto& b = pBullets_.emplace_back();
	// Playerの現在位置から、カメラ前方へ飛ばす（速度はプレイヤーAttackに合わせる）
	// ここでは前方（-Z）へやや速く
	const Vector3 pos = player_.GetWorldPosition();
	const Vector3 vel = {0.0f, 0.0f, -1.2f};
	b.Initialize(pos, vel); // PlayerBulletは内部でcube共有
}

void GameScene::SpawnEnemyBullet() {
	auto& b = eBullets_.emplace_back();
	// Enemy側のマズル位置ユーティリティを利用
	b.Initialize(enemy_.GetMuzzlePos()); // 省略版：+Z方向にデフォルト速度
}

void GameScene::HandleCollisions() {
	// === Player弾（GameScene管理）→ Enemy（距離判定） ===
	for (auto it = pBullets_.begin(); it != pBullets_.end();) {
		const Vector3 d = Subtract(enemy_.GetPos(), it->GetPos());
		const float dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
		const float r = enemy_.GetRadius() + it->GetRadius();

		if (dist2 < r * r && enemy_.GetHP() > 0) {
			enemy_.Damage(it->GetDamage()); // 10ダメージ
			it = pBullets_.erase(it);       // 命中で消滅
			if (enemy_.GetHP() <= 0) {
				next_ = true;
				nextScene_ = SceneState::GameClear;
			}
		} else {
			if (!it->IsAlive())
				it = pBullets_.erase(it);
			else
				++it;
		}
	}

	// === Enemy弾 → Player（Collision同士） ===
	const Collision& playerCol = player_.GetCollision();
	for (auto it = eBullets_.begin(); it != eBullets_.end();) {
		if (playerCol.CheckCollision(it->GetCollision())) {
			player_.TakeDamage(it->GetDamage()); // 50ダメージ
			it = eBullets_.erase(it);
			if (player_.IsDead()) {
				next_ = true;
				nextScene_ = SceneState::GameOver;
			}
		} else {
			if (!it->IsAlive())
				it = eBullets_.erase(it);
			else
				++it;
		}
	}
}

void GameScene::Update() {
	// ★ Player は内部で入力＆攻撃（左クリック）を処理
	player_.Update();

	// GameScene 管理の弾：SPACE と 左クリックのトリガ
	const bool spaceTrig = input_->TriggerKey(DIK_SPACE);
	const bool mouseDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	const bool mouseTrig = (mouseDown && !mousePrev_);
	if (spaceTrig || mouseTrig) {
		SpawnPlayerBullet();
	}
	mousePrev_ = mouseDown;

	// Enemy 左右往復
	enemy_.Update(left_ + 1.0f, right_ - 1.0f);

	// 敵の射撃（どちらでもOK）
	// if (enemy_.CanShoot(enemyShotInterval_)) { SpawnEnemyBullet(); }
	if (enemy_.CanShoot()) { // ★ 追加した固定間隔（既定5秒）
		SpawnEnemyBullet();
	}

	// 弾更新
	for (auto& b : pBullets_)
		b.Update();
	for (auto& b : eBullets_)
		b.Update();

	// 当たり判定
	HandleCollisions();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();

	// 2D（使っていれば）
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprEarth_)
		sprEarth_->Draw();
	Sprite::PostDraw();

	// 3D のため深度クリア
	dx_->ClearDepthBuffer();

	// 3D
	Model::PreDraw(); // ★ 引数ありで統一
	enemy_.Draw(camera_);
	player_.Draw(camera_); // Player内の弾もここで描画される
	for (auto& b : pBullets_)
		b.Draw(camera_); // GameScene管理の自機弾
	for (auto& b : eBullets_)
		b.Draw(camera_); // 敵弾
	Model::PostDraw();
}
