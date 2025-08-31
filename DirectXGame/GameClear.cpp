#include "GameClear.h"

void GameClear::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

sprClear_ = Sprite::Create(TextureManager::Load("./Resources/Scene/clear.png"), {0.0f, 0.0f});
}

void GameClear::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		next_ = true;
		nextScene_ = SceneState::Title;
	}
}

void GameClear::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprClear_)
		sprClear_->Draw();
	Sprite::PostDraw();
	dx_->ClearDepthBuffer();
}
