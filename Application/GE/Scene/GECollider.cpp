#include "GECollider.h"

GEColliderScene::GEColliderScene()
{
	camera_ = std::make_unique<Camera>();
	plane_ = std::make_unique<Plane>();
	wall = std::make_unique<OBBCollider>();
	sphere = std::make_unique<SphereCollider>();

}

GEColliderScene::~GEColliderScene()
{
}

void GEColliderScene::Initialize()
{
	camera_->Initialize();
	plane_->Initialize();
	wall->Initialize("");
	sphere->Initialize("");
}

void GEColliderScene::Update()
{

	plane_->Update();
	wall->Update();
	sphere->Update();
}

void GEColliderScene::Draw()
{
	plane_->Draw(*camera_);
	wall->Draw();
	sphere->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}
