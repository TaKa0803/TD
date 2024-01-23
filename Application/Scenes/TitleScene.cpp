#include "TitleScene.h"
#include"TextureManager/TextureManager.h"
#include<imgui.h>
#include"Scenes.h"

TitleScene::TitleScene() {

	input_=Input::GetInstance();

	/*int texture = TextureManager::LoadTex("resources/AppResource/Title/Title.png");
	sprite_.reset(Sprite::Create(texture,{320,180},{320,180},{1280,720}));

	texture = TextureManager::LoadTex("resources/AppResource/Title/Press.png");
	pressSp_.reset(Sprite::Create(texture, { 160,90 }, { 160,90 }, { 100,70 }));
	pressSp_->SetPosition({570,500});
	pressSp_->SetScale({170,100});

	texture = TextureManager::LoadTex("resources/AppResource/UI/B.png");
	BButtonSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 70,70 }));
	BButtonSp_->SetPosition({ 750,500 });*/
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize() {
}

void TitleScene::Update() {

	
	Debug();

	
}

void TitleScene::Draw() {

}

void TitleScene::Debug() {

#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	ImGui::Text("Space or B to GameScene");
	ImGui::Text("ESC to leave game");
	ImGui::End();
#endif // _DEBUG


}

void TitleScene::SceneCahnge()
{


	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = GAME;
	}

	if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
		sceneNo = GAME;
	}

	if (input_->TriggerKey(DIK_ESCAPE)) {
		leaveGame = true;
	}
}
