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
	camera_->Initialize();
	camera_->SetTarget(&object_->GetWorld());
}

void GEModelScene::Update()
{
	camera_->DrawDebugWindow("camera");
	

	object_->Update();

	camera_->Update();
}

void GEModelScene::Draw()
{
	object_->Draw(*camera_);
}
