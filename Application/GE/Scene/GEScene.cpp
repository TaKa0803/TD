#include "GEScene.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"

GEScene::GEScene()
{
	camera_ = std::make_unique<Camera>();

	int tex = TextureManager::LoadTex("resources/SystemResources/uvChecker.png");
	sprite_.reset(Sprite::Create(tex, { 512,512 }, { 512,512 }, { 512,512 }));
}

GEScene::~GEScene()
{
}

void GEScene::Initialize()
{
	camera_->Initialize();

	
}

void GEScene::Update()
{
	camera_->DrawDebugWindow("camera");
	sprite_->DrawDebugImGui("sprite");
	
	camera_->Update();

}

void GEScene::Draw()
{
	sprite_->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}
