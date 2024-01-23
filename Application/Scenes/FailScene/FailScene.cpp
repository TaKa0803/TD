#include "FailScene.h"
#include<imgui.h>
#include"Scenes/Scenes.h"


FailScene::FailScene()
{
	input_ = Input::GetInstance();
}

FailScene::~FailScene()
{
}

void FailScene::Initialize()
{
}

void FailScene::Update()
{
	Debug();

	SceneCahnge();
}

void FailScene::Draw()
{
}

void FailScene::Debug()
{
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	ImGui::Text("Space or B to TitleScene");
	ImGui::Text("ESC to leave game");
	ImGui::End();
#endif // _DEBUG

}

void FailScene::SceneCahnge()
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
