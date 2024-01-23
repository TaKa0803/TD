#include "ClearScene.h"
#include<imgui.h>
#include"Scenes/Scenes.h"

ClearScene::ClearScene()
{
	input_ = Input::GetInstance();
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
}

void ClearScene::Update()
{
	Debug();

	SceneCahnge();
}

void ClearScene::Draw()
{
}

void ClearScene::Debug()
{
#ifdef _DEBUG
	ImGui::Begin("ClearScene");
	ImGui::Text("Space or B to TitleScene");
	ImGui::Text("ESC to leave game");
	ImGui::End();
#endif // _DEBUG
}

void ClearScene::SceneCahnge()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}

	if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
		sceneNo = TITLE;
	}

	if (input_->TriggerKey(DIK_ESCAPE)) {
		leaveGame = true;
	}
}
