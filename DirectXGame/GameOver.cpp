#include "GameOver.h"

void GameOver::Initialize() {
	dx_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();
sprOver_ = Sprite::Create(TextureManager::Load("./Resources/Scene/gameover.png"), {0.0f, 0.0f});
}

void GameOver::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		next_ = true;
		nextScene_ = SceneState::Title;
	}
}

void GameOver::Draw() {
	ID3D12GraphicsCommandList* cmd = dx_->GetCommandList();
	Sprite::PreDraw(cmd);
	if (sprBG_)
		sprBG_->Draw();
	if (sprOver_)
		sprOver_->Draw();
	Sprite::PostDraw();
	dx_->ClearDepthBuffer();
}
