#include "GEModelScene.h"

GEModelScene::GEModelScene()
{
	camera_ = std::make_unique<Camera>();

	object_ = std::make_unique<GameObject>();
}

GEModelScene::~GEModelScene()
{
}

void GEModelScene::Initialize()
{
	object_->Initialize("talker");
}

void GEModelScene::Update()
{
	camera_->DrawDebugWindow("camera");
	

	object_->Update();

	camera_->Update();
}

void GEModelScene::Draw()
{
	object_->Draw(camera_->GetViewProjectionMatrix());
}
